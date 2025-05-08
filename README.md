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

#### a. Registrasi dan Login Hunter
#### Soal :
- Hunter bisa register & login.
- Saat register, hunter punya:

- Level = 1
- EXP = 0
- ATK = 10
- HP = 100
- DEF = 5
- Key unik untuk shared memory.
#### Implementasi :
#### Di system.c:


