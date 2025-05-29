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
  printf("========== MENU UTAMA ==========\n");
  printf("1. Register\n");
  printf("2. Login\n");
  printf("0. Keluar\n");
  printf("================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar(); // Bersihkan newline
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
