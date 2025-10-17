/**
 * @file TicTacToe.cpp
 * @brief Реализация игры TicTacToe
 * @details Содержит реализации всех классов для игры в крестики-нолики
 */

#include "../include/TicTacToe.h"
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
//#include <algorithm>
#include <string>

const int WINCONDITION = 3;

// Реализация GameBoard
void GameBoard::initialize(int size) {
    size_ = size;
    grid_.resize(size, std::vector<Player>(size, Player::NONE));
}

bool GameBoard::isValidMove(int row, int col) const {
    return row >= 0 && row < size_ && col >= 0 && col < size_ && grid_[row][col] == Player::NONE;
}



bool GameBoard::makeMove(int row, int col, Player player) {
    if (!isValidMove(row, col)) {
        return false;
    }
    grid_[row][col] = player;
    return true;
}

bool GameBoard::isFull() const {
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            if (cell == Player::NONE) {
                return false;
            }
        }
    }
    return true;
}

char GameBoard::getPlayerSymbol(Player player) const {
    switch (player) {
        case Player::X: return 'X';
        case Player::O: return 'O';
        default: return '-';
    }
}

int GameBoard::getSize() const {
    return size_;
}

Player GameBoard::getCell(int row, int col) const {
    if (row >= 0 && row < size_ && col >= 0 && col < size_) {
        return grid_[row][col];
    }
    return Player::NONE;
}

// реализация StandartGameLogic
StandardGameLogic::StandardGameLogic(std::shared_ptr<Board> board) : board_(std::move(board)) {}

bool StandardGameLogic::checkWin(int row, int col, Player player) const {
    // Проверка направлений
    return checkLine(row, col, 1, 0, player) || // вертикаль
           checkLine(row, col, 0, 1, player) || // горизонталь
           checkLine(row, col, 1, 1, player) || // диагональ пв-лн
           checkLine(row, col, 1, -1, player); // диагональ пн-л
}

int StandardGameLogic::getWinCondition() const {
    return WINCONDITION;
}

bool StandardGameLogic::checkLine(int row, int col, int dr, int dc, Player player) const {
    int count = 0;
    const int WinCondition = getWinCondition();
    
    //Проверка в обе стороны от текущей позиции
    for (int i = -WinCondition +1; i < WinCondition; i++) {
        int r = row + i * dr;
        int c = col + i * dc;

        if (board_->getCell(r, c) == player) {
            count++;
            if (count >= WinCondition) {
                return true;
            }
        } else {
            count = 0;
        }
    }
    
    return false;
}

// реализация ConsoleInputHandler
ConsoleInputHandler::ConsoleInputHandler(std::shared_ptr<Board> board) : board_(std::move(board)) {}

int ConsoleInputHandler::getBoardSize() {
    int size = 0;
    while (size < 3) {
        std:: cout << "Введи размер поля(минимум 3): ";
        std:: string input;
        std::getline(std::cin, input);

        try {
            size = std::stoi(input);
            if (size < 3) {
                std::cout << "Размер должен быть минимум 3. Попробуйте снова. \n";
            }
        } catch (const std::exception&) {
            std::cout << "Неверный ввод. Пожалуйста, введите число. \n";
        }
    }
    return size;
}

bool ConsoleInputHandler::getPlayerMove(Player curentPlayer, int& row, int& col) {
    std::string input;
    std::cout << "Игрок " << board_->getPlayerSymbol(curentPlayer)
              << ", введи свой ход (row, col): ";
    std::getline(std::cin, input);

    if (!parseMove(input, row, col)) {
        std::cout << "Неверный формат.\n";
        return false;
    }

    // Переход к нидексным значениям
    int rowIndex = row--;
    int colIndex = col--;

    if (!board_->isValidMove(row, col)) {
        std::cout << "Неверный ход. Клетке занята или находится вне поля. \n";
        return false;
    }

    return true;
}

bool ConsoleInputHandler::parseMove(const std::string& input, int& row, int& col) const {
    std::istringstream iss(input);
    return (iss >> row >> col) && (iss.eof());
}

// реализация ConsoleOutputHandlet
ConsoleOutputHandler::ConsoleOutputHandler(std::shared_ptr<Board> board) : board_(std::move(board)) {}

void ConsoleOutputHandler::displayBoard() const {
    const int size = board_->getSize();

    // Отображение колонки
    std::cout << "\n ";
    for (int col = 0; col < size; col++) {
        std::cout << " " << col + 1 << " ";
    }
    std::cout << "\n";

    // Отображение строк и клеток
    for (int row = 0; row < size; row++) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < size; col++) {
            std::cout << " " << board_->getPlayerSymbol(board_->getCell(row, col)) << " ";
            if (col < size -1) {
                std::cout << "|";
            }
        }
        std::cout << "\n";
        
        if (row < size -1) {
            std::cout << " ";
            for (int col = 0; col < size; col++) {
                std::cout << "---";
                if (col < size - 1) {
                    std::cout << "+";
                }
            }
            std::cout << "\n";
        } 
    }
    std::cout << "\n";
}

void ConsoleOutputHandler::displayWelcome() const {
    std::cout << "=== Добро пожаловать в крестики-нолики! ===\n";
    std::cout << "Игроки буду ходить и размещать на поле свои символыю\n";
    std::cout << "Первый кто получит " << WINCONDITION << " победит!\n\n";
}

void ConsoleOutputHandler::displayWinner(Player player) const {
    std::cout << "Игрок " << board_->getPlayerSymbol(player) << " победил!\n";
}

void ConsoleOutputHandler::displayDraw() const {
    std::cout << "Ничья!\n";
}

void ConsoleOutputHandler::displayError(const std::string& message) const {
    std::cout << "Ошибка: " << message << "\n";
}

void ConsoleOutputHandler::displayCurrentPlayer(Player player) const {
    std::cout << "Текущий игрок: " << board_->getPlayerSymbol(player) << "\n";
}

// реализация Game
Game::Game(std::shared_ptr<Board> board, 
            std::shared_ptr<GameLogic> gameLogic,
            std::shared_ptr<InputHandler> inputHandler,
            std::shared_ptr<OutputHandler> outputHandler)
        : board_(std::move(board))
        , gameLogic_(std::move(gameLogic))
        , inputHandler_(std::move(inputHandler))
        , outputHandler_(outputHandler)
        , currentPlayer_(Player::X)
        , gameOver_(false) {}

void Game::run() {
    outputHandler_->displayWelcome();
    initializeGame();

    while (!gameOver_) {
        if (!playRound()) {
            break;
        }
    }
}

void Game::switchPlayer() {
    currentPlayer_ = (currentPlayer_ == Player::X) ? Player::O : Player::X;
}

void Game::initializeGame() {
    int size = inputHandler_->getBoardSize();
    board_->initialize(size);
    gameOver_ = false;
    currentPlayer_ = Player::X;
}

bool Game::playRound() {
    outputHandler_->displayBoard();
    outputHandler_->displayCurrentPlayer(currentPlayer_);
    
    int row, col;
    while (!inputHandler_->getPlayerMove(currentPlayer_, row, col)) {
        // Продолжаем запрашивать ввод, пока не получим корректный
        
    }
    
    if (!board_->makeMove(row, col, currentPlayer_)) {
        std::cout << row << "-" << col;
        outputHandler_->displayError("Ошибка при совершении хода");
        return false;
    }
    
    if (checkGameOver(row, col)) {
        outputHandler_->displayBoard();
        return false;
    }
    
    switchPlayer();
    return true;
}

bool Game::checkGameOver(int lastRow, int lastCol) {
    if (gameLogic_->checkWin(lastRow, lastCol, currentPlayer_)) {
        outputHandler_->displayWinner(currentPlayer_);
        gameOver_ = true;
        return true;
    }

    if (board_->isFull()) {
        outputHandler_->displayDraw();
        gameOver_ = true;
        return true;
    }

    return false;
}