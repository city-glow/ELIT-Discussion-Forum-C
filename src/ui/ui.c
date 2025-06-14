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
#include "../../include/vote/vote.h"
#include "../../include/vote/voteList.h"
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
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
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
  getchar();
  return choice;
}

int ui_show_dashboard(User user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("--- Selamat datang, %s!--\n", user.username);
  printf("========================================\n");
  printf("1. Add Post\n");
  printf("2. See Profile\n");
  printf("3. Trending Posts\n");
  printf("4. See Boards\n");
  printf("0. Logout\n");
  printf("========================================\n");
  printf("Pilihan: ");

  int choice;
  scanf("%d", &choice);
  getchar(); // Bersihkan newline
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

  printf("Masukkan judul post (maksimal 100 karakter): ");
  fgets(title, sizeof(title), stdin);
  title[strcspn(title, "\n")] = 0;

  printf("Masukkan konten post (maksimal 1024 karakter):\n");
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
                          int total_items, int offset, User logged_user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("-- Top Comments\n");
  printf("========================================\n");
  if (total_items == 0) {
    printf("No comments yet...\n");
  } else {

    for (int i = offset; i < offset + 10 && i < total_items; i++) {
      UserAddress found_user =
          user_search_by_id(user_list.first, items->info.c.user_id);
      printf("%d. user[%s]: %s | ", i - offset + 1, found_user->info.username,
             items[i].info.c.content);
      int vote_sum;
      Id my_vote_id;
      bool has_voted;
      get_vote_result(vote_list, &vote_sum, logged_user.id, items->id,
                      VOTE_TARGET_COMMENT, &my_vote_id, &has_voted);
      VoteAddress my_vote = NULL;
      if (has_voted) {
        my_vote = vote_search_by_id(vote_list.first, my_vote_id);
      }
      print_vote(has_voted, my_vote ? my_vote->info : (Vote){0}, vote_sum);
      printf("\n");
    }
  }
  printf("\nNavigasi:\n");
  if (offset > 0)
    printf("P. Previous Page\n");
  if (offset + 10 < total_items)
    printf("N. Next Page\n");
  printf("0. Kembali\n");

  printf("========================================\n");
  printf("Pilih nomor atau navigasi: ");
}

int ui_show_single_comment(Comment comment, bool has_parent,
                           Comment comment_parent, Post post, User poster,
                           Board board, int vote_sum, bool has_voted,
                           Vote my_vote, char *parent_commenter_username,
                           User logged_user) {
  ui_clear_screen();
  printf("========================================\n");
  printf("-- COMMENT by user[%s]\n", poster.username);
  printf("========================================\n");
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
void handle_dashboard(BoardList *board_list, PostList *post_list,
                      UserList *user_list, VoteList *vote_list,
                      CommentTreeList *comment_tree_list, const User *user) {
  int dashboard_choice;
  do {
    dashboard_choice = ui_show_dashboard(*user);

    if (dashboard_choice == 1) { // Create new post
      char board_title[MAX_TITLE + 1];

      printf("Masukkan nama board tempat anda ingin post (maksimal 100): ");
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
      handle_post_page(1, post_list, vote_list, *user, user_list, board_list,
                       comment_tree_list);
    }

  } while (dashboard_choice != 0);
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
          vote_delete_by_id(&(vote_list->first), my_vote_id, &X);
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

      comment_tree_list_insert(comment_tree_list, new_comment_node);
    }
    if (menu_choice == 3) {
      int total_items, offset = 0;
      Item *top_comments = generate_top_comments_array(
          *comment_tree_list, *vote_list, &total_items);
      int exit_comments = 0;
      while (!exit_comments) {
        ui_clear_screen();
        display_top_comments(top_comments, *user_list, *vote_list, total_items,
                             offset, logged_user);
        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        if (choice[0] == 'N' && offset + 10 < total_items) {
          offset += 10;
        } else if (choice[0] == 'P' && offset > 0) {
          offset -= 10;
        } else if (choice[0] == '0') {
          exit_comments = 1;
        } else {
          int selected = atoi(choice);
          if (selected > 0 && selected <= 10 &&
              offset + selected <= total_items) {
            handle_comment_selection(top_comments[offset + selected - 1],
                                     user_list, comment_tree_list, vote_list,
                                     post_list, board_list, logged_user);
          }
        }
      }
      free(top_comments);
    }
  } while (menu_choice != 0);
}

void handle_single_comment_page(Id comment_id, PostList *post_list,
                                VoteList *vote_list, User logged_user,
                                UserList *user_list, BoardList *board_list,
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
    BoardAddress board =
        board_search_by_id(board_list->first, this_post->info.board_id);
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
        this_post->info, poster->info, board->info, vote_sum, has_voted,
        my_vote ? my_vote->info : (Vote){0}, parent_commenter_username,
        logged_user);
    if (menu_choice == 1) {
      if (has_voted) {
        if (my_vote->info.is_upvote) {
          my_vote->info.is_upvote = false;
        } else {
          Vote X;
          vote_delete_by_id(&(vote_list->first), my_vote_id, &X);
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
    }
    free(parent_commenter_username);
  } while ((menu_choice != 0) && (menu_choice != 3));
}

void handle_comment_selection(Item selected_item, UserList *user_list,
                              CommentTreeList *comment_list,
                              VoteList *vote_list, PostList *post_list,
                              BoardList *board_list, User logged_user) {
  CommentTreeAddress comment_tree = comment_tree_list_search_by_root_id(
      comment_list->first, selected_item.id);
  if (comment_tree != NULL) {
    if (comment_tree_node_count(comment_tree->info) == 1) {
      int original_node_count;
      original_node_count = comment_tree_node_count(comment_tree->info);

      handle_single_comment_page(comment_tree->info.root->info.id, post_list,
                                 vote_list, logged_user, user_list, board_list,
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
                                     logged_user, user_list, board_list,
                                     comment_list);
        }
      }
    } while (choice != 0);
    // Add interaction options (vote, reply, etc.)
    // Implement based on user input handling
  }
}
