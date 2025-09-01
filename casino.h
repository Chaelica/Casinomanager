#ifndef CASINO_H
#define CASINO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NAME_LENGTH 50
#define ERROR_EXIT 1
#define SAFE_FREE(p) do { if ((p) != NULL) { free(p); (p) = NULL; } } while(0)

typedef enum {
    SLOT_MACHINE,
    ROULETTE,
    BLACKJACK,
    POKER
} GameType;

typedef union {
    int spinNumber;
    char dealerName[MAX_NAME_LENGTH];
} BetData;

typedef struct {
    int playerID;
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    double balance;
} Player;

typedef struct {
    int gameID;
    char gameName[MAX_NAME_LENGTH];
    double minBet;
    double maxBet;
    GameType type;
    BetData extraData;
} Game;

typedef enum {
    DODAJ_IGRACA = 1,
    PRIKAZI_IGRACE,
    AZURIRAJ_IGRACA,
    OBRISI_IGRACA,
    DODAJ_IGRU,
    PRIKAZI_IGRE,
    AZURIRAJ_IGRU,
    OBRISI_IGRU,
    SORTIRAJ_IGRACE,
    PRETRAZI_IGRACE,
    PRINTAJ_REKURZIVNO,
    SORTIRAJ_IGRE,
    PREIMENUJ_DATOTEKU,
    OBRISI_DATOTEKU,
    ISPISI_MATRICU,
    SORTIRAJ_IGRACE_FP,
    IZLAZ
} MenuOption;

extern Player* players;
extern Game* games;
extern int playerCount;
extern int gameCount;
extern int nextPlayerID;
extern int nextGameID;

void addPlayer();
void listPlayers();
void updatePlayer();
void deletePlayer();
void savePlayers();
void loadPlayers();
void sortPlayers();

int comparePlayerByID(const void* a, const void* b);
Player* findPlayerByID(int id);
Player* searchPlayerBinary(int id);
void recursivePrintPlayers(int index);

void addGame();
void listGames();
void updateGame();
void deleteGame();
void saveGames();
void loadGames();
void sortGames();

void freeMemory();

void renameFile(const char* oldName, const char* newName);
void deleteFile(const char* filename);

typedef int (*CompareFunc)(const void*, const void*);
void sortPlayersWithFunctionPointer(CompareFunc cmp);

int compareByBalance(const void* a, const void* b);

#endif
