from logging import Manager
from math import exp, cos, sin
from matplotlib.pyplot import show, subplots
import numpy as np

#--------------------------#
# МОДЕЛИРОВАНИЕ ПОЛЕТА БЛА #
#--------------------------#
class FlightModelSimulator:
    """ Моделирование полета БЛА.
        - V_air             : воздушная скорость БЛА;
        - V_wind            : скорость ветра;
        - K_angle           : курсовой угол БЛА;
        - wind_angle        : угол ветра;
        - A_angle           : угол атаки БЛА;
        - flight_duration   : временной промежуток моделирования полета БЛА (продолжительность);
        - dt                : временной шаг моделирования;
        - x                 : координаты положения БЛА по долготе (м);
        - y                 : координаты положения БЛА по широте (м).
    """
    def __init__(self, V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt):
        self.V_air = V_air
        self.V_wind = V_wind
        self.K_angle = K_angle
        self.wind_angle = wind_angle
        self.A_angle = A_angle
        self.flight_duration = flight_duration
        self.dt = dt
        self.x = []
        self.y = []

    def simulate(self):
        """ Вычисляет (симулирует) траекторию БЛА """
        # Цикл по временным шагам.
        for t in np.arange(0, self.flight_duration, self.dt):
            V_air = self.V_air
            V_wind = self.V_wind
            K = np.deg2rad(self.K_angle)
            A = np.deg2rad(self.A_angle)
            d = np.deg2rad(self.wind_angle)

            # Вычисление смещений по долготе и широте.
            dx = (V_air * cos(K) + V_wind * cos(d)) * cos(A) * self.dt
            dy = (V_air * sin(K) + V_wind * sin(d)) * cos(A) * self.dt

            # Если это начальный момент времени, то начальная точка (0;0).
            if t <= 1e-6:
                self.x.append(0)
                self.y.append(0)
            else: # Иначе синтетически моделируем методом Эйлером (или считаем интеграл как сумму Римана).
                self.x.append(self.x[-1] + dx * self.dt)
                self.y.append(self.y[-1] + dy * self.dt)
        return self.x, self.y

#----------------------------------------------------------------#
#       МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (из 2 л/р)             #
# - моделирование спутника GPS (искусственное изменение ошибки)  #
# - моделирование погрешности бортовой системы БЛА               #
#----------------------------------------------------------------#
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

    def simulate(self):
        """ Симуляция """
        # Вычисление ошибок с комплексированием и без комплексирования.
        return self._calculate_with_correction(), self._calculate_without_correction()

#------------------------#
# МЕНЕДЖЕР МОДЕЛИРОВАНИЯ #
#------------------------#
class ModelManager:
    """ Управление симуляцией полета и вычисления ошибок определения координат положения (без комплексирования и с комплексированием (GPS))
        - V_air                 : воздушная скорость БЛА;
        - V_wind                : скорость ветра;
        - K_angle               : курсовой угол БЛА;
        - wind_angle            : угол ветра;
        - A_angle               : угол атаки БЛА;
        - GPS_period            : период до появления спутника (GPS);
        - GPS_visibleduration   : время видимости спутника (GPS);
        - C                     : коэффициент роста квадратичной зависимости ошибки определения положения координат БЛА от времени полета;
        - flight_duration       : временной промежуток моделирования;
        - dt                    : временной шаг моделирования.
    """
    def __init__(self, V_air=100, V_wind=25, K_angle=45, wind_angle=120, A_angle=5,
                 GPS_period=1200, GPS_visible_duration=60, accuracy=0.95, C=0.001,
                 flight_duration=3600, dt=0.1):
        # Моделирование полета БЛА.
        self.BLA_flight = FlightModelSimulator(V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt)
        # Моделирование ошибок полета БЛА.
        self.BLA_errors = ComplexationSimulator(flight_duration, GPS_period, GPS_visible_duration, accuracy, C, dt)

    def run(self):
        """ Реализация моделирования полета БЛА и расчета ошибок """
        x, y = self.BLA_flight.simulate()
        err_with_corr, err_without_corr = self.BLA_errors.simulate()
        return x, y, err_with_corr, err_without_corr

    def show_plots(self):
        """ Отрисовка.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Чтение данных.
        # - Траектория БЛА (синтетика)
        x, y, err_with_corr, err_without_corr = self.run()
        t = np.arange(0, self.BLA_flight.flight_duration, self.BLA_flight.dt)

        # Вычисление величин для сравнительного анализа.
        start_complexation_ind = int(self.BLA_errors.GPS_period/self.BLA_errors.dt)
        E0 = err_without_corr[start_complexation_ind]
        q = (1 - np.exp(-self.BLA_errors.k * self.BLA_errors.GPS_visible_duration)) * E0
        Eq = E0 - q
        percent = (q / E0) * 100
        
        # Настройка отрисовки окна с графиками.
        fig, axs = subplots(2, 2, figsize=(12, 6))
        fig.canvas.manager.set_window_title("Имитационная модель курсовой системы БЛА с комплексированием и без комплексирования")
        fig.suptitle(
            f"Время полета={self.BLA_flight.flight_duration} с, Скорость БЛА={self.BLA_flight.V_air} м/c,"
            f"Период до появления спутника={self.BLA_errors.GPS_period} с, Продолжительность видимости спутника={self.BLA_errors.GPS_visible_duration} c, "
            f"\nПараметр комплексирования k={self.BLA_errors.k:.5f} c\u207B\u00B9, "
            f"\nСкорость ветра={self.BLA_flight.V_wind} м/с, Направление ветра={self.BLA_flight.wind_angle}\u00B0, "
            f"\nКурсовой угол={self.BLA_flight.K_angle}\u00B0, Угол атаки={self.BLA_flight.A_angle}\u00B0, "
            f"\nОшибка на начало комплексирования E0={E0:.3f} м, Запомненная ошибка q={q:.3f} м, "
            f"\nОшибка после комплексирования Eq={Eq:.3f} м, Процент запомненной ошибки={percent:.2f}%",
            fontsize=12
        )
        
        # Расчет значений.
        x_no_corr = [x[i] + err_without_corr[i] for i in range(len(x))]
        x_corr = [x[i] + err_with_corr[i] for i in range(len(x))]
        y_no_corr = [y[i] + err_without_corr[i] for i in range(len(y))]
        y_corr = [y[i] + err_with_corr[i] for i in range(len(y))]
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + время).
        axs[0, 0].plot(t, x, label="Траектория полета БЛА", color='green')
        axs[0, 0].plot(t, x_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 0].plot(t, x_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 0].set_title("Сравнение всех тректорий полета БЛА (долгота)", fontsize=10)
        axs[0, 0].set_xlabel("t (время, с)", fontsize=9)
        axs[0, 0].set_ylabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[0, 0].legend(fontsize=8, loc='upper left')
        axs[0, 0].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (долгота + время).
        axs[0, 1].plot(t, y, label="Траектория полета БЛА", color='green')
        axs[0, 1].plot(t, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 1].plot(t, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 1].set_title("Сравнение всех тректорий полета БЛА (широта)", fontsize=10)
        axs[0, 1].set_xlabel("t (время, с)", fontsize=9)
        axs[0, 1].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[0, 1].legend(fontsize=8, loc='upper left')
        axs[0, 1].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + долгота).
        axs[1, 0].plot(x, y, label="Траектория полета БЛА", color='green')
        axs[1, 0].plot(x_no_corr, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[1, 0].plot(x_corr, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[1, 0].set_title("Сравнение всех тректорий полета БЛА", fontsize=10)
        axs[1, 0].set_xlabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[1, 0].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[1, 0].legend(fontsize=8, loc='upper left')
        axs[1, 0].grid(True)

        # Ошибка без коплексирования и с комплексированием.
        axs[1, 1].plot(t, err_with_corr, label="Ошибка системы комплексирования", color='blue')
        axs[1, 1].plot(t, err_without_corr, label="Ошибка бортовой системы БЛА", color='red')
        axs[1, 1].plot(self.BLA_errors.GPS_period, E0, 'co', markersize=4, label='$E_0$ - начало комплексирования (в первый раз)')
        axs[1, 1].plot(self.BLA_errors.GPS_period + self.BLA_errors.GPS_visible_duration, Eq, 'co', markersize=4, label='$E_q$ - конец комплексирования (в первый раз)')
        axs[1, 1].set_title("Сравнение ошибок с комплексированием и без", fontsize=10)
        axs[1, 1].set_xlabel("t (время, с)", fontsize=9)
        axs[1, 1].set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs[1, 1].legend(fontsize=8, loc='upper left')
        axs[1, 1].grid(True)
        
        fig.tight_layout(rect=[0.05, 0.05, 0.95, 0.95]) # left, bottom, right, top
        show()        
        
#-----------------#
# ИСХОДНЫЕ ДАННЫЕ #
#-----------------# 
if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """  
    # Константы.
    flight_duration = 60 * 60       # Время моделирования (мин -> c)
    V_air = 50                      # Воздушная скорость БЛА (м/с)
    V_wind = 25                     # Скорость ветра (м/с)
    K_angle = 45                    # Курсовой угол БЛА (градусы)
    wind_angle = 120                # Угол ветра (градусы)
    A_angle = 5                     # Угол атаки (градусы)
    dt = 0.1                        # Шаг времени моделирования (с)
    
    # GPS.
    GPS_period = 20 * 60            # Период видимости спутника (мин -> c)
    GPS_visible_duration = 1 * 60   # Время видимости спутника (мин -> c)
    
    # Коэффициент квадратичного роста ошибки определения координат положения.
    C = 0.001
    
    # Моделирование.
    sim_BLA = ModelManager(V_air=V_air, V_wind=V_wind, K_angle=K_angle, wind_angle=wind_angle, A_angle=A_angle,
                           GPS_period=GPS_period, GPS_visible_duration=GPS_visible_duration, C=C,
                           flight_duration=flight_duration, dt=dt)
    sim_BLA.show_plots()