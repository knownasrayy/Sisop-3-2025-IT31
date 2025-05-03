#include "shm_common.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void print_hunters(struct SystemData *sysdata) {
    printf("\n== HUNTER INFO ==\n");
    if (sysdata->num_hunters == 0) {
        printf("No hunters available.\n");
        return;
    }
    for (int i = 0; i < sysdata->num_hunters; i++) {
        struct Hunter h = sysdata->hunters[i];
        printf("Name: %s\tLevel: %d\tEXP: %d\tATK: %d\tHP: %d\tDEF: %d\tStatus: %s\n",
               h.username, h.level, h.exp, h.atk, h.hp, h.def,
               h.banned ? "BANNED" : "ACTIVE");
    }
}

void print_dungeons(struct SystemData *sysdata) {
    printf("\n== DUNGEON INFO ==\n");
    if (sysdata->num_dungeons == 0) {
        printf("No dungeons available.\n");
        return;
    }
    for (int i = 0; i < sysdata->num_dungeons; i++) {
        struct Dungeon d = sysdata->dungeons[i];
        printf("[Dungeon %d]\n", i + 1);
        printf("Name: %s\n", d.name);
        printf("Minimum Level: %d\n", d.min_level);
        printf("EXP Reward: %d\n", d.exp);
        printf("ATK: %d\n", d.atk);
        printf("HP: %d\n", d.hp);
        printf("DEF: %d\n", d.def);
        printf("Key: %d\n\n", d.shm_key);
    }
}

void generate_dungeon(struct SystemData *sysdata) {
    if (sysdata->num_dungeons >= MAX_DUNGEONS) {
        printf("Dungeon limit reached (%d).\n", MAX_DUNGEONS);
        return;
    }

    const char *names[] = {
        "Double Dungeon", "Demon Castle", "Pyramid Dungeon", "Red Gate Dungeon",
        "Hunters Guild Dungeon", "Busan A-Rank Dungeon", "Insects Dungeon",
        "Goblins Dungeon", "D-Rank Dungeon", "Gwanak Mountain Dungeon",
        "Hapjeong Subway Station Dungeon"
    };

    int idx = rand() % (sizeof(names) / sizeof(names[0]));
    struct Dungeon *d = &sysdata->dungeons[sysdata->num_dungeons];

    strcpy(d->name, names[idx]);
    d->min_level = rand() % 5 + 1;
    d->exp = rand() % 151 + 150;
    d->atk = rand() % 51 + 100;
    d->hp = rand() % 51 + 50;
    d->def = rand() % 26 + 25;
    d->shm_key = ftok("/tmp", rand() % 100 + 65);

    sysdata->num_dungeons++;

    printf("\nNew dungeon generated!\n");
    printf("Name: %s\n", d->name);
    printf("Minimum Level: %d\n", d->min_level);
}

void ban_hunter(struct SystemData *sysdata) {
    char name[50];
    printf("Enter hunter name to ban: ");
    scanf("%49s", name);
    for (int i = 0; i < sysdata->num_hunters; i++) {
        if (strcmp(sysdata->hunters[i].username, name) == 0) {
            sysdata->hunters[i].banned = 1;
            printf("Hunter %s has been banned.\n", name);
            return;
        }
    }
    printf("Hunter not found.\n");
}

void unban_hunter(struct SystemData *sysdata) {
    char name[50];
    printf("Enter hunter name to unban: ");
    scanf("%49s", name);
    for (int i = 0; i < sysdata->num_hunters; i++) {
        if (strcmp(sysdata->hunters[i].username, name) == 0) {
            sysdata->hunters[i].banned = 0;
            printf("Hunter %s has been unbanned.\n", name);
            return;
        }
    }
    printf("Hunter not found.\n");
}

void reset_hunter(struct SystemData *sysdata) {
    char name[50];
    printf("Enter hunter name to reset: ");
    scanf("%49s", name);
    for (int i = 0; i < sysdata->num_hunters; i++) {
        if (strcmp(sysdata->hunters[i].username, name) == 0) {
            sysdata->hunters[i].level = 1;
            sysdata->hunters[i].exp = 0;
            sysdata->hunters[i].atk = 10;
            sysdata->hunters[i].hp = 100;
            sysdata->hunters[i].def = 5;
            printf("Hunter %s has been reset.\n", name);
            return;
        }
    }
    printf("Hunter not found.\n");
}

int main() {
    srand(time(NULL));

    key_t key = get_system_key();
    int shmid = shmget(key, sizeof(struct SystemData), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    struct SystemData *sysdata = (struct SystemData *)shmat(shmid, NULL, 0);
    if (sysdata == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (sysdata->num_hunters == 0 && sysdata->num_dungeons == 0) {
        sysdata->num_hunters = 0;
        sysdata->num_dungeons = 0;
        printf("System initialized.\n");
    }

    int choice;
    while (1) {
        printf("\n== SYSTEM MENU ==\n");
        printf("1. Hunter Info\n");
        printf("2. Dungeon Info\n");
        printf("3. Generate Dungeon\n");
        printf("4. Ban Hunter\n");
        printf("5. Unban Hunter\n");
        printf("6. Reset Hunter Stats\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }
        getchar(); // flush newline

        switch (choice) {
            case 1: print_hunters(sysdata); break;
            case 2: print_dungeons(sysdata); break;
            case 3: generate_dungeon(sysdata); break;
            case 4: ban_hunter(sysdata); break;
            case 5: unban_hunter(sysdata); break;
            case 6: reset_hunter(sysdata); break;
            case 7:
                shmdt(sysdata);
                printf("System exited.\n");
                exit(0);
            default:
                printf("Invalid choice. Please select from 1 to 7.\n");
        }
    }

    return 0;
}
