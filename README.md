# Sisop-3-2025-IT31

- Erlangga Valdhio Putra Sulistio_5027241030
- Rayhan Agnan Kusuma_5027241102
- S. Farhan Baig_5027241097

# The Legend of Rootkids – Nomor 1a

## 🧠 Deskripsi Soal

> **Tahun 2045**, dunia sedang mengalami krisis dalam infrastruktur siber. Sebagai mahasiswa Departemen Teknologi Informasi ITS, saya memiliki ide untuk kembali ke masa lalu (tahun 2025) untuk mencari seorang hacker legendaris asal Semarang bernama **“rootkids”**.  
>
> Satu-satunya petunjuk adalah dari situs deep web yang berisi beberapa file teks aneh dan instruksi untuk membuat sistem **RPC client-server** guna mengubah file teks menjadi gambar **JPEG**.  
>
> Tugas saya pada **nomor 1a** adalah menyiapkan folder dan file sesuai dengan struktur direktori contoh dari deep web tersebut.

---

## ✅ Tujuan Nomor 1a

- ✅ Men-download & mengekstrak file dari deep web (boleh manual).
- ✅ Menyusun struktur direktori proyek sesuai contoh.
- ❌ Belum membuat atau menjalankan server/client RPC (itu bagian soal selanjutnya).

---

## 🗂️ Struktur Folder Final

Berikut adalah struktur direktori setelah saya menyelesaikan tahap 1a:
.
├── client
│ ├── 1744403652.jpeg
│ ├── 1744403687.jpeg
│ ├── image_client
│ └── secrets
│ ├── input_1.txt
│ ├── input_2.txt
│ ├── input_3.txt
│ ├── input_4.txt
│ └── input_5.txt
├── image_client.c
├── image_server.c
└── server
├── database
│ ├── 1744403652.jpeg
│ └── 1744403687.jpeg
├── image_server
└── server.log


---

## 🔧 Penjelasan Tiap Komponen

### 📁 `client/`
- `1744403652.jpeg`, `1744403687.jpeg`: Hasil output dari proses konversi text-to-jpeg.
- `image_client`: File executable yang nantinya akan dijalankan oleh client untuk memanggil fungsi RPC ke server.
- `secrets/`: Folder berisi lima file teks (`input_1.txt` hingga `input_5.txt`) yang berisi data rahasia untuk dikonversi.

### 📄 `image_client.c`
- Merupakan source code program client RPC. Belum dikompilasi atau dijelaskan di tahap ini.

### 📄 `image_server.c`
- Merupakan source code program server RPC. Sama seperti client, belum perlu dijalankan atau dijelaskan di soal nomor 1a.

### 📁 `server/`
- `image_server`: File hasil kompilasi dari `image_server.c`, yaitu server RPC yang nantinya akan menerima permintaan dari client.
- `server.log`: Log file untuk mencatat aktivitas server.
- `database/`: Folder yang menyimpan data JPEG untuk proses validasi atau pembandingan saat server menerima input dari client.

---

## 🧾 Catatan Pengerjaan

- File teks (`input_1.txt`–`input_5.txt`) dan file gambar (`*.jpeg`) diperoleh dari link di soal dan dipindahkan manual ke lokasi masing-masing.
- Semua nama file, ekstensi, dan penempatan folder **disamakan persis seperti contoh di soal** untuk menghindari error di tahap-tahap berikutnya.
- Penamaan file `image_clnt.c` telah diperbaiki menjadi `image_client.c` agar konsisten dan mudah dipahami.

---

## 🏁 Status

✅ Nomor 1a telah SELESAI.  
▶️ Siap lanjut ke tahap pemrograman RPC di nomor berikutnya.



## Soal_2 
### Deskripsi_Singkat
Sistem RushGo terdiri dari dua program berbasis shared memory untuk mengelola dan memroses pesanan:
1. Delivery_agent.c
- Membaca file delivery_order.csv (skip header bila ada)
- Membuat/shared memory (/rushgo_shm) dan menyimpan semua order

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
