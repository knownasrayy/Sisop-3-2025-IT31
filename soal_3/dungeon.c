#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef enum { PASSIVE_NONE, PASSIVE_CRIT, PASSIVE_INSTA } PassiveType;
typedef struct {
    int id;
    char name[50];
    int price;
    int damage;
    PassiveType passiveType;
    int passiveChance;
} Weapon;
extern Weapon weapons[];
extern void print_shop(int sock);
extern int  is_valid_weapon(int id);
extern Weapon* get_weapon(int id);

#define PORT        12345
#define MAX_CLIENTS 10
#define BUF_SIZE    1024

typedef struct {
    int sock;
    struct sockaddr_in addr;
} Client;

typedef struct {
    int    gold;
    Weapon *equipped;
    Weapon *inventory[20];
    int    inv_count;
    int    kills;
} Player;

void send_msg(int sock, const char *msg) {
    send(sock, msg, strlen(msg), 0);
}

int recv_line(int sock, char *buf, size_t len) {
    int total = 0;
    while (total < (int)len - 1) {
        char c;
        int n = recv(sock, &c, 1, 0);
        if (n <= 0) return -1;
        if (c == '\r') continue;
        if (c == '\n') break;
        buf[total++] = c;
    }
    buf[total] = '\0';
    return total;
}

void print_menu(int sock) {
    const char *m =
      "\n===== MAIN MENU =====\n"
      "1. Show Player Stats\n"
      "2. Shop (Buy Weapons)\n"
      "3. View Inventory & Equip Weapons\n"
      "4. Battle Mode\n"
      "5. Exit Game\n"
      "Choose an option: ";
    send_msg(sock, m);
}

void handle_show_stats(int sock, Player *p) {
    char tmp[BUF_SIZE];
    snprintf(tmp, sizeof(tmp),
        "\n=== PLAYER STATS ===\n"
        "Gold: %d | Equipped Weapon: %s | Base Damage: %d | Kills: %d",
        p->gold, p->equipped->name, p->equipped->damage, p->kills
    );
    send_msg(sock, tmp);
    if (p->equipped->passiveType == PASSIVE_CRIT) {
        snprintf(tmp, sizeof(tmp),
            " | Passive: Increased Crit Chance (%d%%)\n",
            p->equipped->passiveChance
        );
        send_msg(sock, tmp);
    } else if (p->equipped->passiveType == PASSIVE_INSTA) {
        snprintf(tmp, sizeof(tmp),
            " | Passive: %d%% Instant Kill Chance\n",
            p->equipped->passiveChance
        );
        send_msg(sock, tmp);
    } else {
        send_msg(sock, "\n");
    }
}

void handle_shop(int sock, Player *p) {
    char buf[BUF_SIZE], in[BUF_SIZE];
    print_shop(sock);
    if (recv_line(sock, in, sizeof(in)) <= 0) return;
    int wid = atoi(in);
    if (wid == 0) {
        send_msg(sock, "Cancelled.\n");
        return;
    }
    if (!is_valid_weapon(wid)) {
        send_msg(sock, "Invalid option. Please try again.\n");
        return;
    }
    Weapon *w = get_weapon(wid);
    if (p->gold < w->price) {
        send_msg(sock, "Not enough gold.\n");
        return;
    }
    p->gold -= w->price;
    if (p->inv_count < 20) {
        p->inventory[p->inv_count++] = w;
        snprintf(buf, sizeof(buf), "Purchased %s!\n", w->name);
        send_msg(sock, buf);
    } else {
        send_msg(sock, "Inventory full.\n");
    }
}

void handle_inventory(int sock, Player *p) {
    char buf[BUF_SIZE], in[BUF_SIZE];
    send_msg(sock, "\n=== YOUR INVENTORY ===\n");
    for (int i = 0; i < p->inv_count; i++) {
        snprintf(buf, sizeof(buf), "[%d] %s", i, p->inventory[i]->name);
        send_msg(sock, buf);
        if (p->inventory[i]->passiveType == PASSIVE_CRIT) {
            snprintf(buf, sizeof(buf),
                " (Passive: %d%% Crit Chance)",
                p->inventory[i]->passiveChance
            );
            send_msg(sock, buf);
        } else if (p->inventory[i]->passiveType == PASSIVE_INSTA) {
            snprintf(buf, sizeof(buf),
                " (Passive: %d%% Instant Kill Chance)",
                p->inventory[i]->passiveChance
            );
            send_msg(sock, buf);
        }
        if (p->inventory[i] == p->equipped) send_msg(sock, " (EQUIPPED)");
        send_msg(sock, "\n");
    }
    send_msg(sock, "Enter weapon number to equip (or -1 to cancel): ");
    if (recv_line(sock, in, sizeof(in)) <= 0) return;
    int idx = atoi(in);
    if (idx < 0 || idx >= p->inv_count) {
        send_msg(sock, "Invalid option. Please try again.\n");
        return;
    }
    p->equipped = p->inventory[idx];
    send_msg(sock, "Weapon equipped!\n");
}

void handle_battle(int sock, Player *p) {
    char buf[BUF_SIZE], in[BUF_SIZE];
    int enemy_hp = rand() % 151 + 50;
    int max_hp   = enemy_hp;
    const int barWidth = 20;

    send_msg(sock, "\n=== BATTLE STARTED ===\n");
    while (1) {
        // build health bar
        int filled = (int)((float)enemy_hp / max_hp * barWidth);
        if (filled < 0) filled = 0;
        char bar[barWidth+1];
        for (int i = 0; i < barWidth; i++)
            bar[i] = (i < filled ? '#' : ' ');
        bar[barWidth] = '\0';

        snprintf(buf, sizeof(buf),
            "Enemy health: [%s] %d/%d HP\n"
            "Type 'attack' to attack or 'exit' to leave battle.\n> ",
            bar, enemy_hp, max_hp
        );
        send_msg(sock, buf);

        if (recv_line(sock, in, sizeof(in)) <= 0) return;
        if (strcmp(in, "exit") == 0) {
            send_msg(sock, "Exiting battle...\n");
            return;
        } else if (strcmp(in, "attack") != 0) {
            send_msg(sock, "Invalid option. Please try again.\n");
            continue;
        }

        int damage = p->equipped->damage + rand() % (p->equipped->damage + 1);

        if (p->equipped->passiveType == PASSIVE_INSTA &&
            rand() % 100 < p->equipped->passiveChance) {
            snprintf(buf, sizeof(buf),
                "\n=== INSTANT KILL! ===\n"
                "Your %s unleashed a beam of pure energy!\n"
                "Enemy was instantly destroyed!\n",
                p->equipped->name
            );
            send_msg(sock, buf);
            enemy_hp = 0;
        } else {
            int critChance = (p->equipped->passiveType == PASSIVE_CRIT)
                             ? p->equipped->passiveChance
                             : 10;
            if (rand() % 100 < critChance) {
                damage *= 2;
                snprintf(buf, sizeof(buf),
                    "\n=== CRITICAL HIT! ===\nYou dealt %d damage!\n",
                    damage
                );
                send_msg(sock, buf);
            } else {
                snprintf(buf, sizeof(buf),
                    "\nYou dealt %d damage!\n", damage
                );
                send_msg(sock, buf);
            }
            enemy_hp -= damage;
        }

        if (enemy_hp <= 0) {
            p->kills++;
            int reward = rand() % 151 + 50;
            p->gold += reward;
            snprintf(buf, sizeof(buf),
                "\nYou earned %d gold!\n"
                "=== NEW ENEMY ===\n",
                reward
            );
            send_msg(sock, buf);
            enemy_hp = rand() % 151 + 50;
            max_hp   = enemy_hp;
        }
    }
}

void* client_handler(void *arg) {
    Client *c = arg;
    Player p = { .gold = 500, .equipped = &weapons[0], .inv_count = 1, .kills = 0 };
    p.inventory[0] = &weapons[0];

    srand(time(NULL) ^ c->sock);
    send_msg(c->sock, "Connected to the dungeon server!\n");

    while (1) {
        print_menu(c->sock);
        char line[BUF_SIZE];
        if (recv_line(c->sock, line, sizeof(line)) <= 0) break;
        int opt = atoi(line);
        switch (opt) {
            case 1: handle_show_stats(c->sock, &p);    break;
            case 2: handle_shop(c->sock,       &p);    break;
            case 3: handle_inventory(c->sock, &p);    break;
            case 4: handle_battle(c->sock,    &p);    break;
            case 5:
                send_msg(c->sock, "Goodbye!\n");
                close(c->sock);
                free(c);
                return NULL;
            default:
                send_msg(c->sock, "Invalid option. Please try again.\n");
        }
    }
    close(c->sock);
    free(c);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    int yes = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = htons(PORT)
    };
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen"); exit(1);
    }

    printf("Dungeon server running on port %d...\n", PORT);
    while (1) {
        Client *c = malloc(sizeof(Client));
        socklen_t len = sizeof(c->addr);
        c->sock = accept(server_fd, (struct sockaddr*)&c->addr, &len);
        if (c->sock < 0) {
            perror("accept");
            free(c);
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, c);
        pthread_detach(tid);
    }
    return 0;
}
