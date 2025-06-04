#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "../../include/board/board.h"
#include "../../include/board/boardList.h"
#include "../../include/post/post.h"
#include "../../include/post/postList.h"
#include "../../include/ui/ui.h"
#include "../../include/vote/vote.h"
#include "../../include/vote/voteList.h"
#include <stdbool.h>
#include <string.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"

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
  printf("\n========================================\n");
  printf("--- Selamat datang, %s!--\n", user.username);
  printf("========================================\n");
  printf("1. Add Post\n");
  printf("0. Logout\n");

  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar(); // Bersihkan newline
  return choice;
}

int get_or_create_board(BoardList *board_list, char *board_title, int user_id) {
  BoardAddress found = board_search_by_title(board_list->first, board_title);
  if (!board_is_empty(found)) {
    printf("Board \"%s\" ditemukan. Menggunakan board yang sudah ada.\n",
           board_title);
    return found->info.id;
  }

  printf("Board tidak ditemukan. Buat board baru? (y/n): ");
  char choice_line[8]; // small buffer
  fgets(choice_line, sizeof(choice_line), stdin);

  // No need to clean buffer again; fgets consumes the newline
  if (choice_line[0] != 'y' && choice_line[0] != 'Y') {
    printf("Pembuatan board dibatalkan.\n");
    return -1;
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
                      UserList *user_list, VoteList *vote_list,
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
    } else if (dashboard_choice == 98) {
      ui_show_post(1, post_list, vote_list, *user, user_list, board_list);
    }

  } while (dashboard_choice != 0);
}

int ui_show_post(Id post_id, PostList *post_list, VoteList *vote_list,
                 User user, UserList *user_list, BoardList *board_list) {
  PostAddress this_post = post_search_by_id(post_list->first, post_id);
  UserAddress poster =
      user_search_by_id(user_list->first, this_post->info.user_id);
  BoardAddress board =
      board_search_by_id(board_list->first, this_post->info.board_id);
  int vote_sum;
  Id my_vote_id;
  bool has_voted;

  get_vote_result(*vote_list, &vote_sum, user.id, post_id, VOTE_TARGET_POST,
                  &my_vote_id, &has_voted);

  VoteAddress my_vote = vote_search_by_id(vote_list->first, my_vote_id);

  printf("\n========================================\n");
  printf("                    POST                  \n");
  printf("\n========================================\n");
  printf("by %s\nin board [%s]\n", poster->info.username, board->info.title);
  printf("%sTitle:%s %s\n", ANSI_BOLD, ANSI_RESET, this_post->info.title);
  printf("%sContent:%s\n%s\n", ANSI_BOLD, ANSI_RESET, this_post->info.content);
  const char *arrow = vote_sum >= 0 ? "▲" : "▼";
  const char *color = vote_sum >= 0 ? ANSI_RED : ANSI_BLUE;
  const char *my_vote_info =
      has_voted ? my_vote->info.is_upvote ? "▲" : "▼" : "None";
  printf("%s%s %d%s\n", color, arrow, vote_sum, ANSI_RESET);
  printf("\nYour vote: %s\n", my_vote_info);
  printf("\n");
  printf("0. Kembali\n");

  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar();
  return choice;
};

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

void get_vote_result(VoteList vote_list, int *vote_sum, Id current_user_id,
                     Id target_id, VoteTargetType target_type, Id *my_vote_id,
                     bool *has_voted) {
  int upvotes = 0;
  int downvotes = 0;
  *my_vote_id = 0;
  *has_voted = false;

  VoteAddress current = vote_list.first;

  while (current != NULL) {
    if (current->info.target_id == target_id &&
        current->info.target_type == target_type) {

      if (current->info.is_upvote) {
        upvotes++;
      } else {
        downvotes++;
      }

      if (current->info.user_id == current_user_id) {
        *my_vote_id = current->info.id;
        *has_voted = true;
      }
    }

    current = current->next;
  }

  *vote_sum = upvotes - downvotes;
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
