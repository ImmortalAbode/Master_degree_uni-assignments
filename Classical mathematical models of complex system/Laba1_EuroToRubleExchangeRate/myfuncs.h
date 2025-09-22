#ifndef MYFUNCS_H
#define MYFUNCS_H

#include "QString"
#include "QTableView"
#include "QVector"
#include "RegressionValues.h"

/*-----------------
 |     БЛОК 1     |
 ----------------*/
// ---- Функции таблицы ---- //
bool loadDataFromExcel(const QString &filePath, QTableView *tableView);
void readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues);
void fillTotalTable(QTableView *tableView,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<double> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct);

// ---- Расчетные функции ---- //
void calculateRegressionValues(const QVector<double> numericDates, const QVector<double> &cursValues,
                               QVector<double> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                               RegressionValues &values);


/*-----------------
 |     БЛОК 2     |
 ----------------*/
// ---- Функции таблицы  ---- //
void fillPredictTable(QTableView *tableView,
                      const QVector<QString> &dataColumn,
                      const QVector<double> &numericDates,
                      const QVector<double> &cursValues,
                      const QVector<double> &predicts,
                      const RegressionValues &values);

// ---- Расчетные функции ---- //
void getRegConnDesc(RegressionValues &values);
void fillLabelsAuxiliaryQuantities(const int &mode,
                                   const QVector<double> &numericDates,
                                   const QVector<double> &cursValues,
                                   QVector<double> &predicts,
                                   RegressionValues &values);


#endif // MYFUNCS_H
