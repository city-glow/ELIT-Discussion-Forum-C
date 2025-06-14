# ELIT - CLI Discussion Forum Application
**Versi 0.2 (Active Development)**

## Deskripsi
Forum Diskusi ELIT adalah aplikasi forum diskusi berbasis CLI (Command Line Interface) yang dikembangkan dengan bahasa C. Proyek ini berfokus pada pembelajaran struktur data dan algoritma, serta praktik pengelolaan data secara persisten di aplikasi nyata.

## Fitur Utama (Update 2025)
- **Autentikasi Pengguna & Persistent Login**
  - Sistem login/register dengan password terenkripsi (Argon2)
  - Login otomatis jika data login tersimpan (`login.dat`)
  - Logout menghapus data login, exit dari dashboard tetap menyimpan sesi
- **Board Kategorisasi**
  - Pembuatan, pencarian, dan pengelolaan board (kategori diskusi)
- **Komentar Bersarang & Tree**
  - Komentar mendukung reply bertingkat tanpa batas (tree)
  - Penghapusan komentar/post otomatis menghapus seluruh subtree dan votes terkait
- **Sistem Voting**
  - Upvote/downvote untuk post & komentar
  - Penghapusan post/komentar juga menghapus semua vote terkait
- **Penyimpanan Data Persisten**
  - Semua data (user, board, post, komentar, vote) disimpan ke file `.dat` di folder `storage/`
  - Data otomatis dimuat saat aplikasi dijalankan dan disimpan saat keluar
- **Dashboard & Navigasi**
  - Dashboard dengan menu utama, logout, dan fitur exit langsung (tanpa logout)
  - Menu interaktif dengan validasi input dan error handling yang lebih baik
- **Moderasi Konten**
  - Antrian moderasi untuk board dengan akses owner
- **Pencarian & Trending**
  - Fitur pencarian instan dan daftar trending berbasis array

## Struktur Data yang Digunakan
| No | Struktur Data       | Aplikasi dalam Sistem                |
|----|---------------------|-------------------------------------|
| 1  | Array               | Trending, pencarian, dan tampilan top komentar/post |
| 2  | Linked List         | User, post, board, vote, dan antrian ID |
| 3  | Queue               | Antrian permintaan moderasi         |
| 4  | Stack               | (Planned) Notifikasi LIFO           |
| 5  | Non-Binary Tree     | Komentar bersarang (tree)           |

## Requirements
- Compiler GCC (GNU Compiler Collection)
- Library standar C (stdio.h, stdlib.h, string.h)
- OS: Linux/macOS/Windows (terminal/console support)
- (Opsional) Library Argon2 untuk hashing password

## Catatan Penggunaan
- Jalankan aplikasi dari root folder project.
- Semua data akan otomatis tersimpan di folder `storage/`.
- Jika login otomatis aktif, aplikasi langsung masuk dashboard user terakhir.
- Pilihan "Logout" akan menghapus sesi login, sedangkan "Exit" hanya keluar aplikasi tanpa logout.
