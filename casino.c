#define _CRT_SECURE_NO_WARNINGS
#include "casino.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char* PLAYERS_FILENAME = "players.dat";
static const char* GAMES_FILENAME = "games.dat";

static void logOperation(const char* op) {
    printf("[LOG] %s\n", op);
}

Player* players = NULL;
Game* games = NULL;
int playerCount = 0;
int gameCount = 0;
int nextPlayerID = 1;
int nextGameID = 1;

void freeMemory() {
    free(players);
    free(games);
    players = NULL;
    games = NULL;
}

void addPlayer() {
    Player* tmp = realloc(players, (playerCount + 1) * sizeof(Player));
    if (!tmp) {
        perror("Neuspjesno alociranje memorije za igrace");
        exit(EXIT_FAILURE);
    }
    players = tmp;

    Player* p = &players[playerCount];
    p->playerID = nextPlayerID++;
    printf("Unesite ime igraca: ");
    (void)scanf("%49s", p->firstName);
    printf("Unesite prezime igraca: ");
    (void)scanf("%49s", p->lastName);
    printf("Unesite saldo: ");
    (void)scanf("%lf", &p->balance);

    playerCount++;
    savePlayers();
    printf("Igrac dodan: %s %s (ID: %d)\n", p->firstName, p->lastName, p->playerID);
}

void listPlayers() {
    printf("\nLista svih igrača:\n");
    for (int i = 0; i < playerCount; i++) {
        printf("%d. %s %s - Saldo: %.2lf\n",
            players[i].playerID,
            players[i].firstName,
            players[i].lastName,
            players[i].balance);
    }
}

void updatePlayer() {
    int id;
    printf("Unesite ID igraca za azuriranje: ");
    (void)scanf("%d", &id);

    qsort(players, playerCount, sizeof(Player), comparePlayerByID);

    Player* p = findPlayerByID(id);
    if (p) {
        printf("Unesite novi saldo: ");
        (void)scanf("%lf", &p->balance);
        savePlayers();
        printf("Igrac azuriran.\n");
        return;
    }
    printf("Igrac s tim ID-om nije pronaden.\n");
}

void deletePlayer() {
    int id;
    printf("Unesite ID igraca za brisanje: ");
    (void)scanf("%d", &id);

    qsort(players, playerCount, sizeof(Player), comparePlayerByID);

    Player* p = findPlayerByID(id);
    if (p) {
        int idx = (int)(p - players);
        for (int j = idx; j < playerCount - 1; j++) {
            players[j] = players[j + 1];
        }
        playerCount--;
        savePlayers();
        printf("Igrac obrisan.\n");
        return;
    }
    printf("Igrac s tim ID-om nije pronaden.\n");
}

void savePlayers() {
    FILE* file = fopen(PLAYERS_FILENAME, "wb");
    if (!file) {
        perror("Greska pri otvaranju datoteke za zapis igraca");
        return;
    }
    fwrite(&playerCount, sizeof(int), 1, file);
    fwrite(players, sizeof(Player), playerCount, file);
    fclose(file);
}

void loadPlayers() {
    FILE* file = fopen(PLAYERS_FILENAME, "rb");
    if (!file) return;

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Greska prilikom pomicanja na kraj datoteke");
        fclose(file);
        return;
    }
    long fileSize = ftell(file);
    if (fileSize == -1L) {
        perror("Greska prilikom citanja pozicije u datoteci");
        fclose(file);
        return;
    }
    rewind(file);

    fread(&playerCount, sizeof(int), 1, file);
    players = malloc(playerCount * sizeof(Player));
    if (!players && playerCount > 0) {
        perror("Neuspjesno ucitavanje memorije za igrace");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fread(players, sizeof(Player), playerCount, file);
    nextPlayerID = playerCount > 0 ? players[playerCount - 1].playerID + 1 : 1;
    fclose(file);
    logOperation("Ucitani su podaci o igracima iz datoteke.");
}

int comparePlayerByID(const void* a, const void* b) {
    int idA = ((const Player*)a)->playerID;
    int idB = ((const Player*)b)->playerID;
    return idA - idB;
}

Player* findPlayerByID(int id) {
    Player key;
    key.playerID = id;
    return (Player*)bsearch(&key, players, playerCount, sizeof(Player), comparePlayerByID);
}

void recursivePrintPlayers(int index) {
    if (index >= playerCount)
        return;
    printf("%d. %s %s - Saldo: %.2lf\n",
        players[index].playerID,
        players[index].firstName,
        players[index].lastName,
        players[index].balance);
    recursivePrintPlayers(index + 1);
}
