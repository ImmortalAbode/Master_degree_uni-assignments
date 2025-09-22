#ifndef FORMREGRESSION_H
#define FORMREGRESSION_H

#include <QWidget>
#include <QTableView>

#include "RegressionValues.h"

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

    QVector<QString> dataColumn{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};

    QVector<double> xSquared{};
    QVector<double> ySquared{};
    QVector<double> xyProduct{};
    RegressionValues values{};

    QVector<double> predicts{};

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void backToMain();

private slots:
    void on_pushButton_Back_clicked();
};

#endif // FORMREGRESSION_H
