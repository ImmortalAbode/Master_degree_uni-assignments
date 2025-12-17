import pygame
import numpy as np
import os
from datetime import datetime

#===========================#
#    КЛАСС ИГРЫ "ЖИЗНЬ"     #
#===========================#
class GameOfLife:
    """
        Класс игры "Жизнь" (клеточный автомат).
        ---------------------------------------
        height: размер матричной сетки (высота, число строк);
        width : размер матричной сетки (ширина, число столбцов).
    """
    def __init__(self, height, width):
        self.height = height
        self.width = width
        self.grid = np.zeros((height, width), dtype=np.uint8)

    def init_random_cells_grid(self, p=0.25, seed=None):
        """ Инициализация матричной сетки значениями 0 и 1 """
        self.p = p
        self.seed = seed
        rng = np.random.default_rng(seed)
        self.grid = (rng.random((self.height, self.width)) < p).astype(np.uint8)

    def _calc_alive_neighbors(self, i, j):
        """ Вычисление количества живых соседей текущей клетки (i,j) """
        alive_neighbors = 0
        # Обход соседей текущей клетки (i,j)
        for di in range (i-1, i+2):
            for dj in range(j-1, j+2):
                if 0 <= di < self.height and 0 <= dj < self.width:
                    alive_neighbors += self.grid[di, dj]
        # Сама клетка (i,j) не учитывается за своего живого соседа.
        alive_neighbors -= self.grid[i,j]

        return alive_neighbors

    def step(self):
        """
            Обновление матричной сетки за один шаг по правилам игры.
            --------------------------------------------------------
            Основные правила Конвея:
            • ячейка остается живой, если ее окружают две или три живые ячейки;
            • ячейка умирает от перенаселения, если ее окружают более трех живых ячеек;
            • ячейка умирает от изоляции, если у нее меньше двух живых соседей;
            • мертвая ячейка становится живой, если она имеет ровно три живых соседа.
        """
        new_grid = np.zeros((self.height, self.width), dtype=np.uint8)
        # Обход матричной сетки.
        for i in range(self.height):
            for j in range(self.width):
                # Вычисление живых соседей текущей клетки (i,j)
                neighbors = self._calc_alive_neighbors(i, j)
                # Применение правил перехода между состояниями с запоминанием новых состояний.
                if self.grid[i,j] == 1:
                    if neighbors == 2 or neighbors == 3:
                        new_grid[i,j] = 1
                else:
                    if neighbors == 3:
                        new_grid[i,j] = 1
        # Замена старых состояний новыми после обхода всех ячеек.
        self.grid = new_grid

#===========================#
#        ОТЧЕТНОСТЬ         #
#===========================#
class Logger:
    """
        Класс логирования.
        ------------------
        results_dir : имя директории с записью результатов логов;
        game        : объект класса GameOfLife;
        save_frames : флаг сохранения кадров визуализации.
    """
    def __init__(self, results_dir="results", game: GameOfLife | None=None, save_frames=False):
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
                        f"[LOG] Доля мертвых клеток: q = {1 - game.p}\n"
                        f"[LOG] SEED = {game.seed}\n"
                        f"{'='*100}\n")
        self.file.flush()

        # Директория кадровой записи.
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
        self.file.write(f"[LOG] Живые клетки (количество): {np.sum(self.game.grid)}/{self.game.height*self.game.width}\n")
        self.file.write(f"[LOG] Мертвые клетки (количество): {self.game.height*self.game.width - np.sum(self.game.grid)}/{self.game.height*self.game.width}\n\n")
        for row in self.game.grid:
            self.file.write(" ".join(str(cell) for cell in row) + "\n")
        self.file.write(f"{'-'*100}\n")
        self.file.flush()

    def log_error(self, message):
        """ Логирование ошибки """
        if not self.enabled:
            return
        self.file.write(f"\n[ERROR] {message}\n")
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

#===========================#
#   ВИЗУАЛИЗАЦИЯ (PyGame)   #
#===========================#
class GameOfLifeApp:
    """
        Класс визуализации модели игры "Жизнь" (клеточный автомат).
        -----------------------------------------------------------
        game        : объект класса GameOfLife;
        fps         : частота обновления визуализации;
        log         : флаг логирования;
        save_frames : флаг сохранения кадров визуализации (при условии, что log=True).
    """
    def __init__(self, game: GameOfLife, fps=10, log=False, save_frames=False):
        # Исходные данные.
        self.game = game
        self.fps = fps
        self.paused = True
        self.model_time = 0
        self.pauseAction = False

        # Логгер (отчетность).
        self.logger = Logger(game=game, save_frames=save_frames) if log else None

        # Цветовая раскраска.
        self.bg_color = (0, 0, 0)           # черный цвет (задний фон)
        self.cell_color = (0, 255, 0)       # зеленый цвет (живые клетки) 
        self.grid_color = (50, 50, 50)      # темносерый цвет (линии сетки) 
        self.text_color = (255, 255, 255)   # белый цвет (текст)

        # Инициализация PyGame.
        pygame.init()

        # Установка размеров и названия окна PyGame.
        self.screen_width = 900
        self.screen_height = 750
        self.info_width = 350
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        pygame.display.set_caption("4199 Горшков Ан.О. Игра \"Жизнь\" Конвея")
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
        """ Визуализация матричной сетки из клеток """
        for i in range(self.game.height):
            for j in range(self.game.width):
                if self.game.grid[i, j]:
                    rect = pygame.Rect(
                        self.grid_offset_x + j * self.cell_size,
                        self.grid_offset_y + i * self.cell_size,
                        self.cell_size,
                        self.cell_size
                    )
                    pygame.draw.rect(self.screen, self.cell_color, rect)

    def draw_info(self):
        """ Визуализация текстовых надписей """
        # Подсчет живых клеток
        alive_cells = np.sum(self.game.grid)
        info_lines = [
            "Управление:",
            "ESCAPE - Выход",
            "SPACE - Пауза/Продолжить",
            "ENTER - Шаг (при паузе)",
            "MouseButton - Инвертировать клетку (при паузе)",
            f"{'-'*80}",
            "Параметры игры:",
            f"Матричная сетка: {self.game.height}x{self.game.width}",
            f"Частота обновления: {self.fps}",
            f"Доля живых клеток: p = {self.game.p}",
            f"Доля мертвых клеток: q = {1 - self.game.p}",
            f"SEED = {self.game.seed}",
            f"{'-'*80}",
            "Статистика:",
            f"Поколение (модельное время): {self.model_time}",
            f"Живые клетки (количество)  : {alive_cells}/{self.game.height * self.game.width}",
            f"Мертвые клетки (количество): {self.game.height * self.game.width - alive_cells}/{self.game.height * self.game.width}"
        ]
        x_offset = self.screen_width - self.info_width + 10
        y_offset = 10
        for line in info_lines:
            text_surf = self.font.render(line, True, self.text_color)
            self.screen.blit(text_surf, (x_offset, y_offset))
            y_offset += 20

    def handle_events(self):
        """ Обработчик событий ввода (клавиши, мышь) """
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:    # ESCAPE
                    if self.logger:
                        self.logger.file.write(f"[LOG] Нажата клавиша ESCAPE [выход]\n")
                    return False
                elif event.key == pygame.K_SPACE:   # SPACE
                    self.paused = not self.paused
                    if self.logger:
                        self.logger.file.write(f"[LOG] Нажата клавиша SPACE [пауза={self.paused}]\n")
                elif event.key == pygame.K_RETURN:  # ENTER
                    if self.paused:
                        self.game.step()
                        self.model_time += 1
                        self.pauseAction = True
                        if self.logger:
                            self.logger.file.write(f"[LOG] Нажата клавиша ENTER [шаг сделан, поколение {self.model_time}]\n")
                            self.logger.log_grid(self.model_time)
            elif event.type == pygame.MOUSEBUTTONDOWN: # MOUSEBUTTON
                if self.paused:
                    x, y = pygame.mouse.get_pos()
                    j = (x - self.grid_offset_x) // self.cell_size
                    i = (y - self.grid_offset_y) // self.cell_size
                    self.pauseAction = True
                    if 0 <= i < self.game.height and 0 <= j < self.game.width:
                        self.game.grid[i, j] ^= 1 # XOR
                        if self.logger:
                            self.logger.file.write(f"[LOG] Клик мыши в клетку ({i}, {j}) [новое состояние {self.game.grid[i, j]}]\n")
                            self.logger.log_grid(self.model_time)
        return True

    def run(self):
        """ Запуск визуализации игры """
        # Инициализация (поколение 0).
        self.screen.fill(self.bg_color)
        self.draw_cells()
        self.draw_grid()
        self.draw_info()
        pygame.display.flip()
        if self.logger:
            self.logger.log_grid(self.model_time)
            self.logger.save_frame(self.screen)

        # Запуск визуализации игры.
        running = True
        while running:
            # Заполнение заднего фона.
            self.screen.fill(self.bg_color)
            # Обработка событий.
            running = self.handle_events()
            # Шаг.
            if not self.paused:
                self.game.step()
                self.model_time += 1
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

#===========================#
#     ЗАПУСК ПРОГРАММЫ      #
#===========================#
if __name__ == "__main__":
    print(f"{'-'*100}")
    # -- Постоянные --
    GAME_HEIGHT = 60    # высота сетки
    GAME_WIDTH = 50     # ширина сетки
    P = 0.25            # доля живых клеток
    SEED = 42           # зерно случайности
    # Матричная сетка.
    game = GameOfLife(height=GAME_HEIGHT, width=GAME_WIDTH)
    print(f"[INFO] Создана матричная сетка размерности {GAME_HEIGHT}x{GAME_WIDTH}.")
    # Инициализация живых клеток.
    game.init_random_cells_grid(p=P, seed=SEED)
    print(f"[INFO] Сетка инициализирована значениями 0 и 1 с долей живых клеток p = {P}, мертвых клеток q = {1 - P}  и seed = {SEED}.")
    # Запуск визуализации.
    print("[INFO] Запущена визуализация...")
    app = GameOfLifeApp(game, fps=10, log=True, save_frames=True)
    app.run()
    print("[INFO] Визуализация завершена.")