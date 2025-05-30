#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "../../include/board/board.h"
#include "../../include/board/boardList.h"
#include "../../include/post/post.h"
#include "../../include/post/postList.h"
#include "../../include/ui/ui.h"
#include <stdbool.h>
#include <string.h>

void ui_clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void ui_pause() {
  printf("\nTekan ENTER untuk melanjutkan...");
  // getchar();
  // getchar();
  // Bersihkan buffer
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


int ui_show_dashboard(User user) {

  ui_clear_screen();
  printf("--- Selamat datang, %s!--\n", user.username);
  printf("================================\n");
  printf("1. Add Post\n");
  printf("0. Logout\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar(); // Bersihkan newline
  return choice;
}

int get_or_create_board(BoardList *board_list, char *board_title, int user_id) {
  BoardAddress found = board_search_by_title(board_list->first, board_title);
  if (!board_is_empty(found)) {
    return found->info.id;
  }

  printf("Board tidak ditemukan. Buat board baru? (y/n): ");
  char choice;
  scanf(" %c", &choice);
  if (choice != 'y' && choice != 'Y') {
    return -1; // User canceled
  }

  Board new_board;
  BoardAddress new_node;
  create_board(&new_board, user_id, board_title);
  board_create_node(&new_node);
  board_isi_node(&new_node, new_board);
  return board_insert_and_get_id(board_list, new_node);
}

Post *create_post_input(int board_id, int author_id) {
  Post *new_post = (Post *)malloc(sizeof(Post));
  if (!new_post) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  char title[MAX_TITLE + 1];
  char content[1024];
  getchar();

  printf("Masukkan judul post: ");
  fgets(title, sizeof(title), stdin);
  title[strcspn(title, "\n")] = 0;

  printf("Masukkan konten post:\n");
  fgets(content, sizeof(content), stdin);
  content[strcspn(content, "\n")] = 0;

  strcpy(new_post->title, title);
  new_post->content = strdup(content);
  new_post->board_id = board_id;
  new_post->user_id = author_id;

  if (!new_post->content) {
    fprintf(stderr, "Failed to duplicate strings\n");
    free(new_post->content);
    free(new_post);
    return NULL;
  }

  return new_post;
}

void handle_dashboard(BoardList *board_list, PostList *post_list,
                      const User *user) {
  int dashboard_choice;
  do {
    dashboard_choice = ui_show_dashboard(*user);

    if (dashboard_choice == 1) { // Create new post
      char board_title[MAX_TITLE + 1];

      printf("Masukkan nama board: ");
      fgets(board_title, sizeof(board_title), stdin);
      board_title[strcspn(board_title, "\n")] = 0;

      int board_id = get_or_create_board(board_list, board_title, user->id);
      if (board_id == -1) {
        continue;
      }

      Post *new_post = create_post_input(board_id, user->id);
      if (new_post) {
        PostAddress post_node;
        post_create_node(&post_node);
        post_isi_node(&post_node, *new_post);
        post_insert(post_list, post_node);
        free(new_post->content);
        free(new_post);
      }
    } else if (dashboard_choice == 99) {
      post_tampil_list(post_list->first);
      ui_pause();
    }

  } while (dashboard_choice != 0);
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
