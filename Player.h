#ifndef PLAYER_H
#define PLAYER_H

#include <string>

enum Role { MAFIA, CITIZEN, DOCTOR, DETECTIVE };

struct Player {
    std::string name;
    Role role;
    bool alive;
    bool aliveAfterNight; // ���� ��� ������������, ��� �� ����� ������ ��������

    Player(const std::string& playerName) : name(playerName), alive(true), aliveAfterNight(false) {}
};

#endif // PLAYER_H
