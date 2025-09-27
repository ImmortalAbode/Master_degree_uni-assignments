#ifndef REGRESSIONVALUES_H
#define REGRESSIONVALUES_H

#include <QString>
#include <QVector>
#include <QHash>

struct RegressionValues
{
    int n = 0;
    // Расчеты по таблице.
    QVector <double> SUMs{};
    double sumX = 0.0;
    double sumY = 0.0;
    double sumY2 = 0.0; // для обратной линейной регрессии тут хранится E(x^2)
    double meanX = 0.0;
    double meanY = 0.0;

    // Расчеты по регрессии (общие).
    QHash<QString, double> coeffs{};// коэффициенты уравнения
    double Sx2 = 0.0;               // несмещенная дисперсия по x
    double Sy2 = 0.0;               // несмещенная дисперсия по y
    double SxMean = 0.0;            // ср. квадратическое откл. выборочного среднего по x
    double SyMean = 0.0;            // ср. квадратическое откл. выборочного среднего по y
    double r = 0.0;                 // коэффициент корреляции (Пирсона) считается через вспомогательные величины, не прямой формулой

    QString funcStr{};              // итоговый вид уравнения
    QString regCoefStr{};           // описание коээфициентов
    QString regConStr{};            // описание связи корреляции/детерминации

    double MSE = 0.0;               // среднеквадратическая ошибка
    double R2 = 0.0;                // коэффициент детерминации
    double Sost = 0.0;              // остаточная сумма квадратов
    double Sregr = 0.0;             // регрессионная сумма квадратов
    double Spoln = 0.0;             // полная сумма квадратов
};

#endif // REGRESSIONVALUES_H
