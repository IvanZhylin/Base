/**
* @file TicTacToe.h
* @brief Заголовочный файл для игры TicTacToe
* @details Содержит объявления классво для реализации игры в крестики-нолики
*/

#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>
#include <string>
#include <memory>

/**
 * @enum Player
 * @brief Перечисление игроков 
 */
enum class Player {
    NONE, ///< Пустая клетка
    X, ///< Игрок X
    O, ///< Игрок O
};

/**
 * @class Board
 * @brief Интерфейс игрового поля
 */
class Board {
public: 
    virtual ~Board() = default;

    /**
     * @brief Инициализация поля
     * @param size Размер поля
     */
    virtual void initialize(int size) = 0;

    /**
     * @brief Проверка правильности кода
     * @param row Строка
     * @param col Колонка
     * @return true Если ход правильный
     */
    virtual bool isValidMove(int row, int col) const = 0;

    /**
     * @brief Выполенение хода
     * @param row Строка
     * @param col Колонка
     * @param player Игрок
     * @return ture если ход выполнен успешно
     */
    virtual bool makeMove(int row, int col, Player player) = 0;

    /**
     * @brief Проверяет, заполнено ли поле
     * @return true, если поле заполнено
     */
    virtual bool isFull() const = 0;

    /**
     * @brief Получает символ игрока для отображения
     * @param player Игрок
     * @return Символ для отображения
     */
    virtual char getPlayerSymbol(Player player) const = 0;

    /**
     * @brief Получает размер поля
     * @return Размер поля
     */
    virtual int getSize() const = 0;

    /**
     * @brief Получает игрока в указанной клетке
     * @param row Строка
     * @param col Колонка
     * @return Игрок в клетке
     */
    virtual Player getCell(int row, int col) const = 0;
};

/**
 * @class GameLogic
 * @brief Интерфейс игровой логики
 */
class GameLogic {
public:
    virtual ~GameLogic() = default;

    /**
     * @brief Проверяет победу
     * @param row Строка последнего хода
     * @param col Колонка последнего хода
     * @param player Игрок
     * @return true, если игрок победил
     */
    virtual bool checkWin(int row, int col, Player player) const = 0;

    /**
     * @brief Получает количество клеток для победы
     * @return Количество клеток для победы
     */
    virtual int getWinCondition() const = 0;
};

/**
 * @class InputHandler
 * @brief Интерфейс обработки ввода
 */
class InputHandler {
public:
    virtual ~InputHandler() = default;

    /**
     * @brief Получает размер поля от пользователя
     * @return Размер поля
     */
    virtual int getBoardSize() = 0;

    /**
     * @brief Получает ход от пользователя
     * @param currentPlayer Текущий игрок
     * @param row Ссылка для сохранения строки
     * @param col Ссылка для сохранения колонки
     * @return true, если ввод корректен
     */
    virtual bool getPlayerMove(Player currentPlayer, int& row, int& col) = 0;
};

/**
 * @class OutputHandler
 * @brief Интерфейс вывода
 */
class OutputHandler{
public:
    virtual ~OutputHandler() = default;

    /**
     * @brief Отображает игровое поле
     */
    virtual void displayBoard() const = 0;

    /**
     * @brief Отображает приветствие 
     */
    virtual void displayWelcome() const = 0;

    /**
     * @brief Отображает приветсвие 
     * @param player Победитель
     */
    virtual void displayWinner(Player player) const = 0;

    /**
     * @brief Отображает сообщение о ничьей
     */
    virtual void displayDraw() const = 0;

    /**
     * @brief Отображает сообщение об ошибке
     * @param message Сообщение об ошибке
     */
    virtual void displayError(const std::string& message) const = 0;

    /**
     * @brief Отображает текущего игрока
     * @param player Текущий игрок
     */
    virtual void displayCurrentPlayer(Player player) const = 0;
};

/**
 * @class GameBoard
 * @brief Реализация игрового поля
 */
class GameBoard : public Board {
private:
    std::vector<std::vector<Player>> grid_;
    int size_;

public:
    void initialize(int size) override;
    bool isValidMove(int row, int col) const override;
    bool makeMove(int row, int col, Player player) override;
    bool isFull() const override;
    char getPlayerSymbol(Player player) const override;
    int getSize() const override;
    Player getCell(int row, int col) const override;
};

/**
 * @class StandardGameLogic
 * @brief Стфндартная игровая логика для TicTacToe
 */
class StandardGameLogic : public GameLogic{ 
private:
    std::shared_ptr<Board> board_;

public:
    explicit StandardGameLogic(std::shared_ptr<Board> board);
    bool checkWin(int row, int col, Player player) const override;
    int getWinCondition() const override;

private:
    bool checkLine(int row, int col, int dr, int dc, Player player) const;
};

/**
 * @class ConsoleInputHandler
 * @brief Обработчик ввода с консоли
 */
class ConsoleInputHandler : public InputHandler {
private:
    std::shared_ptr<Board> board_;

public:
    explicit ConsoleInputHandler(std::shared_ptr<Board> board);
    int getBoardSize() override;
    bool getPlayerMove(Player currentPlayer, int& row, int& col) override;

private: 
    bool parseMove(const std::string& input, int& row, int& col) const;
};

/**
 * @class ConsoleOutputHandler
 * @brief Обработчик вывода в консоль
 */
class ConsoleOutputHandler : public OutputHandler {
private:
    std::shared_ptr<Board> board_;
public:
    explicit ConsoleOutputHandler(std::shared_ptr<Board> board);
    void displayBoard() const override;
    void displayWelcome() const override;
    void displayDraw() const override;
    void displayWinner(Player player) const override;
    void displayError(const std::string& message)const override;
    void displayCurrentPlayer(Player player) const override;
};

/**
 * @class Game
 * @brief Основной класс игры
 */
class Game {
private:
    std::shared_ptr<Board> board_;
    std::shared_ptr<GameLogic> gameLogic_;
    std::shared_ptr<InputHandler> inputHandler_;
    std::shared_ptr<OutputHandler> outputHandler_;
    Player currentPlayer_;
    bool gameOver_;

public:
    Game(std::shared_ptr<Board> board_,
        std::shared_ptr<GameLogic> gameLogic_,
        std::shared_ptr<InputHandler> inputHandler_,
        std::shared_ptr<OutputHandler> outputHandler_);

    /**
     * @brief Запускает игру
     */
    void run();

    /**
     * @brief Переключение текущего игрока
     */
    void switchPlayer();

private:
    /**
     * @brief Инициализация игры
     */
    void initializeGame();

    /**
     * @brief Выполняет игровой раунд
     * @return true, если игра должна продолжиться
     */
    bool playRound();

    /**
     * @brief Проверяет окончание игры
     * @param lastRow Строка последнего хода
     * @param lastCol Колонка последнего хода
     * @return true, если игра окончена
     */
    bool checkGameOver(int lastRow, int lastCol);
};

#endif