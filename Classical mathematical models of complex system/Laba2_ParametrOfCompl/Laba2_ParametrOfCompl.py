from math import exp
from matplotlib.pyplot import show, subplots
import numpy as np

#-----------------------------------#
# МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ #
#-----------------------------------#
class ComplexationSimulator:
    """ Моделирование погрешностей бортовой системы БЛА и системы косплексирования.
        - flight_duration       : временной промежуток моделирования полета БЛА (продолжительность);
        - GPS_period            : период до появления спутника;
        - GPS_visible_duration  : продолжительность видимости спутника;
        - accuracy              : точность (доля запомненной ошибки);
        - С                     : коэффициент роста квадратичной зависимости ошибки определения положения координат БЛА от времени полета;
        - dt                    : временной шаг моделирования.
    """
    def __init__(self, 
                 flight_duration=3600, GPS_period=1200, GPS_visible_duration=60, 
                 accuracy=0.95, C=0.001, dt=0.1):
        self.flight_duration = flight_duration
        self.GPS_period = GPS_period
        self.GPS_visible_duration = GPS_visible_duration
        self.accuracy = accuracy
        self.C = C
        self.dt = dt
        # Параметр комплексирования k и массивы прогрешностей систем.
        self.k = self._calculate_k_parameter()
        self.errors_without_correction = self._calculate_without_correction()
        self.errors_with_correction = self._calculate_with_correction()

    def _calculate_k_parameter(self):
        """ Запомненная ошибка комплексирования вычисляется из формулы:
                q = (1 - exp(-k*tau)) * (y - x), где:
        - k   : основной параметр комплексирования;
        - tau : время комплексирования (GPS_visible_duration);
        - y   : координата (долготы/широты) со спутника;
        - x   : координата (долготы/широты) по СВС (система воздушных сигналов).
        """
        # Необходимо, чтобы q >= 0.95 * (y-x) (по усл.) => 1 - exp(-k * tau) >= 0.95 => k >= -ln(0.05)/tau.
        # Из теории автоматического управления известно, что exp(-5) < 0.01, т.е. k=5/tau покрывает как минимум 99% ошибки определения координат положения.
        # В случае k >= -ln(0.05) покрывается как минимум 95% ошибки определения координат положения.
        # Тогда в качестве k необходимо вернуть: 5 / self.GPS_visible_duration
        k = -np.log(1 - self.accuracy) / self.GPS_visible_duration
        return k

    def _calculate_with_correction(self):
        """ Вычисление погрешности системы комплексирования """
        # Погрешность.
        error = 0   # При t=0 ошибка=0.
        errors = [] # Память ошибок по долготе и широте (в данном случае они одинаковые).
        time_between_interval = 0   # Повторяющийся интервал без спутника и со спутником (self.GPS_period + self.GPS_visible_duration).
        # Цикл по временным шагам.
        for t in np.arange(0, self.flight_duration, self.dt):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден.
            if time_between_interval - self.dt >= self.GPS_period:
                error -= error * (1 - exp(-self.k * self.dt))
                # Если time_between_interval == self.GPS_period + self.GPS_visible_duration, то интервал закончился.
                # eps = 1e-6 - особенность машинного вычисления.
                if np.abs(time_between_interval - (self.GPS_period + self.GPS_visible_duration)) <= 1e-6:
                       time_between_interval = 0
                time_between_interval += self.dt
            # Иначе спутник не виден и погрешность накапливается квадратично.
            else:
                error += self.C * time_between_interval * self.dt
                time_between_interval += self.dt
            errors.append(error)
        return errors

    def _calculate_without_correction(self):
        """ Вычисление погрешности бортовой системы БЛА """
        # Погрешность.
        error = 0   # При t=0 ошибка=0.
        errors = [] # Память ошибок по долготе и широте (в данном случае они одинаковые).
        # Цикл по временным шагам.
        for t in np.arange(0, self.flight_duration, self.dt):
            error += self.C * t * self.dt
            errors.append(error)
        return errors

    def show_plot(self):
        """ Отрисовка графиков """
        # Вычисление величин для сравнительного анализа.
        start_complexation_ind = int(self.GPS_period/self.dt)
        E0 = self.errors_without_correction[start_complexation_ind]
        q = (1 - np.exp(-self.k * self.GPS_visible_duration)) * E0
        Eq = E0 - q
        percent = (q / E0) * 100

        # Настройка отрисовки окна с графиками.
        fig, axs = subplots(1, 1, figsize=(12,6))
        fig.canvas.manager.set_window_title("Моделирование погрешности бортовой системы БЛА и комплексной системы")
        fig.suptitle(
            f"Время полета={self.flight_duration} c, Период до появления спутника={self.GPS_period} с, "
            f"Продолжительность видимости спутника={self.GPS_visible_duration} c, "
            f"\nПараметр комплексирования k={self.k:.5f} c\u207B\u00B9, "
            f"\nОшибка на начало комплексирования E0={E0:.3f} м, Запомненная ошибка q={q:.3f} м, "
            f"\nОшибка после комплексирования Eq={Eq:.3f} м, Процент запомненной ошибки={percent:.2f}%",
            fontsize=12
        )

        # Расчет значений.
        t = [i for i in np.arange(0, self.flight_duration, self.dt)]

        # Отрисовка графиков погрешностей и маркеров сравнительного анализа (один раз, чтобы не перегружать график).
        axs.plot(t, self.errors_with_correction, label="Ошибка системы комплексирования", color='blue', linestyle='--')
        axs.plot(t, self.errors_without_correction, label="Ошибка бортовой системы БЛА", color='red')
        axs.plot(self.GPS_period, E0, 'co', markersize=4, label='$E_0$ - начало комплексирования (в первый раз)')
        axs.plot(self.GPS_period + self.GPS_visible_duration, Eq, 'co', markersize=4, label='$E_q$ - конец комплексирования (в первый раз)')
        axs.set_title("Сравнение погрешностей бортовой и комплексной системы", fontsize=14)
        axs.set_xlabel("t (время, с)", fontsize=9)
        axs.set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs.legend(fontsize=8, loc='upper left')
        axs.grid(True)

        fig.tight_layout(rect=[0.05, 0.05, 0.95, 0.95])
        show()   

#-----------------#
# ИСХОДНЫЕ ДАННЫЕ #
#-----------------# 
if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """  
    # Константы.
    flight_duration = 60 * 60       # Время моделирования (мин -> c)
    dt = 0.1                        # Шаг времени моделирования (с)
    
    # GPS.
    GPS_period = 20 * 60            # Период видимости спутника (мин -> c)
    GPS_visible_duration = 1 * 60   # Время видимости спутника (мин -> c)
    
    # Коэффициент квадратичного роста ошибки определения координат положения.
    C = 0.001
    
    # Моделирование.
    error_simulator = ComplexationSimulator(flight_duration=flight_duration, GPS_period=GPS_period, GPS_visible_duration=GPS_visible_duration, 
                                            accuracy=0.95, C=C, dt=dt)
    error_simulator.show_plot()