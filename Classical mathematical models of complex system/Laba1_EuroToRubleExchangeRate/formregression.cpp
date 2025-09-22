#include "formregression.h"
#include "ui_formregression.h"

#include "myfuncs.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

FormRegression::FormRegression(const int &mode, const QTableView *tableView_TableData, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormRegression)
{
    ui->setupUi(this);
    this->showMaximized();
    connect(ui->pushButton_Back, &QPushButton::clicked, this, &FormRegression::on_pushButton_Back_clicked);
    switch(mode){
    case 0:
        setWindowTitle("Линейная регрессия");
        break;
    case 1:
        setWindowTitle("Обратная линейная регрессия");
        break;
    case 2:
        setWindowTitle("Экспоненциальная регрессия");
        break;
    case 3:
        setWindowTitle("Гиперболическая регрессия");
        break;
    case 4:
        setWindowTitle("Параболическая регрессия");
        break;
    case 5:
        setWindowTitle("Логарифмическая регрессия");
        break;
    case 6:
        setWindowTitle("Степенная регрессия");
        break;
    case 7:
        setWindowTitle("Полиномиальная регрессия");
        break;
    default:
        setWindowTitle("Нерассматриваемый тип регрессии");
        break;
    }
    // ---- ПРЕДОБРАБОТКА ---- //
    // Чтение исходных данных и преобразование к нужному виду.
    readDataAndCurs(tableView_TableData, this->dataColumn, this->numericDates, this->cursValues);

    // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
    // Вычисление значений квадратов, сумм и средних значений.
    calculateRegressionValues(this->numericDates, this->cursValues, this->xSquared, this->ySquared, this->xyProduct, this->values);
    // Заполнение таблицы.
    fillTotalTable(ui->tableView_Calculations, this->dataColumn, this->numericDates, this->cursValues,
                    this->xSquared, this->ySquared, this->xyProduct);
    // Отображение значений величин под таблицей.
    ui->label_sumX->setText(ui->label_sumX->text() + QString::number(this->values.sumX, 'f', 6));
    ui->label_sumY->setText(ui->label_sumY->text() + QString::number(this->values.sumY, 'f', 6));
    ui->label_sumX2->setText(ui->label_sumX2->text() + QString::number(this->values.sumX2, 'f', 6));
    ui->label_sumY2->setText(ui->label_sumY2->text() + QString::number(this->values.sumY2, 'f', 6));
    ui->label_sumXY->setText(ui->label_sumXY->text() + QString::number(this->values.sumXY, 'f', 6));
    ui->label_meanX->setText(ui->label_meanX->text() + QString::number(this->values.meanX, 'f', 6));
    ui->label_meanY->setText(ui->label_meanY->text() + QString::number(this->values.meanY, 'f', 6));

    // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
    // Вычисление значений коэффициентов и вспомогательных величин в зависимости от типа регрессии (общая функция).
    fillLabelsAuxiliaryQuantities(mode, this->numericDates, this->cursValues, this->predicts, this->values);
    // Заполнение таблицы предсказаний модели.
    fillPredictTable(ui->tableView_CalcPredicts, this->dataColumn, this->numericDates, this->cursValues, this->predicts, this->values);
    // Отображение найденных значений под таблицей.
    ui->label_n->setText(ui->label_n->text() + QString::number(this->values.n));
    ui->label_Conclusion->setText(this->values.funcStr);
    ui->textEdit_RegCoef->setText(this->values.regCoefStr);
    ui->textEdit_RegCon->setText(this->values.regConStr);
    ui->label_Sx2->setText(ui->label_Sx2->text() + QString::number(this->values.Sx2, 'f', 12));
    ui->label_Sy2->setText(ui->label_Sy2->text() + QString::number(this->values.Sy2, 'f', 6));
    ui->label_SxMean->setText(ui->label_SxMean->text() + QString::number(this->values.SxMean, 'f', 12));
    ui->label_SyMean->setText(ui->label_SyMean->text() + QString::number(this->values.SyMean, 'f', 6));
    ui->label_R2->setText(ui->label_R2->text() + QString::number(this->values.R2, 'f', 6));
    ui->label_Spoln->setText(ui->label_Spoln->text() + QString::number(this->values.Spoln, 'f', 6));
    ui->label_Sost->setText(ui->label_Sost->text() + QString::number(this->values.Sost, 'f', 6));
    ui->label_Sregr->setText(ui->label_Sregr->text() + QString::number(this->values.Sregr, 'f', 6));
    ui->label_MSE->setText(ui->label_MSE->text() + QString::number(this->values.MSE, 'f', 6));
    ui->label_R2Concl->setText(ui->label_R2Concl->text() + (this->values.R2 * 100 >= 75 ? "имеется." : "не рекомендуется."));

    // Отрисовка графика.
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

