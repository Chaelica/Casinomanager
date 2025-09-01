#define _CRT_SECURE_NO_WARNINGS
#include "casino.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int comparePlayerByID(const void* a, const void* b);
Player* findPlayerByID(int id);

static int compareGameByID(const void* a, const void* b);
static Game* findGameByID(int id);

static const char* PLAYERS_FILENAME = "players.dat";
static const char* GAMES_FILENAME = "games.dat";

static void logOperation(const char* op) {
    printf("[LOG] %s\n", op);
}

/* Globalni podaci */
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
    playerCount = 0;
    gameCount = 0;
}


void addPlayer() {
    Player* tmp = (Player*)realloc(players, (playerCount + 1) * sizeof(Player));
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
    if (playerCount == 0) {
        printf("Nema igraca.\n");
        return;
    }
    printf("\nLista svih igraca:\n");
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

        if (playerCount == 0) {
            free(players);
            players = NULL;
        }
        else {
            Player* tmp = (Player*)realloc(players, playerCount * sizeof(Player));
            if (tmp) players = tmp;
        }

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
    (void)fwrite(&playerCount, sizeof(int), 1, file);
    if (playerCount > 0 && players) {
        (void)fwrite(players, sizeof(Player), (size_t)playerCount, file);
    }
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

    if (fread(&playerCount, sizeof(int), 1, file) != 1) {
        perror("Greska pri citanju broja igraca");
        fclose(file);
        return;
    }

    free(players);
    players = NULL;

    if (playerCount > 0) {
        players = (Player*)malloc(playerCount * sizeof(Player));
        if (!players) {
            perror("Neuspjesno alociranje memorije za igrace");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        if (fread(players, sizeof(Player), (size_t)playerCount, file) != (size_t)playerCount) {
            perror("Greska pri citanju igraca");
            free(players);
            players = NULL;
            playerCount = 0;
            fclose(file);
            return;
        }
        nextPlayerID = players[playerCount - 1].playerID + 1;
    }
    else {
        nextPlayerID = 1;
    }

    fclose(file);
    logOperation("Ucitani su podaci o igracima iz datoteke.");
}

int compareByBalance(const void* a, const void* b) {
    const Player* pa = (const Player*)a;
    const Player* pb = (const Player*)b;
    if (pa->balance < pb->balance) return -1;
    if (pa->balance > pb->balance) return 1;
    return 0;
}

void sortPlayers() {
    if (playerCount > 1 && players) {
        qsort(players, playerCount, sizeof(Player), compareByBalance);
        printf("Igraci sortirani po balansu (rastuce).\n");
    }
    else {
        printf("Nedovoljno igraca za sortiranje.\n");
    }
}

void sortPlayersWithFunctionPointer(CompareFunc cmp) {
    if (!cmp) cmp = compareByBalance;
    if (playerCount > 1 && players) {
        qsort(players, playerCount, sizeof(Player), cmp);
        printf("Igraci sortirani korisnickim komparatorom.\n");
    }
    else {
        printf("Nedovoljno igraca za sortiranje.\n");
    }
}

int comparePlayerByID(const void* a, const void* b) {
    int idA = ((const Player*)a)->playerID;
    int idB = ((const Player*)b)->playerID;
    return (idA > idB) - (idA < idB);
}

Player* findPlayerByID(int id) {
    Player key;
    memset(&key, 0, sizeof key);
    key.playerID = id;
    return (Player*)bsearch(&key, players, (size_t)playerCount, sizeof(Player), comparePlayerByID);
}

Player* searchPlayerBinary(int id) {
    if (playerCount <= 0) return NULL;
    qsort(players, playerCount, sizeof(Player), comparePlayerByID);
    return findPlayerByID(id);
}

void recursivePrintPlayers(int index) {
    if (index >= playerCount) return;
    printf("%d. %s %s - Saldo: %.2lf\n",
        players[index].playerID,
        players[index].firstName,
        players[index].lastName,
        players[index].balance);
    recursivePrintPlayers(index + 1);
}


void addGame() {
    Game* tmp = (Game*)realloc(games, (gameCount + 1) * sizeof(Game));
    if (!tmp) {
        perror("Neuspjesno alociranje memorije za igre");
        exit(EXIT_FAILURE);
    }
    games = tmp;

    Game* g = &games[gameCount];
    g->gameID = nextGameID++;

    printf("Unesite naziv igre: ");
    (void)scanf("%49s", g->gameName);
    printf("Unesite minimalni ulog: ");
    (void)scanf("%lf", &g->minBet);
    printf("Unesite maksimalni ulog: ");
    (void)scanf("%lf", &g->maxBet);

    int t;
    printf("Odaberi tip (0=SLOT_MACHINE, 1=ROULETTE, 2=BLACKJACK, 3=POKER): ");
    (void)scanf("%d", &t);
    if (t < 0 || t > 3) t = 0;
    g->type = (GameType)t;

    if (g->type == SLOT_MACHINE) {
        printf("Unesite broj okretaja (spinNumber): ");
        (void)scanf("%d", &g->extraData.spinNumber);
    }
    else {
        printf("Unesite ime djelitelja (dealerName): ");
        (void)scanf("%49s", g->extraData.dealerName);
    }

    gameCount++;
    saveGames();
    printf("Igra dodana: %s (ID: %d)\n", g->gameName, g->gameID);
}

void listGames() {
    if (gameCount == 0) {
        printf("Nema igara.\n");
        return;
    }
    printf("\nLista svih igara:\n");
    for (int i = 0; i < gameCount; i++) {
        printf("%d. %s [min: %.2lf, max: %.2lf, tip: %d]\n",
            games[i].gameID,
            games[i].gameName,
            games[i].minBet,
            games[i].maxBet,
            (int)games[i].type);
    }
}

void updateGame() {
    int id;
    printf("Unesite ID igre za azuriranje: ");
    (void)scanf("%d", &id);

    qsort(games, gameCount, sizeof(Game), compareGameByID);
    Game* g = findGameByID(id);
    if (!g) {
        printf("Igra s tim ID-om nije pronadena.\n");
        return;
    }

    printf("Unesite novi minimalni ulog: ");
    (void)scanf("%lf", &g->minBet);
    printf("Unesite novi maksimalni ulog: ");
    (void)scanf("%lf", &g->maxBet);
    saveGames();
    printf("Igra azurirana.\n");
}

void deleteGame() {
    int id;
    printf("Unesite ID igre za brisanje: ");
    (void)scanf("%d", &id);

    qsort(games, gameCount, sizeof(Game), compareGameByID);
    Game* g = findGameByID(id);
    if (!g) {
        printf("Igra s tim ID-om nije pronadena.\n");
        return;
    }

    int idx = (int)(g - games);
    for (int j = idx; j < gameCount - 1; j++) {
        games[j] = games[j + 1];
    }
    gameCount--;

    if (gameCount == 0) {
        free(games);
        games = NULL;
    }
    else {
        Game* tmp = (Game*)realloc(games, gameCount * sizeof(Game));
        if (tmp) games = tmp;
    }

    saveGames();
    printf("Igra obrisana.\n");
}

void saveGames() {
    FILE* file = fopen(GAMES_FILENAME, "wb");
    if (!file) {
        perror("Greska pri otvaranju datoteke za zapis igara");
        return;
    }
    (void)fwrite(&gameCount, sizeof(int), 1, file);
    if (gameCount > 0 && games) {
        (void)fwrite(games, sizeof(Game), (size_t)gameCount, file);
    }
    fclose(file);
}

void loadGames() {
    FILE* file = fopen(GAMES_FILENAME, "rb");
    if (!file) return;

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Greska prilikom pomicanja na kraj datoteke (igre)");
        fclose(file);
        return;
    }
    long fileSize = ftell(file);
    if (fileSize == -1L) {
        perror("Greska prilikom citanja pozicije u datoteci (igre)");
        fclose(file);
        return;
    }
    rewind(file);

    if (fread(&gameCount, sizeof(int), 1, file) != 1) {
        perror("Greska pri citanju broja igara");
        fclose(file);
        return;
    }

    free(games);
    games = NULL;

    if (gameCount > 0) {
        games = (Game*)malloc(gameCount * sizeof(Game));
        if (!games) {
            perror("Neuspjesno alociranje memorije za igre");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        if (fread(games, sizeof(Game), (size_t)gameCount, file) != (size_t)gameCount) {
            perror("Greska pri citanju igara");
            free(games);
            games = NULL;
            gameCount = 0;
            fclose(file);
            return;
        }
        nextGameID = games[gameCount - 1].gameID + 1;
    }
    else {
        nextGameID = 1;
    }

    fclose(file);
    logOperation("Ucitani su podaci o igrama iz datoteke.");
}

static int compareGameByID(const void* a, const void* b) {
    int idA = ((const Game*)a)->gameID;
    int idB = ((const Game*)b)->gameID;
    return (idA > idB) - (idA < idB);
}

static Game* findGameByID(int id) {
    Game key;
    memset(&key, 0, sizeof key);
    key.gameID = id;
    return (Game*)bsearch(&key, games, (size_t)gameCount, sizeof(Game), compareGameByID);
}

void sortGames() {
    if (gameCount > 1 && games) {
        qsort(games, gameCount, sizeof(Game), compareGameByID);
        printf("Igre sortirane po ID-u (rastuce).\n");
    }
    else {
        printf("Nedovoljno igara za sortiranje.\n");
    }
}


void renameFile(const char* oldName, const char* newName) {
    if (!oldName || !newName) {
        fprintf(stderr, "Neispravni parametri za renameFile.\n");
        return;
    }
    if (rename(oldName, newName) != 0) {
        perror("Greska pri preimenovanju datoteke");
    }
    else {
        printf("Datoteka preimenovana iz '%s' u '%s'.\n", oldName, newName);
    }
}

void deleteFile(const char* filename) {
    if (!filename) {
        fprintf(stderr, "Neispravan parametar za deleteFile.\n");
        return;
    }
    if (remove(filename) != 0) {
        perror("Greska pri brisanju datoteke");
    }
    else {
        printf("Datoteka '%s' obrisana.\n", filename);
    }
}
