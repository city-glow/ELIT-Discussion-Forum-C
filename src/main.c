#include <stdio.h>
// #include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/auth/auth.h"
#include "../include/board/boardList.h"
#include "../include/comment/commentTreeList.h"
#include "../include/post/postList.h"
#include "../include/ui/ui.h"
#include "../include/user/userList.h"
#include "../include/vote/voteList.h"
#include "../include/ui/navigationStack.h"

#ifdef _WIN32
#include <windows.h>
void enable_windows_ansi() {
  system("chcp 65001 > nul"); // Enable UTF-8
  // HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  // if (hOut != INVALID_HANDLE_VALUE) {
  //   DWORD dwMode = 0;
  //   if (GetConsoleMode(hOut, &dwMode)) {
  //     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  //     SetConsoleMode(hOut, dwMode);
  //   }
 //  }
}
#else
void enable_windows_ansi() {} // No-op on non-Windows
#endif

int main() {
  enable_windows_ansi();

  UserList user_list;
  BoardList board_list;
  PostList post_list;
  ModerateList moderate_list;
  VoteList vote_list;
  CommentTreeList comment_tree_list;
  NavigationStack nav_stack;
  navigation_stack_create(&nav_stack);
  load_navigation_stack(&nav_stack, "../storage/navigation.dat");



  // comment_tree_list_create_list(&comment_tree_list);

  load_comment_tree_list(&comment_tree_list, "../storage/comments.dat");
  load_user_list(&user_list, "../storage/users.dat");
  load_vote_list(&vote_list, "../storage/votes.dat");
  load_board_list(&board_list, "../storage/boards.dat");
  load_post_list(&post_list, "../storage/posts.dat");
  moderate_create_list(&moderate_list);

  // Populate moderate_list from all boards' queues
  BoardAddress curr_board = board_list.first;
  while (curr_board != NULL) {
    ModerateQueueAddress curr_req = curr_board->info.queue.front;
    while (curr_req != NULL) {
      ModerateAddress new_node;
      moderate_create_node(&new_node);
      ModerateRequest req = curr_req->info;
      moderate_isi_node(&new_node, req);
      moderate_insert(&moderate_list, new_node);
      curr_req = curr_req->next;
    }
    curr_board = curr_board->next;
  }

  int pilihan;
  User logged_user;
  bool is_logged_in = false;

  // Persistent login: try to load login.dat
  FILE *login_file = fopen("../storage/login.dat", "r");
  if (login_file) {
    char saved_username[MAX_USERNAME + 1];
    if (fgets(saved_username, sizeof(saved_username), login_file)) {
      saved_username[strcspn(saved_username, "\n")] = 0;
      UserAddress curr = user_list.first;
      while (curr != NULL) {
        if (strcmp(curr->info.username, saved_username) == 0) {
          logged_user = curr->info;
          is_logged_in = true;
          break;
        }
        curr = curr->next;
      }
    }
    fclose(login_file);
  }

  do {
    if (!is_logged_in) {
      pilihan = ui_show_main_menu();
      switch (pilihan) {
      case 1: {
        char username[MAX_USERNAME + 1], password[100];
        int try_again = 1;
        while (try_again) {
          do {
            printf("Masukkan username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;
            if (!is_username_valid(username, user_list)) {
              printf("Coba lagi? (y/n): ");
              char yn[8];
              fgets(yn, sizeof(yn), stdin);
              if (yn[0] != 'y' && yn[0] != 'Y') {
                try_again = 0;
                break;
              }
            } else {
              break;
            }
          } while (1);

          if (!try_again)
            break;

          printf("Masukkan password: ");
          fgets(password, sizeof(password), stdin);
          password[strcspn(password, "\n")] = 0;

          if (!register_user(&user_list, username, password)) {
            printf("Coba lagi? (y/n): ");
            char yn[8];
            fgets(yn, sizeof(yn), stdin);
            if (yn[0] != 'y' && yn[0] != 'Y') {
              try_again = 0;
            }
          } else {
            try_again = 0;
          }
        }
        ui_pause();
        break;
      }
      case 2: {
        char username[MAX_USERNAME + 1], password[100];
        int try_again = 1;
        while (try_again) {
          printf("Masukkan username: ");
          fgets(username, sizeof(username), stdin);
          username[strcspn(username, "\n")] = 0;

          printf("Masukkan password: ");
          fgets(password, sizeof(password), stdin);
          password[strcspn(password, "\n")] = 0;

          if (login(user_list, username, password, &logged_user)) {
            // Simpan login ke file
            FILE *login_file = fopen("../storage/login.dat", "w");
            if (login_file) {
              fprintf(login_file, "%s\n", username);
              fclose(login_file);
            }
            is_logged_in = true;
            try_again = 0;
            // PANGGIL FUNGSI NAVIGASI
            resume_last_navigation(&nav_stack, &logged_user, &board_list, &post_list, &moderate_list, &user_list, &vote_list, &comment_tree_list);
          } else {
            printf("Coba lagi? (y/n): ");
            char yn[8];
            fgets(yn, sizeof(yn), stdin);
            if (yn[0] != 'y' && yn[0] != 'Y') {
              try_again = 0;
            }
          }
        }
        break;
      }

      }
    } else {
      // Already logged in (persistent login)
      handle_dashboard(&board_list, &post_list, &moderate_list, &user_list, &vote_list,
                       &comment_tree_list, &logged_user, &nav_stack);
      printf("Logout berhasil.\n");
      is_logged_in = false;
      save_navigation_stack(nav_stack, "../storage/navigation.dat");
      remove("../storage/login.dat");
      navigation_stack_clear(&nav_stack);  // Bersihkan stack setelah logout
      ui_pause();
      pilihan = -1; // Force loop to continue and show main menu
    }
  } while (pilihan != 0);

  printf("Terima kasih telah menggunakan aplikasi ini.\n");
  save_board_list(&board_list, "../storage/boards.dat");
  save_vote_list(&vote_list, "../storage/votes.dat");
  save_user_list(&user_list, "../storage/users.dat");
  save_post_list(&post_list, "../storage/posts.dat");
  save_comment_tree_list(&comment_tree_list, "../storage/comments.dat");

  // Deallocate all resources
  user_deallocation(&user_list.first);
  board_deallocation(&board_list.first);
  post_deallocation(&post_list.first);
  vote_deallocation(&vote_list.first);
  comment_tree_list_deallocation(&comment_tree_list.first);
  save_navigation_stack(nav_stack, "../storage/navigation.dat");
  return 0;
}
