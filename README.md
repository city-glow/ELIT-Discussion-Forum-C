# ELIT - CLI Discussion Forum Application
**Versi 1.0 (Stable Release)**

## Deskripsi
Forum Diskusi ELIT adalah aplikasi forum diskusi berbasis CLI (Command Line Interface) yang dikembangkan dengan bahasa C. Proyek ini berfokus pada pembelajaran struktur data dan algoritma, serta praktik pengelolaan data secara persisten di aplikasi nyata.

**Status:**
Versi 1.0 (Selesai, stabil).
Proyek telah selesai dan seluruh fitur utama telah diimplementasikan.

## Fitur Utama
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
| 2  | Linked List         | User, post, board, dan vote |
| 3  | Queue               | Antrian permintaan moderasi         |
| 4  | Stack               | Navigasi halaman   |
| 5  | Non-Binary Tree     | Komentar bersarang (tree)           |

## Requirements
- Compiler GCC (GNU Compiler Collection)
- Library standar C (stdio.h, stdlib.h, string.h)
- OS: Linux/macOS/Windows (terminal/console support)
- Library Argon2 untuk hashing password (sudah terintegrasi di repo)

## Catatan Penggunaan
- Compile
  - di linux "make clean" lalu "make"
  - di Windows "mingw32-make clean" lalu "mingw32-make"
- Jalankan aplikasi dari build folder project.
- Semua data akan otomatis tersimpan di folder `storage/`.
- Jika login otomatis aktif, aplikasi langsung masuk dashboard user terakhir.
- Pilihan "Logout" akan menghapus sesi login, sedangkan "Exit" hanya keluar aplikasi tanpa logout.

## Status Pengembangan
**Versi 1.0 - Selesai.**
Seluruh fitur utama telah diimplementasikan dan aplikasi siap digunakan.
Tidak ada pengembangan aktif lebih lanjut, kecuali perbaikan bug minor jika ditemukan.
