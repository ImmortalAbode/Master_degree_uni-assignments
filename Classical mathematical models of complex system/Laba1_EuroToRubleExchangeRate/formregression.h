#ifndef FORMREGRESSION_H
#define FORMREGRESSION_H

#include <QWidget>
#include <QTableView>
#include <QDate>

#include "RegressionValues.h"
#include "qcustomplot.h"

namespace Ui {
class FormRegression;
}

class FormRegression : public QWidget
{
    Q_OBJECT

public:
    explicit FormRegression(const int &mode, const QTableView *data_tableView, QWidget *parent = nullptr);
    ~FormRegression();

private:
    Ui::FormRegression *ui;
    RegressionValues values{};

    QVector<QString> dataColumn{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};

    QVector<double> xSquared{};
    QVector<double> ySquared{};
    QVector<double> xyProduct{};

    QVector<double> predicts{};
    QDate select_date{};
    QCPGraph* forecastGraph = nullptr;

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void backToMain();

private slots:
    void on_pushButton_Back_clicked();
    void makePlot();
    void on_pushButton_Forecast_clicked();
};

#endif // FORMREGRESSION_H
