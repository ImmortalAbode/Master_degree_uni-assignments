#ifndef MYFUNCS_H
#define MYFUNCS_H

#include "QString"
#include "QTableView"
#include "QVector"
#include "RegressionValues.h"

/*------------------
 |  ПРЕДОБРАБОТКА  |
 -----------------*/
// Чтение исходных данных из таблицы и приведение к нужному виду.
bool readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues);

/*-----------------
 |  ГЛАВНОЕ ОКНО  |
 ----------------*/
// Загрузка исходных данных из Excel в таблицу.
bool loadDataFromExcel(const QString &filePath, QTableView *tableView);

/*-----------------
 |     БЛОК 1     |
 ----------------*/
// ---- Функции заполнения ---- //
//Заполнение таблицы значениями.
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<QVector<double>> &CalcColumns,
                    const QVector<double> &ySquared,
                    const int &degree);
// Заполнение текстового поля с суммами.
QString fillSUMsTextEdit(const int &mode, const RegressionValues &values, const int &degree);

// ---- Расчетные функции ---- //
// Для полиномиальных видов регрессий (расчет суммы квадратов, кубов и т.д. и произведений величин).
void calculatePolynomRegressionSums(const QVector<double> &numericDates,
                                      const QVector<double> &cursValues,
                                      const int &degree,
                                      QVector<QVector<double>> &CalcColumns,
                                      QVector<double> &ySquared,
                                      RegressionValues &values);
// Метод Гаусса.
bool gaussSolve(QVector<QVector<double>> A, QVector<double> b);
// Для полиномиальных видов регрессий (потсроение матриц A и B для метода Гаусса).
bool buildAndSolvePolynomRegression(RegressionValues &values, const int &degree);

/*-----------------
 |     БЛОК 2     |
 ----------------*/
// ---- Функции заполнения  ---- //
// Заполнение таблицы значениями.
void fillPredictTable(QTableView *tableView,
                      const int &mode,
                      const QVector<QString> &dataColumn,
                      const QVector<double> &numericDates,
                      const QVector<double> &cursValues,
                      const QVector<double> &predicts,
                      const RegressionValues &values);

// ---- Расчетные функции ---- //
// -- Обшие --
// Получение выводов о коэффициенте Пирсона (коэф. корреляции о линейности связи между данными).
void getRegConnDesc(RegressionValues &values);
// Вспомогательные величины и коэффициенты для линейных видов регрессии.
void getCoefsForLinearOrInverse(const bool &inverse, RegressionValues &values);

// -- Линейная и обратная линейная регрессия, а также сводящиеся к ним --
// Сотавление вида функции и описания коэффициентов.
void getFuncCoefDescr(const int& mode, RegressionValues &values);
// Подсчет всего остального.
void calculateLinearOrInverseRegression(const int& mode,
                                        const QVector<double> &numericDates,
                                        const QVector<double> &cursValues,
                                        QVector<double> &predicts,
                                        RegressionValues &values);
// -- Полиномиальные регрессии --
// Полином - подсчет всего остального.
void calculatePolynomRegression(const int &degree,
                                const QVector<double> &numericDates,
                                const QVector<double> &cursValues,
                                QVector<double> &predicts,
                                RegressionValues &values);


/*-----------------
 |     ПОМОЩЬ     |
 ----------------*/
// Перевод степени в верхний индекс.
QString toSuperscript(const int &number);
// Центрирование значений в таблицах.
void centerTableItems(QTableView *tableView);


#endif // MYFUNCS_H
