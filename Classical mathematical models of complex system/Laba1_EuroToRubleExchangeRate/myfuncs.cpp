#include "myfuncs.h"
#include "xlsxdocument.h"
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QMessageBox>
#include <cmath>

/*-----------------
 |     БЛОК 1     |
 ----------------*/
// Загрузка исходных данных из Excel в таблицу.
bool loadDataFromExcel(const QString &filePath, QTableView *tableView)
{
    QFileInfo fileInfo(filePath);
    // 1. Проверка расширения.
    if (fileInfo.suffix().toLower() != "xlsx")
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Неверное расширение файла. Требуется .xlsx");
        return false;
    }

    // 2. Попытка открыть файл.
    QXlsx::Document xlsx(filePath);
    if (!xlsx.isLoadPackage())
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл Excel!");
        return false;
    }

    // 3. Определяем диапазон данных.
    auto dim = xlsx.dimension();
    if (!dim.isValid())
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Файл пуст или данные отсутствуют (необходим формат .xlsx)!");
        return false;
    }

    int rowCount = dim.rowCount();
    int colCount = dim.columnCount();

    // Создаем модель для QTableView.
    QStandardItemModel *model = new QStandardItemModel(rowCount - 1, colCount, tableView);

    // Заголовки из первой строки.
    QStringList headers;
    for (int col = 1; col <= colCount; ++col)
    {
        auto cell = xlsx.cellAt(1, col);
        if (cell)
            headers << cell->value().toString();
        else
            headers << QString("Column%1").arg(col);
    }
    model->setHorizontalHeaderLabels(headers);

    // Считываем все данные
    for (int row = 2; row <= rowCount; ++row)
    {
        for (int col = 1; col <= colCount; ++col)
        {
            auto cell = xlsx.cellAt(row, col);
            QString value{};

            // Если ячейка читаемая и не пустая.
            if (cell) {

                QVariant cellValue = cell->value();
                bool ok = false;

                // Попытка конвертировать в число.
                double d = cellValue.toDouble(&ok);
                if (ok && col != 2) // если это не дата
                {
                    value = QString::number(d, 'g', 6);
                }
                else if (col == 2) // столбец с датой
                {
                    // Попытка распарсить как строку даты.
                    QDate date = QDate::fromString(cellValue.toString(), "dd.MM.yyyy");
                    if (!date.isValid())
                        date = QDate::fromString(cellValue.toString(), Qt::ISODate);
                    // Попытка как Excel-число.
                    if (!date.isValid())
                    {

                        double excelDate = cellValue.toDouble(&ok);
                        if (ok)
                            date = QDate(1900, 1, 1).addDays(int(excelDate) - 2); //1900г. считается високосным + смещение Excel (с 1)
                    }
                    value = date.isValid() ? date.toString("dd.MM.yyyy") : cellValue.toString();
                }
                else // просто текст
                {
                    value = cellValue.toString();
                }
            }
            else
            {
                value = "";
            }

            QStandardItem *item = new QStandardItem(value);
            model->setItem(row - 2, col - 1, item);
        }
    }

    // Привязываем модель к QTableView.
    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Информационное сообщение.
    QMessageBox::information(nullptr, "Успех", "Данные успешно загружены!");

    return true;
}
// Чтение исходных данных из таблицы и приведение к нужному виду.
void readDataAndCurs(const QTableView *tableView_TableData, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues)
{
    const QAbstractItemModel *model = tableView_TableData->model();
    if (!model) return;

    int colData = -1;
    int colCurs = -1;
    for (int c = 0; c < model->columnCount(); ++c)
    {
        QString header = model->headerData(c, Qt::Horizontal).toString().trimmed().toLower();
        if (header == "data") colData = c;
        if (header == "curs") colCurs = c;
    }

    if (colData == -1 && colCurs == -1)
    {
        QMessageBox::critical(nullptr, "Ошибка", "Отсутствуют столбцы 'data' и 'curs' для вычислений!");
        return;
    }

    numericDates.clear();
    cursValues.clear();

    QDate epoch(1970, 1, 1);

    for (int r = 0; r < model->rowCount(); ++r)
    {
        // Конвертация даты в число дней.
        QVariant valData = model->index(r, colData).data();
        QDate date = QDate::fromString(valData.toString(), "dd.MM.yyyy");
        if (date.isValid())
        {
            dataColumn.append(date.toString("dd.MM.yyyy"));
            numericDates.append(epoch.daysTo(date)/10000.0); // Чтобы не работать с большими числами, уменьшим в 10000 раз число дней.
        }
        else
        {
            numericDates.append(0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать дату:%1!").arg(valData.toString()));
            return;
        }

        // Конвертация курса в число.
        QVariant valCurs = model->index(r, colCurs).data();
        bool ok;
        double curs = valCurs.toString().toDouble(&ok);
        if (ok)
        {
            cursValues.append(curs);
        }
        else
        {
            cursValues.append(0.0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать курс:%1!").arg(valCurs.toString()));
            return;
        }
    }
}
//Вычисление значений, необходимых для построения регрессии
void calculateRegressionValues(const QVector<double> numericDates, const QVector<double> &cursValues,
                               QVector<double> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                               RegressionValues &values)
{
    values.n = numericDates.size();
    if (values.n == 0) return;

    xSquared.clear();
    ySquared.clear();
    xyProduct.clear();

    values.sumX = values.sumY = values.sumX2 = values.sumY2 = values.sumXY = 0.0;

    for (int i = 0; i < values.n; ++i)
    {
        double x = numericDates[i];
        double y = cursValues[i];

        double x2 = x * x;
        double y2 = y * y;
        double xy = x * y;

        xSquared.append(x2);
        ySquared.append(y2);
        xyProduct.append(xy);

        values.sumX  += x;
        values.sumY  += y;
        values.sumX2 += x2;
        values.sumY2 += y2;
        values.sumXY += xy;
    }

    values.meanX = values.sumX / values.n;
    values.meanY = values.sumY / values.n;
}
//Заполнение таблицы значениями.
void fillTotalTable(QTableView *tableView,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<double> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct)
{
    int rowCount = numericDates.size();
    int colCount = 6; // data, xi, yi, xi^2, yi^2, xi*yi

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки.
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, "xi");
    model->setHeaderData(2, Qt::Horizontal, "yi");
    model->setHeaderData(3, Qt::Horizontal, "xi^2");
    model->setHeaderData(4, Qt::Horizontal, "yi^2");
    model->setHeaderData(5, Qt::Horizontal, "xi*yi");

    for (int r = 0; r < rowCount; ++r)
    {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'g', 6)));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'g', 6)));
        model->setItem(r, 3, new QStandardItem(QString::number(xSquared[r], 'g', 6)));
        model->setItem(r, 4, new QStandardItem(QString::number(ySquared[r], 'g', 6)));
        model->setItem(r, 5, new QStandardItem(QString::number(xyProduct[r], 'g', 6)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}

/*-----------------
 |     БЛОК 2     |
 ----------------*/
// Получение выводов о коэффициенте Пирсона (коэф. корреляции о линейности связи между данными).
void getRegConnDesc(RegressionValues &values)
{
    if (values.r == 0)
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: отсутствует.\n"
                                   "Интерпретация связи: -.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r > 0 && values.r < 1)
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: вероятностная, прямая.\n"
                                   "Интерпретация связи: С увеличением X увеличивается Y.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r < 0 && values.r > -1)
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: вероятностная, обратная.\n"
                                   "Интерпретация связи: С увеличением X уменьшается Y, и наоборот.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r == 1)
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: функциональная, прямая.\n"
                                   "Интерпретация связи: Каждому значению факторного признака строго соответствует одно значение функции, с увеличением X увеличивается Y.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r == -1)
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: функциональная, обратная.\n"
                                   "Интерпретация связи: Каждому значению факторного признака строго соответствует одно значение функции, увеличением X уменьшается Y, и наоборот.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else
    {
        values.regConStr = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Ошибка вычислений.")
                               .arg(QString::number(values.r, 'f', 2));
        return;
    }

    // Характер связи.
    if (std::abs(values.r) < 0.3)
        values.regConStr += QString("\nХарактер связи: Практически отсустствует.");
    else if (std::abs(values.r) >= 0.3 && std::abs(values.r) < 0.5)
        values.regConStr += QString("\nХарактер связи: Слабая.");
    else if (std::abs(values.r) >= 0.5 && std::abs(values.r) < 0.7)
        values.regConStr += QString("\nХарактер связи: Умеренная.");
    else
        values.regConStr += QString("\nХарактер связи: Сильная.");

}
// Вычисление коэффициентов и вспомогательных величин.
void fillLabelsAuxiliaryQuantities(const int& mode,
                                   const QVector<double> &numericDates,
                                   const QVector<double> &cursValues,
                                   QVector<double> &predicts,
                                   RegressionValues &values)
{
    values.coeffs.clear();
    predicts.clear();

    // Частные расчеты.
    switch(mode)
    {
    case 0:
    {
        // Вычисление вспомогательных величин и коэффициентов.
        double A{}, A0{}, A1{}, B{}, a0{}, a1{};
        A = values.n * values.sumX2 - values.sumX * values.sumX;
        A0 = values.sumY * values.sumX2 - values.sumX * values.sumXY;
        A1 = values.n * values.sumXY - values.sumX * values.sumY; // = B1
        B = values.n * values.sumY2 - values.sumY * values.sumY;

        // Заполнение вектора коэффициентов.
        a0 = A0/A;
        values.coeffs.append(a0);

        a1 = A1/A;
        values.coeffs.append(a1);

        // Заполнение данных о коэффициентах.
        values.regCoefStr = QString("Вспомогательные величины:\n"
                                    "A = %1\n"
                                    "A0 = %2\n"
                                    "A1 = %3\n"
                                    "\n"
                                    "B = %4\n"
                                    "\n"
                                    "Коэффициенты:\n"
                                    "a0 = %5\n"
                                    "a1 = %6")
                                .arg(QString::number(A, 'f', 6))
                                .arg(QString::number(A0, 'f', 6))
                                .arg(QString::number(A1, 'f', 6))
                                .arg(QString::number(B, 'f', 6))
                                .arg(QString::number(a0, 'f', 6))
                                .arg(QString::number(a1, 'f', 6));
        // Заполнение итоговой функции.
        values.funcStr = QString("y = %1 + %2x").arg(QString::number(a0, 'f', 6)).arg(QString::number(a1, 'f', 6));
        // Заполнение описания коэффициента Пирсона.
        values.r = A1 / std::sqrt(A * B); // или B1 / std::sqrt(A * B)
        getRegConnDesc(values);
        // Подсчет других величин.
        values.Sx2 = 1.0 / (values.n - 1.0) * (values.sumX2 - 1.0 / values.n * std::pow(values.sumX, 2));
        values.Sy2 = 1.0 / (values.n - 1.0) * (values.sumY2 - 1.0 / values.n * std::pow(values.sumY, 2));
        values.SxMean = std::sqrt(values.Sx2)/std::sqrt(values.n);
        values.SyMean = std::sqrt(values.Sy2)/std::sqrt(values.n);
        // Вычисление предсказаний модели.
        for (int i = 0; i < values.n; ++i)
        {
            double x{ numericDates[i] };
            double y{ cursValues[i] };
            double y_pred{};
            y_pred = values.coeffs[0] + values.coeffs[1] * x;
            predicts.append(y_pred);

            // Вычисление величин по предсказаниям.
            values.Sost += std::pow(y - y_pred, 2);
            values.Sregr += std::pow(y_pred - values.meanY, 2);

        }
        values.Spoln = values.Sost + values.Sregr;
        values.R2 = 1 - values.Sost/values.Spoln; // или Sregr/Spoln
        values.MSE = values.Sost / values.n;
        break;
    }
    // case 1:
    //     setWindowTitle("Обратная линейная регрессия");
    //     break;
    // case 2:
    //     setWindowTitle("Экспоненциальная регрессия");
    //     break;
    // case 3:
    //     setWindowTitle("Гиперболическая регрессия");
    //     break;
    // case 4:
    //     setWindowTitle("Параболическая регрессия");
    //     break;
    // case 5:
    //     setWindowTitle("Логарифмическая регрессия");
    //     break;
    // case 6:
    //     setWindowTitle("Степенная регрессия");
    //     break;
    // case 7:
    //     setWindowTitle("Полиномиальная регрессия");
    //     break;
    default:
        break;
    }

}
// Заполнение таблицы значениями.
void fillPredictTable(QTableView *tableView,
                      const QVector<QString> &dataColumn,
                      const QVector<double> &numericDates,
                      const QVector<double> &cursValues,
                      const QVector<double> &predicts,
                      const RegressionValues &values)
{
    int rowCount = numericDates.size();
    int colCount = 7; // data, xi, yi, yi^T, (yi-yi^T)^2, (yi^T-mean(y))^2, (yi-mean(y))^2

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки.
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, "xi");
    model->setHeaderData(2, Qt::Horizontal, "yi");
    model->setHeaderData(3, Qt::Horizontal, "yi^T");
    model->setHeaderData(4, Qt::Horizontal, "(yi-yi^T)^2");
    model->setHeaderData(5, Qt::Horizontal, "(yi^T-mean(y))^2");
    model->setHeaderData(6, Qt::Horizontal, "(yi-mean(y))^2");

    for (int r = 0; r < rowCount; ++r)
    {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r])));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'g', 6)));
        model->setItem(r, 3, new QStandardItem(QString::number(predicts[r], 'g', 6)));
        model->setItem(r, 4, new QStandardItem(QString::number(std::pow(cursValues[r]-predicts[r], 2), 'g', 6)));
        model->setItem(r, 5, new QStandardItem(QString::number(std::pow(predicts[r] - values.meanY, 2), 'g', 6)));
        model->setItem(r, 6, new QStandardItem(QString::number(std::pow(cursValues[r]-values.meanY, 2), 'g', 6)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}
