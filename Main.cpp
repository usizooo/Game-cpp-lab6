// main.cpp
#include "mafia_game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int numPlayers;
    std::cout << "������� ���������� ������� (�� ����� 5): ";
    std::cin >> numPlayers;

    if (numPlayers < 5) {
        std::cerr << "������: ���������� ������� ������ ���� �� ����� 5." << std::endl;
        return 1;
    }

    MafiaGame game(numPlayers);
    game.playGame();

    return 0;
}
