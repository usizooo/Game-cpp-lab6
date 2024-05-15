#include "mafia_game.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

MafiaGame::MafiaGame(int numPlayers) : numPlayers(numPlayers) {
    if (numPlayers < 5) {
        std::cerr << "Ошибка: количество игроков должно быть не менее 5." << std::endl;
        exit(1);
    }
    gameEnd = false;

    // Создаем игроков и присваиваем им номера
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(Player(std::to_string(i + 1))); // Присваиваем номера от 1 до numPlayers
    }

    // Гарантирование наличия ролей "Мафия", "Доктор" и "Детектив"
    mafiaCount = 1;
    citizenCount = numPlayers - 3;

    roles.reserve(numPlayers);
    roles.push_back(MAFIA); // Добавляем роль "Мафия"
    roles.push_back(DOCTOR); // Добавляем роль "Доктор"
    roles.push_back(DETECTIVE); // Добавляем роль "Детектив"

    // Добавляем оставшиеся мирные роли
    for (int i = 0; i < citizenCount; ++i) {
        roles.push_back(CITIZEN);
    }

    std::srand(std::time(nullptr));
    std::random_shuffle(roles.begin(), roles.end());

    for (int i = 0; i < numPlayers; ++i) {
        players[i].role = roles[i];
        if (roles[i] == DOCTOR) {
            doctorIndex = i;
        }
        else if (roles[i] == DETECTIVE) {
            detectiveIndex = i;
        }
    }
}


void MafiaGame::playGame() {
    while (!checkGameOver()) {
        performNightActions();
        if (checkGameOver()) break;
        performDayActions();
        if (checkGameOver()) break;
    }
    printGameResult();
}

bool MafiaGame::checkGameOver() {
    int mafiaAlive = 0, citizenAlive = 0;
    for (const auto& player : players) {
        if (player.alive) {
            if (player.role == MAFIA) {
                mafiaAlive++;
            }
            else {
                citizenAlive++;
            }
        }
    }
    if (mafiaAlive == 0) {
        std::cout << "Победа мирных жителей!" << std::endl;
        return true;
    }
    else if (mafiaAlive >= citizenAlive) {
        std::cout << "Победа мафии!" << std::endl;
        return true;
    }
    return false;
}

void MafiaGame::printGameResult() {
    std::cout << "Результаты игры:" << std::endl;
    for (const auto& player : players) {
        std::cout << player.name << " - ";
        if (player.role == MAFIA) {
            std::cout << "Мафия";
        }
        else if (player.role == CITIZEN) {
            std::cout << "Мирный житель";
        }
        else if (player.role == DOCTOR) {
            std::cout << "Доктор";
        }
        else if (player.role == DETECTIVE) {
            std::cout << "Детектив";
        }
        if (!player.alive) {
            std::cout << " (Мертв)";
        }
        std::cout << std::endl;
    }
}

void MafiaGame::performNightActions() {
    std::cout << "Наступает ночь..." << std::endl;

    // Мафия выбирает цель для убийства
    int mafiaPlayerIndex = -1;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].role == MAFIA) {
            mafiaPlayerIndex = i;
            break;
        }
    }

    if (mafiaPlayerIndex != -1) {
        int targetNumber;
        do {
            std::cout << "МАФИЯ (Игрок " << mafiaPlayerIndex + 1 << ") выбирает цель для убийства:" << std::endl;
            std::cout << "Введите номер цели для убийства: ";
            std::cin >> targetNumber;
        } while (targetNumber < 1 || targetNumber > numPlayers || targetNumber == mafiaPlayerIndex + 1);
        killedPlayerIndex = targetNumber - 1;
        players[killedPlayerIndex].alive = false; // Изменяем статус выбранного игрока на "мертвый"
        revealMafia(); // Раскрываем информацию о выборе мафии
    }
    // Доктор лечит игрока
    int doctorPlayerIndex = -1;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].role == DOCTOR) {
            doctorPlayerIndex = i;
            break;
        }
    }

    if (doctorPlayerIndex != -1 && doctorPlayerIndex != killedPlayerIndex) {
        std::cout << "ДОКТОР (Игрок " << doctorPlayerIndex + 1 << ") лечит игрока:" << std::endl;
        int healNumber;
        do {
            std::cout << "Введите номер игрока для лечения: ";
            std::cin >> healNumber;
        } while (healNumber < 1 || healNumber > numPlayers || healNumber == doctorPlayerIndex + 1);

        int healIndex = healNumber - 1;
        revealDoctorSave(healNumber); // Раскрываем информацию о лечении доктором

        if (healIndex == killedPlayerIndex) { // Если лечение применено к убитому игроку
            players[healIndex].alive = true; // Устанавливаем статус "живой"
            players[healIndex].aliveAfterNight = true; // Устанавливаем статус "живой после ночи"
        }
    }

    // Детектив проверяет роль игрока
    int detectivePlayerIndex = -1;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].role == DETECTIVE && players[i].alive) {
            detectivePlayerIndex = i;
            break;
        }
    }

    if (detectivePlayerIndex != -1 && players[detectivePlayerIndex].alive) {
        std::cout << "ДЕТЕКТИВ (Игрок " << detectivePlayerIndex + 1 << ") проверяет роль игрока:" << std::endl;
        int checkNumber;
        do {
            std::cout << "Введите номер игрока для проверки: ";
            std::cin >> checkNumber;
        } while (checkNumber < 1 || checkNumber > numPlayers || checkNumber == detectivePlayerIndex + 1);

        std::cout << "Детектив (Игрок " << detectivePlayerIndex + 1 << ") проверяет роль игрока " << checkNumber << ": ";
        Role checkedRole = players[checkNumber - 1].role;
        switch (checkedRole) {
        case MAFIA:
            std::cout << "Мафия" << std::endl;
            break;
        case CITIZEN:
            std::cout << "Мирный житель" << std::endl;
            break;
        case DOCTOR:
            std::cout << "Доктор" << std::endl;
            break;
        case DETECTIVE:
            std::cout << "Детектив" << std::endl;
            break;
        }

    }
}

void MafiaGame::performDayActions() {
    // Наступает день...
    std::cout << "Наступает день..." << std::endl;

    // Если кто-то был убит ночью, выводим сообщение об убийстве
    if (!players[killedPlayerIndex].aliveAfterNight) {
        std::cout << "Сегодня ночью был убит игрок " << killedPlayerIndex + 1 << "." << std::endl;
        players[killedPlayerIndex].alive = false;
    }


    // Голосование
    std::cout << "Голосование:" << std::endl;
    std::vector<int> votes(numPlayers, 0);
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].alive) { // Проверяем, что игрок жив
            int vote;
            bool invalidVote;
            do {
                invalidVote = false; // Сбрасываем флаг неправильного голоса перед каждым вводом
                std::cout << "Игрок " << i + 1 << ", введите номер игрока для голосования: ";
                std::cin >> vote;
                if (vote < 1 || vote > numPlayers) {
                    std::cout << "Неправильный номер игрока. Попробуйте снова." << std::endl;
                    invalidVote = true; // Устанавливаем флаг неправильного голоса
                }
                else if (vote == i + 1) {
                    std::cout << "Вы не можете голосовать за себя. Выберите другого игрока." << std::endl;
                    invalidVote = true; // Устанавливаем флаг неправильного голоса
                }
                else if (!players[vote - 1].alive) {
                    std::cout << "Этот игрок уже мертв. Выберите другого." << std::endl;
                    invalidVote = true; // Устанавливаем флаг неправильного голоса
                }
            } while (invalidVote); // Повторяем ввод, если был получен неправильный голос

            // Увеличиваем количество голосов за выбранного игрока
            votes[vote - 1]++;
        }
    }



    // Подсчитываем общее количество голосов
    int totalVotes = 0;
    for (int i = 0; i < votes.size(); ++i) {
        totalVotes += votes[i];
    }

    // Определяем порог голосов, который нужно превысить
    int threshold = totalVotes / 2;

    // Переменная для хранения индекса исключаемого игрока
    int excludedPlayerIndex = -1;

    // Исключаем игрока, если количество голосов за него превышает порог
    for (int i = 0; i < votes.size(); ++i) {
        if (players[i].alive && votes[i] > threshold) {
            players[i].alive = false;
            excludedPlayerIndex = i;
            std::cout << "Игрок " << i + 1 << " исключен." << std::endl;
            break; // Прерываем цикл после исключения первого игрока, чье количество голосов превысило порог
        }
    }

    // Если никто не превысил порог, никто не исключается
    if (excludedPlayerIndex == -1) {
        std::cout << "Никто не исключен." << std::endl;
    }

}

void MafiaGame::revealMafia() {
    std::cout << "Игрок " << players[killedPlayerIndex].name << " был убит мафией." << std::endl;
}


void MafiaGame::revealDoctorSave(int healNumber) {
    std::cout << "Игрок " << healNumber << " был спасен доктором." << std::endl;
}

void MafiaGame::excludePlayer() {
    auto killedPlayer = std::find_if(players.begin(), players.end(),
        [&](const Player& player) { return player.alive && &player == &players[killedPlayerIndex]; });

    if (killedPlayer == players.end()) {
        std::cout << "Ошибка: убитый игрок не найден." << std::endl;
        return;
    }

    killedPlayer->alive = false;
}

