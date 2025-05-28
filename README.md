# ELIT - CLI Discussion Forum Application
**Versi 0.1 (WIP - Work In Progress)**

## Deskripsi
Forum Diskusi ELIT adalah aplikasi forum diskusi berbasis CLI (Command Line Interface) yang dikembangkan menggunakan bahasa pemrograman C. Dirancang sebagai alat pembelajaran untuk implementasi struktur data dan algoritma, aplikasi ini menawarkan pengalaman diskusi cepat dan ringan tanpa ketergantungan pada antarmuka grafis.

## Fitur Utama
- **Autentikasi Pengguna**
  Sistem login/register dengan penyimpanan akun terenkripsi
- **Board Kategorisasi**
  Pembuatan dan pengelolaan forum kategori (mirip subreddit di Reddit)
- **Komentar Bersarang**
  Dukungan untuk reply tanpa batas menggunakan struktur tree
- **Sistem Voting**
  Mekanisme upvote/downvote untuk menentukan relevansi konten
- **Moderasi Konten**
  Antrian moderasi dengan akses kontrol untuk owner
- **Notifikasi Realtime**
  Sistem stack untuk notifikasi terbaru yang otomatis terupdate
- **Pencarian & Trending**
  Fitur pencarian instan dan daftar topik trending berbasis array

## Struktur Data yang Digunakan
| No | Struktur Data       | Aplikasi dalam Sistem                |
|----|---------------------|-------------------------------------|
| 1  | Array               | Menyimpan hasil pencarian & trending (max 10) |
| 2  | Linked List         | Manajemen daftar pengguna, post, board, dan votes |
| 3  | Queue               | Antrian konten menunggu moderasi  |
| 4  | Stack               | Sistem notifikasi LIFO (Last-In-First-Out) |
| 5  | Non-Binary Tree     | Hierarki komentar bersarang (parent-child) |

## Requirements
- Compiler GCC (GNU Compiler Collection)
- Library standar C (stdio.h, stdlib.h, string.h)
- OS kompatibel: Linux/macOS/Windows (dengan terminal support)
