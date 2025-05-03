
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

typedef enum { PASSIVE_NONE, PASSIVE_CRIT, PASSIVE_INSTA } PassiveType;
typedef struct {
    int id;
    char name[50];
    int price;
    int damage;
    PassiveType passiveType;
    int passiveChance; 
} Weapon;

#define NUM_WEAPONS 6
Weapon weapons[NUM_WEAPONS] = {
    {0, "Knuckle",            0,   5,  PASSIVE_NONE,  0},
    {1, "Guling",           150,  25,  PASSIVE_NONE,  0},
    {2, "Light Saber",      120,  20,  PASSIVE_CRIT,  10}, 
    {3, "Infinity Blade",   300,  50,  PASSIVE_INSTA, 10},
    {4, "Blade of Despair", 500,  80,  PASSIVE_CRIT,  50}, 
    {5, "Tung Tung Pentung", 50,  10,  PASSIVE_CRIT,  90}  
};


extern void send_msg(int sock, const char *msg);


void print_shop(int sock) {
    char buf[128];
    send_msg(sock, "\n=== WEAPON SHOP ===\n");
    for (int i = 0; i < NUM_WEAPONS; i++) {
        snprintf(buf, sizeof(buf),
            "[%d] %s - Price: %d gold, Damage: %d",
            weapons[i].id,
            weapons[i].name,
            weapons[i].price,
            weapons[i].damage
        );
        send_msg(sock, buf);
        if (weapons[i].passiveType == PASSIVE_CRIT) {
            snprintf(buf, sizeof(buf),
                " (Passive: %d%% Crit Chance)",
                weapons[i].passiveChance
            );
            send_msg(sock, buf);
        } else if (weapons[i].passiveType == PASSIVE_INSTA) {
            snprintf(buf, sizeof(buf),
                " (Passive: %d%% Instant Kill Chance)",
                weapons[i].passiveChance
            );
            send_msg(sock, buf);
        }
        send_msg(sock, "\n");
    }
    send_msg(sock, "Enter weapon number to buy (0 to cancel): ");
}

int is_valid_weapon(int id) {
    return id >= 0 && id < NUM_WEAPONS;
}

Weapon* get_weapon(int id) {
    return is_valid_weapon(id) ? &weapons[id] : NULL;
}
