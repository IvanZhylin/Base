/**
 * @file main.cpp
 * @brief Главный файл программы TicTacToe
 * @details Создает экземпляры классов и запускает игру
 */

#include "../include/TicTacToe.h"
#include <iostream>
#include <memory>

/**
 * @brief Точка входа в программу
 * @return Код завершения программы
 */
int main() {
    try {
        // Создаем зависимости с помощью умных указателей
        auto board = std::make_shared<GameBoard>();
        auto gameLogic = std::make_shared<StandardGameLogic>(board);
        auto inputHandler = std::make_shared<ConsoleInputHandler>(board);
        auto outputHandler = std::make_shared<ConsoleOutputHandler>(board);
        
        // Создаем и запускаем игру
        Game game(board, gameLogic, inputHandler, outputHandler);
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}