#include <stdio.h>
// #include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/auth/auth.h"
#include "../include/board/boardList.h"
#include "../include/post/postList.h"
#include "../include/ui/ui.h"
#include "../include/user/userList.h"

int main() {
  UserList user_list;
  BoardList board_list;
  PostList post_list;
  VoteList vote_list;

  user_create_list(&user_list);
  board_create_list(&board_list);
  post_create_list(&post_list);
  vote_create_list(&vote_list);

  int pilihan;
  User logged_user;
  bool is_logged_in = false;

  do {
    if (!is_logged_in) {
      pilihan = ui_show_main_menu();
      switch (pilihan) {
      case 1: {
        char username[MAX_USERNAME + 1], password[100];
        printf("Masukkan username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Masukkan password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        register_user(&user_list, username, password);
        ui_pause();
        break;
      }
      case 2: {
        char username[MAX_USERNAME + 1], password[100];
        printf("Masukkan username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Masukkan password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        if (login(user_list, username, password, &logged_user)) {
          is_logged_in = true;
          handle_dashboard(&board_list, &post_list, &user_list, &vote_list, &logged_user);
        } else {
          ui_pause();
        }
        break;
      }
      case 3:
        user_tampil_list(user_list.first);
        ui_pause();
        break;
      }
    } else {
      printf("Logout berhasil.\n");
      is_logged_in = false;
      ui_pause();
    }
  } while (pilihan != 0);

  printf("Terima kasih telah menggunakan aplikasi ini.\n");

  // Deallocate all resources
  user_deallocation(&user_list.first);
  board_deallocation(&board_list.first);
  post_deallocation(&post_list.first);

  return 0;
}
