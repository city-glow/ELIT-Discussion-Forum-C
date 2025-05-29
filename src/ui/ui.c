#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "../../include/ui/ui.h"
#include <stdbool.h>

void ui_clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void ui_pause() {
  printf("\nTekan ENTER untuk melanjutkan...");
  while (getchar() != '\n')
    ;        // Bersihkan buffer
  getchar(); // Tunggu ENTER
}

int ui_show_main_menu() {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("              MENU UTAMA                \n");
    printf("========================================\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Keluar\n");
    printf("========================================\n");
    printf("Pilihan: ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    return choice;
}


void ui_show_dashboard(User user) {
  ui_clear_screen();
  printf("--- Selamat datang, %s!--\n", user.username);
  printf("================================\n");
  printf("1. Add Post\n");
  printf("0. Logout\n");
  printf("Pilihan: ");
}

#include <stdio.h>
#include <stdlib.h>
#include "../../include/ui/ui.h"
#include <stdbool.h>

void ui_clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ui_pause() {
    printf("\nTekan ENTER untuk melanjutkan...");
    while (getchar() != '\n');
    getchar(); 
}

int ui_show_main_menu() {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("              MENU UTAMA                \n");
    printf("========================================\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Keluar\n");
    printf("========================================\n");
    printf("Pilihan: ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    return choice;
}

void ui_show_trending_posts() {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("             TRENDING POSTS           \n");
    printf("========================================\n");
    printf("1. Pilih Post\n");
    printf("2. Lihat Komentar\n");
    printf("3. Vote Up/Down\n");
    printf("4. Balas Komentar\n");
    printf("0. Kembali ke Menu Utama\n");
    printf("========================================\n");
    printf("Pilihan: ");
}

void ui_show_profile(User user) {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("                 PROFIL %s              \n", user.username);
    printf("========================================\n");
    printf("1. Lihat Postingan Saya\n");
    printf("2. Lihat Notifikasi\n");
    printf("3. Lihat Board yang Dibuat\n");
    printf("0. Kembali ke Menu Utama\n");
    printf("========================================\n");
    printf("Pilihan: ");
}

void ui_create_post() {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("               BUAT POST             \n");
    printf("========================================\n");
    printf("Masukkan Judul: ");
    // Input judul
    printf("Masukkan Isi: ");
    // Input isi
    printf("Post Berhasil Dibuat!\n");
    ui_pause();
}

void ui_search_post() {
    ui_clear_screen();
    printf("\n========================================\n");
    printf("               CARI POST              \n");
    printf("========================================\n");
    printf("Masukkan Judul Post: ");
    // Input pencarian
    printf(" Menampilkan Hasil Pencarian...\n");
    ui_pause();
}

void ui_logout() {
    ui_clear_screen();
    printf("\n Anda telah keluar dari sistem.\n");
    ui_pause();
}

