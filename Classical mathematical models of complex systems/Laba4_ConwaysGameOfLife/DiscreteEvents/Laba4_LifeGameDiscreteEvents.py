import pygame
import numpy as np
from enum import IntEnum
import os
from datetime import datetime

#********************************************************************************************************

#=================================#
#       КЛАСС ИГРЫ "ЖИЗНЬ"        #
#=================================#
class GameOfLifeByEvents:
    """
        Класс игры "Жизнь" (клеточный автомат на базе дискретных событий).
        ------------------------------------------------------------------
        height  : размер матричной сетки (высота, число строк);
        width   : размер матричной сетки (ширина, число столбцов).
    """
    def __init__(self, height, width):
        # Исходные данные.
        self.height = height
        self.width = width
        # Текущая и следующая матричная сетка.
        self.current_grid = np.zeros((height, width), dtype=np.uint8)
        self.next_grid = np.copy(self.current_grid)
        # Часы модельного времени.
        self.time = 0
        # Список событий.
        self.events_list = EventsList()
        # Текущая обрабатываемая клетка (i,j) (событие типа I).
        self.current_processing_cell = None
        # Текущее обрабатываемое событие.
        self.event = None

        # --- Статические счетчики ---
        # Количество живых клеток (сейчас).
        self.alive_cells = 0
        # Количество мертвых клеток (сейчас).
        self.dead_cells = self.height * self.width
        # Родившиеся клетки (вообще).
        self.born_cells_total = 0
        # Родившиеся клетки (сейчас).
        self.born_cells_now = 0
        # Умершие клетки (вообще).
        self.died_cells_total = 0
        # Умершие клетки (сейчас).
        self.died_cells_now = 0
        # Количество обработанных событий типа I.
        self.processed_events_I = 0
        # Количество обработанных событий типа II.
        self.processed_events_II = 0

    def init_random_cells_grid(self, p=0.25, seed=None):
        """ Инициализация матричной сетки значениями 0 и 1 """
        self.p = p
        self.seed = seed
        rng = np.random.default_rng(seed)
        self.current_grid = (rng.random((self.height, self.width)) < p).astype(np.uint8)
        self.next_grid = np.copy(self.current_grid)

    def calc_alive_neighbors(self, i, j):
        """ Вычисление количества живых соседей текущей клетки (i,j) """
        alive_neighbors = 0
        # Обход соседей текущей клетки (i,j).
        for di in (-1, 0, 1):
            for dj in (-1, 0, 1):
                # Текущая клетка (i,j) не учитывается за своего живого соседа.
                if di == 0 and dj == 0:
                    continue
                # Тороидальная матричная сетка.
                ni = (i + di) % self.height
                nj = (j + dj) % self.width
                # Вычисление количества живых соседей.
                alive_neighbors += self.current_grid[ni, nj]
        return alive_neighbors

    def _update_statistics(self):
        """ Обновление статистических счетчиков """
        self.alive_cells = np.sum(self.next_grid)
        self.dead_cells = self.height * self.width - self.alive_cells
        self.born_cells_now = np.sum((self.current_grid == 0) & (self.next_grid == 1))
        self.died_cells_now = np.sum((self.current_grid == 1) & (self.next_grid == 0))
        self.born_cells_total += self.born_cells_now
        self.died_cells_total += self.died_cells_now

    def _process_next_event(self):
        """ Обработка одного дискретного события из списка событий """
        # Если список событий пуст, добавляется событие типа II (стартовое событие).
        if self.events_list.is_empty():
            self.events_list.add(FieldScanEvent(self.time))

        # Извлечение события из списка событий.
        self.event = self.events_list.get_next()
        # Обновление статистических счетчиков после события типа II.
        if self.event.type == EventType.FIELD_SCAN:
            self._update_statistics()
        # Обработка события.
        self.event.execute(self)

    def process_all_events(self, generations=100):
        """ Реализация последовательности и обработки дискретных событий """
        # Пока список событий не пуст или не прошло заданное число поколений.
        while not self.events_list.is_empty() or self.time-1 <= generations:
            # Извлечение события из списка событий и его обработка.
            self._process_next_event()

#=================================#
#       СОБЫТИЯ ТИПА I и II       #
#=================================#
class EventType(IntEnum):
    # Просмотр одной ячейки поля и модификация ячеек в следующем.
    CELL_PROCESS = 0
    # Просмотр всего поля и выбор ячеек для просмотра и модификации.
    FIELD_SCAN = 1

#=================================#
#    КЛАССЫ СОБЫТИй ТИПА I и II   #
#=================================#
class Event:
    """
        Класс-интерфейс для обработки событий.
        --------------------------------------
        time: часы модельного времени события;
        type: тип события (I или II).
    """
    def __init__(self, time, event_type):
        self.time = time
        self.type = event_type

    def execute(self, model : GameOfLifeByEvents):
        """ Обработчик события, который необходимо реализовать в классах-реализациях """
        raise NotImplementedError

class CellProcessEvent(Event):
    """ 
        Класс-обработчик события типа I.
        --------------------------------
        * Наследник интерфейса Event *
        i: номер строки текущей клетки (i,j);
        j: номер столбца текущей клетки (i,j).
    """
    def __init__(self, time, i, j):
        super().__init__(time, EventType.CELL_PROCESS)
        self.i = i
        self.j = j

    def execute(self, model :GameOfLifeByEvents):
        """ Обработка события типа I по правилам Конвея для тороидальной сетки """
        #   Основные правила Конвея:  
        # • ячейка остается живой, если ее окружают две или три живые ячейки;  
        # • ячейка умирает от перенаселения, если ее окружает более трех живых ячеек; 
        # • ячейка умирает от изоляции, если у нее меньше двух живых соседей; 
        # • мертвая ячейка становится живой, если она имеет ровно три живых соседа.

        # Текущая рассматриваемая клетка (i,j).
        model.current_processing_cell = (self.i, self.j)

        # Количество живых клеток-соседей текущей клетки (i,j).
        alive_neighbors = model.calc_alive_neighbors(self.i, self.j)
        # Текущее состояние клетки (i,j) (живая[1]/мертвая[0]).
        state = model.current_grid[self.i, self.j]
        # Обновление состояния клетки (i,j).
        if state == 1: # Живая
            model.next_grid[self.i, self.j] = 1 if alive_neighbors in (2,3) else 0
        else:
            model.next_grid[self.i, self.j] = 1 if alive_neighbors == 3 else 0

        # Статистика.
        model.processed_events_I += 1

class FieldScanEvent(Event):
    """
        Класс-обработчик события типа II.
        ---------------------------------
        * Наследник интерфейса Event *
    """
    def __init__(self, time):
        super().__init__(time, EventType.FIELD_SCAN)

    def execute(self, model):
        """ Обработтка события типа II """
        # Сейчас нет рассматриваемой текущей клетки (i,j).
        model.current_processing_cell = None

        # Порождение события типа I для каждой клетки матричной сетки.
        for i in range(model.height):
            for j in range(model.width):
                model.events_list.add(CellProcessEvent(self.time, i, j))

        # Обновление матричной сетки (текущей на следующую).
        model.current_grid[:, :] = model.next_grid
        # Увеличение модельного времени на 1.
        model.time += 1

        # Добавляем события типа II для зацикливания.
        model.events_list.add(FieldScanEvent(model.time))

        # Статистика.
        model.processed_events_I = 0
        model.processed_events_II += 1

#=================================#
#       КЛАСС СПИСКА СОБЫТИЙ      #
#=================================#
class EventsList:
    """ Класс реализации списка событий """
    def __init__(self):
        self.events = []

    def add(self, event: Event):
        """ Добавление событий определенного типа """
        self.events.append(event)

    def get_next(self):
        """ Извлечение событий определенного типа """
        return self.events.pop(0)

    def is_empty(self):
        """ Проверка списка событий на пустоту """
        return len(self.events) == 0

#********************************************************************************************************

#=================================#
#           ОТЧЕТНОСТЬ            #
#=================================#
class Logger:
    """
        Класс логирования.
        ------------------
        results_dir : имя директории с записью результатов логов;
        game        : объект класса GameOfLifeByEvents;
        save_frames : флаг сохранения кадров визуализации.
    """
    def __init__(self, results_dir="results", game: GameOfLifeByEvents | None=None, save_frames=False):
        # Исходные данные.
        self.game = game
        self.enabled = game is not None
        self.save_frames = save_frames
        if not self.enabled:
            return

        # Создание папки results_dir.
        os.makedirs(results_dir, exist_ok=True)

        # Создание подпапки с датой и временем.
        timestamp = datetime.now().strftime("%d-%m-%Y_%H-%M-%S")
        self.session_dir = os.path.join(results_dir, timestamp)
        os.makedirs(self.session_dir, exist_ok=True)

        # Файл логов и открытие для записи.
        self.log_file_path = os.path.join(self.session_dir, "game_log.txt")
        self.file = open(self.log_file_path, "w")

        # Запись логирования с исходными данными.
        self.file.write(f"ИНФОРМАЦИЯ:\n"
                        f"[LOG] Автор: 4199 Горшков Андрей Олегович (КНИТУ-КАИ им. А.Н.Туполева)\n"
                        f"[LOG] Запуск новой сессии\n"
                        f"[LOG] Дата и время: {timestamp}\n"
                        f"{'='*100}\n"
                        f"ПАРАМЕТРЫ:\n"
                        f"[LOG] Матричная сетка размерности: {game.height}x{game.width}\n"
                        f"[LOG] Доля живых клеток: p = {game.p}\n"
                        f"[LOG] Доля мертвых клеток: q = {1-game.p}\n"
                        f"[LOG] SEED = {game.seed}\n"
                        f"{'='*100}\n")
        self.file.flush()

        # Директория кадров.
        if self.save_frames:
            self.frames_dir = os.path.join(self.session_dir, "frames")
            os.makedirs(self.frames_dir, exist_ok=True)
            self.frame_counter = 0

    def log_grid(self, model_time):
        """ Логирование текущей матричной сетки """
        if not self.enabled:
            return
        self.file.write(f"\n*-- Поколение {model_time} --*\n")
        self.file.write("[LOG] Статистика:\n")
        self.file.write(f"[LOG] Живые клетки (количество): {self.game.alive_cells}/{self.game.height * self.game.width}\n")
        self.file.write(f"[LOG] Мертвые клетки (количество): {self.game.dead_cells}/{self.game.height * self.game.width}\n")
        self.file.write(f"[LOG] Родившиеся клетки (сейчас/вообще): {self.game.born_cells_now}/{self.game.born_cells_total}\n")
        self.file.write(f"[LOG] Умершие клетки (сейчас/вообще): {self.game.died_cells_now}/{self.game.died_cells_total}\n")
        self.file.write(f"[LOG] Событий типа I обработано: {self.game.processed_events_I}/{self.game.height * self.game.width}\n")
        self.file.write(f"[LOG] Событий типа II обработано: {self.game.processed_events_II}\n\n")
        for row in self.game.current_grid:
            self.file.write(" ".join(str(cell) for cell in row) + "\n")
        self.file.write(f"{'-'*100}\n")
        self.file.flush()

    def log_error(self, message):
        """ Логирование ошибки """
        if not self.enabled:
            return
        self.file.write(f"\n{message}\n")
        self.file.flush()

    def close(self):
        """ Закрытие файла """
        if self.enabled:
            self.file.close()

    def save_frame(self, surface):
        """ Сохранение кадра PyGame в .png """
        if self.enabled and self.save_frames:
            frame_path = os.path.join(self.frames_dir, f"frame_{self.frame_counter:07d}.png")
            pygame.image.save(surface, frame_path)
            self.frame_counter += 1

#********************************************************************************************************

#=================================#
#     ВИЗУАЛИЗАЦИЯ (PyGame)       #
#=================================#
class GameOfLifeByEventsApp:
    """ Класс визуализации модели игры "Жизнь" (дискретные события).
        ------------------------------------------------------------
        game        : объект класса GameOfLifeByEvents;
        fps         : частота обновления визуализации;
        log         : флаг логирования;
        save_frames : флаг сохранения кадров визуализации (при условии, что log=True).
    """
    def __init__(self, game: GameOfLifeByEvents, fps=10, log=False, save_frames=False):
        # Исходные данные
        self.game = game
        self.fps = fps
        self.log = log
        self.paused = True
        self.model_time = 0
        self.pauseAction = False

        # Логгер.
        self.logger = Logger(game=game, save_frames=save_frames) if self.log else None

        # Цветовая раскраска.
        self.bg_color = (0, 0, 0)               # черный цвет (задний фон)
        self.cell_color = (0, 255, 0)           # зеленый цвет (живые клетки)
        self.grid_color = (50, 50, 50)          # темносерый цвет (линии сетки)
        self.text_color = (255, 255, 255)       # белый цвет (текст)
        self.processing_color = (255, 255, 0)   # желтый цвет (обрабатываемая клетка)

        # Инициализация PyGame.
        pygame.init()

        # Установка размеров и названия окна PyGame.
        self.screen_width = 900
        self.screen_height = 750
        self.info_width = 350
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        pygame.display.set_caption("4199 Горшков Ан.О., Игра \"Жизнь\" Конвея")
        self.clock = pygame.time.Clock()

        # Установка шрифта текста.
        self.font = pygame.font.SysFont("Arial", 16)

        # Размер клеток.
        max_grid_width = self.screen_width - self.info_width - 20
        max_grid_height = self.screen_height - 20
        self.cell_size = min(max_grid_width // self.game.width, max_grid_height // self.game.height)
        self._check_cell_size()

        # Смещения.
        self.grid_offset_x = (max_grid_width - self.game.width * self.cell_size) // 2
        self.grid_offset_y = (max_grid_height - self.game.height * self.cell_size) // 2

    def _check_cell_size(self):
        """ Проверка минимального размера клетки """
        MIN_CELL_SIZE = 5
        if self.cell_size < MIN_CELL_SIZE:
            err_str = f"[ERROR] Размер клетки слишком мал: {self.cell_size}px. Сетка слишком большая для окна {self.screen_width}x{self.screen_height}."
            print(err_str)
            if self.logger:
                self.logger.log_error(err_str)
                self.logger.close()
            pygame.quit()
            exit(1)

    def draw_grid(self):
        """ Визуализация линий сетки """
        for x in range(self.game.width + 1):
            pygame.draw.line(
                self.screen, self.grid_color,
                (self.grid_offset_x + x * self.cell_size, self.grid_offset_y),
                (self.grid_offset_x + x * self.cell_size, self.grid_offset_y + self.game.height * self.cell_size)
            )
        for y in range(self.game.height + 1):
            pygame.draw.line(
                self.screen, self.grid_color,
                (self.grid_offset_x, self.grid_offset_y + y * self.cell_size),
                (self.grid_offset_x + self.game.width * self.cell_size, self.grid_offset_y + y * self.cell_size)
            )

    def draw_cells(self):
        """ Визуализация матричной сетки из клеток с учетом текущего события """
        for i in range(self.game.height):
            for j in range(self.game.width):
                rect = pygame.Rect(
                    self.grid_offset_x + j * self.cell_size,
                    self.grid_offset_y + i * self.cell_size,
                    self.cell_size,
                    self.cell_size
                )
                if self.game.current_processing_cell == (i, j): # обрабатываемая клетка (i,j)
                    color = self.processing_color
                elif self.game.current_grid[i, j]:              # живая клетка (i,j)
                    color = self.cell_color
                else:                                           # мертвая клетка (i,j)
                    color = self.bg_color
                pygame.draw.rect(self.screen, color, rect)

    def draw_info(self):
        """ Визуализация текстовых надписей """
        # Вывод текстовой информации.
        info_lines = [
            "Управление:",
            "ESCAPE - Выход",
            "SPACE - Пауза/Продолжить",
            "ENTER - Следующее событие (при паузе)",
            f"{'-'*80}",
            "Параметры игры:",
            f"Матричная сетка: {self.game.height}x{self.game.width}",
            f"Частота обновления: {self.fps}",
            f"Инициализация живой клетки: p = {self.game.p}",
            f"Инициализация мертвой клетки: q = {1 - self.game.p}",
            f"Seed={self.game.seed}",
            f"{'-'*80}",
            "Статистика:",
            f"Поколение (модельное время): {self.model_time}",
            f"Живые клетки (количество): {self.game.alive_cells}/{self.game.height * self.game.width}",
            f"Мертвые клетки (количество): {self.game.dead_cells}/{self.game.height * self.game.width}",
            f"Родившиеся клетки (сейчас/вообще): {self.game.born_cells_now}/{self.game.born_cells_total}",
            f"Умершие клетки (сейчас/вообще): {self.game.died_cells_now}/{self.game.died_cells_total}",
            f"Событий типа I обработано: {self.game.processed_events_I}/{self.game.height * self.game.width}",
            f"Событий типа II обработано: {self.game.processed_events_II}"
        ]
        x_offset = self.screen_width - self.info_width + 10
        y_offset = 10
        for line in info_lines:
            text_surf = self.font.render(line, True, self.text_color)
            self.screen.blit(text_surf, (x_offset, y_offset))
            y_offset += 20

    def handle_events(self):
        """ Обработчик событий ввода (клавиши) """
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:       # ESCAPE
                    if self.logger:
                        self.logger.file.write(f"[LOG] Нажата клавиша ESCAPE [выход]\n")
                    return False
                elif event.key == pygame.K_SPACE:      # SPACE
                    self.paused = not self.paused
                    if self.logger:
                        self.logger.file.write(f"[LOG] Нажата клавиша SPACE [пауза={self.paused}]\n")
                elif event.key == pygame.K_RETURN:     # ENTER
                    if self.paused:
                        self.game._process_next_event()
                        self.model_time = self.game.event.time
                        self.pauseAction = True
                        if self.logger:
                            # Тип события.
                            ev_type = "I" if self.game.event.type == EventType.CELL_PROCESS else "II"
                            # Номер события.
                            if ev_type == "I":
                                ev_num = f"{self.game.processed_events_I}/{self.game.height * self.game.width}"
                            else:
                                ev_num = self.game.processed_events_II
                            self.logger.file.write(f"[LOG] Нажата клавиша ENTER [cледующее событие: тип {ev_type}, номер: {ev_num}]\n")
                            self.logger.log_grid(self.model_time)
        return True

    def run(self):
        """ Запуск визуализации игры """
        # Запуск визуализации игры.
        running = True
        while running:
            # Заполнение заднего фона.
            self.screen.fill(self.bg_color)
            # Обработка событий ввода.
            running = self.handle_events()
            # Следующее событие игры.
            if not self.paused:
                self.game._process_next_event()
                self.model_time = self.game.event.time
                if self.logger:
                    self.logger.log_grid(self.model_time)
            # Визуализация.
            self.draw_cells()
            self.draw_grid()
            self.draw_info()
            # Обновление визуализации.
            pygame.display.flip()
            # Сохранение кадра.
            if self.logger and (not self.paused or self.pauseAction):
                self.logger.save_frame(self.screen)
                self.pauseAction = False
            self.clock.tick(self.fps)
        # Завершение визуализации игры.
        pygame.quit()
        if self.logger:
            self.logger.close()
            print(f"[INFO] Результаты логирования сохранены в файл: {self.logger.log_file_path}")
            if self.logger.save_frames:
                print(f"[INFO] Кадры игры сохранены в: {self.logger.frames_dir}")

#********************************************************************************************************

#=================================#
#        ЗАПУСК ПРОГРАММЫ         #
#=================================#
if __name__ == "__main__":
    print(f"{'-'*100}")
    # -- Постоянные --
    GAME_HEIGHT = 15    # высота сетки
    GAME_WIDTH = 10     # ширина сетки
    P = 0.25            # доля живых клеток
    SEED = 42           # зерно случайности
    # Матричная сетка.
    game = GameOfLifeByEvents(height=GAME_HEIGHT, width=GAME_WIDTH)
    print(f"[INFO] Создана матричная сетка размерности {GAME_HEIGHT}x{GAME_WIDTH}.")
    # Инициализация живых клеток.
    game.init_random_cells_grid(p=P, seed=SEED)
    print(f"[INFO] Сетка инициализирована значениями 0 и 1 с долей живых клеток p = {P}, мертвых клеток q = {1-P} и seed={SEED}.")
    # Запуск визуализации.
    print("[INFO] Запущена визуализация...")
    app = GameOfLifeByEventsApp(game, fps=60, log=True, save_frames=True)
    app.run()
    print("[INFO] Визуализация завершена.")