#ifndef FORMREGRESSION_H
#define FORMREGRESSION_H

#include <QWidget>
#include <QTableView>
#include <QDate>

#include "RegressionValues.h"

namespace Ui {
class FormRegression;
}

class FormRegression : public QWidget
{
    Q_OBJECT

public:
    explicit FormRegression(const int &mode, const QVector<QString> &dataColumn, const QVector<double> &numericDates, const QVector<double> &cursValues,
                            QWidget *parent = nullptr);
    ~FormRegression();

private:
    // окно
    Ui::FormRegression *ui;
    // тип регрессии
    int mode{};
    int degree{};

    // различные величины регрессии
    RegressionValues values{};
    // матрица расчетных значений таблицы
    QVector<QVector<double>> CalcColumns{};
    // величина y^2 для линейной регрессии (для обратной тут хранится x^2)
    QVector<double> ySquared{};

    // исходные данные
    QVector<QString> dataColumn{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};

    // предсказания модели
    QVector<double> predicts{};

    // выбранная дата
    QDate select_date{};

    // флаг прогноза модели
    bool forecast_enabled{false};

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void backToMain();

private slots:
    // отрисовка графика
    void makePlot();
    void makeInversePlot();

    // кнопки
    void on_pushButton_Back_clicked();
    void on_pushButton_Forecast_clicked();
};

#endif // FORMREGRESSION_H
