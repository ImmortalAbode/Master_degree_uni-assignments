#include "formregression.h"
#include "ui_formregression.h"

#include "myfuncs.h"
#include "qcustomplot.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

FormRegression::FormRegression(const int &mode, const QTableView *tableView_TableData, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormRegression)
{
    ui->setupUi(this);
    this->showMaximized();
    connect(ui->pushButton_Back, &QPushButton::clicked, this, &FormRegression::on_pushButton_Back_clicked);

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
    // Вычисление значений коэффициентов, констант и вспомогательных величин в зависимости от типа регрессии + построение ураввнения регрессии.
    switch(mode){
    case 0:
        setWindowTitle("Линейная регрессия");
        calculateLinearRegression(this->numericDates, this->cursValues, this->predicts, this->values);
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
        QMessageBox::warning(nullptr, "Предупреждение", "Неизвестный тип регрессии, не удалось вычислить коэффициенты!");
        break;
    }

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

    // ---- ОТРИСОВКА ГРАФИКА ---- //
    FormRegression::makePlot();

    // ---- УСТАНОВКА ДАТЫ ---- //
    ui->dateEdit->setDate(QDate::currentDate().addDays(1)); // след. день, как минимум
}

void FormRegression::makePlot()
{
    // Проверка данных.
    if (this->dataColumn.isEmpty() || this->cursValues.isEmpty() || this->predicts.isEmpty())
        return;

    // Очистка холста.
    ui->QCustomPlot_graphic->clearGraphs();

    // Подготовка векторов.
    int n = this->dataColumn.size();
    QVector<double> x(n), y(n), y_T(n);
    QVector<QDateTime> dateTimes(n);

    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumn[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = dateTimes[i].toSecsSinceEpoch(); // координата X
        y[i] = cursValues[i];                   // исходные данные
        y_T[i] = predicts[i];                   // предсказание модели
    }

    // ----------------- График экспериментальных данных ----------------- //
    ui->QCustomPlot_graphic->addGraph();
    ui->QCustomPlot_graphic->graph(0)->setData(x, y);
    ui->QCustomPlot_graphic->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->QCustomPlot_graphic->graph(0)->setPen(QPen(Qt::transparent));
    ui->QCustomPlot_graphic->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(Qt::blue), 6));
    ui->QCustomPlot_graphic->graph(0)->setName("Экспериментальные данные");

    // ----------------- График модели ----------------- //
    ui->QCustomPlot_graphic->addGraph();
    ui->QCustomPlot_graphic->graph(1)->setData(x, y_T);
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

    QDateTime minDate = QDateTime::fromSecsSinceEpoch(*std::min_element(x.constBegin(), x.constEnd()));
    QDateTime maxDate = QDateTime::fromSecsSinceEpoch(*std::max_element(x.constBegin(), x.constEnd()));
    int days = minDate.daysTo(maxDate);
    int roundedDays = ((days + 5) / 10) * 10; // общее число показываемых дней
    int mainTicks = roundedDays / 10 + 1; // число основных делений

    dateTicker->setTickCount(mainTicks);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);

    ui->QCustomPlot_graphic->xAxis->setTicker(dateTicker);
    ui->QCustomPlot_graphic->xAxis->setTickLabelRotation(45);


    /* Диапазоны осей */
    // Ось X.
    ui->QCustomPlot_graphic->xAxis->setRange(
        minDate.toSecsSinceEpoch(),
        minDate.addDays(roundedDays).toSecsSinceEpoch()
        );

    // Ось Y.
    double minY = *std::min_element(y.constBegin(), y.constEnd());
    double maxY = *std::max_element(y.constBegin(), y.constEnd());
    ui->QCustomPlot_graphic->yAxis->setRange(minY - 0.5, maxY + 0.5);

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

    // ---------------- Padding от краев графика -----------------//
    ui->QCustomPlot_graphic->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // ----------------- Заголовок ----------------- //
    QString title = QString("Курс Евро с %1 по %2")
                        .arg(minDate.date().toString("dd.MM.yyyy"))
                        .arg(maxDate.date().toString("dd.MM.yyyy"));
    ui->QCustomPlot_graphic->plotLayout()->insertRow(0);
    QCPTextElement *titleElement = new QCPTextElement(ui->QCustomPlot_graphic, title, QFont("Arial", 12, QFont::Bold));
    titleElement->setMargins(QMargins(0, 10, 0, 0));
    ui->QCustomPlot_graphic->plotLayout()->addElement(0, 0, titleElement);

    // ----------------- Легенда ----------------- //
    QFont legendFont = ui->QCustomPlot_graphic->legend->font();
    legendFont.setPointSize(8);
    legendFont.setWeight(QFont::Normal);
    ui->QCustomPlot_graphic->legend->setFont(legendFont);
    ui->QCustomPlot_graphic->legend->setVisible(true);
    ui->QCustomPlot_graphic->legend->setBrush(QColor(255, 255, 255, 150));
    ui->QCustomPlot_graphic->legend->setBorderPen(QPen(QColor(150, 150, 150, 180)));
    ui->QCustomPlot_graphic->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);

    // Отрисовка
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
        return;
    }
    else
    {
        this->select_date = selectedDate;
        if (!this->forecastGraph)
        {
            // Создание графика прогноза только 1 раз.
            ui->QCustomPlot_graphic->addGraph();
            forecastGraph = ui->QCustomPlot_graphic->graph(2);
            forecastGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
            forecastGraph->setLineStyle(QCPGraph::lsNone);

            QCPSelectionDecorator *decor2 = new QCPSelectionDecorator;
            decor2->setPen(QPen(Qt::green, 2));
            forecastGraph->setSelectionDecorator(decor2);

            forecastGraph->setName("Прогноз");
        }
        // Преобразуем в QDateTime и в секунды указанную дату.
        QDateTime selectedDT(this->select_date, QTime(0,0));
        // Координата X для графика.
        double xForecast_graphic = selectedDT.toSecsSinceEpoch();

        // Координата X для расчетов.
        QDate epoch(1970, 1, 1);
        double xForecast_calc = epoch.daysTo(this->select_date)/10000.0;

        // Координата Y.
        double yForecast{};
        yForecast = this->values.coeffs[0] + this->values.coeffs[1] * xForecast_calc;

        // Обновляем холст.
        forecastGraph->setData({xForecast_graphic}, {yForecast});
        ui->QCustomPlot_graphic->replot();
        // Обновляем прогноз.
        ui->label_Forecast->setText("Прогноз курса Евро: " + QString::number(yForecast, 'g', 6));
    }
}

