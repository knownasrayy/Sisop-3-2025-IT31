# Sisop-3-2025-IT31

- Erlangga Valdhio Putra Sulistio_5027241030
- Rayhan Agnan Kusuma_5027241102
- S. Farhan Baig_5027241097

## Soal_4 – Sung Jin Woo's Hunter System
### Deskripsi Singkat
Dalam dunia alternatif di mana Sung Jin Woo bereinkarnasi menjadi seorang admin, ia membangun sebuah sistem pelatihan untuk para hunter. Sistem ini menggunakan shared memory untuk mengelola data hunter, dungeon, dan pertarungan secara real-time. Proyek ini terdiri dari 2 program utama:
-system.c: Sebagai pusat pengelola shared memory dan data global (admin) .
-hunter.c: Client-side untuk hunter yang ingin login, berlatih, atau bertarung.

### Struktur File
-system.c : Program utama sistem (harus dijalankan dulu)
-hunter.c : Program client hunter (dijalankan setelah sistem berjalan)
-shm_common.h : Berisi definisi struktur dan key shared memory

