#ifndef ITEMARRAY_C
#define ITEMARRAY_C
#include "../../include/array/itemArray.h"
#include <stdlib.h>
#include <stdio.h>

int compare_vote_sum(const void *a, const void *b) {
  return ((Item *)b)->vote_sum - ((Item *)a)->vote_sum;
}

int get_comment_vote_sum(VoteList vote_list, Id comment_id) {
  int sum = 0;
  VoteAddress current = vote_list.first;
  while (current != NULL) {
    if (current->info.target_id == comment_id &&
        current->info.target_type == VOTE_TARGET_COMMENT) {
      sum += current->info.is_upvote ? 1 : -1;
    }
    current = current->next;
  }
  return sum;
}

Item *generate_top_comments_array(CommentTreeList comment_list,
                                  VoteList vote_list, int *count) {
                                    printf("in generate top comments array rn");
  int size = comment_tree_list_count(comment_list.first);
  Item *items = (Item *)malloc(size * sizeof(Item));
  *count = 0;
  CommentTreeAddress current = comment_list.first;
  while (current != NULL) {
    if (current->info.root != NULL) {
      items[*count].info.c = current->info.root->info;
      items[*count].type = ITEM_TYPE_COMMENT;
      items[*count].id = current->info.root->info.id;
      items[*count].vote_sum =
          get_comment_vote_sum(vote_list, items[*count].id);
      (*count)++;
    }
    current = current->next;
  }
  qsort(items, *count, sizeof(Item), compare_vote_sum);
  return items;
}

Item *generate_top_posts_array(PostList post_list, VoteList vote_list,
                               int *count, Id board) {
  int size = post_count(post_list.first);
  Item *items = (Item *)malloc(size * sizeof(Item));
  *count = 0;
  PostAddress current = post_list.first;
  if (board == -1) {

    while (current != NULL) {
      items[*count].info.p = current->info;
      items[*count].type = ITEM_TYPE_POST;
      items[*count].id = current->info.id;
      items[*count].vote_sum =
          get_comment_vote_sum(vote_list, items[*count].id);
      (*count)++;
      current = current->next;
    }
  } else {
    while (current != NULL) {
      if (current->info.board_id == board) {
        items[*count].info.p = current->info;
        items[*count].type = ITEM_TYPE_POST;
        items[*count].id = current->info.id;
        items[*count].vote_sum =
            get_comment_vote_sum(vote_list, items[*count].id);
        (*count)++;
        current = current->next;
      }
    }
  }
  qsort(items, *count, sizeof(Item), compare_vote_sum);
  return items;
}

#endif
