from math import exp, cos, sin
from matplotlib.pyplot import show, subplots
from pathlib import Path
import numpy as np
import pandas as pd

#----------------------------#
# ИЗВЛЕЧЕНИЕ ДАННЫХ ИЗ EXCEL #
#----------------------------#
class ExcelParser:
    """ Получение пути к экспериментальным данным в Excel """
    def __init__(self, filepath: str):
        self.filepath = Path(filepath)
        if not self.filepath.exists():
            raise FileNotFoundError(f"[ERROR] Файл не найден: {self.filepath}.")

    def _list_sheets(self):
        """ Возвращает список всех листов Excel """
        xls = pd.ExcelFile(self.filepath)
        return xls.sheet_names

    def _read_sheet(self, sheet_name=0):
        """ Возвращает обработанный DataFrame для чтения указанного листа Excel с заголовками """
        df = pd.read_excel(self.filepath, sheet_name=sheet_name, header=None)   # Формирует DataFrame с листа с заголовками.
        df = df[pd.to_numeric(df.iloc[:, 0], errors='coerce').notna()]          # Фильтр на только числовые данные, остальне убираются.
        df = df.reset_index(drop=True)                                          # Заново расставляем индексы в данных.
        return df

    def _extract_variables(self, df):
        """ Извлекает переменные из таблицы:
            - t, H, Vist, Vpr, tkk, betta, teta, gamma, psi, X, Z, W, omega - переменные;
            - Возвращает словарь numpy-массивов;
            - (!) - не используются напрямую в моделировании.
        """
        data = {
            't': df.iloc[:, 0].to_numpy(),          #      время                    (с)
            'H': df.iloc[:, 1].to_numpy(),          # (!)  высота                   (м) 
            'Vist': df.iloc[:, 3].to_numpy(),       # (!)  истинная скорость        (м/с)       с учетом ветра
            'Vpr': df.iloc[:, 5].to_numpy(),        #      приборная скорость       (м/с)       без учета ветра, т.е. собственная
            'tkk': df.iloc[:, 6].to_numpy(),        #      угол атаки               (градусы)
            'betta': df.iloc[:, 7].to_numpy(),      # (!)  угол скольжения          (градусы)
            'teta': df.iloc[:, 8].to_numpy(),       # (!)  тангаж                   (градусы)
            'gamma': df.iloc[:, 9].to_numpy(),      # (!)  крен                     (градусы)
            'psi': df.iloc[:, 10].to_numpy(),       #      курс                     (градусы)
            'X': df.iloc[:, 11].to_numpy(),         #      X спутника (широта)      (м)
            'Z': df.iloc[:, 12].to_numpy(),         #      Z спутника (долгота)     (м)
            'W': df.iloc[:, 13].to_numpy(),         #      скорость ветра           (м/с)
            'omega': df.iloc[:, 14].to_numpy(),     #      угол (направление) ветра (градусы)
            }
        return data

    def execute_data_from_Excel(self, sheet_name: str):
        """ Чтение экспериментальных данных из Excel """
        sheets = self._list_sheets()            # список листов в Excel
        if sheet_name in sheets:                # поиск нужного листа
            df = self._read_sheet(sheet_name)   # DataFrame данных листа
            data = self._extract_variables(df)  # извлечение данных
        else:
            raise ValueError(f"[ERROR] Лист {sheet_name} не найден в Excel!")
        return data

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
        - dt                : временной шаг моделирования.
    """
    def __init__(self, V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt):
        self.V_air = V_air
        self.V_wind = V_wind
        self.K_angle = K_angle
        self.wind_angle = wind_angle
        self.A_angle = A_angle
        self.flight_duration = flight_duration
        self.dt = dt
        # Вычисляемые данные.
        self.x = [] # Координаты положения БЛА по долготе (м).
        self.y = [] # Координаты положения БЛА по широте (м).

    def simulate(self):
        """ Вычисляет (симулирует) траекторию БЛА """
        # Цикл по временным шагам.
        t_values = np.arange(0, self.flight_duration + self.dt, self.dt)
        for i, _ in enumerate(t_values):
            # В случае массива (экспериментальные данные Excel) - по индексу, иначе - как константа.
            V_air = self.V_air[i] if isinstance(self.V_air, (list, np.ndarray)) else self.V_air
            V_wind = self.V_wind[i] if isinstance(self.V_wind, (list, np.ndarray)) else self.V_wind
            K = np.deg2rad(self.K_angle[i] if isinstance(self.K_angle, (list, np.ndarray)) else self.K_angle)
            A = np.deg2rad(self.A_angle[i] if isinstance(self.A_angle, (list, np.ndarray)) else self.A_angle)
            d = np.deg2rad(self.wind_angle[i] if isinstance(self.wind_angle, (list, np.ndarray)) else self.wind_angle)

            # Вычисление широтной и долготной составляющей путевой скорости БЛА.
            dx = (V_air * sin(K) + V_wind * sin(d)) * cos(A)
            dy = (V_air * cos(K) + V_wind * cos(d)) * cos(A)

            # Если это начальный момент времени, то начальная точка (0;0).
            if t_values[i] <= 1e-6:
                self.x.append(0)
                self.y.append(0)
            else: # Иначе синтетически моделируем методом Эйлером (или считаем интеграл как сумму Римана).
                self.x.append(self.x[-1] + dx * self.dt)
                self.y.append(self.y[-1] + dy * self.dt)
        return self.x, self.y

#----------------------------------------------------------------#
#       МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (СИМУЛЯЦИЯ, 2 л/р)     #
# - моделирование погрешности системы комплексирования           #
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
        # Параметр комплексирования k.
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
        error_x = 0   # В момент t=0 ошибка по долготе
        error_y = 0   # и широте равна 0.
        errors_x = [] # Память ошибок по долготе
        errors_y = [] # и широте.
        time_between_interval = 0   # Повторяющийся интервал без спутника и со спутником (self.GPS_period + self.GPS_visible_duration).
        # Цикл по временным шагам.
        t_values = np.arange(0, self.flight_duration + self.dt, self.dt)
        for i, _ in enumerate(t_values):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден.
            if time_between_interval - self.GPS_period >= 1e-6:
                error_x -= error_x * (1 - exp(-self.k * self.dt))
                error_y -= error_y * (1 - exp(-self.k * self.dt))
                # Если time_between_interval == self.GPS_period + self.GPS_visible_duration, то интервал закончился.
                # eps = 1e-6 - особенность машинного вычисления.
                if np.abs(time_between_interval - (self.GPS_period + self.GPS_visible_duration)) <= 1e-6:
                       time_between_interval = 0
                time_between_interval += self.dt
            # Иначе спутник не виден, и рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф).
            else:
                error_x += self.C * time_between_interval * self.dt
                error_y += self.C * time_between_interval * self.dt
                time_between_interval += self.dt
            errors_x.append(error_x)
            errors_y.append(error_y)
        return errors_x, errors_y

    def _calculate_without_correction(self):
        """ Вычисление погрешности бортовой системы БЛА """
        # Погрешность.
        error_x = 0   # В момент t=0 ошибка по долготе
        error_y = 0   # и широте равна 0.
        errors_x = [] # Память ошибок по долготе
        errors_y = [] # и широте.
        # Цикл по временным шагам.
        t_values = np.arange(0, self.flight_duration + self.dt, self.dt)
        for i, t in enumerate(t_values):
            error_x += self.C * t * self.dt
            error_y += self.C * t * self.dt
            errors_x.append(error_x)
            errors_y.append(error_y)
        return errors_x, errors_y

    def simulate(self):
        """ Симуляция """
        # Вычисление ошибок с комплексированием и без комплексирования.
        return self._calculate_with_correction(), self._calculate_without_correction()

#----------------------------------------------------------------#
#       МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (ЭКСПЕРИМЕНТАЛЬНО)     #
# - моделирование погрешности системы комплексирования           #
# - моделирование погрешности бортовой системы БЛА               #
#----------------------------------------------------------------#
class RealisticComplexation:
    """ Моделирование погрешностей бортовой системы БЛА и системы косплексирования.
        - flight_duration       : временной промежуток моделирования полета БЛА (продолжительность);
        - GPS_period            : период до появления спутника;
        - GPS_visible_duration  : продолжительность видимости спутника;
        - accuracy              : точность (доля запомненной ошибки);
        - С                     : коэффициент роста квадратичной зависимости ошибки определения положения координат БЛА от времени полета;
        - dt                    : временной шаг моделирования.
    """
    def __init__(self, 
                 flight_duration, GPS_period, GPS_visible_duration, accuracy, Cx, Cy, dt):
        self.flight_duration = flight_duration
        self.GPS_period = GPS_period
        self.GPS_visible_duration = GPS_visible_duration
        self.accuracy = accuracy
        self.Cx = Cx
        self.Cy = Cy
        self.dt = dt
        # Параметр комплексирования k.
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

    def _calculate_with_correction(self, x=None, y=None, x_sp=None, y_sp=None):
        """ Вычисление погрешности системы комплексирования """
        # Погрешность.
        error_x = 0   # В момент t=0 ошибка по долготе
        error_y = 0   # и широте равна 0.
        errors_x = [] # Память ошибок по долготе
        errors_y = [] # и широте.
        # Запомненная ошибка.
        q_x = 0
        q_y = 0
        errors_q_x = []
        errors_q_y = []
        time_between_interval = 0   # Повторяющийся интервал без спутника и со спутником (self.GPS_period + self.GPS_visible_duration).
        # Цикл по временным шагам.
        t_values = np.arange(0, self.flight_duration + self.dt, self.dt)
        GPS = True
        for i, _ in enumerate(t_values):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден.
            if time_between_interval - self.GPS_period >= 1e-6:
                if GPS:
                    error_x = x_sp[i] - x[i] - q_x
                    error_y = y_sp[i] - y[i] - q_y
                    GPS = False
                q_x += error_x * (1 - exp(-self.k * self.dt))
                q_y += error_y * (1 - exp(-self.k * self.dt))
                error_x -= error_x * (1 - exp(-self.k * self.dt))
                error_y -= error_y * (1 - exp(-self.k * self.dt))
                # Если time_between_interval == self.GPS_period + self.GPS_visible_duration, то интервал закончился.
                # eps = 1e-6 - особенность машинного вычисления.
                if np.abs(time_between_interval - (self.GPS_period + self.GPS_visible_duration)) <= 1e-6:
                       time_between_interval = 0
                       GPS = True
                time_between_interval += self.dt
            # Иначе спутник не виден.
            else:
                # Последняя запомненная ошибка.
                error_x += self.Cx * time_between_interval * self.dt
                error_y += self.Cy * time_between_interval * self.dt
                time_between_interval += self.dt
            errors_x.append(error_x)
            errors_y.append(error_y)
            errors_q_x.append(q_x)
            errors_q_y.append(q_y)
        return errors_x, errors_y, errors_q_x, errors_q_y

    def _calculate_without_correction(self, x=None, y=None, x_sp=None, y_sp=None):
        """ Вычисление погрешности бортовой системы БЛА """
        # Погрешность.
        error_x = 0   # В момент t=0 ошибка по долготе
        error_y = 0   # и широте равна 0.
        errors_x = [] # Память ошибок по долготе
        errors_y = [] # и широте.
        # Цикл по временным шагам.
        t_values = np.arange(0, self.flight_duration + self.dt, self.dt)
        for i, t in enumerate(t_values):
            # Ошибка = GPS - СВС (система воздушных сигналов). По идее изменяется квадратичным законом.
            error_x += self.Cx * t * self.dt
            error_y += self.Cy * t * self.dt
            errors_x.append(error_x)
            errors_y.append(error_y)
        return errors_x, errors_y

    def simulate(self, x=None, y=None, x_sp=None, y_sp=None):
        """ Симуляция """
        # Вычисление ошибок с комплексированием и без комплексирования.
        return self._calculate_with_correction(x, y, x_sp, y_sp), self._calculate_without_correction(x, y, x_sp, y_sp)

#------------------------#
# МЕНЕДЖЕР МОДЕЛИРОВАНИЯ #
#------------------------#
class ModelManager:
    """ Управление симуляцией полета и вычисления ошибок определения координат положения (без комплексирования и с комплексированием (GPS))
        - V_air                 : воздушная скорость БЛА (м/с);
        - V_wind                : скорость ветра (м/с);
        - K_angle               : курсовой угол БЛА (градусы);
        - wind_angle            : угол ветра (градусы);
        - A_angle               : угол атаки БЛА (градусы);
        - GPS_period            : период до появления спутника (GPS) (с);
        - GPS_visible_duration  : время видимости спутника (GPS) (с);
        - accuracy              : точность (для запомненной ошибки);
        Коэффициент роста квадратичной зависимости ошибки определения координат положения БЛА от времени полета (дрейф):
        -*- Cx                  : коэффициент роста квадратичной зависимости ошибки определения положения координат БЛА от времени полета по долготе;
        -*- Cy                  : коэффициент роста квадратичной зависимости ошибки определения положения координат БЛА от времени полета по широте;
        - flight_duration       : временной промежуток моделирования;
        - dt                    : временной шаг моделирования;
        - x_sp                  : координаты положения БЛА по GPS - долгота (м);
        - y_sp                  : координаты положения БЛА по GPS - широта (м).
    """
    def __init__(self, V_air=100, V_wind=25, K_angle=45, wind_angle=120, A_angle=5,
                 GPS_period=1200, GPS_visible_duration=60, accuracy=0.95, Cx=0.001, Cy=0.001,
                 flight_duration=3600, dt=0.1, x_sp=None, y_sp=None):
        # GPS.
        self.x_sp = x_sp
        self.y_sp = y_sp

        # Моделирование полета БЛА.
        self.BLA_flight = FlightModelSimulator(V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt)
        # Моделирование ошибок полета БЛА.
        if self.x_sp is None and self.y_sp is None:
            self.BLA_errors = ComplexationSimulator(flight_duration, GPS_period, GPS_visible_duration, accuracy, Cx, dt)
        else:
            self.realistic_BLA_errors = RealisticComplexation(flight_duration, GPS_period, GPS_visible_duration, accuracy, Cx, Cy, dt)

    def _compute_path(self, x, y):
        """ Вычисление пройденного пути БЛА """
        # Приведение к массивам NumPy.
        x = np.array(x, dtype=float)
        y = np.array(y, dtype=float)
        # Вычисление.
        dx = np.diff(x)              # приращения по долготе
        dy = np.diff(y)              # приращения по широте
        ds = np.sqrt(dx**2 + dy**2)  # пройденный путь в каждый момент времени
        path = np.concatenate(([0], np.cumsum(ds))) # накопленный путь в каждый момент времени
        return path

    def run(self):
        """ Реализация моделирования полета БЛА и расчета ошибок """
        x, y = self.BLA_flight.simulate()
        errors = (
            self.BLA_errors.simulate()
            if self.x_sp is None and self.y_sp is None
            else self.realistic_BLA_errors.simulate(x, y, self.x_sp, self.y_sp)
        )
        with_corr, without_corr = errors
        return x, y, with_corr, without_corr

    def show_plots(self, excel_path="not specified", sheet_name="not defined"):
        """ Отрисовка.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Чтение данных.
        # - Траектория БЛА (синтетика)
        x, y, with_corr, without_corr = self.run()
        if len(with_corr) == 2:
            err_with_corr_x, err_with_corr_y = with_corr
        else:
            err_with_corr_x, err_with_corr_y, q_x, q_y = with_corr
        err_without_corr_x, err_without_corr_y  = without_corr
        t = np.arange(0, self.BLA_flight.flight_duration + self.BLA_flight.dt, self.BLA_flight.dt)
        
        # Настройка отрисовки окна с графиками.
        fig, axs = subplots(2, 2, figsize=(12, 6))
        fig.canvas.manager.set_window_title("Имитационная модель курсовой системы БЛА с комплексированием и без комплексирования")
        if self.x_sp is None and self.y_sp is None:
            fig.suptitle(
                f"Время полета: {self.BLA_flight.flight_duration} с, Скорость БЛА: {self.BLA_flight.V_air} м/c,"
                f"Период до появления спутника: {self.BLA_errors.GPS_period} с, Продолжительность видимости спутника: {self.BLA_errors.GPS_visible_duration} c, "
                f"\nПараметр комплексирования k: {self.BLA_errors.k:.5f} с$^{{-1}}$, "
                f"Скорость ветра: {self.BLA_flight.V_wind} м/с, Направление ветра: {self.BLA_flight.wind_angle}\u00B0, "
                f"\nКурсовой угол: {self.BLA_flight.K_angle}\u00B0, Угол атаки: {self.BLA_flight.A_angle}\u00B0, "
                f"\nПроцент запомненной ошибки: {(1 - exp(-self.BLA_errors.k * self.BLA_errors.GPS_visible_duration)) * 100}%",
                fontsize=12
            )
        else:
            fig.suptitle(f"Исходные данные взяты из Excel: {excel_path} [Лист: {sheet_name}]"
                         f"\n\nВремя полета = {self.BLA_flight.flight_duration} с, "
                         f"Период видимости спутника: {self.realistic_BLA_errors.GPS_period} с, Время комплексирования: {self.realistic_BLA_errors.GPS_visible_duration} c, "
                         f"\nПараметр комплексирования k: {self.realistic_BLA_errors.k:.5f} с$^{{-1}}$, Процент запомненной ошибки: {(1 - exp(-self.realistic_BLA_errors.k * self.realistic_BLA_errors.GPS_visible_duration)) * 100}%",
                         fontsize=12
            )
        
        # Определение координат положения БЛА без комплексирования и с комплексированием
        # Синтетические данные (x и y - это спутник/GPS)
        if (self.x_sp is None and self.y_sp is None):
            x_GPS = x
            y_GPS = y
            x_without_corr = [x[i] + err_without_corr_x[i] for i in range(len(x))]
            y_without_corr = [y[i] + err_without_corr_y[i] for i in range(len(y))]
            x_with_corr = [x[i] + err_with_corr_x[i] for i in range(len(x))]
            y_with_corr = [y[i] + err_with_corr_y[i] for i in range(len(y))]
        # Реальные данные (x и y - это система воздушных сигналов (СВС))
        else:
            x_GPS = self.x_sp
            y_GPS = self.y_sp
            x_without_corr = x
            y_without_corr = y
            x_with_corr = [x[i] + q_x[i] for i in range(len(x))]
            y_with_corr = [y[i] + q_y[i] for i in range(len(y))]

        # Вывод.
        print("Среднеквадратические ошибки определения координат положения БЛА:")
        MSE_X_without_GPS = np.sqrt(np.mean((np.array(x_GPS) - np.array(x_without_corr)) ** 2))
        MSE_Y_without_GPS = np.sqrt(np.mean((np.array(y_GPS) - np.array(y_without_corr)) ** 2))
        MSE_X_with_GPS = np.sqrt(np.mean((np.array(x_GPS) - np.array(x_with_corr)) ** 2))
        MSE_Y_with_GPS = np.sqrt(np.mean((np.array(y_GPS) - np.array(y_with_corr)) ** 2))
        print(f"- Долгота [без комплексирования]: MSE = {MSE_X_without_GPS:5f} м")
        print(f"- Широта  [без комплексирования]: MSE = {MSE_Y_without_GPS:5f} м")
        print(f"- Долгота [с комплексированием]: MSE = {MSE_X_with_GPS:5f} м")
        print(f"- Широта  [с комплексированием]: MSE = {MSE_Y_with_GPS:5f} м")
        print(f"- Снижение MSE (долгота) : MSE = {100 - MSE_X_with_GPS/MSE_X_without_GPS * 100:5f}%")
        print(f"- Снижение MSE  (широта) : MSE = {100 - MSE_Y_with_GPS/MSE_Y_without_GPS * 100:5f}%")
        print()

        print("Среднеквадратические ошибки вычисления пройденного пути БЛА:")
        path_without_GPS = self._compute_path(x_without_corr, y_without_corr)
        path_with_GPS = self._compute_path(x_with_corr, y_with_corr)
        path_GPS = self._compute_path(x_GPS, y_GPS)
        MSE_path_without_GPS = np.sqrt(np.mean((np.array(path_GPS) - np.array(path_without_GPS))**2))
        MSE_path_with_GPS = np.sqrt(np.mean((np.array(path_GPS) - np.array(path_with_GPS))**2))
        print(f"- [без комплексирования] MSE = {MSE_path_without_GPS:5f} м")
        print(f"- [c комплексированием]  MSE = {MSE_path_with_GPS:5f} м")
        print(f"- Снижение MSE: {100 - MSE_path_with_GPS/MSE_path_without_GPS * 100:5f}%")
        print("\n")
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (долгота + время).
        axs[0, 0].plot(t, x_GPS, label="Траектория полета БЛА по спутнику/GPS", color='green')
        axs[0, 0].plot(t, x_without_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 0].plot(t, x_with_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 0].set_title("Сравнение всех тректорий полета БЛА (долгота)", fontsize=10)
        axs[0, 0].set_xlabel("t (время, с)", fontsize=9)
        axs[0, 0].set_ylabel("$\lambda$ (долгота, м)", fontsize=9)
        axs[0, 0].legend(fontsize=8)
        axs[0, 0].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + время).
        axs[0, 1].plot(t, y_GPS, label="Траектория полета БЛА по спутнику/GPS", color='green')
        axs[0, 1].plot(t, y_without_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 1].plot(t, y_with_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 1].set_title("Сравнение всех тректорий полета БЛА (широта)", fontsize=10)
        axs[0, 1].set_xlabel("t (время, с)", fontsize=9)
        axs[0, 1].set_ylabel("$\phi$ (широта, м)", fontsize=9)
        axs[0, 1].legend(fontsize=8)
        axs[0, 1].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (время + путь).
        axs[1, 0].plot(t, path_GPS, label="Путь полета БЛА по спутнику/GPS", color='green')
        axs[1, 0].plot(t, path_without_GPS, label="Путь полета БЛА без комплексирования", color='red', linestyle='--')
        axs[1, 0].plot(t, path_with_GPS, label="Путь полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[1, 0].set_title("Сравнение всех путей полета БЛА", fontsize=10)
        axs[1, 0].set_xlabel("t (время, с)", fontsize=9)
        axs[1, 0].set_ylabel("s (путь, м)", fontsize=9)
        axs[1, 0].legend(fontsize=8)
        axs[1, 0].grid(True)

        # Ошибка без коплексирования и с комплексированием.
        axs[1, 1].plot(t, err_without_corr_x, label="Ошибка бортовой системы БЛА (долгота)", color='red', linestyle='-.')
        axs[1, 1].plot(t, err_with_corr_x, label="Ошибка системы комплексирования (долгота)", color='blue', linestyle='-')
        axs[1, 1].plot(t, err_without_corr_y, label="Ошибка бортовой системы БЛА (широта)", color='green', linestyle=':')
        axs[1, 1].plot(t, err_with_corr_y, label="Ошибка системы комплексирования (широта)", color='orange', linestyle='--')
        axs[1, 1].set_title("Сравнение погрешностей бортовой и комплексной системы", fontsize=10)
        axs[1, 1].set_xlabel("t (время, с)", fontsize=9)
        axs[1, 1].set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs[1, 1].legend(fontsize=8)
        axs[1, 1].grid(True)
        
        fig.tight_layout(rect=[0, 0, 1, 1]) # left, bottom, right, top
        show()        
        
#-----------------#
# ИСХОДНЫЕ ДАННЫЕ #
#-----------------# 
if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """  
    # 1. Синтетические исходные данные.
    # --- Моделирование ---
    sim_BLA = ModelManager(V_air=100, V_wind=25, K_angle=90, wind_angle=120, A_angle=3,
                           GPS_period=20 * 60, GPS_visible_duration=1 * 60, Cx=0.001, Cy=0.001,
                           flight_duration=60 * 60, dt=0.1)

    # 2. Экспериментальные данные.
    # --- Инициализация ---
    filepath = "../ЛР2-3 Исходные данные.xls" # путь до Excel файла
    sheet_name = "Лист2"                      # название листа в Excel файле
    # --- Извлечение данных ---
    data = ExcelParser(filepath=filepath).execute_data_from_Excel(sheet_name=sheet_name)
    # --- Моделирование ---
    realistic_BLA = ModelManager(V_air=data['Vpr'], V_wind=data['W'], K_angle=data['psi'], wind_angle=data['omega'], A_angle=data['tkk'], 
                                 GPS_period=200, GPS_visible_duration=50, accuracy=0.95, Cx=0.057, Cy=0.1, flight_duration=data['t'][-1], 
                                 dt=data['t'][1]-data['t'][0], x_sp=data['Z'],y_sp=data['X'])
    
    # 1,2. --- Визуализация ---
    sim_BLA.show_plots()
    realistic_BLA.show_plots(excel_path=filepath, sheet_name=sheet_name)