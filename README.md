# Sisop-3-2025-IT31

- Erlangga Valdhio Putra Sulistio_5027241030
- Rayhan Agnan Kusuma_5027241102
- S. Farhan Baig_5027241097

## Soal_1

## Deskripsi Soal

Pada soal ini diminta untuk menyusun struktur folder dan file sesuai contoh dari situs deep web. Petunjuk yang diberikan merupakan file teks yang harus diubah menjadi file jpeg menggunakan sistem RPC.

##  Tujuan Soal Nomor 1a

- Download & extract file text dan jpeg dari situs deep web.
- Menyusun struktur direktori seperti yang dicantumkan di soal.

## Proses Pengerjaan

Berikut ini adalah langkah-langkah untuk menyusun struktur direktori sesuai soal:

### 1. Masuk ke folder kerja
Masuk ke direktori tempat saya ingin menyusun proyek:
```bash 
cd ~/Downloads/temp_secrets
```

### 2. Membuat folder sesuai struktur
Membuat semua folder yang dibutuhkan menggunakan mkdir:
```bash 
mkdir -p client/secrets
mkdir -p server/database
```

### 3. Menyalin/memindahkan file ke folder yang benar
Setelah file teks dan gambar saya ekstrak atau download secara manual, bisa kita pindahkan ke lokasi yang benar:
Pindahkan input teks ke client/secrets:
```bash 
mv input_1.txt input_2.txt input_3.txt input_4.txt input_5.txt client/secrets/
```

Pindahkan file JPEG ke client/:
```bash
mv 1744403652.jpeg 1744403687.jpeg client/
```

Duplikat file JPEG ke server/database:
```bash
cp client/1744403652.jpeg client/1744403687.jpeg server/database/
```

### 4. Menambahkan placeholder file
Jika belum compile file .c, kita bisa membuat placeholder dulu:
```bash
touch client/image_client
touch server/image_server
touch server/server.log
```

### 5. Mengecek struktur dengan tree
Dan untuk yang terakhir:
```bash
tree
```

Dan hasilnya sesuai dengan yang diinginkan:
```bash
.
├── client
│   ├── 1744403652.jpeg
│   ├── 1744403687.jpeg
│   ├── image_client
│   └── secrets
│       ├── input_1.txt
│       ├── input_2.txt
│       ├── input_3.txt
│       ├── input_4.txt
│       └── input_5.txt
├── image_client.c
├── image_server.c
└── server
    ├── database
    │   ├── 1744403652.jpeg
    │   └── 1744403687.jpeg
    ├── image_server
    └── server.log
```

## Penjelasan Struktur Folder
### client/
- `image_client`: Executable hasil kompilasi client, akan digunakan untuk mengirim permintaan ke server.

- `secrets/`: Berisi file teks rahasia (input_1.txt sampai input_5.txt).

- `1744403652.jpeg`, `1744403687.jpeg`: File JPEG hasil konversi yang disimpan oleh client.

### server/
- `database/`: Tempat menyimpan referensi hasil konversi JPEG dari client.

- `image_server`: Executable hasil kompilasi server.

- `server.log`: Untuk mencatat log permintaan dari client.

## File utama di root
- `image_client.c`: Source code program client RPC.

- `image_server.c`: Source code program server RPC.

---

## Soal_2 
### Deskripsi_Singkat
Sistem RushGo terdiri dari dua program berbasis shared memory untuk mengelola dan memroses pesanan:
1. Delivery_agent.c
  - Membaca file delivery_order.csv
  - Membuat/shared memory (/rushgo_shm) dan menyimpan semua order

2. Dispatcher.c
  - Mengakses shared memory yang sama
  - Menyediakan antarmuka baris-perintah dengan opsi list, deliver, dan status

### Penjelasan Kode
A. Download file CSV
```bash
wget "https://drive.google.com/uc?export=download&id=1OJfRuLgsBnIBWtdRXbRsD2sG6NhMKOg9" -O delivery_order.csv

```
- Berfungsi untuk mendownload file csv dari google drive dan menamai nya delivery_order.csv

B. Pengiriman Bertipe Express
```bash
pthread_t th[3];
for (int i = 0; i < 3; i++) {
  int *arg = malloc(sizeof(int));
  *arg = i;
  pthread_create(&th[i], NULL, agent_thread, arg);
}
```
- Kunci mutex, cari orders[i].type == 'E' && delivered==0
- Tandai delivered = 1, isi agent = "AGENT A/B/C" secara bergilir
- Lepas mutex
- Panggil log_delivery(agent, &orders[i])
```bash
fprintf(f, "[%d/%m/%Y %H:%M:%S] [%s] Express package delivered to %s in %s\n",
        buf, agent, o->name, o->address);

```
- Tulis kedalam log
  
C. Pengiriman Bertipe Reguler ( gunakan `./dispatcher -deliver <Nama>
`)
```bash
int fd = shm_open(SHM_NAME, O_RDWR, 0);
mmap(...);

```
- Membuka shared memory
```bash
if (orders[i].type=='R' && strcmp(orders[i].name,target)==0 && delivered==0) {
  orders[i].delivered = 1;
  strcpy(orders[i].agent, agent_str); 
  log_delivery(agent_str, &orders[i]);
  printf("Delivered Reguler package to %s by %s\n", target, agent_str);
}

```
- Mencari dan proses
```bash
fprintf(f, "[%d/%m/%Y %H:%M:%S] [%s] Reguler package delivered to %s in %s\n",
        buf, agent, o->name, o->address);

```
- Tulis kedalam log

D. Mengecek Status Pesanan (gunakan `./dispatcher -status <Nama>
`)

```bash
for (i=0; i<*num_orders; i++) {
  if (strcmp(orders[i].name, target)==0) {
    if (orders[i].delivered)
      printf("Status for %s: Delivered by %s\n", target, orders[i].agent);
    else
      printf("Status for %s: Pending\n", target);
    break;
  }
}
if (!found) printf("Order '%s' not found.\n", target);

```
- Loop Check

E. Melihat Daftar Semua Pesanan (gunakan `./dipatcher -list`)
```bash
printf("Daftar semua pesanan:\n");
for (i=0; i<*num_orders; i++) {
  const char *type = (orders[i].type=='E')?"Express":"Reguler";
  if (orders[i].delivered)
    printf("- %s (%s): Delivered by %s\n",
           orders[i].name, type, orders[i].agent);
  else
    printf("- %s (%s): Pending\n",
           orders[i].name, type);
}

```

## Soal_3 - The Lost Dungeon
### Deskripsi Singkat
Sistem “The Lost Dungeon” adalah aplikasi client-server berbasis TCP yang menghadirkan pengalaman RPG sederhana.
- `Dungeon.c `bertindak sebagai server multithreaded pada port 12345, menerima koneksi dari banyak client secara bersamaan, menampilkan menu utama, dan mengelola alur permainan (status, toko, inventori, pertarungan).
- `Player.c `adalah client yang terhubung ke server, menggunakan select() untuk menangani input user dan output server secara asinkron.
- `Shop.c / shop.h ` menyimpan daftar senjata dan menyediakan fungsi cetak toko, validasi, serta pengambilan data senjata.

### Penjelasan Kode

A. Entering The Dungeon
```bash
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
bind(server_fd, …, htons(PORT));      
listen(server_fd, MAX_CLIENTS);     

```
- Menyiapkan server TCP

B. Sightseeing
```bash
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
```
dan 
```bash
void print_menu(int sock) {
  send_msg(sock,
    "\n===== MAIN MENU =====\n"
    "1. Show Player Stats\n"
    "2. Shop (Buy Weapons)\n"
    "3. View Inventory & Equip Weapons\n"
    "4. Battle Mode\n"
    "5. Exit Game\n"
    "Choose an option: ");
}

```
- Berfungsi menampilkan opsi-opsi


C. Status Check
```bash
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
```
- Mencetak gold, nama & damage senjata, kill count, plus deskripsi passive jika berlaku.

D. Weapon Shop
1. Shop.c
   ```bash
   for (i=0; i<NUM_WEAPONS; i++){ snprintf(buf, …, "[%d] %s - Price: %d, Damage: %d",
    weapons[i].id, weapons[i].name, weapons[i].price, weapons[i].damage);send_msg(sock, buf);}
   send_msg(sock, "Enter weapon number to buy (0 to cancel): "); ```
2. dungeon.c
   ```bash
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
    }}
   ```
   - Validasi ID, cek gold, tambahkan ke inventory (max 20 slot).

E. Handy Inventory
```bash
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
```
- Daftar semua senjata, tampilkan passive, pilih index untuk equip.

F. Enemy Encounter
```bash
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

```
- Loop menampilkan health bar, terima “attack” atau “exit”.

G. Other Battle Logic
```bash
int damage = p->equipped->damage + rand() % (p->equipped->damage + 1);

if (p->equipped->passiveType == PASSIVE_INSTA && rand()%100 < p->equipped->passiveChance) {
  /* langsung enemy_hp = 0 */
}

else {
  int critChance = (p->equipped->passiveType == PASSIVE_CRIT)
                   ? p->equipped->passiveChance : 10;
  if (rand()%100 < critChance) damage *= 2;
  enemy_hp -= damage;
}


```
- Logika damage acak, crit double damage, instant kill, dan reward gold acak.

H. Error Handling 
- Menu utama >> (default case) menampilkan "Invalid option. Please try again.\n"

- Shop >> wid==0 menampilkan "Cancelled."; invalid ID menampilkan "Invalid option."; gold kurang menampilkan "Not enough gold."; inventori penuh menampilkan"Inventory full."

- Inventory >> index out-of-range menampilkan "Invalid option."

- Battle >> selain “attack”/“exit” menampilkan "Invalid option."




## Soal_4 – Sung Jin Woo's Hunter System
### Deskripsi Singkat
Dalam dunia alternatif di mana Sung Jin Woo bereinkarnasi menjadi seorang admin, ia membangun sebuah sistem pelatihan untuk para hunter. Sistem ini menggunakan shared memory untuk mengelola data hunter, dungeon, dan pertarungan secara real-time. Proyek ini terdiri dari 2 program utama:
-  system.c: Sebagai pusat pengelola shared memory dan data global (admin) .
-  hunter.c: Client-side untuk hunter yang ingin login, berlatih, atau bertarung.

### Struktur File
-  system.c : Program utama sistem (harus dijalankan dulu)
-  hunter.c : Program client hunter (dijalankan setelah sistem berjalan)
-  shm_common.h : Berisi definisi struktur dan key shared memory

### Penjelasan Code ```hunter.c``` dan ```system.c```  berdasarkan kategori soal
#### 1. Implementasi Sistem Hunter dan Client menggunakan Shared Memory IPC
#### Soal : system.c berperan sebagai server yang membuat dan mengelola shared memory utama, sedangkan ```hunter.c``` bertindak sebagai client yang mengakses dan memodifikasi data hunter di shared memory tersebut, dengan syarat hanya dapat dijalankan setelah ```system.c``` aktif.
#### Implementasi :
#### Di system.c:
-  Ada fungsi seperti generate_dungeon, ban_hunter, reset_hunter ini mirip dengan soal shift di mana admin memanipulasi data di shared memory.
-  Menggunakan shmget dan shmat untuk koneksi ke shared memory.

Di hunter.c:
-  Hunter bisa melakukan raid dungeon yaitu raid_dungeon
-  Bisa battle dengan hunter lain yaitu hunters_battle
-  Data hunter dan dungeon diakses bareng-bareng via shared memory sysdata

#### 2. Registrasi dan Login Hunter
#### Soal :
- Hunter bisa register & login.
- Saat register, hunter punya:

- Level = 1
- EXP = 0
- ATK = 10
- HP = 100
- DEF = 5
- Key unik untuk shared memory.
#### Implementasi Code :
```bash
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

```

#### 3. List Dungeon Sesuai Level Hunter
#### Soal : Hunter hanya bisa melihat dungeon yang levelnya sesuai (minimal level terpenuhi).
#### Implementasi Code :
```bash
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

```

#### 4. Raid Dungeon + Reward Stat
#### Soal :
- Hunter bisa raid dungeon.

- Setelah menang:

  - Hunter dapat stat reward.
  - Dungeon dihapus dari list.
  - EXP +500 berarti naik level.
#### Implementasi Code :
```bash
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

```

#### 5. Hunter Battle (1 vs 1)
#### Soal :
- Hunter bisa battle dengan hunter lain.
- Pemenang: dapat semua stats musuh.
- Yang kalah: dihapus dari list hunter.
#### Implementasi Code :
```bash
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
```

#### 6. Fitur Ban & Unban Hunter
#### Soal :
- Admin bisa ban hunter dan hunter tersebut tidak bisa raid/battle.
- Admin juga bisa unban.
#### Implementasi Code :
```bash

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
```


#### 7. Fitur Reset Stats Hunter
#### Soal :
Admin bisa reset hunter:

- Level = 1
- EXP = 0
- ATK = 10
- HP = 100
- DEF = 5
#### Implementasi Code :
```bash
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

```

#### 8. Notifikasi Dungeon (Dynamic)
#### Soal :
Ada notifikasi dungeon yang update tiap 3 detik:
Info: jumlah hunter, jumlah dungeon, top hunter.

#### Implementasi Code :
```bash
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

```

#### 9. Shared Memory Antar Hunter & Sistem
#### Soal :
- Semua hunter pakai shared memory.
- Semua memory harus terhapus saat sistem mati.

#### Implementasi Code :
```bash
key_t key = get_system_key();
int shmid = shmget(key, sizeof(struct SystemData), 0666);
struct SystemData *sysdata = shmat(shmid, NULL, 0);
```
