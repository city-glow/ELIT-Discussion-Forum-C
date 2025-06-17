#ifndef ITEMARRAY_C
#define ITEMARRAY_C
#include "../../include/array/itemArray.h"
#include <stdlib.h>
#include <string.h>

int compare_vote_sum(const void *a, const void *b) {
  return ((Item *)b)->vote_sum - ((Item *)a)->vote_sum;
}

int get_vote_sum(VoteList vote_list, Id comment_id,
                 VoteTargetType target_type) {
  int sum = 0;
  VoteAddress current = vote_list.first;
  while (current != NULL) {
    if (current->info.target_id == comment_id &&
        current->info.target_type == target_type) {
      sum += current->info.is_upvote ? 1 : -1;
    }
    current = current->next;
  }
  return sum;
}

bool word_exact_case_insensitive_match(const char *word1, const char *word2) {
  if (!word1 || !word2)
    return false;
  return strcasecmp(word1, word2) == 0;
}

bool text_contains_word(const char *text, const char *search_word) {
  if (!text || !search_word)
    return false;

  char buffer[1024];
  strncpy(buffer, text, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';

  char *token = strtok(buffer, " ");
  while (token != NULL) {
    if (word_exact_case_insensitive_match(token, search_word)) {
      return true;
    }
    token = strtok(NULL, " ");
  }

  return false;
}

bool post_matches_search(Post post, const char *search_term) {
  if (!search_term || strlen(search_term) == 0)
    return true;

  char buffer[256];
  strncpy(buffer, search_term, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';

  char *token = strtok(buffer, " ");
  while (token != NULL) {
    if (text_contains_word(post.title, token) ||
        text_contains_word(post.content, token)) {
      return true; // match on a full word
    }
    token = strtok(NULL, " ");
  }

  return false;
}

bool comment_matches_search(Comment post, const char *search_term) {
  if (!search_term || strlen(search_term) == 0)
    return true;

  char buffer[256];
  strncpy(buffer, search_term, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';

  char *token = strtok(buffer, " ");
  while (token != NULL) {
    if (text_contains_word(post.content, token)) {
      return true; // match on a full word
    }
    token = strtok(NULL, " ");
  }

  return false;
}

bool board_matches_search(Board board, const char *search_term) {
  if (!search_term || strlen(search_term) == 0)
    return true;

  char buffer[256];
  strncpy(buffer, search_term, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';

  char *token = strtok(buffer, " ");
  while (token != NULL) {
    if (text_contains_word(board.title, token)) {
      return true; // match on a full word
    }
    token = strtok(NULL, " ");
  }

  return false;
}

Item *generate_top_comments_array(CommentTreeList comment_list,
                                  VoteList vote_list, int *count, Id post_id,
                                  const char *search_term, bool sort_by_new,
                                  Id user) {
  int size = comment_tree_list_count(comment_list.first);
  Item *items = (Item *)malloc(size * sizeof(Item));
  *count = 0;
  CommentTreeAddress current = comment_list.first;
  while (current != NULL) {
    if (current->info.root != NULL) {
      if (current->info.post_id == post_id) {

        bool user_match =
            (user == -1 || current->info.root->info.user_id == user);
        bool search_match =
            comment_matches_search(current->info.root->info, search_term);
        if (user_match && search_match) {

          items[*count].info.c = current->info.root->info;
          items[*count].type = ITEM_TYPE_COMMENT;
          items[*count].id = current->info.root->info.id;
          items[*count].vote_sum =
              get_vote_sum(vote_list, items[*count].id, VOTE_TARGET_COMMENT);
          (*count)++;
        }
      }
    }
    current = current->next;
  }
  qsort(items, *count, sizeof(Item), compare_vote_sum);
  return items;
}

Item *generate_top_posts_array(PostList post_list, VoteList vote_list,
                               int *count, Id board, bool sort_by_new, Id user,
                               const char *search_term) {
  int size = post_count(post_list.first);
  Item *items = (Item *)malloc(size * sizeof(Item));
  *count = 0;
  PostAddress current = post_list.first;
  while (current != NULL) {
    Post post = current->info;
    bool board_match = (board == -1 || post.board_id == board);
    bool user_match = (user == -1 || post.user_id == user);
    bool search_match = post_matches_search(post, search_term);

    if (board_match && user_match && search_match) {
      items[*count].info.p = post;
      items[*count].type = ITEM_TYPE_POST;
      items[*count].id = post.id;
      items[*count].vote_sum =
          get_vote_sum(vote_list, post.id, VOTE_TARGET_POST);
      (*count)++;
    }

    current = current->next;
  }

  if (sort_by_new) {
  	int i = 0;
    for (i; i < *count / 2; i++) {
      Item temp = items[i];
      items[i] = items[*count - 1 - i];
      items[*count - 1 - i] = temp;
    }
  } else {

    qsort(items, *count, sizeof(Item), compare_vote_sum);
  }
  return items;
}

int compare_boards_by_title(const void *a, const void *b) {
  const Item *itemA = (const Item *)a;
  const Item *itemB = (const Item *)b;
  return strcmp(itemA->info.b.title, itemB->info.b.title);
}

Item *generate_top_boards_array(BoardList board_list, int *count,
                                bool sort_by_new, Id user,
                                const char *search_term) {
  int size = board_count(board_list.first);
  Item *items = (Item *)malloc(size * sizeof(Item));
  *count = 0;
  BoardAddress current = board_list.first;
  while (current != NULL) {
    Board board = current->info;
    bool user_match = (user == -1 || board.owner_id == user);
    bool search_match = board_matches_search(board, search_term);

    if (user_match && search_match) {
      items[*count].info.b = board;
      items[*count].type = ITEM_TYPE_BOARD;
      items[*count].id = board.id;
      (*count)++;
    }

    current = current->next;
  }

  if (sort_by_new) {
  	int i = 0;
    for (i = 0; i < *count / 2; i++) {
      Item temp = items[i];
      items[i] = items[*count - 1 - i];
      items[*count - 1 - i] = temp;
    }
  } else {
    qsort(items, *count, sizeof(Item), compare_boards_by_title);
  }
  return items;
}

#endif
