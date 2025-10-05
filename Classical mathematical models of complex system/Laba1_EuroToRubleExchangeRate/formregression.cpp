#include "formregression.h"
#include "ui_formregression.h"

#include "myfuncs.h"
#include "qcustomplot.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

FormRegression::FormRegression(const int &mode, const QVector<QString> &dataColumn, const QVector<double> &numericDates, const QVector<double> &cursValues,
                               QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormRegression)
    , mode(mode)
    , dataColumn(dataColumn)
    , numericDates(numericDates)
    , cursValues(cursValues)
{
    ui->setupUi(this);
    this->showMaximized();
    connect(ui->pushButton_Back, &QPushButton::clicked, this, &FormRegression::on_pushButton_Back_clicked);

    // Вычисление значений коэффициентов, констант и вспомогательных величин в зависимости от типа регрессии + построение ураввнения регрессии.
    switch(this->mode){
    case 0:
    {
        setWindowTitle("Линейная регрессия");

        // y = a0 + a1*x
        //
        // Система:
        // {a0*n + a1*E(xi) = E(yi)
        // {a0*E(xi) + a1*E(xi^2) = E(xi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(this->numericDates, this->cursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(false, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, this->numericDates, this->cursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->predicts, this->values);
        break;
    }
    case 1:
    {
        setWindowTitle("Обратная линейная регрессия");

        // y = a0 + a1*x => x = (y-a0) / a1 = - a0/a1 + 1/a1 * y = b0 + b1*y
        // где b0 = -a0/a1, b1 = 1/a1
        //
        // Система:
        // {b0*n + b1*E(xi) = E(yi)
        // {b0*E(xi) + b1*E(xi^2) = E(xi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений (входные данные уже перевернуты).
        calculatePolynomRegressionSums(this->cursValues, this->numericDates, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы (входные данные уже перевернуты).
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->cursValues, this->numericDates, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами (входные данные уже перевернуты).
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(true, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, this->cursValues, this->numericDates, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, this->cursValues, this->numericDates, this->predicts, this->values);
        break;
    }
    case 2:
    {
        setWindowTitle("Экспоненциальная регрессия");
        // y = a0e^(a1*x)
        // ln(y) = ln(a0) + a1*x
        //
        // Переобозначим: z = b0 + b1*x, где z = ln(y), b0 = ln(a0), b1 = a1.
        //
        // Система:
        // {b0*n + b1*E(xi) = E(zi)
        // {b0*E(xi) + b1*E(xi^2) = E(xi*zi)

        // ---- ПРЕОБРАЗОВАНИЯ К ЛИНЕЙНОЙ РЕГРЕССИИ ---- //
        QVector<double> lnCursValues(cursValues.size());
        for (int i = 0; i < cursValues.size(); ++i)
            lnCursValues[i] = std::log(cursValues[i]);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(this->numericDates, lnCursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(false, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, this->numericDates, lnCursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, this->numericDates, lnCursValues, this->predicts, this->values);
        break;
    }
    case 3:
    {
        setWindowTitle("Гиперболическая регрессия");
        // y = a0 + a1/x
        //
        // Переобозначим: y = a0 + a1*x, где z = 1/x
        //
        // Система:
        // {a0*n + b1*E(zi) = E(yi)
        // {a0*E(zi) + a1*E(zi^2) = E(zi*yi)

        // ---- ПРЕОБРАЗОВАНИЯ К ЛИНЕЙНОЙ РЕГРЕССИИ ---- //
        QVector<double> invNumericDates(numericDates.size());
        for (int i = 0; i < numericDates.size(); ++i)
            invNumericDates[i] = 1.0 / numericDates[i];

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(invNumericDates, this->cursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(false, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, invNumericDates, this->cursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, invNumericDates, this->cursValues, this->predicts, this->values);
        break;
    }
    case 4:
    {
        setWindowTitle("Параболическая регрессия");

        // y = a0 + a1*x + a2*x^2
        //
        // Система:
        // {a0*n + a1*E(xi) + a2*E(xi^2) = E(yi)
        // {a0*E(xi) + a1*E(xi^2) + a2*E(xi^3) = E(xi*yi)
        // {a0*E(xi^2) + a1*E(xi^3) + a2*E(xi^4) = E(xi^2*y)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 2;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(this->numericDates, this->cursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление коэффициентов.
        if (!buildAndSolvePolynomRegression(this->values, this->degree))
        {
            QMessageBox::critical(this, "Ошибка", QString("Не удалось построить параболическую регрессию: система уравнений вырождена или точек меньше 3."));
            return;
        }
        // Подсчет величин регрессии.
        calculatePolynomRegression(this->degree, this->numericDates, this->cursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->predicts, this->values);
        break;
    }
    case 5:
    {
        setWindowTitle("Логарифмическая регрессия");
        // y = a0 + a1*ln(x)
        //
        // Переобозначим: y = a0 + a1*z, где z = ln(x).
        //
        // Система:
        // {a0*n + a1*E(zi) = E(yi)
        // {a0*E(zi) + a1*E(zi^2) = E(zi*yi)

        // ---- ПРЕОБРАЗОВАНИЯ К ЛИНЕЙНОЙ РЕГРЕССИИ ---- //
        QVector<double> lnNumericDates(numericDates.size());
        for (int i = 0; i < cursValues.size(); ++i)
            lnNumericDates[i] = std::log(numericDates[i]);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(lnNumericDates, this->cursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(false, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, lnNumericDates, this->cursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, lnNumericDates, this->cursValues, this->predicts, this->values);
        break;
    }
    case 6:
    {
        setWindowTitle("Степенная регрессия");
        // y = a0x^a1
        // ln(y) = ln(a0) + a1*lnx
        //
        // Переобозначим: g = b0 + b1*z, где g = ln(y), b0 = ln(a0), b1 = a1, z = ln(x).
        //
        // Система:
        // {b0*n + b1*E(zi) = E(gi)
        // {b0*E(zi) + b1*E(zi^2) = E(zi*gi)

        // ---- ПРЕОБРАЗОВАНИЯ К ЛИНЕЙНОЙ РЕГРЕССИИ ---- //
        QVector<double> lnCursValues(cursValues.size());
        QVector<double> lnNumericDates(numericDates.size());
        for (int i = 0; i < cursValues.size(); ++i)
            lnCursValues[i] = std::log(cursValues[i]);
        for (int i = 0; i < numericDates.size(); ++i)
            lnNumericDates[i] = std::log(numericDates[i]);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(lnNumericDates, lnCursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера и для коэффициента Пирсона) и коэффициентов.
        getCoefsForLinearOrInverse(false, this->values);
        // Подсчет величин регрессии.
        calculateLinearOrInverseRegression(this->mode, lnNumericDates, lnCursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, lnNumericDates, lnCursValues, this->predicts, this->values);
        break;
    }
    case 7:
    {
        setWindowTitle("Полиномиальная регрессия");
        // Получение степени полинома.
        QInputDialog dlg(this);
        dlg.setInputMode(QInputDialog::IntInput);
        dlg.setWindowTitle("Степень полиномиальной регрессиии (3 - 20)");
        dlg.setLabelText("Укажите степень полинома (по умолчанию 6):");
        dlg.setIntRange(3, 20);
        dlg.setIntStep(1);
        dlg.setIntValue(6);
        dlg.resize(400, 200);
        int degree = dlg.exec() == QDialog::Accepted ? dlg.intValue() : 6;

        // y = a0 + a1*x + a2*x^2 + ... + an*x^n
        //
        // Система:
        // {a0*n       + a1*E(xi)       + a2*E(xi^2)     + ... + an*E(xi^n)     = E(yi)
        // {a0*E(xi)   + a1*E(xi^2)     + a2*E(xi^3)     + ... + an*E(xi*(n+1)) = E(xi*yi)
        // {a0*E(xi^2) + a1*E(xi^3)     + a2*E(xi^4)     + ... + an*E(xi*(n+2)  = E(xi^2*y)
        // {................................................................................
        // {a0*E(xi^n) + a1*E(xi^(n+1)) + a2*E(xi^(n+2)) + ... + an*E(xi*(n+n)) = E(xi^n*y)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = degree;
        // Вычисление значений квадратов, сумм и средних значений.
        calculatePolynomRegressionSums(this->numericDates, this->cursValues, this->degree, this->CalcColumns, this->ySquared, this->values);
        // Заполнение таблицы.
        fillTotalTable(ui->tableView_Calculations, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->CalcColumns, this->ySquared, this->degree);
        // Заполнение текстового поля с суммами.
        QString text_SUMs{ fillSUMsTextEdit(this->mode, this->values, this->degree) };
        ui->textEdit_Sums->setText(text_SUMs);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление коэффициентов.
        if (!buildAndSolvePolynomRegression(this->values, this->degree))
        {
            QMessageBox::critical(this, "Ошибка", QString("Не удалось построить полиномиальную регрессию: система уравнений вырождена или точек меньше степени полинома."));
            return;
        }
        // Подсчет величин регрессии.
        calculatePolynomRegression(this->degree, this->numericDates, this->cursValues, this->predicts, this->values);
        // Заполнение таблицы предсказаний модели.
        fillPredictTable(ui->tableView_CalcPredicts, this->mode, this->dataColumn, this->numericDates, this->cursValues, this->predicts, this->values);
        break;
    }
    default:
        setWindowTitle("Нерассматриваемый тип регрессии");
        QMessageBox::warning(nullptr, "Предупреждение", "Неизвестный тип регрессии, не удалось вычислить коэффициенты!");
        return;
    }

    // Отображение значений величин под таблицей.
    if (mode == 2)
    {
        ui->label_meanX->setText(("x\u0304 = ")  + QString::number(this->values.meanX, 'g', 6));
        ui->label_meanY->setText(("lny\u0304 = ")  + QString::number(this->values.meanY, 'g', 6));
    }
    else if (mode == 3)
    {
        ui->label_meanX->setText(("1/x\u0304 = ")  + QString::number(this->values.meanX, 'g', 6));
        ui->label_meanY->setText(("y\u0304 = ")  + QString::number(this->values.meanY, 'g', 6));;
    }
    else if (mode == 5)
    {
        ui->label_meanX->setText(("lnx\u0304 = ")  + QString::number(this->values.meanX, 'g', 6));
        ui->label_meanY->setText(("y\u0304 = ")  + QString::number(this->values.meanY, 'g', 6));;
    }
    else if (mode == 6)
    {
        ui->label_meanX->setText(("lnx\u0304 = ")  + QString::number(this->values.meanX, 'g', 6));
        ui->label_meanY->setText(("lny\u0304 = ")  + QString::number(this->values.meanY, 'g', 6));;
    }
    else
    {
        ui->label_meanX->setText(((mode != 1) ? "x\u0304 = " : "y\u0304 = ")  + QString::number(this->values.meanX, 'g', 6));
        ui->label_meanY->setText(((mode != 1) ? "y\u0304 = " : "x\u0304 = ")  + QString::number(this->values.meanY, 'g', 6));
    }

    // Отображение найденных значений под таблицей.
    ui->label_n->setText("n = " + QString::number(this->values.n));

    ui->label_Conclusion->setText(this->values.funcStr);
    ui->textEdit_RegCoef->setText(this->values.regCoefStr);
    ui->textEdit_RegCon->setText(this->values.regConStr);

    if (mode == 2)
    {
        ui->label_Sx2->setText(("(S<sub>x</sub>)<sup>2</sup> = ") + QString::number(this->values.Sx2, 'g', 6));
        ui->label_Sy2->setText(("(S<sub>lny</sub>)<sup>2</sup> = ") + QString::number(this->values.Sy2, 'g', 6));
        ui->label_SxMean->setText(("S<sub>x\u0304</sub> = ") + QString::number(this->values.SxMean, 'g', 6));
        ui->label_SyMean->setText(("S<sub>lny\u0304</sub> = ") + QString::number(this->values.SyMean, 'g', 6));
    }
    else if (mode == 3)
    {
        ui->label_Sx2->setText(("(S<sub>1/x</sub>)<sup>2</sup> = ") + QString::number(this->values.Sx2, 'g', 6));
        ui->label_Sy2->setText(("(S<sub>y</sub>)<sup>2</sup> = ") + QString::number(this->values.Sy2, 'g', 6));
        ui->label_SxMean->setText(("S<sub>1/x\u0304</sub> = ") + QString::number(this->values.SxMean, 'g', 6));
        ui->label_SyMean->setText(("S<sub>y\u0304</sub> = ") + QString::number(this->values.SyMean, 'g', 6));
    }
    else if (mode == 5)
    {
        ui->label_Sx2->setText(("(S<sub>lnx</sub>)<sup>2</sup> = ") + QString::number(this->values.Sx2, 'g', 6));
        ui->label_Sy2->setText(("(S<sub>y</sub>)<sup>2</sup> = ") + QString::number(this->values.Sy2, 'g', 6));
        ui->label_SxMean->setText(("S<sub>lnx\u0304</sub> = ") + QString::number(this->values.SxMean, 'g', 6));
        ui->label_SyMean->setText(("S<sub>y\u0304</sub> = ") + QString::number(this->values.SyMean, 'g', 6));
    }
    else if (mode == 6)
    {
        ui->label_Sx2->setText(("(S<sub>lnx</sub>)<sup>2</sup> = ") + QString::number(this->values.Sx2, 'g', 6));
        ui->label_Sy2->setText(("(S<sub>lny</sub>)<sup>2</sup> = ") + QString::number(this->values.Sy2, 'g', 6));
        ui->label_SxMean->setText(("S<sub>lnx\u0304</sub> = ") + QString::number(this->values.SxMean, 'g', 6));
        ui->label_SyMean->setText(("S<sub>lny\u0304</sub> = ") + QString::number(this->values.SyMean, 'g', 6));
    }
    else
    {
        ui->label_Sx2->setText(((mode != 1) ? "(S<sub>x</sub>)<sup>2</sup> = " : "(S<sub>y</sub>)<sup>2</sup> = ") + QString::number(this->values.Sx2, 'g', 6));
        ui->label_Sy2->setText(((mode != 1) ? "(S<sub>y</sub>)<sup>2</sup> = " : "(S<sub>x</sub>)<sup>2</sup> = ") + QString::number(this->values.Sy2, 'g', 6));
        ui->label_SxMean->setText(((mode != 1) ? "S<sub>x\u0304</sub> = " : "S<sub>y\u0304</sub> = ") + QString::number(this->values.SxMean, 'g', 6));
        ui->label_SyMean->setText(((mode != 1) ? "S<sub>y\u0304</sub> = "  : "S<sub>x\u0304</sub> = ") + QString::number(this->values.SyMean, 'g', 6));
    }

    ui->label_R2->setText("R<sup>2</sup> = " + QString::number(this->values.R2, 'g', 6));
    ui->label_Spoln->setText("S<sub>полн.</sub> = " + QString::number(this->values.Spoln, 'g', 6));
    ui->label_Sost->setText("S<sub>ост.</sub> = " + QString::number(this->values.Sost, 'g', 6));
    ui->label_Sregr->setText("S<sub>регр.</sub> = " + QString::number(this->values.Sregr, 'g', 6));
    ui->label_MSE->setText("MSE = " + QString::number(this->values.MSE, 'g', 6));
    ui->label_R2Concl->setText("<b>Возможность прогноза (R<sup>2</sup>>=75%):</b> " + QString("<i>%1</i>").arg(this->values.R2 * 100 >= 75 ? "имеется." : "отсутствует."));

    // ---- ОТРИСОВКА ГРАФИКА ---- //
    (mode != 1) ? FormRegression::makePlot() : FormRegression::makeInversePlot();

    // ---- УСТАНОВКА ДАТЫ ---- //
    QDate lastDate = QDate::fromString(this->dataColumn.first(), "dd.MM.yyyy");
    QDate newDate = lastDate.addDays(1);
    ui->dateEdit->setDate(newDate);
}

void FormRegression::makePlot()
{
    // Проверка данных.
    if (this->dataColumn.isEmpty() || this->cursValues.isEmpty() || this->predicts.isEmpty())
        return;

    // Очистка графиков холста.
    ui->QCustomPlot_graphic->clearGraphs();;

    // Подготовка векторов.
    int n = this->dataColumn.size();
    QVector<double> x(n), y(n), y_T(n);
    QVector<QDateTime> dateTimes(n);

    // Вычисление векторов для построения графика.
    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumn[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = dateTimes[i].toSecsSinceEpoch(); // координата X
        y[i] = this->cursValues[i];             // координата Y
        // предсказание модели
        if (mode == 2 || mode == 6)
            y_T[i] = std::exp(this->predicts[i]);
        else
            y_T[i] = this->predicts[i];
    }

    // Определение максимальной и минимальной даты.
    double minX = *std::min_element(x.constBegin(), x.constEnd());
    double maxX = *std::max_element(x.constBegin(), x.constEnd());
    QDateTime minDate = QDateTime::fromSecsSinceEpoch(minX);
    QDateTime maxDate = QDateTime::fromSecsSinceEpoch(maxX);

    // Если имеется точка прогноза.
    double xForecast_graphic{}, yForecast{};
    if (this->forecast_enabled)
    {
        // Преобразуем в QDateTime и в секунды указанную дату.
        QDateTime selectedDT(this->select_date, QTime(0,0));
        // Координата X для графика.
        xForecast_graphic = selectedDT.toSecsSinceEpoch();

        // Координата X для расчетов.
        QDate epoch(1970, 1, 1);
        double xForecast_calc = 0.01 + 0.99 * double(epoch.daysTo(this->select_date) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date()));

        // Координата Y.
        switch(this->mode)
        {
        case 0:
            yForecast = this->values.coeffs["a0"] + this->values.coeffs["a1"] * xForecast_calc;
            break;
        case 2:
            yForecast = std::exp(this->values.coeffs["a0"] + this->values.coeffs["a1"] * xForecast_calc);
            break;
        case 3:
            yForecast = this->values.coeffs["a0"] + this->values.coeffs["a1"] * 1.0 / xForecast_calc;
            break;
        case 4:
            for (int i = 0; i <= this->degree; ++i)
                yForecast += values.coeffs.value(QString("a%1").arg(i), 0.0) * std::pow(xForecast_calc, i);
            break;
        case 5:
            yForecast = this->values.coeffs["a0"] + this->values.coeffs["a1"] * std::log(xForecast_calc);
            break;
        case 6:
            yForecast = std::exp(this->values.coeffs["a0"] + this->values.coeffs["a1"] * std::log(xForecast_calc));
            break;
        case 7:
            for (int i = 0; i <= this->degree; ++i)
                yForecast += values.coeffs.value(QString("a%1").arg(i), 0.0) * std::pow(xForecast_calc, i);
            break;
        default:
            break;
        }
    }

    // ----------------- График экспериментальных данных ----------------- //
    ui->QCustomPlot_graphic->addGraph();
    ui->QCustomPlot_graphic->graph(0)->setData(x, y);
    ui->QCustomPlot_graphic->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->QCustomPlot_graphic->graph(0)->setPen(QPen(Qt::transparent));
    ui->QCustomPlot_graphic->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(Qt::blue), 6));
    ui->QCustomPlot_graphic->graph(0)->setName("Экспериментальные данные");

    // ----------------- График модели ----------------- //
    QVector<double> temp_x = x;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yT = y_T;

    ui->QCustomPlot_graphic->addGraph();
    if (this->forecast_enabled)
    {
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения - иначе просто прямой соединит до точки прогноза.
        while (lastDate < this->select_date)
        {
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);

            // Значение даты в секундах для оси абсцисс (по убыванию идут)
            temp_x.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут).
            temp_numericDates.prepend(0.01 + 0.99 * double(epoch.daysTo(lastDate) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())));

            // --- ВЫБОР МОДЕЛИ ---
            if (this->mode == 0) temp_yT.prepend(values.coeffs["a0"] + values.coeffs["a1"] * temp_numericDates[0]);
            if (this->mode == 2) temp_yT.prepend(std::exp(this->values.coeffs["a0"] + this->values.coeffs["a1"] * temp_numericDates[0]));
            if (this->mode == 3) temp_yT.prepend(this->values.coeffs["a0"] + this->values.coeffs["a1"] * 1.0 / temp_numericDates[0]);
            if (this->mode == 4 || this->mode == 7)
            {
                double temp_y{};
                for (int i = 0; i <= this->degree; ++i)
                    temp_y += values.coeffs.value(QString("a%1").arg(i), 0.0) * std::pow(temp_numericDates[0], i);
                temp_yT.prepend(temp_y);
            }
            if (this->mode == 5) temp_yT.prepend(this->values.coeffs["a0"] + this->values.coeffs["a1"] * std::log(temp_numericDates[0]));
            if (this->mode == 6) temp_yT.prepend(std::exp(this->values.coeffs["a0"] + this->values.coeffs["a1"] * std::log(temp_numericDates[0])));
        }
        temp_x.prepend(xForecast_graphic);
        temp_yT.prepend(yForecast);
        // Переопределение максимальной даты.
        maxDate = QDateTime(lastDate, QTime(0,0));
    }
    ui->QCustomPlot_graphic->graph(1)->setData(temp_x, temp_yT);
    ui->QCustomPlot_graphic->graph(1)->setPen(QPen(Qt::red, 2));

    // ----------------- Подпись коэф. детерминации у уравнению модели  ----------------- //
    QString trendLabel = this->values.funcStr + QString("\nR² = %1").arg(this->values.R2, 0, 'f', 4);
    ui->QCustomPlot_graphic->graph(1)->setName(trendLabel);

    // ----------------- Оси ----------------- //
    // Название.
    ui->QCustomPlot_graphic->xAxis->setLabel("Дата");
    ui->QCustomPlot_graphic->yAxis->setLabel("Курс Евро к Рублю");

    /* Стиль */
    // Стиль названий осей.
    QFont axisFont;
    axisFont.setBold(false);
    axisFont.setPointSize(10);
    ui->QCustomPlot_graphic->xAxis->setLabelFont(axisFont);
    ui->QCustomPlot_graphic->yAxis->setLabelFont(axisFont);

    // Стиль подписей осей.
    QFont tickFont;
    tickFont.setItalic(true);
    tickFont.setPointSize(8);
    ui->QCustomPlot_graphic->xAxis->setTickLabelFont(tickFont);
    ui->QCustomPlot_graphic->yAxis->setTickLabelFont(tickFont);

    /* Шаги делений */
    // Ось Y.
    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setTickStep(1.0);
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->QCustomPlot_graphic->yAxis->setTicker(yTicker);
    ui->QCustomPlot_graphic->yAxis->setSubTicks(true);
    connect(ui->QCustomPlot_graphic->yAxis,
            static_cast<void (QCPAxis::*)(const QCPRange &)>(&QCPAxis::rangeChanged),
            this,
            [=](const QCPRange &newRange)
            {
                // Текущий масштаб.
                double range = newRange.size();

                // * Масштабирование шага * //
                // Примерный шаг делений оси.
                double roughStep = range / 20.0;
                // Представим: roughStep = fraction * 10^exponent ([что-то между 1 и 10] * 10^n).
                double exponent = std::floor(std::log10(roughStep));
                double fraction = roughStep / std::pow(10, exponent);
                double niceFraction{};

                // Красивый Fraction.
                if (fraction <= 1.0) niceFraction = 1.0;
                else if (fraction <= 2.0) niceFraction = 2.0;
                else if (fraction <= 5.0) niceFraction = 5.0;
                else niceFraction = 10.0;

                // Полученный шаг.
                double step = niceFraction * std::pow(10, exponent);

                // Установка шага.
                yTicker->setTickStep(step);
                ui->QCustomPlot_graphic->yAxis->setTicker(yTicker);
            });

    // Ось X.
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy");
    ui->QCustomPlot_graphic->xAxis->setSubTicks(true);
    dateTicker->setTickCount(10);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    ui->QCustomPlot_graphic->xAxis->setTicker(dateTicker);
    ui->QCustomPlot_graphic->xAxis->setTickLabelRotation(30);

    /* Диапазоны осей */
    ui->QCustomPlot_graphic->rescaleAxes(); // Автоматическое изменение диапазона осей
    auto xRange = ui->QCustomPlot_graphic->xAxis->range();
    auto yRange = ui->QCustomPlot_graphic->yAxis->range();

    double xPadding = (temp_x.first() - x.first()) / 10.0 + 86400; // 1 день отступ
    double yPadding = (temp_yT.first() - y.first()) / 10.0 + 0.5;   // 0.5 отступ

    ui->QCustomPlot_graphic->xAxis->setRange(xRange.lower - xPadding, xRange.upper + xPadding);
    ui->QCustomPlot_graphic->yAxis->setRange(yRange.lower - yPadding, yRange.upper + yPadding);

    // ----------------- Масштабирование и drag ----------------- //
    ui->QCustomPlot_graphic->setInteraction(QCP::iRangeDrag, true);         // перетаскивание мышью
    ui->QCustomPlot_graphic->setInteraction(QCP::iRangeZoom, true);         // масштаб графика
    ui->QCustomPlot_graphic->setInteraction(QCP::iSelectPlottables, true);  // подсветка выбранного графика

    // ----------------- Подсветка графика ------------------- //
    // Для графика 0.
    QCPSelectionDecorator *decor0 = new QCPSelectionDecorator;
    decor0->setPen(QPen(Qt::green, 2));
    ui->QCustomPlot_graphic->graph(0)->setSelectionDecorator(decor0);

    // Для графика 1.
    QCPSelectionDecorator *decor1 = new QCPSelectionDecorator;
    decor1->setPen(QPen(Qt::green, 2));
    ui->QCustomPlot_graphic->graph(1)->setSelectionDecorator(decor1);

    // ----------------- График прогноза ------------------- //
    if (this->forecast_enabled)
    {
        // Создание графика прогноза.
        ui->QCustomPlot_graphic->addGraph();
        ui->QCustomPlot_graphic->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
        ui->QCustomPlot_graphic->graph(2)->setLineStyle(QCPGraph::lsNone);

        QCPSelectionDecorator *decor2 = new QCPSelectionDecorator;
        decor2->setPen(QPen(Qt::green, 2));
        ui->QCustomPlot_graphic->graph(2)->setSelectionDecorator(decor2);

        ui->QCustomPlot_graphic->graph(2)->setName("Прогноз");

        ui->QCustomPlot_graphic->graph(2)->setData({xForecast_graphic}, {yForecast});
        ui->label_Forecast->setText("Прогноз курса Евро: " + QString::number(yForecast, 'g', 6) + " руб.");
    }

    // ---------------- Padding от краев графика -----------------//
    ui->QCustomPlot_graphic->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // ----------------- Заголовок ----------------- //
    QString title = QString("Курс Евро с %1 по %2")
                        .arg(minDate.date().toString("dd.MM.yyyy"))
                        .arg(maxDate.date().toString("dd.MM.yyyy"));
    // Паддинг от краев графика.
    ui->QCustomPlot_graphic->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // Проверка на существование заголовка в окне. Если нет - добавление, иначе - замена текста.
    if (!ui->QCustomPlot_graphic->plotLayout()->elementCount() ||
        !qobject_cast<QCPTextElement*>(ui->QCustomPlot_graphic->plotLayout()->elementAt(0)))
    {
        // Установка названия графика.
        ui->QCustomPlot_graphic->plotLayout()->insertRow(0);
        QCPTextElement *titleElement = new QCPTextElement(ui->QCustomPlot_graphic, title, QFont("Arial", 12, QFont::Bold));
        titleElement->setMargins(QMargins(0, 10, 0, 0));
        ui->QCustomPlot_graphic->plotLayout()->addElement(0, 0, titleElement);
    }
    else
    {
        auto header = qobject_cast<QCPTextElement*>(ui->QCustomPlot_graphic->plotLayout()->elementAt(0));
        header->setText(title);
    }

    // ----------------- Легенда ----------------- //
    QFont legendFont = ui->QCustomPlot_graphic->legend->font();
    legendFont.setPointSize(8);
    legendFont.setWeight(QFont::Normal);
    ui->QCustomPlot_graphic->legend->setFont(legendFont);
    if (this->degree > 6) ui->QCustomPlot_graphic->legend->setVisible(false);
    else ui->QCustomPlot_graphic->legend->setVisible(true);
    ui->QCustomPlot_graphic->legend->setBrush(QColor(255, 255, 255, 150));
    ui->QCustomPlot_graphic->legend->setBorderPen(QPen(QColor(150, 150, 150, 180)));
    ui->QCustomPlot_graphic->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

    // Отрисовка.
    ui->QCustomPlot_graphic->replot();
}


void FormRegression::makeInversePlot()
{
    // Проверка данных.
    if (this->dataColumn.isEmpty() || this->cursValues.isEmpty() || this->predicts.isEmpty())
        return;

    // Очистка графиков холста.
    ui->QCustomPlot_graphic->clearGraphs();;

    // Подготовка векторов.
    int n = this->dataColumn.size();
    QVector<double> x(n), y(n), yReg(n);
    QVector<QDateTime> dateTimes(n);

    // Вычисление векторов для построения графика.
    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumn[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = this->cursValues[i];                 // координата X (курс)
        y[i] = dateTimes[i].toSecsSinceEpoch();     // координата Y (дата)

    }

    // Определение максимальной и минимальной даты.
    double minY = *std::min_element(y.constBegin(), y.constEnd());
    double maxY = *std::max_element(y.constBegin(), y.constEnd());
    QDateTime minDate = QDateTime::fromSecsSinceEpoch(minY);
    QDateTime maxDate = QDateTime::fromSecsSinceEpoch(maxY);

    // значения регрессии
    for (int i = 0; i < n; ++i)
    {
        QDate epoch(1970, 1, 1);
        yReg[i] = ((this->predicts[i] - 0.01) / 0.99 * double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())) + epoch.daysTo(minDate.date()))*24*60*60;
    }

    // Если имеется точка прогноза.
    double yForecast_graphic{}, xForecast{};
    if (this->forecast_enabled)
    {
        // Преобразуем в QDateTime и в секунды указанную дату.
        QDateTime selectedDT(this->select_date, QTime(0,0));
        // Координата X для графика.
        yForecast_graphic = selectedDT.toSecsSinceEpoch();

        // Координата X для расчетов.
        QDate epoch(1970, 1, 1);
        double yForecast_calc = 0.01 + 0.99 * double(epoch.daysTo(this->select_date) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date()));

        // --- Координата Y ---
        // Решение уравнения [x = b0 + b1 * y] будет относительно неизвестной y (курса Евро), т.к. мы хотим предсказывать курс по дате, а не наоборот.
        // В этой функции полагается регрессия вида:
        // x = b0 + b1 * y, где y - курс Евро, x - предсказанная дата, уравнение которой решается относительно y.
        // Иными словами, уравнение решается не относительно x (как положено по обратной регрессии), а относительно y,
        // т.е. мы предсказываем не дату по курсу Евро, а как будто бы по уже предсказанной дате моделируем курс Евро:
        // y = (x - b0) / b1.
        xForecast = (yForecast_calc - this->values.coeffs["b0"])/this->values.coeffs["b1"];
    }

    // ----------------- График экспериментальных данных ----------------- //
    ui->QCustomPlot_graphic->addGraph();
    ui->QCustomPlot_graphic->graph(0)->setData(x, y);
    ui->QCustomPlot_graphic->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->QCustomPlot_graphic->graph(0)->setPen(QPen(Qt::transparent));
    ui->QCustomPlot_graphic->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(Qt::blue), 6));
    ui->QCustomPlot_graphic->graph(0)->setName("Экспериментальные данные");

    // ----------------- График модели ----------------- //
    QVector<double> temp_x = x;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = yReg;

    ui->QCustomPlot_graphic->addGraph();
    if (this->forecast_enabled)
    {
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения - иначе просто прямой соединит до точки прогноза.
        while (lastDate < this->select_date)
        {
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);

            // Значение даты в секундах для оси ординат (по убыванию идут)
            temp_yReg.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут).
            temp_numericDates.prepend(0.01 + 0.99 * double(epoch.daysTo(lastDate) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())));

            temp_x.prepend((temp_numericDates[0] - this->values.coeffs["b0"])/this->values.coeffs["b1"]);
        }
        temp_x.prepend(xForecast);
        temp_yReg.prepend(yForecast_graphic);
        // Переопределение максимальной даты.
        maxDate = QDateTime(lastDate, QTime(0,0));
    }
    ui->QCustomPlot_graphic->graph(1)->setData(temp_x, temp_yReg);
    ui->QCustomPlot_graphic->graph(1)->setPen(QPen(Qt::red, 2));

    // ----------------- Подпись коэф. детерминации у уравнению модели  ----------------- //
    QString trendLabel = this->values.funcStr + QString("\nR² = %1").arg(this->values.R2, 0, 'f', 4);
    ui->QCustomPlot_graphic->graph(1)->setName(trendLabel);

    // ----------------- Оси ----------------- //
    // Название.
    ui->QCustomPlot_graphic->xAxis->setLabel("Курс Евро к Рублю");
    ui->QCustomPlot_graphic->yAxis->setLabel("Дата");

    /* Стиль */
    // Стиль названий осей.
    QFont axisFont;
    axisFont.setBold(false);
    axisFont.setPointSize(10);
    ui->QCustomPlot_graphic->xAxis->setLabelFont(axisFont);
    ui->QCustomPlot_graphic->yAxis->setLabelFont(axisFont);

    // Стиль подписей осей.
    QFont tickFont;
    tickFont.setItalic(true);
    tickFont.setPointSize(8);
    ui->QCustomPlot_graphic->xAxis->setTickLabelFont(tickFont);
    ui->QCustomPlot_graphic->yAxis->setTickLabelFont(tickFont);

    /* Шаги делений */
    // Ось X.
    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
    xTicker->setTickStep(1.0);
    xTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->QCustomPlot_graphic->xAxis->setTicker(xTicker);
    ui->QCustomPlot_graphic->xAxis->setSubTicks(true);
    connect(ui->QCustomPlot_graphic->xAxis,
            static_cast<void (QCPAxis::*)(const QCPRange &)>(&QCPAxis::rangeChanged),
            this,
            [=](const QCPRange &newRange)
            {
                // Текущий масштаб.
                double range = newRange.size();

                // * Масштабирование шага * //
                // Примерный шаг делений оси.
                double roughStep = range / 20.0;
                // Представим: roughStep = fraction * 10^exponent ([что-то между 1 и 10] * 10^n).
                double exponent = std::floor(std::log10(roughStep));
                double fraction = roughStep / std::pow(10, exponent);
                double niceFraction{};

                // Красивый Fraction.
                if (fraction <= 1.0) niceFraction = 1.0;
                else if (fraction <= 2.0) niceFraction = 2.0;
                else if (fraction <= 5.0) niceFraction = 5.0;
                else niceFraction = 10.0;

                // Полученный шаг.
                double step = niceFraction * std::pow(10, exponent);

                // Установка шага.
                xTicker->setTickStep(step);
                ui->QCustomPlot_graphic->xAxis->setTicker(xTicker);
            });

    // Ось Y.
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy");
    ui->QCustomPlot_graphic->yAxis->setSubTicks(true);
    dateTicker->setTickCount(10);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    ui->QCustomPlot_graphic->yAxis->setTicker(dateTicker);
    ui->QCustomPlot_graphic->yAxis->setTickLabelRotation(30);

    /* Диапазоны осей */
    ui->QCustomPlot_graphic->rescaleAxes(); // Автоматическое изменение диапазона осей
    auto xRange = ui->QCustomPlot_graphic->xAxis->range();
    auto yRange = ui->QCustomPlot_graphic->yAxis->range();

    double yPadding = (temp_yReg.first() - y.first()) / 10.0 + 86400; // 1 день отступ
    double xPadding = (temp_x.first() - x.first()) / 10.0 + 0.5;   // 0.5 отступ

    if (this->forecast_enabled) xPadding += std::abs(xForecast - x.last()) / 10.0;
    if (this->forecast_enabled) yPadding += (yForecast_graphic - yReg.last()) / 10.0;

    ui->QCustomPlot_graphic->yAxis->setRange(yRange.lower - yPadding, yRange.upper + yPadding);
    ui->QCustomPlot_graphic->xAxis->setRange(xRange.lower - xPadding, xRange.upper + xPadding);

    // ----------------- Масштабирование и drag ----------------- //
    ui->QCustomPlot_graphic->setInteraction(QCP::iRangeDrag, true);         // перетаскивание мышью
    ui->QCustomPlot_graphic->setInteraction(QCP::iRangeZoom, true);         // масштаб графика
    ui->QCustomPlot_graphic->setInteraction(QCP::iSelectPlottables, true);  // подсветка выбранного графика

    // ----------------- Подсветка графика ------------------- //
    // Для графика 0.
    QCPSelectionDecorator *decor0 = new QCPSelectionDecorator;
    decor0->setPen(QPen(Qt::green, 2));
    ui->QCustomPlot_graphic->graph(0)->setSelectionDecorator(decor0);

    // Для графика 1.
    QCPSelectionDecorator *decor1 = new QCPSelectionDecorator;
    decor1->setPen(QPen(Qt::green, 2));
    ui->QCustomPlot_graphic->graph(1)->setSelectionDecorator(decor1);

    // ----------------- График прогноза ------------------- //
    if (this->forecast_enabled)
    {
        // Создание графика прогноза.
        ui->QCustomPlot_graphic->addGraph();
        ui->QCustomPlot_graphic->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
        ui->QCustomPlot_graphic->graph(2)->setLineStyle(QCPGraph::lsNone);

        QCPSelectionDecorator *decor2 = new QCPSelectionDecorator;
        decor2->setPen(QPen(Qt::green, 2));
        ui->QCustomPlot_graphic->graph(2)->setSelectionDecorator(decor2);

        ui->QCustomPlot_graphic->graph(2)->setName("Прогноз");

        ui->QCustomPlot_graphic->graph(2)->setData({xForecast}, {yForecast_graphic});
        ui->label_Forecast->setText("Прогноз курса Евро: " + QString::number(xForecast, 'g', 6) + " руб.");
    }

    // ---------------- Padding от краев графика -----------------//
    ui->QCustomPlot_graphic->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // ----------------- Заголовок ----------------- //
    QString title = QString("Курс Евро с %1 по %2")
                        .arg(minDate.date().toString("dd.MM.yyyy"))
                        .arg(maxDate.date().toString("dd.MM.yyyy"));
    // Паддинг от краев графика.
    ui->QCustomPlot_graphic->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // Проверка на существование заголовка в окне. Если нет - добавление, иначе - замена текста.
    if (!ui->QCustomPlot_graphic->plotLayout()->elementCount() ||
        !qobject_cast<QCPTextElement*>(ui->QCustomPlot_graphic->plotLayout()->elementAt(0)))
    {
        // Установка названия графика.
        ui->QCustomPlot_graphic->plotLayout()->insertRow(0);
        QCPTextElement *titleElement = new QCPTextElement(ui->QCustomPlot_graphic, title, QFont("Arial", 12, QFont::Bold));
        titleElement->setMargins(QMargins(0, 10, 0, 0));
        ui->QCustomPlot_graphic->plotLayout()->addElement(0, 0, titleElement);
    }
    else
    {
        auto header = qobject_cast<QCPTextElement*>(ui->QCustomPlot_graphic->plotLayout()->elementAt(0));
        header->setText(title);
    }

    // ----------------- Легенда ----------------- //
    QFont legendFont = ui->QCustomPlot_graphic->legend->font();
    legendFont.setPointSize(8);
    legendFont.setWeight(QFont::Normal);
    ui->QCustomPlot_graphic->legend->setFont(legendFont);
    ui->QCustomPlot_graphic->legend->setVisible(true);
    ui->QCustomPlot_graphic->legend->setBrush(QColor(255, 255, 255, 150));
    ui->QCustomPlot_graphic->legend->setBorderPen(QPen(QColor(150, 150, 150, 180)));
    ui->QCustomPlot_graphic->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

    // Отрисовка.
    ui->QCustomPlot_graphic->replot();
}

FormRegression::~FormRegression()
{
    delete ui;
}

void FormRegression::on_pushButton_Back_clicked()
{
    emit backToMain();
    this->close();
}

void FormRegression::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit backToMain();
}

void FormRegression::on_pushButton_Forecast_clicked()
{
    // Введенная пользователем дата.
    QDate selectedDate = ui->dateEdit->date();

    // Максимальная дата в исходных данных.
    QDate lastDate = QDate();
    for (int i = 0; i < dataColumn.size(); ++i)
    {
        QDate d = QDate::fromString(dataColumn[i], "dd.MM.yyyy");
        if (d > lastDate) lastDate = d;
    }
    // Нужна дата позже максимальной даты в исходных данных.
    if (selectedDate <= lastDate)
    {
        QMessageBox::critical(this, "Ошибка", QString("Дата должна быть позже %1, так как интерес представляет будущее.").arg(lastDate.toString("dd.MM.yyyy")));
        this->forecast_enabled = false;
        (mode != 1) ? FormRegression::makePlot() : FormRegression::makeInversePlot();
        return;
    }

    this->select_date = selectedDate;
    this->forecast_enabled = true;
    (mode != 1) ? FormRegression::makePlot() : FormRegression::makeInversePlot();
}

