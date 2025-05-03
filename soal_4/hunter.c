#include "shm_common.h"

void list_dungeons(struct SystemData *sysdata, struct Hunter *h) {
    printf("\n== AVAILABLE DUNGEONS ==\n");
    int count = 0;
    for (int i = 0; i < sysdata->num_dungeons; i++) {
        struct Dungeon d = sysdata->dungeons[i];
        if (h->level >= d.min_level) {
            printf("%d. %s\t(Level %d+)\n", ++count, d.name, d.min_level);
        }
    }

    if (count == 0) {
        printf("No dungeons available for your level.\n");
    }

    printf("\nPress enter to continue...");
    getchar(); getchar();
}

void raid_dungeon(struct SystemData *sysdata, struct Hunter *logged) {
    if (sysdata->num_dungeons == 0) {
        printf("No dungeons available in the system.\n");
        return;
    }

    printf("\n== RAIDABLE DUNGEONS ==\n");
    int options[MAX_DUNGEONS];
    int count = 0;

    for (int i = 0; i < sysdata->num_dungeons; i++) {
        struct Dungeon d = sysdata->dungeons[i];
        if (logged->level >= d.min_level) {
            printf("%d. %s\t(Level %d+)\n", count + 1, d.name, d.min_level);
            options[count++] = i;
        }
    }

    if (count == 0) {
        printf("No dungeons available for your level.\n");
        return;
    }

    int pick;
    printf("Choose Dungeon: ");
    scanf("%d", &pick);
    getchar(); // flush newline

    if (pick < 1 || pick > count) {
        printf("Invalid dungeon choice.\n");
        return;
    }

    struct Dungeon chosen = sysdata->dungeons[options[pick - 1]];

    logged->atk += chosen.atk;
    logged->hp += chosen.hp;
    logged->def += chosen.def;
    logged->exp += chosen.exp;

    printf("\nRaid success! Gained:\n");
    printf("ATK: %d\n", chosen.atk);
    printf("HP: %d\n", chosen.hp);
    printf("DEF: %d\n", chosen.def);
    printf("EXP: %d\n", chosen.exp);

    if (logged->exp >= 500) {
        logged->level += 1;
        logged->exp = 0;
        printf("\nCongratulations! You leveled up to Level %d!\n", logged->level);
    }

    for (int i = options[pick - 1]; i < sysdata->num_dungeons - 1; i++) {
        sysdata->dungeons[i] = sysdata->dungeons[i + 1];
    }
    sysdata->num_dungeons--;

    printf("\nPress enter to continue...");
    getchar();
}

void hunters_battle(struct SystemData *sysdata, struct Hunter *logged) {
    if (sysdata->num_hunters <= 1) {
        printf("No available opponents.\n");
        return;
    }

    printf("\n== AVAILABLE OPPONENTS ==\n");
    int indices[MAX_HUNTERS];
    int count = 0;

    for (int i = 0; i < sysdata->num_hunters; i++) {
        if (strcmp(sysdata->hunters[i].username, logged->username) != 0) {
            printf("%d. %s (Level %d) - Stats: ATK %d, HP %d, DEF %d\n",
                   count + 1,
                   sysdata->hunters[i].username,
                   sysdata->hunters[i].level,
                   sysdata->hunters[i].atk,
                   sysdata->hunters[i].hp,
                   sysdata->hunters[i].def);
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf("No valid opponents found.\n");
        return;
    }

    int pick;
    printf("Choose opponent: ");
    scanf("%d", &pick);
    getchar();

    if (pick < 1 || pick > count) {
        printf("Invalid opponent choice.\n");
        return;
    }

    struct Hunter *opponent = &sysdata->hunters[indices[pick - 1]];

    int logged_power = logged->atk + logged->hp + logged->def;
    int opponent_power = opponent->atk + opponent->hp + opponent->def;

    printf("\nYour Power: %d | Opponent Power: %d\n", logged_power, opponent_power);

    if (logged_power >= opponent_power) {
        logged->atk += opponent->atk;
        logged->hp += opponent->hp;
        logged->def += opponent->def;

        printf("You won the battle! Gained all stats from %s.\n", opponent->username);

        for (int i = indices[pick - 1]; i < sysdata->num_hunters - 1; i++) {
            sysdata->hunters[i] = sysdata->hunters[i + 1];
        }
        sysdata->num_hunters--;

    } else {
        opponent->atk += logged->atk;
        opponent->hp += logged->hp;
        opponent->def += logged->def;

        printf("You lost the battle! You have been eliminated.\n");

        for (int i = 0; i < sysdata->num_hunters; i++) {
            if (strcmp(sysdata->hunters[i].username, logged->username) == 0) {
                for (int j = i; j < sysdata->num_hunters - 1; j++) {
                    sysdata->hunters[j] = sysdata->hunters[j + 1];
                }
                sysdata->num_hunters--;
                break;
            }
        }

        printf("Press enter to return to main menu...\n");
        getchar();
        return;
    }

    printf("Press enter to continue...\n");
    getchar();
}

void show_notification(struct SystemData *sysdata, struct Hunter *logged) {
    printf("\n== NOTIFICATIONS ==\n");

    printf("Total Hunters: %d\n", sysdata->num_hunters);
    printf("Available Dungeons: %d\n", sysdata->num_dungeons);

    int max_level = logged->level;
    char top_hunter[50];
    strcpy(top_hunter, logged->username);

    for (int i = 0; i < sysdata->num_hunters; i++) {
        if (sysdata->hunters[i].level > max_level) {
            max_level = sysdata->hunters[i].level;
            strcpy(top_hunter, sysdata->hunters[i].username);
        }
    }

    printf("Top Hunter: %s (Level %d)\n", top_hunter, max_level);

    printf("\nPress enter to continue...");
    getchar(); getchar();
}

int main() {
    key_t key = get_system_key();
    int shmid = shmget(key, sizeof(struct SystemData), 0666);
    if (shmid == -1) {
        printf("System not running. Please run system first.\n");
        return 1;
    }

    struct SystemData *sysdata = (struct SystemData *) shmat(shmid, NULL, 0);
    if (sysdata == (void *) -1) {
        perror("shmat failed");
        return 1;
    }

    int choice;
    char username[50];

    while (1) {
        printf("\n== HUNTER MENU ==\n1. Register\n2. Login\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;

            int exists = 0;
            for (int i = 0; i < sysdata->num_hunters; i++) {
                if (strcmp(sysdata->hunters[i].username, username) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (exists) {
                printf("Username already registered.\n");
                continue;
            }

            struct Hunter *h = &sysdata->hunters[sysdata->num_hunters++];
            strcpy(h->username, username);
            h->level = 1;
            h->exp = 0;
            h->atk = 10;
            h->hp = 100;
            h->def = 5;
            h->banned = 0;
            h->shm_key = ftok("/tmp", username[0]);

            printf("Registration successful!\n");

        } else if (choice == 2) {
            printf("Username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;

            int found = -1;
            for (int i = 0; i < sysdata->num_hunters; i++) {
                if (strcmp(sysdata->hunters[i].username, username) == 0) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                printf("User not found.\n");
                continue;
            }

            struct Hunter *logged = &sysdata->hunters[found];

            if (logged->banned) {
                printf("Your account is banned.\n");
                continue;
            }

            printf("\nLogin successful! Welcome %s.\n", logged->username);

            int sub_choice;
            do {
                printf("\n== %s's MENU ==\n", logged->username);
                printf("1. Dungeon List\n");
                printf("2. Dungeon Raid\n");
                printf("3. Hunters Battle\n");
                printf("4. Notification\n");
                printf("5. Exit\n");
                printf("Choice: ");
                scanf("%d", &sub_choice);

                switch (sub_choice) {
                    case 1: list_dungeons(sysdata, logged); break;
                    case 2: raid_dungeon(sysdata, logged); break;
                    case 3: hunters_battle(sysdata, logged);
                            if (sysdata->num_hunters > 0 && strcmp(sysdata->hunters[found].username, logged->username) != 0) {
                                sub_choice = 5;
                            }
                            break;
                    case 4: show_notification(sysdata, logged); break;
                    case 5: printf("Logging out...\n"); break;
                    default: printf("Invalid choice.\n");
                }

            } while (sub_choice != 5);

        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
