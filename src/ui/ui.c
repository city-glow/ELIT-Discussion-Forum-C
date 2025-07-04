#include "../../include/ui/ui.h"
#include "../../include/array/item.h"
#include "../../include/array/itemArray.h"
#include "../../include/board/board.h"
#include "../../include/board/boardList.h"
#include "../../include/comment/comment.h"
#include "../../include/comment/commentTree.h"
#include "../../include/comment/commentTreeList.h"
#include "../../include/post/post.h"
#include "../../include/post/postList.h"
#include "../../include/ui/navigationStack.h"
#include "../../include/vote/vote.h"
#include "../../include/vote/voteList.h"
#include "../../include/moderateThis/moderateThisQueue.h"
#include "../../include/moderateThis/moderateThisList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================
// ANSI Color Macros
// =======================
#define ANSI_RED "\x1b[31m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_ITALIC "\x1b[3m"

// =======================
// Utility/UI Helper Functions
// =======================
void ui_clear_screen() {
  // #ifdef _WIN32
  //   system("cls");
  // #else
  //   system("clear");
  // #endif
  // for college requirements
}

void ui_pause() {
  printf("\nTekan ENTER untuk melanjutkan...");
  getchar(); // Tunggu ENTER
}

// =======================
// Menu/UI Display Functions
// =======================
int ui_show_main_menu() {
  ui_clear_screen();
  printf("========================================\n");
  printf("              MENU UTAMA                \n");
  printf("========================================\n");
  printf("1. Register\n");
  printf("2. Login\n");
  printf("0. Keluar\n");
  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar(); // flush input buffer
  return choice;
}

int ui_show_dashboard(User user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("--- Selamat datang, %s!\n", user.username);
  printf("========================================\n");
  printf("1. Add Post\n");
  printf("2. Moderate Post Queue\n");
  printf("3. See Profile\n");
  printf("4. Trending Posts\n");
  printf("5. See Boards\n");
  printf("0. Logout\n");
  printf("9. Exit\n");
  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);

  getchar(); // flush input buffer

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

// =======================
// Input/Creation Functions
// =======================
//
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
  char content[1025];
  content[0] = '\0';

  printf("Masukkan judul post (maksimal 100 karakter): ");
  fgets(title, sizeof(title), stdin);
  title[strcspn(title, "\n")] = 0;

  printf("Masukkan konten post (maksimal 1024 karakter, akhiri dengan baris "
         "berisi ---END---):\n");

  char line[256];
  while (fgets(line, sizeof(line), stdin)) {
    if (strcmp(line, "---END---\n") == 0)
      break;
    if (strlen(content) + strlen(line) >= 1024) {
      fprintf(stderr, "Konten melebihi batas maksimum\n");
      break;
    }
    strcat(content, line);
  }

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

// =======================
// Display/Interaction Functions
// =======================
void print_vote(bool has_voted, Vote my_vote, int vote_sum) {
  const char *arrow_up = "▲";
  const char *arrow_down = "▼";
  const char *color = vote_sum >= 0 ? vote_sum > 0 ? ANSI_RED : "" : ANSI_BLUE;
  if (has_voted) {
    if (my_vote.is_upvote) {
      printf("%s%s%s%s %s%d%s", ANSI_RED, arrow_up, ANSI_RESET, arrow_down,
             color, vote_sum, ANSI_RESET);
    } else {
      printf("%s%s%s%s %s%d%s", arrow_up, ANSI_BLUE, arrow_down, ANSI_RESET,
             color, vote_sum, ANSI_RESET);
    }
  } else {
    printf("%s%s %s%d%s", arrow_up, arrow_down, color, vote_sum, ANSI_RESET);
  }
}

int ui_show_post(Post post, User poster, Board board, int vote_sum,
                 bool has_voted, Vote my_vote, User logged_user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("-- POST %sby user[%s] in board[%s]%s\n", ANSI_ITALIC, poster.username,
         board.title, ANSI_RESET);
  printf("========================================\n");
  printf("%s%s%s\n", ANSI_BOLD, post.title, ANSI_RESET);
  printf("%s\n", post.content);
  print_vote(has_voted, my_vote, vote_sum);
  printf("\n");

  printf("\n");
  printf("1. Toggle Upvote/Downvote/No_vote\n");
  printf("2. Comment\n");
  printf("3. View Top Comments\n");
  if (poster.id == logged_user.id) {
    printf("4. Delete\n");
  }
  printf("0. Kembali\n");
  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar();
  return choice;
}

void display_top_comments(Item *items, UserList user_list, VoteList vote_list,
                          int total_items, int offset, User logged_user,
                          CommentTreeList comment_tree_list, bool search,
                          bool sort_by_new, Id user_id) {
  ui_clear_screen();
  printf("========================================\n");
  printf("-- Top Comments\n");
  printf("========================================\n");
  if (total_items == 0) {
    printf("No comments yet...\n");
  } else {
    int i = offset;
    for (i = offset; i < offset + 10 && i < total_items; i++) {
      UserAddress found_user =
          user_search_by_id(user_list.first, items[i].info.c.user_id);
      printf("%d. user[%s]: %s | ", i - offset + 1, found_user->info.username,
             items[i].info.c.content);
      int vote_sum;
      Id my_vote_id;
      bool has_voted;
      get_vote_result(vote_list, &vote_sum, logged_user.id, items[i].id,
                      VOTE_TARGET_COMMENT, &my_vote_id, &has_voted);
      VoteAddress my_vote = NULL;
      if (has_voted) {
        my_vote = vote_search_by_id(vote_list.first, my_vote_id);
      }
      print_vote(has_voted, my_vote ? my_vote->info : (Vote){0}, vote_sum);
      int reply_count =
          comment_tree_node_count(comment_tree_list_search_by_root_id(
                                      comment_tree_list.first, items[i].id)
                                      ->info) -
          1;

      printf(" | %d %s \n", reply_count,
             reply_count <= 1 ? "reply" : "replies");
    }
  }
  printf("\nNavigasi:\n");
  if (offset > 0)
    printf("P. Previous Page\n");
  if (offset + 10 < total_items)
    printf("N. Next Page\n");
  if (sort_by_new) {
    printf("S. Sort by name\n");
  } else {
    printf("S. Sort by new\n");
  }
  if (user_id != -1) {
    printf("M. Every comments (not only mine)\n");
  } else {
    printf("M. Only my comment\n");
  }
  if (search) {
    printf("Q. Delete search query\n");
  } else {
    printf("Q. Search\n");
  }
  printf("W. Add comment\n");
  printf("0. Kembali\n");

  printf("========================================\n");
  printf("Pilih nomor atau navigasi: ");
}

void display_top_posts(Item *items, UserList user_list, VoteList vote_list,
                       BoardList board_list, int total_items, int offset,
                       User logged_user, bool sort_by_new, Id user_id,
                       Id board_id, bool search) {
  ui_clear_screen();
  printf("========================================\n");
  if (board_id == -1) {
    if (sort_by_new) {
      printf("-- New Posts\n");
    } else {
      printf("-- Trending Posts\n");
    }
  } else {
    BoardAddress this_board = board_search_by_id(board_list.first, board_id);
    if (sort_by_new) {
      printf("-- New Posts in board[%s]\n", this_board->info.title);
    } else {
      printf("-- Trending Posts in board[%s]\n", this_board->info.title);
    }
  }
  printf("========================================\n");
  if (total_items == 0) {
    printf("No posts found...\n");
  } else {
    int i = offset;
    for (i = offset; i < offset + 10 && i < total_items; i++) {
      UserAddress found_user =
          user_search_by_id(user_list.first, items[i].info.p.user_id);
      printf("%d. user[%s]: %s%s%s | ", i - offset + 1,
             found_user->info.username, ANSI_BOLD, items[i].info.p.title,
             ANSI_RESET);
      int vote_sum;
      Id my_vote_id;
      bool has_voted;
      get_vote_result(vote_list, &vote_sum, logged_user.id, items[i].id,
                      VOTE_TARGET_POST, &my_vote_id, &has_voted);
      VoteAddress my_vote = NULL;
      if (has_voted) {
        my_vote = vote_search_by_id(vote_list.first, my_vote_id);
      }
      print_vote(has_voted, my_vote ? my_vote->info : (Vote){0}, vote_sum);
      if (board_id == -1) {
        BoardAddress this_board =
            board_search_by_id(board_list.first, items[i].info.p.board_id);
        if (this_board != NULL) {
          printf(" | in board[%s%s%s]", ANSI_BOLD, this_board->info.title,
                 ANSI_RESET);
        } else {
          printf(" | in board[Unknown]");
        }
      }
      printf("\n");
    }
  }
  printf("\nNavigasi:\n");
  if (offset > 0)
    printf("P. Previous Page\n");
  if (offset + 10 < total_items)
    printf("N. Next Page\n");
  if (sort_by_new) {
    printf("S. Sort by vote\n");
  } else {
    printf("S. Sort by new\n");
  }
  if (user_id != -1) {
    printf("M. Every posts (not only mine)\n");
  } else {
    printf("M. Only my posts\n");
  }
  if (search) {
    printf("Q. Delete search query\n");
  } else {
    printf("Q. Search\n");
  }
  if (board_id != -1) {
    printf("W. Make post in this board\n");
    BoardAddress this_board = board_search_by_id(board_list.first, board_id);
    if (this_board->info.owner_id == logged_user.id) {
      printf("D. Delete board\n");
    }
  } else {
    printf("W. Make post\n");
  }
  printf("0. Kembali\n");

  printf("========================================\n");
  printf("Pilih nomor atau navigasi: ");
}

void display_top_boards(Item *items, int total_items, int offset,
                        bool sort_by_new, Id user_id, bool search) {
  ui_clear_screen();
  printf("========================================\n");
  if (sort_by_new) {
    printf("-- New Boards\n");
  } else {
    printf("-- Boards by Name\n");
  }
  printf("========================================\n");
  if (total_items == 0) {
    printf("No boards found...\n");
  } else {
    int i = offset;
    for (i = offset; i < offset + 10 && i < total_items; i++) {
      printf("%d. board[%s%s%s]", i - offset + 1, ANSI_BOLD,
             items[i].info.b.title, ANSI_RESET);
      printf("\n");
    }
  }
  printf("\nNavigasi:\n");
  if (offset > 0)
    printf("P. Previous Page\n");
  if (offset + 10 < total_items)
    printf("N. Next Page\n");
  if (sort_by_new) {
    printf("S. Sort by name\n");
  } else {
    printf("S. Sort by new\n");
  }
  if (user_id != -1) {
    printf("M. Every boards (not only mine)\n");
  } else {
    printf("M. Only my boards\n");
  }
  if (search) {
    printf("Q. Delete search query\n");
  } else {
    printf("Q. Search\n");
  }
  printf("W. Make New Board\n");
  printf("0. Kembali\n");

  printf("========================================\n");
  printf("Pilih nomor atau navigasi: ");
}

int ui_show_single_comment(Comment comment, bool has_parent,
                           Comment comment_parent, Post post, User poster,
                           int vote_sum, bool has_voted, Vote my_vote,
                           char *parent_commenter_username, User logged_user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("-- COMMENT by user[%s]\n", poster.username);
  printf("========================================\n");
  printf("in post %s%s%s\n", ANSI_BOLD, post.title, ANSI_RESET);
  if (has_parent) {
    printf("reply to %suser[%s]: %s%s\n", ANSI_BOLD, parent_commenter_username,
           comment_parent.content, ANSI_RESET);
  }
  printf("%s\n", comment.content);
  print_vote(has_voted, my_vote, vote_sum);
  printf("\n");

  printf("\n");
  printf("1. Toggle Upvote/Downvote/No_vote\n");
  printf("2. Reply\n");
  if (poster.id == logged_user.id) {
    printf("3. Delete\n");
  }
  printf("0. Kembali\n");
  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf(" %d", &choice);
  getchar();
  return choice;
}

// =======================
// Handler Functions
// =======================


// Function to handle moderation queue UI for a selected board
void handle_moderate_queue(BoardList *board_list, PostList *post_list, ModerateList *moderate_list, User logged_user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("         MODERATE POST QUEUE            \n");
  printf("========================================\n");

  // List boards owned by the user
  BoardAddress current = board_list->first;
  int index = 1;
  BoardAddress owned_boards[100]; // Assuming max 100 boards owned
  int owned_count = 0;

  while (current != NULL) {
    if (current->info.owner_id == logged_user.id) {
      printf("%d. %s\n", index, current->info.title);
      owned_boards[owned_count++] = current;
      index++;
    }
    current = current->next;
  }

  if (owned_count == 0) {
    printf("You do not own any boards.\n");
    ui_pause();
    return;
  }

  printf("Select a board to moderate (0 to cancel): ");
  int choice;
  scanf("%d", &choice);
  getchar();

  if (choice == 0 || choice > owned_count) {
    printf("Moderation cancelled.\n");
    ui_pause();
    return;
  }

  BoardAddress selected_board = owned_boards[choice - 1];

  // Load the moderation queue for the selected board
  ModerateQueue *queue = &selected_board->info.queue;

  int exit_moderation = 0;
  while (!exit_moderation) {
    ui_clear_screen();
    printf("Moderation queue for board: %s\n", selected_board->info.title);
    printf("========================================\n");

    if (moderate_queue_is_empty(*queue)) {
      printf("No posts waiting for moderation.\n");
      ui_pause();
      break;
    }

    // Display only the first request in the queue (FIFO)
    ModerateQueueAddress current = queue->front;
    printf("Request ID: %d, User ID: %d, Approved: %s\n",
           current->info.request_id, current->info.user_id,
           current->info.is_approved ? "Yes" : "No");

    printf("Approve (a) or Reject (r) the request? (q to quit): ");
    char action;
    scanf(" %c", &action);
    getchar();

    if (action == 'q' || action == 'Q') {
      exit_moderation = 1;
      continue;
    }

    if (action == 'a' || action == 'A') {
      if (board_approve_moderate_request(&selected_board->info, current->info.request_id, moderate_list, post_list)) {
        printf("Request approved.\n");
      } else {
        printf("Failed to approve request.\n");
      }
    } else if (action == 'r' || action == 'R') {
      if (board_reject_moderate_request(&selected_board->info, current->info.request_id, post_list)) {
        printf("Request rejected.\n");
      } else {
        printf("Failed to reject request.\n");
      }
    } else {
      printf("Invalid action.\n");
    }
    ui_pause();
  }
  ui_pause();
}

void handle_dashboard(BoardList *board_list,
                      PostList *post_list,
                      ModerateList *moderate_list,
                      UserList *user_list,
                      VoteList *vote_list,
                      CommentTreeList *comment_tree_list,
                      User *logged_user,
                      NavigationStack *nav_stack) {
  navigation_stack_push(nav_stack, "dashboard"); // Push halaman ini ke stack

  int dashboard_choice;
  do {
    dashboard_choice = ui_show_dashboard(*logged_user);

    if (dashboard_choice == 1) {
      // Buat post
      char board_title[MAX_TITLE + 1];
      printf("Masukkan nama board tempat anda ingin post (maksimal 100): ");
      fgets(board_title, sizeof(board_title), stdin);
      board_title[strcspn(board_title, "\n")] = 0;

      int board_id = get_or_create_board(board_list, board_title, logged_user->id);
      if (board_id == -1)
        continue;

      Post *new_post = create_post_input(board_id, logged_user->id);
      if (new_post) {
        PostAddress post_node;
        post_create_node(&post_node);
        post_isi_node(&post_node, *new_post);

        // Check if user is board owner
        BoardAddress board = board_search_by_id(board_list->first, new_post->board_id);
        if (board != NULL && board->info.owner_id == logged_user->id) {
          // Owner posts are auto-approved
          post_node->info.approved = true;
        } else {
          // Non-owner posts require moderation
          post_node->info.approved = false;
          // Create moderation request and add to board queue
          if (board != NULL) {
            board_add_moderate_request(&board->info, post_node->info.id, logged_user->id);
          }
        }

        post_insert(post_list, post_node);
        free(new_post->content);
        free(new_post);
      }

    } else if (dashboard_choice == 2) {
    navigation_stack_push(nav_stack, "moderate_queue");
    handle_moderate_queue(board_list, post_list, moderate_list, *logged_user);

    } else if (dashboard_choice == 3) {
      // See Profile
      // TODO: Implement profile handling here
      //printf("Profile page is under construction.\n");
      handle_profile(*logged_user, post_list, vote_list,user_list, board_list, comment_tree_list, nav_stack);
      ui_pause();

    } else if (dashboard_choice == 4) {
      navigation_stack_push(nav_stack, "trending");
      handle_posts_page(post_list, vote_list, *logged_user, user_list, board_list,
                        comment_tree_list, -1, -1);

    } else if (dashboard_choice == 5) {
      navigation_stack_push(nav_stack, "boards");
      handle_boards_page(post_list, vote_list, *logged_user, user_list, board_list,
                         comment_tree_list, -1);

    } else if (dashboard_choice == 0) {
      // Logout
      printf("Logging out...\n");
      break;

    } else if (dashboard_choice == 9) {
      save_navigation_stack(
          *nav_stack, "../storage/navigation.dat"); // Simpan stack sebelum exit
      save_board_list(board_list, "../storage/boards.dat");
      save_vote_list(vote_list, "../storage/votes.dat");
      save_user_list(user_list, "../storage/users.dat");
      save_post_list(post_list, "../storage/posts.dat");
      save_comment_tree_list(comment_tree_list, "../storage/comments.dat");

      user_deallocation(&user_list->first);
      board_deallocation(&board_list->first);
      post_deallocation(&post_list->first);
      vote_deallocation(&vote_list->first);
      comment_tree_list_deallocation(&comment_tree_list->first);

      printf("Exiting...\n");
      exit(0);
    } else {
      printf("Invalid choice. Please try again.\n");
      ui_pause();
    }

  } while (dashboard_choice != 0); // 0 = logout
  save_navigation_stack(*nav_stack, "../storage/navigation.dat");
}

void handle_boards_page(PostList *post_list, VoteList *vote_list,
                        User logged_user, UserList *user_list,
                        BoardList *board_list,
                        CommentTreeList *comment_tree_list, Id user_id) {
  int total_items, offset = 0;
  bool sort_by_new = false;
  char *search_term = "";
  int exit_boards = 0;
  while (!exit_boards) {
    bool search = false;
    if (strcmp(search_term, "") != 0)
      search = true;
    Item *top_boards = generate_top_boards_array(
        *board_list, &total_items, sort_by_new, user_id, search_term);
    ui_clear_screen();
    display_top_boards(top_boards, total_items, offset, sort_by_new, user_id,
                       search);
    if (total_items == 0) {
      printf("\nBoard ini kosong. Silakan tekan 0 untuk kembali.\n");
      printf("Pilihan: ");
      char choice[10];
      fgets(choice, sizeof(choice), stdin);
      choice[strcspn(choice, "\n")] = 0;
      if (strcmp(choice, "0") == 0) {
        exit_boards = 1;
        ui_clear_screen();
      }
      free(top_boards);
      continue;
    }

    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if (choice[0] == 'N' && offset + 10 < total_items) {
      offset += 10;
    } else if (choice[0] == 'P' && offset > 0) {
      offset -= 10;
    } else if (choice[0] == '0') {
      exit_boards = 1;
      ui_clear_screen();
    } else if (choice[0] == 'S') {
      sort_by_new = !sort_by_new;
    } else if (choice[0] == 'M') {
      if (user_id == -1) {
        user_id = logged_user.id;
      } else {
        user_id = -1;
      }
    } else if (choice[0] == 'Q') {
      if (strcmp(search_term, "") == 0) {

        char content[1025];
        printf("Masukkan search (maksimal 1024 karakter):\n");
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = 0;

        search_term = strdup(content);
      } else {
        search_term = strdup("");
      }
    } else if (choice[0] == 'W') {
      char board_title[MAX_TITLE + 1];
      printf("Masukkan nama board (maksimal 100 karakter): ");
      fgets(board_title, sizeof(board_title), stdin);
      board_title[strcspn(board_title, "\n")] = 0;
      get_or_create_board(board_list, board_title, logged_user.id);
    } else {
      int selected = atoi(choice);
      if (selected > 0 && selected <= 10 && offset + selected <= total_items) {
        handle_posts_page(post_list, vote_list, logged_user, user_list,
                          board_list, comment_tree_list,
                          top_boards[offset + selected - 1].id, -1);
      }
    }
    free(top_boards);
  }
  ui_clear_screen();
}

void handle_posts_page(PostList *post_list, VoteList *vote_list,
                       User logged_user, UserList *user_list,
                       BoardList *board_list,
                       CommentTreeList *comment_tree_list, Id board_id,
                       Id user_id) {
  int total_items, offset = 0;
  bool sort_by_new = false;
  char *search_term = NULL;
  int exit_posts = 0;

  search_term = strdup("");

  while (!exit_posts) {
    bool search = false;
    if (strcmp(search_term, "") != 0)
      search = true;
    Item *top_posts =
    generate_top_posts_array(*post_list, *vote_list, &total_items, board_id,sort_by_new, user_id, search_term);
    ui_clear_screen();
    display_top_posts(top_posts, *user_list, *vote_list, *board_list,
                      total_items, offset, logged_user, sort_by_new, user_id,board_id, search);
    if (total_items == 0) {
    printf("\nPost ini kosong. Silakan tekan 0 untuk kembali.\n");
    printf("Pilihan: ");
    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if (strcmp(choice, "0") == 0) {
      exit_posts = 1;
      ui_clear_screen();
    }
    free(top_posts);
    continue;
    }

    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if (choice[0] == 'N' && offset + 10 < total_items) {
      offset += 10;
    } else if (choice[0] == 'P' && offset > 0) {
      offset -= 10;
    } else if (choice[0] == '0') {
      exit_posts = 1;
      ui_clear_screen();
    } else if (choice[0] == 'S') {
      sort_by_new = !sort_by_new;
    } else if (choice[0] == 'M') {
      if (user_id == -1) {
        user_id = logged_user.id;
      } else {
        user_id = -1;
      }
    } else if (choice[0] == 'Q') {
      if (strcmp(search_term, "") == 0) {

        char content[1025];
        printf("Masukkan search (maksimal 1024 karakter):\n");
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = 0;

        free(search_term);
        search_term = strdup(content);
      } else {
        free(search_term);
        search_term = strdup("");
      }
    } else if (choice[0] == 'W') {
      int target_board_id = board_id;

      if (target_board_id == -1) {
        char board_title[MAX_TITLE + 1];
        printf("Masukkan nama board tempat anda ingin post (maksimal 100): ");
        fgets(board_title, sizeof(board_title), stdin);
        board_title[strcspn(board_title, "\n")] = 0;

        target_board_id =
            get_or_create_board(board_list, board_title, logged_user.id);
        if (target_board_id == -1)
          continue;
      }

      Post *new_post = create_post_input(target_board_id, logged_user.id);
      if (new_post) {
        PostAddress post_node;
        post_create_node(&post_node);
        post_isi_node(&post_node, *new_post);

        // Check if user is board owner
        BoardAddress board = board_search_by_id(board_list->first, new_post->board_id);
        if (board != NULL && board->info.owner_id == logged_user.id) {
          // Owner posts are auto-approved
          post_node->info.approved = true;
        } else {
          // Non-owner posts require moderation
          post_node->info.approved = false;
          // Create moderation request and add to board queue
          if (board != NULL) {
            board_add_moderate_request(&board->info, post_node->info.id, logged_user.id);
          }
        }

        post_insert(post_list, post_node);
        free(new_post->content);
        free(new_post);
      }
    } else if (choice[0] == 'D') {
      BoardAddress this_board = board_search_by_id(board_list->first, board_id);
      if (this_board->info.owner_id == logged_user.id) {
        Board X;
        printf("Board dan seluruh komen dan post di dalamnya akan dihapus. "
               "Apakah anda yakin? (y/n): ");
        char choice_line[8]; // small buffer
        fgets(choice_line, sizeof(choice_line), stdin);

        if (choice_line[0] != 'y' && choice_line[0] != 'Y') {
          printf("Penghapusan board dibatalkan.\n");
        } else {
          board_delete_by_id(board_list, board_id, &X, post_list, vote_list,
                             comment_tree_list);
          printf("Board berhasil dihapus\n");
          ui_pause();
          exit_posts = 1;
        }
      }
    } else {
      int selected = atoi(choice);
      if (selected > 0 && selected <= 10 && offset + selected <= total_items) {
        handle_post_page(top_posts[offset + selected - 1].id, post_list,
                         vote_list, logged_user, user_list, board_list,
                         comment_tree_list);
      }
    }
    free(top_posts);
  }
  free(search_term);
  ui_clear_screen();
}

void handle_post_page(Id post_id, PostList *post_list, VoteList *vote_list,
                      User logged_user, UserList *user_list,
                      BoardList *board_list,
                      CommentTreeList *comment_tree_list) {
  int menu_choice;
  do {
    PostAddress this_post = post_search_by_id(post_list->first, post_id);
    UserAddress poster =
        user_search_by_id(user_list->first, this_post->info.user_id);
    BoardAddress board =
        board_search_by_id(board_list->first, this_post->info.board_id);
    int vote_sum;
    Id my_vote_id;
    bool has_voted;
    get_vote_result(*vote_list, &vote_sum, logged_user.id, post_id,
                    VOTE_TARGET_POST, &my_vote_id, &has_voted);
    VoteAddress my_vote = NULL;
    if (has_voted) {
      my_vote = vote_search_by_id(vote_list->first, my_vote_id);
    }
    menu_choice = ui_show_post(
        this_post->info, poster->info, board->info, vote_sum, has_voted,
        my_vote ? my_vote->info : (Vote){0}, logged_user);

    if (menu_choice == 1) {
      if (has_voted) {
        if (my_vote->info.is_upvote) {
          my_vote->info.is_upvote = false;
        } else {
          Vote X;
          vote_delete_by_id(vote_list, my_vote_id, &X);
        }
      } else {
        Vote new_vote;
        VoteAddress new_vote_node;
        vote_create_node(&new_vote_node);
        create_vote(&new_vote, logged_user.id, post_id, VOTE_TARGET_POST, true);
        vote_isi_node(&new_vote_node, new_vote);
        vote_insert(vote_list, new_vote_node);
      }
    } else if (menu_choice == 2) {
      char content[1025];
      printf("Masukkan konten komentar (maksimal 1024 karakter):\n");
      fgets(content, sizeof(content), stdin);
      content[strcspn(content, "\n")] = 0;

      Comment wa;
      create_comment(&wa, logged_user.id, this_post->info.id, -1, content);
      CommentAddress new_comment_node;
      new_comment_node = (CommentAddress)malloc(sizeof(CommentElmtList));
      new_comment_node->info = wa;
      new_comment_node->info.content = strdup(wa.content);
      new_comment_node->first_child = NULL;
      new_comment_node->next_sibling = NULL;
      new_comment_node->parent = NULL;
      free(wa.content);

      comment_tree_list_insert(comment_tree_list, new_comment_node);
    } else if (menu_choice == 3) {
      handle_top_comments_page(post_list, vote_list, logged_user, user_list,
                               comment_tree_list, -1, post_id);
    } else if (menu_choice == 4 && (logged_user.id == this_post->info.user_id || logged_user.id == board->info.owner_id)) {
      printf("Post dan seluruh komentarnya akan dihapus. Apakah anda "
             "yakin? (y/n): ");
      char choice_line[8]; // small buffer
      fgets(choice_line, sizeof(choice_line), stdin);

      if (choice_line[0] != 'y' && choice_line[0] != 'Y') {
        printf("Penghapusan dibatalkan.\n");
        menu_choice = -1;
      }
    }
  } while (menu_choice != 0 && menu_choice != 4);
  if (menu_choice == 4) {
    PostAddress this_post = post_search_by_id(post_list->first, post_id);
    UserAddress poster =
        user_search_by_id(user_list->first, this_post->info.user_id);
    BoardAddress board =
        board_search_by_id(board_list->first, this_post->info.board_id);
    if (this_post->info.user_id == poster->info.id || logged_user.id == board->info.owner_id) {
      Post X;
      post_delete_by_id(post_list, this_post->info.id, &X, vote_list,
                        comment_tree_list);
      printf("Post deleted successfully...\n");
      ui_pause();
    }
  }
}

void handle_top_comments_page(PostList *post_list, VoteList *vote_list,
                              User logged_user, UserList *user_list,
                              CommentTreeList *comment_tree_list, Id user_id,
                              Id post_id) {
  int exit_comments = 0;
  char *search_term = "";
  int total_items, offset = 0;
  bool sort_by_new = false;
  while (!exit_comments) {
    bool search_bool = false;
    if (strcmp(search_term, "") != 0) {
      search_bool = true;
    }
    Item *top_comments = generate_top_comments_array(
        *comment_tree_list, *vote_list, &total_items, post_id, search_term,
        sort_by_new, user_id);
    ui_clear_screen();

    display_top_comments(top_comments, *user_list, *vote_list, total_items,
                         offset, logged_user, *comment_tree_list, search_bool,
                         sort_by_new, user_id);
    if (total_items == 0) {
      printf("\nKomentar ini kosong. Silakan tekan 0 untuk kembali.\n");
      printf("Pilihan: ");
      char choice[10];
      fgets(choice, sizeof(choice), stdin);
      choice[strcspn(choice, "\n")] = 0;
      if (strcmp(choice, "0") == 0) {
        exit_comments = 1;
        ui_clear_screen();
      }
      free(top_comments);
      continue;
    }

    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if (choice[0] == 'N' && offset + 10 < total_items) {
      offset += 10;
    } else if (choice[0] == 'P' && offset > 0) {
      offset -= 10;
    } else if (choice[0] == '0') {
      exit_comments = 1;
      ui_clear_screen();
    } else if (choice[0] == 'W') {
      char content[1025];
      printf("Masukkan konten komentar (maksimal 1024 karakter):\n");
      fgets(content, sizeof(content), stdin);
      content[strcspn(content, "\n")] = 0;

      Comment wa;
      create_comment(&wa, logged_user.id, post_id, -1, content);
      CommentAddress new_comment_node;
      new_comment_node = (CommentAddress)malloc(sizeof(CommentElmtList));
      new_comment_node->info = wa;
      new_comment_node->info.content = strdup(wa.content);
      new_comment_node->first_child = NULL;
      new_comment_node->next_sibling = NULL;
      new_comment_node->parent = NULL;
      free(wa.content);

      comment_tree_list_insert(comment_tree_list, new_comment_node);
    } else if (choice[0] == 'Q') {
      if (strcmp(search_term, "") == 0) {

        char content[1025];
        printf("Masukkan search (maksimal 1024 karakter):\n");
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = 0;

        search_term = strdup(content);
      } else {
        free(search_term);
        search_term = strdup("");
      }
    } else if (choice[0] == 'S') {
      sort_by_new = !sort_by_new;
    } else if (choice[0] == 'M') {
      if (user_id == -1) {
        user_id = logged_user.id;
      } else {
        user_id = -1;
      }
    } else {
      int selected = atoi(choice);
      if (selected > 0 && selected <= 10 && offset + selected <= total_items) {
        handle_comment_selection(top_comments[offset + selected - 1], user_list,
                                 comment_tree_list, vote_list, post_list,
                                 logged_user);
      }
    }
    free(top_comments);
  }
  ui_clear_screen();
}

void handle_single_comment_page(Id comment_id, PostList *post_list,
                                VoteList *vote_list, User logged_user,
                                UserList *user_list,
                                CommentTreeList *comment_tree_list) {
  int menu_choice;
  do {
    CommentAddress this_comment =
        comment_tree_list_search_by_id(comment_tree_list->first, comment_id);

    if (comment_address_is_empty(this_comment)) {
      break;
    }
    PostAddress this_post =
        post_search_by_id(post_list->first, this_comment->info.post_id);
    UserAddress poster =
        user_search_by_id(user_list->first, this_comment->info.user_id);
    int vote_sum;
    Id my_vote_id;
    bool has_voted;
    get_vote_result(*vote_list, &vote_sum, logged_user.id, comment_id,
                    VOTE_TARGET_COMMENT, &my_vote_id, &has_voted);

    VoteAddress my_vote = NULL;
    if (has_voted) {
      my_vote = vote_search_by_id(vote_list->first, my_vote_id);
    }
    CommentAddress this_comment_parent = this_comment->parent;
    bool has_parent = false;
    UserAddress parent_commenter = NULL;
    if (this_comment_parent) {
      has_parent = true;
      parent_commenter = user_search_by_id(user_list->first,
                                           this_comment_parent->info.user_id);
    }
    char *parent_commenter_username = NULL;
    if (parent_commenter) {
      parent_commenter_username = strdup(parent_commenter->info.username);
    }
    menu_choice = ui_show_single_comment(
        this_comment->info, has_parent,
        this_comment_parent ? this_comment_parent->info : (Comment){0},
        this_post->info, poster->info, vote_sum, has_voted,
        my_vote ? my_vote->info : (Vote){0}, parent_commenter_username,
        logged_user);
    if (menu_choice == 1) {
      if (has_voted) {
        if (my_vote->info.is_upvote) {
          my_vote->info.is_upvote = false;
        } else {
          Vote X;
          vote_delete_by_id(vote_list, my_vote_id, &X);
        }
      } else {
        Vote new_vote;
        VoteAddress new_vote_node;
        vote_create_node(&new_vote_node);
        create_vote(&new_vote, logged_user.id, comment_id, VOTE_TARGET_COMMENT,
                    true);
        vote_isi_node(&new_vote_node, new_vote);
        vote_insert(vote_list, new_vote_node);
      }
    } else if (menu_choice == 2) {
      char content[1025];
      printf("Masukkan konten reply (maksimal 1024 karakter):\n");
      fgets(content, sizeof(content), stdin);
      content[strcspn(content, "\n")] = 0;

      Comment wa;
      create_comment(&wa, logged_user.id, this_post->info.id,
                     this_comment->info.id, content);
      CommentAddress new_comment_node;
      new_comment_node = (CommentAddress)malloc(sizeof(CommentElmtList));
      new_comment_node->info = wa;
      new_comment_node->info.content = strdup(wa.content);

      comment_tree_list_insert(comment_tree_list, new_comment_node);
    } else if (menu_choice == 3 &&
               logged_user.id == this_comment->info.user_id) {
      printf("Komentar dan seluruh balasannya akan dihapus. Apakah anda "
             "yakin? (y/n): ");
      char choice_line[8]; // small buffer
      fgets(choice_line, sizeof(choice_line), stdin);

      if (choice_line[0] != 'y' && choice_line[0] != 'Y') {
        printf("Penghapusan dibatalkan.\n");
        menu_choice = -1;
      }
    }
    free(parent_commenter_username);
  } while ((menu_choice != 0) && (menu_choice != 3));
  if (menu_choice == 3) {
    CommentAddress this_comment =
        comment_tree_list_search_by_id(comment_tree_list->first, comment_id);
    if (this_comment->info.user_id == logged_user.id) {

      printf("Menghapus komentar...\n");
      Comment J;
      comment_tree_list_delete_comment_by_id(
          comment_tree_list, this_comment->info.id, &J, vote_list);

      printf("Komentar berhasil dihapus\n");
      ui_pause();
    }
  }
}

void resume_last_navigation(NavigationStack *nav_stack, User *logged_user, BoardList *board_list, PostList *post_list, ModerateList *moderate_list, UserList *user_list, VoteList *vote_list, CommentTreeList *comment_tree_list) {
  char *last_page = navigation_stack_top(*nav_stack);
  if (last_page != NULL) {
    if (strcmp(last_page, "dashboard") == 0) {
      handle_dashboard(board_list, post_list, moderate_list, user_list, vote_list, comment_tree_list, logged_user, nav_stack);
    } else if (strcmp(last_page, "trending") == 0) {
      handle_posts_page(post_list, vote_list, *logged_user, user_list, board_list, comment_tree_list, -1, -1);
    } else if (strcmp(last_page, "boards") == 0) {
      handle_boards_page(post_list, vote_list, *logged_user, user_list, board_list, comment_tree_list, -1);
    } else if (strcmp(last_page, "profile") == 0) {
      handle_profile(*logged_user, post_list, vote_list, user_list, board_list, comment_tree_list, nav_stack);
    } else if (strcmp(last_page, "moderate_queue") == 0) {
      handle_moderate_queue(board_list, post_list, moderate_list, *logged_user);
    } else if (strcmp(last_page, "post") == 0) {
      // Handle post page
      Id post_id = navigation_stack_get_last_post_id(nav_stack);
      handle_post_page(post_id, post_list, vote_list, *logged_user, user_list, board_list, comment_tree_list);
    } else if (strcmp(last_page, "comment") == 0) {
      // Handle comment page
      Id comment_id = navigation_stack_get_last_comment_id(nav_stack);
      handle_single_comment_page(comment_id, post_list, vote_list, *logged_user, user_list, comment_tree_list);
    } else {
      // Fallback
      navigation_stack_push(nav_stack, "dashboard");
      handle_dashboard(board_list, post_list, moderate_list, user_list, vote_list, comment_tree_list, logged_user, nav_stack);
    }
  } else {
    navigation_stack_push(nav_stack, "dashboard");
    handle_dashboard(board_list, post_list, moderate_list, user_list, vote_list, comment_tree_list, logged_user, nav_stack);
  }
}

void handle_profile(User user,
                    PostList *post_list,
                    VoteList *vote_list,
                    UserList *user_list,
                    BoardList *board_list,
                    CommentTreeList *comment_tree_list,
                    NavigationStack *nav_stack) {
  navigation_stack_push(nav_stack, "profile");

  int total_post = 0, total_comment = 0, total_board = 0;

  // Hitung total post
  PostAddress pcurr = post_list->first;
  while (pcurr != NULL) {
    if (pcurr->info.user_id == user.id)
      total_post++;
    pcurr = pcurr->next;
  }

  // Hitung total comment
  CommentTreeAddress ctree = comment_tree_list->first;
  while (ctree != NULL) {
    CommentAddress curr_comment = ctree->info.root;
    if (curr_comment != NULL && curr_comment->info.user_id == user.id)
      total_comment++;

    CommentAddress stack[100];
    int top = 0;
    if (curr_comment) stack[top++] = curr_comment;

    while (top > 0) {
      CommentAddress node = stack[--top];
      CommentAddress child = node->first_child;
      while (child) {
        if (child->info.user_id == user.id)
          total_comment++;
        stack[top++] = child;
        child = child->next_sibling;
      }
    }

    ctree = ctree->next;
  }

  // Hitung total board
  BoardAddress bcurr = board_list->first;
  while (bcurr != NULL) {
    if (bcurr->info.owner_id == user.id)
      total_board++;
    bcurr = bcurr->next;
  }

  int choice;
  do {
    ui_clear_screen();
    printf("========================================\n");
    printf("         PROFIL %s\n", user.username);
    printf("========================================\n");
    printf("Total Post     : %d\n", total_post);
    printf("Total Komentar : %d\n", total_comment);
    printf("Total Board    : %d\n", total_board);
    printf("----------------------------------------\n");
    printf("1. Lihat Post Saya\n");
    printf("2. Lihat Komentar Saya\n");
    printf("3. Lihat Board Saya\n");
    printf("0. Kembali\n");
    printf("========================================\n");
    printf("Pilihan: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
    case 1:
      handle_posts_page(post_list, vote_list, user, user_list, board_list, comment_tree_list, -1, user.id);
      break;
    case 2:
      handle_top_comments_page(post_list, vote_list, user, user_list, comment_tree_list, user.id, -1);
      break;
    case 3:
      handle_boards_page(post_list, vote_list, user, user_list, board_list, comment_tree_list, user.id);
      break;
    }
  } while (choice != 0);

  ui_clear_screen();
}

void handle_comment_selection(Item selected_item, UserList *user_list,
                              CommentTreeList *comment_list,
                              VoteList *vote_list, PostList *post_list,
                              User logged_user) {
  CommentTreeAddress comment_tree = comment_tree_list_search_by_root_id(
      comment_list->first, selected_item.id);
  if (comment_tree != NULL) {
    if (comment_tree_node_count(comment_tree->info) == 1) {
      int original_node_count;
      original_node_count = comment_tree_node_count(comment_tree->info);

      handle_single_comment_page(comment_tree->info.root->info.id, post_list,
                                 vote_list, logged_user, user_list,
                                 comment_list);

      if (comment_tree_node_count(comment_tree->info) == original_node_count) {
        return;
      }
    }
    int choice;
    do {
      ui_clear_screen();
      printf("========================================\n");
      printf("Comment tree for this comment\n");
      printf("========================================\n");
      comment_tree_print_tree(comment_tree->info, *user_list, *vote_list,
                              logged_user);
      printf("\nNavigasi:\n");
      printf("0. Kembali\n");

      printf("========================================\n");
      printf("Pilih nomor: ");
      scanf(" %d", &choice);
      if (choice > 0) {

        int index = 1;
        CommentAddress get =
            get_preorder(comment_tree->info.root, choice, &index);
        if (get) {
          handle_single_comment_page(get->info.id, post_list, vote_list,
                                     logged_user, user_list, comment_list);
        }
      }
    } while (choice != 0);
    ui_clear_screen();
    // Add interaction options (vote, reply, etc.)
    // Implement based on user input handling
  }

  ui_clear_screen();
}
