#define _CRT_SECURE_NO_WARNINGS
#include "casino.h"

int main() {
    loadPlayers();
    loadGames();

    int choice;
    do {
        printf("\n=== Casino Game Manager ===\n");
        printf("%d. Dodaj igraca\n", DODAJ_IGRACA);
        printf("%d. Prikazi sve igrace\n", PRIKAZI_IGRACE);
        printf("%d. Azuriraj saldo igraca\n", AZURIRAJ_IGRACA);
        printf("%d. Obrisi igraca\n", OBRISI_IGRACA);
        printf("%d. Dodaj igru\n", DODAJ_IGRU);
        printf("%d. Prikazi sve igre\n", PRIKAZI_IGRE);
        printf("%d. Azuriraj igru\n", AZURIRAJ_IGRU);
        printf("%d. Obrisi igru\n", OBRISI_IGRU);
        printf("%d. Sortiraj igrace po saldu (qsort)\n", SORTIRAJ_IGRACE);
        printf("%d. Pretrazi igraca po ID-u (bsearch)\n", PRETRAZI_IGRACE);
        printf("%d. Ispis igraca rekurzijom\n", PRINTAJ_REKURZIVNO);
        printf("%d. Sortiraj igre (qsort)\n", SORTIRAJ_IGRE);
        printf("%d. Preimenuj datoteku\n", PREIMENUJ_DATOTEKU);
        printf("%d. Obrisi datoteku\n", OBRISI_DATOTEKU);
        printf("%d. Sortiraj igrace funkcijskim pokazivacem\n", SORTIRAJ_IGRACE_FP);
        printf("%d. Izlaz\n", IZLAZ);
        printf("Odabir: ");
        (void)scanf("%d", &choice);
        (void)getchar();

        switch ((MenuOption)choice) {
        case DODAJ_IGRACA:             addPlayer(); break;
        case PRIKAZI_IGRACE:           listPlayers(); break;
        case AZURIRAJ_IGRACA:          updatePlayer(); break;
        case OBRISI_IGRACA:            deletePlayer(); break;
        case DODAJ_IGRU:               addGame(); break;
        case PRIKAZI_IGRE:             listGames(); break;
        case AZURIRAJ_IGRU:            updateGame(); break;
        case OBRISI_IGRU:              deleteGame(); break;
        case SORTIRAJ_IGRACE:          sortPlayers(); break;
        case PRETRAZI_IGRACE: {
            int id;
            printf("Unesite ID igraca za pretragu: ");
            (void)scanf("%d", &id);
            Player* p = searchPlayerBinary(id);
            if (p)
                printf("Pronadjen: %s %s - Saldo: %.2lf\n", p->firstName, p->lastName, p->balance);
            else
                printf("Igrac nije pronadjen.\n");
            break;
        }
        case PRINTAJ_REKURZIVNO:       recursivePrintPlayers(0); break;
        case SORTIRAJ_IGRE:            sortGames(); break;
        case PREIMENUJ_DATOTEKU: {
            char oldName[100], newName[100];
            printf("Unesite staro ime datoteke: ");
            (void)scanf("%99s", oldName);
            printf("Unesite novo ime datoteke: ");
            (void)scanf("%99s", newName);
            renameFile(oldName, newName);
            break;
        }
        case OBRISI_DATOTEKU: {
            char filename[100];
            printf("Unesite ime datoteke za brisanje: ");
            (void)scanf("%99s", filename);
            deleteFile(filename);
            break;
        }
        case SORTIRAJ_IGRACE_FP:       sortPlayersWithFunctionPointer(compareByBalance); break;
        case IZLAZ:                    break;
        default:                       printf("Nevazeci odabir.\n");
        }
    } while (choice != IZLAZ);

    freeMemory();
    return 0;
}
