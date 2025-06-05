#ifndef COMMENTLIST_H
#define COMMENTLIST_H
#include "comment.h"
#include <stdbool.h>

typedef struct CommentTElmtList *CommentAddress;
typedef struct CommentTElmtList {
  Comment info;
  CommentAddress first_child;
  CommentAddress next_sibling;
  CommentAddress parent;
} CommentElmtList;
typedef struct {
  CommentAddress root;
  Id post_id;
} CommentTree;


bool comment_tree_is_empty(CommentTree p);
void comment_tree_pre_order(CommentTree p);
void comment_tree_in_order(CommentTree p);
void comment_tree_post_order(CommentTree p);
void comment_tree_level_order(CommentTree p);
void comment_tree_print_tree(CommentTree p);
bool comment_tree_search_by_id(CommentTree p, Id id);
int comment_tree_count(CommentTree p);
int comment_tree_leaf_count(CommentTree P);
int comment_tree_get_level_from_comment(CommentTree p, Comment X);
int comment_tree_depth(CommentTree P);
// int comment_tree_max(infotype Data1, infotype Data2);
#endif
