# Sisop-3-2025-IT31

- Erlangga Valdhio Putra Sulistio_5027241030
- Rayhan Agnan Kusuma_5027241102
- S. Farhan Baig_5027241097

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
#### a. Implementasi Sistem Hunter dan Client menggunakan Shared Memory IPC
#### Soal : system.c berperan sebagai server yang membuat dan mengelola shared memory utama, sedangkan ```hunter.c``` bertindak sebagai client yang mengakses dan memodifikasi data hunter di shared memory tersebut, dengan syarat hanya dapat dijalankan setelah ```system.c``` aktif.
#### Implementasi :
#### Di system.c:
-  Ada fungsi seperti generate_dungeon, ban_hunter, reset_hunter ini mirip dengan soal shift di mana admin memanipulasi data di shared memory.
-  Menggunakan shmget dan shmat untuk koneksi ke shared memory.

Di hunter.c:
-  Hunter bisa melakukan raid dungeon yaitu raid_dungeon
-  Bisa battle dengan hunter lain yaitu hunters_battle
-  Data hunter dan dungeon diakses bareng-bareng via shared memory sysdata

#### b. Registrasi dan Login Hunter
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

#### c. List Dungeon Sesuai Level Hunter
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
