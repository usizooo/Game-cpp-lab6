// mafia_game.h
#ifndef MAFIA_GAME_H
#define MAFIA_GAME_H

#include "player.h"
#include <vector>
#include <string>

class MafiaGame {
private:
    std::vector<Player> players;
    std::vector<Role> roles;
    int mafiaCount;
    int citizenCount;
    int doctorIndex;
    int detectiveIndex;
    int killedPlayerIndex;
    bool gameEnd;
    int numPlayers;

public:
    MafiaGame(int numPlayers);
    void playGame();
    bool checkGameOver();
    void printGameResult();
    void performNightActions();
    void performDayActions();
    bool votePlayer(const std::string& playerName, const std::string& targetName);
    void revealMafia();
    void revealDoctorSave(int healNumber);
    void excludePlayer();
};

#endif // MAFIA_GAME_H
