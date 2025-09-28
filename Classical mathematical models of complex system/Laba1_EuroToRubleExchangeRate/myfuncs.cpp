#include "myfuncs.h"
#include "xlsxdocument.h"
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QMessageBox>
#include <cmath>

/*------------------
 |  ПРЕДОБРАБОТКА  |
 -----------------*/
// Чтение исходных данных из таблицы и приведение к нужному виду.
bool readDataAndCurs(const QTableView *tableView_TableData, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues)
{
    const QAbstractItemModel *model = tableView_TableData->model();
    if (!model) return false;

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
        return false;
    }

    numericDates.clear();
    cursValues.clear();

    QDate epoch(1970, 1, 1);
    QVector<int> rawDates; // временное хранилище дат в днях

    for (int r = 0; r < model->rowCount(); ++r)
    {
        // Конвертация даты в число дней.
        QVariant valData = model->index(r, colData).data();
        QDate date = QDate::fromString(valData.toString(), "dd.MM.yyyy");
        if (date.isValid())
        {
            dataColumn.append(date.toString("dd.MM.yyyy"));
            rawDates.append(epoch.daysTo(date));
        }
        else
        {
            rawDates.append(0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать дату: %1!").arg(valData.toString()));
            return false;
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
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать курс: %1!").arg(valCurs.toString()));
            return false;
        }
    }

    // --- Нормализация дат ---
    if (!rawDates.isEmpty())
    {
        int minDateDays = *std::min_element(rawDates.begin(), rawDates.end());
        int maxDateDays = *std::max_element(rawDates.begin(), rawDates.end());
        int range = maxDateDays - minDateDays;

        numericDates.resize(rawDates.size());
        for (int i = 0; i < rawDates.size(); ++i)
        {
            numericDates[i] = 0.01 + 0.99 * double(rawDates[i] - minDateDays) / double(range);
        }
    }

    return true;
}

/*-----------------
 |  ГЛАВНОЕ ОКНО  |
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

    // Центрирование.
    centerTableItems(tableView);

    // Информационное сообщение.
    QMessageBox::information(nullptr, "Успех", "Данные успешно загружены!");

    return true;
}

/*-----------------
 |     БЛОК 1     |
 ----------------*/
//Заполнение таблицы значениями.
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<QVector<double>> &CalcColumns,
                    const QVector<double> &ySquared,
                    const int &degree)
{
    int rowCount = numericDates.size();
    int colCount = 3 + CalcColumns.size();; // data, xi, yi, (CalcColumns.size) столбцов с вычисленными значениями
    if (degree == 1) colCount += 1; // (yi)^2

    if (mode == 2) colCount += 1;   // ln(yi)
    if (mode == 3) colCount += 1;   // 1/x
    if (mode == 4) colCount += 1;   // ln(x)
    if (mode == 6) colCount += 2;   // ln(xi), ln(yi)


    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // --- Заголовки ---
    // Базовые.
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
    model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");
    int c = 3; // итератор для номера столбца

    if (mode == 2) model->setHeaderData(c++, Qt::Horizontal, "lny\u1D62");
    if (mode == 3) model->setHeaderData(c++, Qt::Horizontal, "1/x\u1D62");
    if (mode == 5) model->setHeaderData(c++, Qt::Horizontal, "lnx\u1D62");
    if (mode == 6)
    {
        model->setHeaderData(c++, Qt::Horizontal, "lnx\u1D62");
        model->setHeaderData(c++, Qt::Horizontal, "lny\u1D62");
    }

    // Автоматические для CalcColumns.
    for (int k = 0; k < CalcColumns.size(); ++k)
    {
        QString header;
        if (k < 2 * degree - 1)
        {
            // k = 0 → x^2, k = 1 → x^3 ... k = 2*degree-2 → x^(2*degree)
            int power = k + 2;
            if (mode == 3)
                header = QString("1/x\u1D62") + toSuperscript(power);
            else if (mode == 5 || mode == 6)
                header = QString("(lnx\u1D62)") + toSuperscript(power);
            else
                header = QString((mode != 1) ? "x\u1D62" : "y\u1D62") + toSuperscript(power);
        }
        else
        {
            // оставшиеся degree штук: x^1*y ... x^degree*y
            int power = k - (2 * degree - 1) + 1; // сдвигаем индекс
            if (mode != 1)
                if (mode == 2) header = "x\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "lny\u1D62";
                else if (mode == 3) header = "(1/x\u1D62)" + ((power == 1) ? "" : toSuperscript(power)) + "y\u1D62";
                else if (mode == 5 || mode == 6) header = "lnx\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "lny\u1D62";
                else header = "x\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "y\u1D62";
            else
                header = "y\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "x\u1D62";
        }

        model->setHeaderData(c++, Qt::Horizontal, header);

        // Вывод y^2 отдельно.
        if (k == 0 && degree == 1)
        {
            if (mode == 2 || mode == 6)  header = QString("(lny\u1D62)") + toSuperscript(2);
            else header = QString((mode != 1) ? "y\u1D62" : "x\u1D62") + toSuperscript(2);
            model->setHeaderData(c++, Qt::Horizontal, header);
        }
    }

    // --- Заполнение таблицы ---
    for (int r = 0; r < rowCount; ++r)
    {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 6)));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));
        int c = 3;

        if (mode == 2) model->setItem(r, c++, new QStandardItem(QString::number(std::log(cursValues[r]), 'f', 6)));
        if (mode == 3) model->setItem(r, c++, new QStandardItem(QString::number(1/numericDates[r], 'f', 6)));
        if (mode == 5) model->setItem(r, c++, new QStandardItem(QString::number(std::log(numericDates[r]), 'f', 6)));
        if (mode == 6)
        {
            model->setItem(r, c++, new QStandardItem(QString::number(std::log(numericDates[r]), 'f', 6)));
            model->setItem(r, c++, new QStandardItem(QString::number(std::log(cursValues[r]), 'f', 6)));
        }

        // CalcColumns
        for (int col = 0; col < CalcColumns.size(); ++col)
        {
            model->setItem(r, c++, new QStandardItem(QString::number(CalcColumns[col][r], 'f', 6)));
            if ((mode == 2 || mode == 3) && c == 5)
                model->setItem(r, c++, new QStandardItem(QString::number(ySquared[r], 'f', 6)));
            else if (mode == 6 && c == 6)
                model->setItem(r, c++, new QStandardItem(QString::number(ySquared[r], 'f', 6)));
            else if (c == 4 && degree == 1)
                model->setItem(r, c++, new QStandardItem(QString::number(ySquared[r], 'f', 6)));
        }
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Центрирование.
    centerTableItems(tableView);
}
// Заполнение текстового поля с суммами.
QString fillSUMsTextEdit(const int &mode, const RegressionValues &values, const int &degree)
{
    QString infoSUMs{};

    // Первая сумма.
    if (mode == 3)
        infoSUMs += QString("\u22111/x = %1\n").arg(QString::number(values.sumX, 'f', 6));
    else if (mode == 5 || mode == 6)
        infoSUMs += QString("\u2211lnx = %1\n").arg(QString::number(values.sumX, 'f', 6));
    else
        infoSUMs += QString((mode != 1) ? "\u2211x = %1\n" : "\u2211y = %1\n").arg(QString::number(values.sumX, 'f', 6));

    // Вторая сумма.
    if (mode == 2 || mode == 6)
        infoSUMs += QString("\u2211lny = %1\n").arg(QString::number(values.sumY, 'f', 6));
    else
        infoSUMs += QString((mode != 1) ? "\u2211y = %1\n" : "\u2211x = %1\n").arg(QString::number(values.sumY, 'f', 6));


    int col = 0;
    // --- x^2, x^3, ..., x^(2 * degree) ---
    for (int k = 2; k <= 2 * degree; ++k)
    {
        if (mode == 3)
            infoSUMs += QString("\u2211(1/x)%1 = %2\n")
                            .arg((k == 1) ? "" : toSuperscript(k))
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else if (mode == 5 || mode == 6)
            infoSUMs += QString("\u2211(lnx)%1 = %2\n")
                            .arg((k == 1) ? "" : toSuperscript(k))
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else
            infoSUMs += QString((mode != 1) ? "\u2211x%1 = %2\n" : "\u2211y%1 = %2\n")
                            .arg((k == 1) ? "" : toSuperscript(k))
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        // вывод SUM(y^2) отдельно
        if (k == 2 && degree == 1)
        {
            if (mode == 2 || mode == 6)
                infoSUMs += QString("\u2211(lny)%1 = %2\n")
                                .arg(toSuperscript(2))
                                .arg(QString::number(values.sumY2, 'f', 6));
            else
                infoSUMs += QString((mode != 1) ? "\u2211y%1 = %2\n" : "\u2211x%1 = %2\n")
                                .arg(toSuperscript(2))
                                .arg(QString::number(values.sumY2, 'f', 6));
        }

        ++col;
    }

    // --- x*y, x^2*y, ..., x^degree*y ---
    for (int k = 1; k <= degree; ++k)
    {
        if (mode == 2)
            infoSUMs += QString("\u2211xlny = %1\n")
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else if (mode == 3)
            infoSUMs += QString("\u2211(1/x)y = %1\n")
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else if (mode == 5)
            infoSUMs += QString("\u2211(lnx)y = %1\n")
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else if (mode == 6)
            infoSUMs += QString("\u2211lnxlny = %1\n")
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        else
            infoSUMs += QString((mode != 1) ? "\u2211x%1y = %2\n" : "\u2211y%1x = %2\n")
                            .arg(k == 1 ? "" : toSuperscript(k))
                            .arg(QString::number(values.SUMs[col], 'f', 6));
        ++col;
    }

    return infoSUMs;
}

// Для полиномиальных видов регрессий (расчет суммы квадратов, кубов и т.д. и произведений величин).
void calculatePolynomRegressionSums(const QVector<double> &numericDates,
                                      const QVector<double> &cursValues,
                                      const int &degree,
                                      QVector<QVector<double>> &CalcColumns,
                                      QVector<double> &ySquared,
                                      RegressionValues &values)
{
    values.n = numericDates.size();
    if (values.n == 0) return;

    CalcColumns.clear();
    values.SUMs.clear();

    // Количество столбцов:
    // * все степени x до (degree + degree) включительно, исключая x^1, т.к.
    // поиск частных производных по последнему коэффициенту потребует умножения
    // на эту степень;
    // * степени y не нужны, как и y^1;
    // * произведения степеней x до degree включительно на y.
    // Итого: (degree + degree - 1) + degree = 3 * degree - 1.
    // Для каждой точки строим: [x^2, x^3, ..., x^(degree+degree), x*y, x^2*y, ..., x^degree*y].
    int numCalcCols = 3 * degree - 1;

    // Преобразование векторов.
    CalcColumns.resize(numCalcCols, QVector<double>(values.n, 0.0));
    values.SUMs.resize(numCalcCols, 0.0);

    for (int i = 0; i < values.n; ++i)
    {
        // Исходные данные.
        double x = numericDates[i];
        double y = cursValues[i];

        double y2 = y*y;
        ySquared.append(y2);

        values.sumX += x;
        values.sumY += y;
        values.sumY2 += y2;

        // Учет подсчитываемого стобца.
        int CalcCol = 0;

        // 1) x^k, k!=1
        for (int k = 2; k <= degree + degree; ++k)
        {
            double xk{std::pow(x, k)};
            CalcColumns[CalcCol][i] = xk;
            values.SUMs[CalcCol] += xk;
            ++CalcCol;
        }

        // 2) x^k * y
        for (int k = 1; k <= degree; ++k)
        {
            double xky{std::pow(x, k) * y};
            CalcColumns[CalcCol][i] = xky;
            values.SUMs[CalcCol] += xky;
            ++CalcCol;
        }
    }

    values.meanX = values.sumX / values.n;
    values.meanY = values.sumY / values.n;
}

// Метод Гаусса (матрица А, очевидно, квадратная).
bool gaussSolve(QVector<QVector<double>> &A, QVector<double> &b, QVector<double> &solve)
{
    int n = A.size();

    // Прямой ход (под главной диагональю 0, главная диагональ - единичная).
    for (int k = 0; k < n; ++k)
    {
        // Поиск главного элемента (pivot).
        // Чем больше он будет, тем устойчивее алгоритм, поэтому поднимаем его наверх.
        int maxRow = k;
        for (int i = k+1; i < n; ++i)
        {
            if (std::abs(A[i][k]) > std::abs(A[maxRow][k]))
                maxRow = i;
        }
        A.swapItemsAt(k, maxRow);   // поменять строки местами в матрице А
        std::swap(b[k], b[maxRow]); // поменять строки местами в матрице-векторе B

        // Делим строку на главный элемент.
        double pivot = A[k][k];
        // Проверка на вырожденность системы (бесконечно много решений), несовместность (решений нет).
        if (std::abs(pivot) < 1e-16)
        {
            std::cerr << "Система не имеет единственного решения!" << std::endl;
            return false;
        }
        for (int j = k; j < n; ++j)
            A[k][j] /= pivot; // при j=k эл-т главное диагонали становится равен 1
        b[k] /= pivot;

        // Обнуляем элементы под главной диагональю.
        for (int i = k+1; i < n; ++i)
        {
            double factor = A[i][k];
            for (int j = k; j < n; ++j)
                A[i][j] -= factor * A[k][j];
            b[i] -= factor * b[k];
        }
    }

    // Обратный ход.
    for (int i = n-1; i >= 0; --i)
    {
        solve[i] = b[i];
        for (int j = i+1; j < n; ++j)
            solve[i] -= A[i][j] * solve[j];
    }

    return true;
}

// Для полиномиальных видов регрессий (потсроение матриц A и B для метода Гаусса).
bool buildAndSolvePolynomRegression(RegressionValues &values, const int &degree)
{
    int m = degree; // для удобства, полином степени m
    QVector<QVector<double>> A(m + 1, QVector<double>(m + 1, 0.0)); // матрица А левой части системы
    QVector<double> B(m + 1, 0.0);  // матрица-вектор B правой части системы

    // --- Матрица A ---
    for (int j = 0; j <= m; ++j)
    {
        for (int k = 0; k <= m; ++k)
        {
            if (j == 0 && k == 0)
                A[j][k] = values.n;
            else
            {
                int power = j + k;
                if (power == 1)
                    A[j][k] = values.sumX;
                else
                    A[j][k] = values.SUMs[power - 2]; // -2, т.к. я не храню values.SUMs E(xi)^0=n и E(xi)^1=Ex
            }
        }
    }

    // --- Вектор B ---
    for (int j = 0; j <= m; ++j)
    {
        if (j == 0)
            B[j] = values.sumY;
        else
        {
            // по соглашению: первые degree-1+degree = 2*degree-1 элементов — E(xi)^p
            // следующие degree элементов — E((xi)^k*yi)
            int idx = (2 * degree - 1) + (j - 1);
            B[j] = values.SUMs[idx];
        }
    }

    // --- Решаем систему методом Гаусса ---
    QVector<double> solve(A.size());
    if (!gaussSolve(A, B, solve))
        return false;

    values.coeffs.clear();

    for (int i = 0; i <= degree; ++i)
    {
        QString key = QString("a%1").arg(i);
        values.coeffs.insert(key, solve[i]);
    }

    return true;
}
/*-----------------
 |     БЛОК 2     |
 ----------------*/
// Заполнение таблицы значениями.
void fillPredictTable(QTableView *tableView,
                      const int &mode,
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
    if (mode == 2)
    {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "x\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "lny\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "(lny\u1D62)\u1D40");
        model->setHeaderData(4, Qt::Horizontal, "(lny\u1D62 - (lny\u1D62)\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "((lny\u1D62)\u1D40 - (lny)\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "(lny\u1D62 - (lny)\u0304)\u00B2");
    }
    else if (mode == 3)
    {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "1/x\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "y\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "(y\u1D62)\u1D40");
        model->setHeaderData(4, Qt::Horizontal, "(y\u1D62 - y\u1D62\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "(y\u1D62\u1D40 - y\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "(y\u1D62 - y\u0304)\u00B2");
    }
    else if (mode == 5)
    {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "lnx\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "y\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "(y\u1D62)\u1D40");
        model->setHeaderData(4, Qt::Horizontal, "(y\u1D62 - y\u1D62\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "(y\u1D62\u1D40 - y\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "(y\u1D62 - y\u0304)\u00B2");
    }
    else if (mode == 6)
    {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "lnx\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "lny\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "(lny\u1D62)\u1D40");
        model->setHeaderData(4, Qt::Horizontal, "(lny\u1D62 - (lny\u1D62)\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "((lny\u1D62)\u1D40 - (lny)\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "(lny\u1D62 - (lny)\u0304)\u00B2");
    }
    else
    {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
        model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");
        model->setHeaderData(3, Qt::Horizontal, (mode != 1) ? "y\u1D62\u1D40" : "x\u1D62\u1D40");
        model->setHeaderData(4, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u1D62\u1D40)\u00B2" : "(x\u1D62 - x\u1D62\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, (mode != 1) ? "(y\u1D62\u1D40 - y\u0304)\u00B2" : "(x\u1D62\u1D40 - x\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u0304)\u00B2" : "(x\u1D62 - x\u0304)\u00B2");
    }

    for (int r = 0; r < rowCount; ++r)
    {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 6)));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));
        model->setItem(r, 3, new QStandardItem(QString::number(predicts[r], 'f', 6)));
        model->setItem(r, 4, new QStandardItem(QString::number(std::pow(cursValues[r]-predicts[r], 2), 'f', 6)));
        model->setItem(r, 5, new QStandardItem(QString::number(std::pow(predicts[r] - values.meanY, 2), 'f', 6)));
        model->setItem(r, 6, new QStandardItem(QString::number(std::pow(cursValues[r]-values.meanY, 2), 'f', 6)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Центрирование.
    centerTableItems(tableView);
}

// Получение выводов о коэффициенте Пирсона (коэф. корреляции о линейности связи между данными).
void getRegConnDesc(RegressionValues &values)
{
    if (values.r == 0)
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>отсутствует</i>.<br>"
                                   "<b>Интерпретация связи:</b> <i>-</i>.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r > 0 && values.r < 1)
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>вероятностная, прямая</i>.<br>"
                                   "<b>Интерпретация связи:</b> <i>С увеличением X увеличивается Y</i>.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r < 0 && values.r > -1)
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>вероятностная, обратная</i>.<br>"
                                   "<b>Интерпретация связи:</b> <i>С увеличением X уменьшается Y, и наоборот</i>.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r == 1)
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>функциональная, прямая</i>.<br>"
                                   "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, с увеличением X увеличивается Y</i>.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else if (values.r == -1)
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>функциональная, обратная</i>.<br>"
                                   "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, увеличением X уменьшается Y, и наоборот</i>.")
                               .arg(QString::number(values.r, 'f', 2));
    }
    else
    {
        values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                                   "<i>r = %1</i><br>"
                                   "<b>Ошибка вычислений.</b>")
                               .arg(QString::number(values.r, 'f', 2));
        return;
    }

    // Характер связи.
    if (std::abs(values.r) < 0.3)
        values.regConStr += QString("<br><b>Теснота связи:</b> <i>Практически отсустствует</i>.");
    else if (std::abs(values.r) >= 0.3 && std::abs(values.r) < 0.5)
        values.regConStr += QString("<br><b>Теснота связи:</b> <i>Слабая</i>.");
    else if (std::abs(values.r) >= 0.5 && std::abs(values.r) < 0.7)
        values.regConStr += QString("<br><b>Теснота связи:</b> <i>Умеренная</i>.");
    else
        values.regConStr += QString("<br><b>Теснота связи:</b> <i>Сильная</i>.");

}

// Вспомогательные величины и коэффициенты для линейных видов регрессии.
// У обратной такая подмена происходит потому, что sumX хранит sumY на самом деле и т.д.
void getCoefsForLinearOrInverse(const bool &inverse, RegressionValues &values)
{
    values.coeffs.clear();
    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1, B, B0, B1, b0, b1)
    // A
    values.coeffs.insert((inverse == false) ? "A" : "B", values.n * values.SUMs[0] - values.sumX * values.sumX);
    // A0
    values.coeffs.insert((inverse == false) ? "A0" : "B0", values.sumY * values.SUMs[0] - values.sumX * values.SUMs[1]);
    // A1
    values.coeffs.insert((inverse == false) ? "A1" : "B1", values.n * values.SUMs[1] - values.sumY * values.sumX);
    // B
    values.coeffs.insert((inverse == false) ? "B" : "A", values.n * values.sumY2 - values.sumY * values.sumY);
    // B0
    values.coeffs.insert((inverse == false) ? "B0" : "A0", values.sumX * values.sumY2 - values.sumY * values.SUMs[1]);
    // B1
    values.coeffs.insert((inverse == false) ? "B1" : "A1", values.n * values.SUMs[1] - values.sumY * values.sumX);
    // a0
    values.coeffs.insert("a0", values.coeffs["A0"] / values.coeffs["A"]);
    // a1
    values.coeffs.insert("a1", values.coeffs["A1"] / values.coeffs["A"]);
    // b0
    values.coeffs.insert("b0", values.coeffs["B0"] / values.coeffs["B"]);
    // b1
    values.coeffs.insert("b1", values.coeffs["B1"] / values.coeffs["B"]);
    // r
    if (inverse) values.r = values.coeffs["B1"] / std::sqrt(values.coeffs["A"] * values.coeffs["B"]);
    else values.r = values.coeffs["A1"] / std::sqrt(values.coeffs["A"] * values.coeffs["B"]);

}

// Сотавление вида функции и описания коэффициентов.
void getFuncCoefDescr(const int& mode, RegressionValues &values)
{
    if (mode == 0 || mode == 1)
    {
        // Заполнение данных о коэффициентах.
        values.regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                                    "A = %1<br>"
                                    "A<sub>0</sub> = %2<br>"
                                    "A<sub>1</sub> = %3<br>"
                                    "<br>"
                                    "B = %4<br>"
                                    "B<sub>0</sub> = %5<br>"
                                    "B<sub>1</sub> = %6<br>"
                                    "<br>"
                                    "<b>Коэффициенты:</b><br>"
                                    "a<sub>0</sub> = %7<br>"
                                    "a<sub>1</sub> = %8<br>"
                                    "<br>"
                                    "b<sub>0</sub> = -a<sub>0</sub>/a<sub>1</sub> = %9<br>"
                                    "b<sub>1</sub> = 1/a<sub>1</sub> = %10")
                                .arg(QString::number(values.coeffs["A"], 'g', 6))
                                .arg(QString::number(values.coeffs["A0"], 'g', 6))
                                .arg(QString::number(values.coeffs["A1"], 'g', 6))
                                .arg(QString::number(values.coeffs["B"], 'g', 6))
                                .arg(QString::number(values.coeffs["B0"], 'g', 6))
                                .arg(QString::number(values.coeffs["B1"], 'g', 6))
                                .arg(QString::number(values.coeffs["a0"], 'g', 6))
                                .arg(QString::number(values.coeffs["a1"], 'g', 6))
                                .arg(QString::number(values.coeffs["b0"], 'g', 6))
                                .arg(QString::number(values.coeffs["b1"], 'g', 6));
    }
    if (mode == 2 || mode == 6)
    {
        values.regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                                    "A = %1<br>"
                                    "A<sub>0</sub> = %2<br>"
                                    "A<sub>1</sub> = %3<br>"
                                    "<br>"
                                    "B = %4<br>"
                                    "<br>"
                                    "<b>Коэффициенты:</b><br>"
                                    "a<sub>0</sub> = %5<br>"
                                    "a<sub>1</sub> = %6<br>"
                                    "<br>"
                                    "b<sub>0</sub> = lna<sub>0</sub> = %7<br>"
                                    "b<sub>1</sub> = a<sub>1</sub> = %8")
                                .arg(QString::number(values.coeffs["A"], 'g', 6))
                                .arg(QString::number(values.coeffs["A0"], 'g', 6))
                                .arg(QString::number(values.coeffs["A1"], 'g', 6))
                                .arg(QString::number(values.coeffs["B"], 'g', 6))
                                .arg(QString::number(std::exp(values.coeffs["a0"]), 'g', 6))
                                .arg(QString::number(values.coeffs["a1"], 'g', 6))
                                .arg(QString::number(values.coeffs["a0"], 'g', 6))
                                .arg(QString::number(values.coeffs["a1"], 'g', 6));
    }
    if (mode == 3 || mode == 5)
    {
        values.regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                                    "A = %1<br>"
                                    "A<sub>0</sub> = %2<br>"
                                    "A<sub>1</sub> = %3<br>"
                                    "<br>"
                                    "B = %4<br>"
                                    "<br>"
                                    "<b>Коэффициенты:</b><br>"
                                    "a<sub>0</sub> = %5<br>"
                                    "a<sub>1</sub> = %6")
                                .arg(QString::number(values.coeffs["A"], 'g', 6))
                                .arg(QString::number(values.coeffs["A0"], 'g', 6))
                                .arg(QString::number(values.coeffs["A1"], 'g', 6))
                                .arg(QString::number(values.coeffs["B"], 'g', 6))
                                .arg(QString::number(values.coeffs["a0"], 'g', 6))
                                .arg(QString::number(values.coeffs["a1"], 'g', 6));
    }


    // Заполнение итоговой функции.
    switch(mode)
    {
    case 0:
    {
        double a0 = values.coeffs.value("a0", 0.0);
        double a1 = values.coeffs.value("a1", 0.0);

        QString sign = (a1 >= 0) ? " + " : " - ";
        values.funcStr = QString("y = %1%2%3 \u00B7 x")
                             .arg(QString::number(a0, 'g', 6))
                             .arg(sign)
                             .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 1:
    {
        double b0 = values.coeffs.value("b0", 0.0);
        double b1 = values.coeffs.value("b1", 0.0);

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        values.funcStr = QString("x = %1%2%3 \u00B7 y")
                             .arg(QString::number(b0, 'g', 6))
                             .arg(sign_b)
                             .arg(QString::number(std::abs(b1), 'g', 6));
        break;
    }
    case 2:
    {
        double b0 = values.coeffs.value("a0", 0.0);
        double b1 = values.coeffs.value("a1", 0.0);

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        values.funcStr = QString("lny = %1%2%3 \u00B7 x")
                             .arg(QString::number(b0, 'g', 6))
                             .arg(sign_b)
                             .arg(QString::number(std::abs(b1), 'g', 6));
        values.funcStr += " или ";
        values.funcStr += QString("y = %1 \u00B7 e^(%2 \u00B7 x)")
                              .arg(QString::number(std::exp(b0), 'g', 6))
                              .arg(QString::number(b1, 'g', 6));
        break;
    }
    case 3:
    {
        double a0 = values.coeffs.value("a0", 0.0);
        double a1 = values.coeffs.value("a1", 0.0);

        QString sign_a = (a1 >= 0) ? " + " : " - ";
        values.funcStr = QString("y = %1%2%3 \u00B7 z")
                             .arg(QString::number(a0, 'g', 6))
                             .arg(sign_a)
                             .arg(QString::number(std::abs(a1), 'g', 6));
        values.funcStr += " или ";
        values.funcStr += QString("y = %1%2%3/x")
                              .arg(QString::number(a0, 'g', 6))
                              .arg(sign_a)
                              .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 5:
    {
        double a0 = values.coeffs.value("a0", 0.0);
        double a1 = values.coeffs.value("a1", 0.0);

        QString sign_a = (a1 >= 0) ? " + " : " - ";
        values.funcStr = QString("y = %1%2%3 \u00B7 z")
                             .arg(QString::number(a0, 'g', 6))
                             .arg(sign_a)
                             .arg(QString::number(std::abs(a1), 'g', 6));
        values.funcStr += " или ";
        values.funcStr += QString("y = %1%2%3 \u00B7 lnx")
                              .arg(QString::number(a0, 'g', 6))
                              .arg(sign_a)
                              .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 6:
    {
        double b0 = values.coeffs.value("a0", 0.0);
        double b1 = values.coeffs.value("a1", 0.0);

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        values.funcStr = QString("y = %1%2%3 \u00B7 z")
                             .arg(QString::number(b0, 'g', 6))
                             .arg(sign_b)
                             .arg(QString::number(std::abs(b1), 'g', 6));
        values.funcStr += " или ";
        values.funcStr += QString("y = %1 \u00B7 x^(%2)")
                              .arg(QString::number(std::exp(b0), 'g', 6))
                              .arg(QString::number(b1, 'g', 6));
        break;
    }
    default:
        break;
    }
}

// Подсчет всего остального (линейная и обратная линейная регрессия, частный случай полиномиальной).
void calculateLinearOrInverseRegression(const int& mode,
                               const QVector<double> &numericDates,
                               const QVector<double> &cursValues,
                               QVector<double> &predicts,
                               RegressionValues &values)
{
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, values);

    // --- Коэффициент Пирсона ---
    // Заполнение описания коэффициента Пирсона.
    getRegConnDesc(values);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values.Sx2 = 1.0 / (values.n - 1.0) * (values.SUMs[0] - 1.0 / values.n * std::pow(values.sumX, 2));
    values.Sy2 = 1.0 / (values.n - 1.0) * (values.sumY2 - 1.0 / values.n * std::pow(values.sumY, 2));
    values.SxMean = std::sqrt(values.Sx2)/std::sqrt(values.n);
    values.SyMean = std::sqrt(values.Sy2)/std::sqrt(values.n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    for (int i = 0; i < values.n; ++i)
    {
        double x{ numericDates[i] };
        double y{ cursValues[i] };
        double y_pred{};
        (mode != 1) ? y_pred = values.coeffs["a0"] + values.coeffs["a1"] * x
                    : y_pred = values.coeffs["b0"] + values.coeffs["b1"] * x;
        predicts.append(y_pred);

        // Вычисление величин по предсказаниям.
        values.Sost += std::pow(y - y_pred, 2);
        values.Sregr += std::pow(y_pred - values.meanY, 2);

    }
    values.Spoln = values.Sost + values.Sregr;
    values.R2 = 1 - values.Sost/values.Spoln; // или Sregr/Spoln
    values.MSE = values.Sost / values.n;
}

// Подсчет всего остального (полиномиальная регрессия, параболическая).
void calculatePolynomRegression(const int &degree,
                                const QVector<double> &numericDates,
                                const QVector<double> &cursValues,
                                QVector<double> &predicts,
                                RegressionValues &values)
{
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    // Заполнение данных о коэффициентах.
    values.regCoefStr = "<b>Коэффициенты:</b><br>";
    // Заполнение итоговой функции.
    values.funcStr = "y = ";

    for (int i = 0; i <= degree; ++i)
    {
        QString key = QString("a%1").arg(i);
        double coef = values.coeffs.value(key, 0.0);

        values.regCoefStr += QString("a<sub>%1</sub> = %2<br>")
                                 .arg(i)
                                 .arg(QString::number(coef, 'f', 6));

        QString coefStr = QString::number(coef, 'f', 6);
        if (i == 0)
            values.funcStr += coefStr;
        else
        {
            if (coef >= 0)
                values.funcStr += " + ";
            else
                values.funcStr += " - ";
            values.funcStr += QString("%1 \u00B7 x").arg(std::abs(coef));
            if (i > 1) values.funcStr += toSuperscript(i);
        }
    }

    // --- Коэффициент Пирсона ---
    values.regConStr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                               "<i>r = NaN</i><br>"
                               "<b>Характер связи:</b> <i>-</i>.<br>"
                               "<b>Интерпретация связи:</b> <i>-</i>."
                               "<br><b>Теснота связи:</b> <i>-</i>.");

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values.Sx2 = 1.0 / (values.n - 1.0) * (values.SUMs[0] - 1.0 / values.n * std::pow(values.sumX, 2));
    values.Sy2 = 1.0 / (values.n - 1.0) * (values.sumY2 - 1.0 / values.n * std::pow(values.sumY, 2));
    values.SxMean = std::sqrt(values.Sx2)/std::sqrt(values.n);
    values.SyMean = std::sqrt(values.Sy2)/std::sqrt(values.n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values.Sost = 0.0;   // остаточная сумма
    values.Sregr = 0.0;  // регрессионная сумма
    values.Spoln = 0.0;  // полная сумма

    for (int i = 0; i < values.n; ++i)
    {
        double x{ numericDates[i] };
        double y{ cursValues[i] };
        double y_pred{0.0};

        for (int j = 0; j <= degree; ++j)
            y_pred += values.coeffs.value(QString("a%1").arg(j), 0.0) * std::pow(x, j);
        predicts.append(y_pred);

        values.Sost += std::pow(y - y_pred, 2);             // остаточная сумма
        values.Sregr += std::pow(y_pred - values.meanY, 2); // регрессионная сумма
    }

    values.Spoln = values.Sost + values.Sregr;
    values.R2 = (values.Spoln != 0.0) ? (1.0 - values.Sost / values.Spoln) : 0.0;
    values.MSE = values.Sost / values.n;
}

/*-----------------
 |     ПОМОЩЬ     |
 ----------------*/
// Перевод степени в верхний индекс.
QString toSuperscript(const int &number)
{
    QString result{};
    for (QChar ch : QString::number(number))
    {
        switch(ch.unicode())
        {
        case '0': result += QChar(0x2070); break;
        case '1': result += QChar(0x00B9); break;
        case '2': result += QChar(0x00B2); break;
        case '3': result += QChar(0x00B3); break;
        case '4': result += QChar(0x2074); break;
        case '5': result += QChar(0x2075); break;
        case '6': result += QChar(0x2076); break;
        case '7': result += QChar(0x2077); break;
        case '8': result += QChar(0x2078); break;
        case '9': result += QChar(0x2079); break;
        }
    }
    return result;
}
// Центрирование значений в таблицах.
void centerTableItems(QTableView *tableView)
{
    if (!tableView)
        return;

    QAbstractItemModel *model = tableView->model();
    if (!model)
        return;

    int rows = model->rowCount();
    int cols = model->columnCount();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QStandardItem *item = qobject_cast<QStandardItemModel*>(model)->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}
