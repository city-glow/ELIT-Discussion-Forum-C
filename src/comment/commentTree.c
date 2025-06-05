#ifndef COMMENTTREE_C
#define COMMENTTREE_C
#include "../../include/comment/commentTree.h"
#include <stddef.h>
#include <stdio.h>
bool comment_tree_is_empty(CommentTree p) {
    return p.root;
}

bool comment_address_is_empty(CommentAddress p) {
    return p == NULL;
}

void comment_tree_pre_order_rec(CommentAddress c) {
    if (comment_address_is_empty(c))
      return;
    CommentAddress child = c->first_child;
    while (!comment_address_is_empty(child)) {
      comment_tree_pre_order_rec(child);
      child = child->next_sibling;
    }
    printf("%d ", c->info.id);
}

void comment_tree_pre_order(CommentTree p) {
    comment_tree_pre_order_rec(p.root);
}

void comment_tree_in_order_rec(CommentAddress c) {
    if (comment_address_is_empty(c))
      return;
    CommentAddress child = c->first_child;
    if (!comment_address_is_empty(child)) {
      comment_tree_in_order_rec(child);
    }

    printf("%d ", c->info.id);

    if (!comment_address_is_empty(child)) {
      child = child->next_sibling;
    }

    while (!comment_address_is_empty(child)) {
      comment_tree_in_order_rec(child);
      child = child->next_sibling;
    }
}
void comment_tree_in_order(CommentTree p);
void comment_tree_post_order(CommentTree p);
void comment_tree_level_order(CommentTree p);
void comment_tree_print_tree(CommentTree p);
bool comment_tree_search_by_id(CommentTree p, Id id);
int comment_tree_count(CommentTree p);
int comment_tree_leaf_count(CommentTree P);
int comment_tree_get_level_from_comment(CommentTree p, Comment X);
int comment_tree_depth(CommentTree P);
#endif
