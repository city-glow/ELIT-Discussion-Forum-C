#ifndef COMMENTTREE_C
#define COMMENTTREE_C
#include "../../include/comment/commentTree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
bool comment_tree_is_empty(CommentTree p) { return p.root; }

bool comment_address_is_empty(CommentAddress p) { return p == NULL; }

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

void comment_tree_in_order(CommentTree p) { comment_tree_in_order_rec(p.root); }

void comment_tree_post_order_rec(CommentAddress c) {
  if (comment_address_is_empty(c))
    return;
  printf("%d ", c->info.id);
  CommentAddress child = c->first_child;
  while (!comment_address_is_empty(child)) {
    comment_tree_post_order_rec(child);
    child = child->next_sibling;
  }
}

void comment_tree_post_order(CommentTree p) {
  comment_tree_post_order_rec(p.root);
}

void comment_tree_level_order(CommentTree p) {
  CommentAddress queue[comment_tree_node_count(p)];
  int front = 0, rear = 0;
  queue[rear++] = p.root;
  while (front != rear) {
    CommentAddress c = queue[front++];
    printf("%d ", c->info.id);
    CommentAddress child = c->next_sibling;
    while (!comment_address_is_empty(child)) {
      queue[rear++] = child;
      child = child->next_sibling;
    }
  }
}

void comment_tree_print_tree_rec(CommentAddress r, int *level,
                                 CommentAddress node, int *num) {
  if (comment_address_is_empty(node))
    return;
  printf("%d. ", *num);
  if (*level >= 2) {
    CommentAddress ancestor = node->parent;
    bool hasSibling[*level - 1];
    int ancestorCount = 0;
    while (!comment_address_is_empty(ancestor->parent)) {
      hasSibling[ancestorCount++] = !comment_address_is_empty(
          ancestor->next_sibling); // ancestor->next_sibling != NULL);
      ancestor = ancestor->parent;
    }
    for (int i = ancestorCount - 1; i >= 0; i--) {
      if (hasSibling[i]) {
        printf("%s", "│  ");
      } else {
        printf("%s", "   ");
      }
    }
  }
  if (*level > 0) {
    if (!comment_address_is_empty(node->next_sibling)) {
      printf("├─ ");
    } else {
      printf("└─ ");
    }
  }
  printf("%c\n", node->info.id);
  CommentAddress child = node->first_child;
  while (!comment_address_is_empty(child)) {
    int nextLevel = *level + 1;
    (*num)++;
    comment_tree_print_tree_rec(r, &nextLevel, child, num);
    child = child->next_sibling;
  }
}

void comment_tree_print_tree(CommentTree p) {
  int level = 0;
  int num = 1;
  comment_tree_print_tree_rec(p.root, &level, p.root, &num);
}

CommentAddress comment_tree_search_by_id_rec(CommentAddress c, Id id) {
  if (comment_address_is_empty(c))
    return NULL;
  if (c->info.id == id)
    return c;
  CommentAddress child = c->first_child;
  while (child != NULL) {
    return comment_tree_search_by_id_rec(child, id);
    child = child->next_sibling;
  }
  return NULL;
}

CommentAddress comment_tree_search_by_id(CommentTree p, Id id) {
  return comment_tree_search_by_id_rec(p.root, id);
}
int comment_tree_node_count_rec(CommentAddress c) {
  if (comment_address_is_empty(c))
    return 0;
  return 1 + comment_tree_node_count_rec(c->first_child) +
         comment_tree_node_count_rec(c->next_sibling);
}
int comment_tree_node_count(CommentTree p) {
  return comment_tree_node_count_rec(p.root);
}

bool create_and_insert_comment_node(CommentTree *p, Comment k) {
  CommentAddress new_comment = malloc(sizeof(CommentElmtList));
  if (!new_comment)
    return false;

  new_comment->info = k;
  new_comment->info.content = strdup(k.content);
  if (!new_comment->info.content) {
    free(new_comment);
    return false;
  }

  new_comment->first_child = NULL;
  new_comment->next_sibling = NULL;
  new_comment->parent = NULL;

  if (k.reply_to == -1) {
    p->root = new_comment;
    p->post_id = k.post_id;
    return true;
  } else {
    CommentAddress found = comment_tree_search_by_id(*p, k.reply_to);
    if (found) {
      new_comment->parent = found;
      CommentAddress *child_ptr = &found->first_child;
      while (*child_ptr != NULL) {
        child_ptr = &(*child_ptr)->next_sibling;
      }
      *child_ptr = new_comment;
      return true;
    } else {
      free(new_comment->info.content);
      free(new_comment);
      return false;
    }
  }
}

void create_comment_tree(CommentTree *p) {
  p->root = NULL;
  p->post_id = -1;
}

// Fungsi untuk menghitung jumlah leaf node
int comment_tree_leaf_count_rec(CommentAddress c) {
  if (c == NULL)
    return 0;
  if (c->first_child == NULL)
    return 1;

  int count = 0;
  CommentAddress child = c->first_child;
  while (child != NULL) {
    count += comment_tree_leaf_count_rec(child);
    child = child->next_sibling;
  }
  return count;
}

int comment_tree_leaf_count(CommentTree P) {
  return comment_tree_leaf_count_rec(P.root);
}

// Fungsi untuk menghitung kedalaman tree
int comment_tree_depth_rec(CommentAddress c) {
  if (c == NULL)
    return 0;

  int max_depth = 0;
  CommentAddress child = c->first_child;
  while (child != NULL) {
    int current_depth = comment_tree_depth_rec(child);
    if (current_depth > max_depth) {
      max_depth = current_depth;
    }
    child = child->next_sibling;
  }
  return 1 + max_depth;
}

int comment_tree_depth(CommentTree P) { return comment_tree_depth_rec(P.root); }

// Fungsi untuk mendapatkan level dari komentar tertentu
int comment_tree_get_level_from_comment(CommentTree p, Comment X) {
  CommentAddress node = comment_tree_search_by_id(p, X.id);
  if (node == NULL)
    return -1; // Tidak ditemukan

  int level = 0;
  CommentAddress current = node;
  while (current->parent != NULL) {
    level++;
    current = current->parent;
  }
  return level;
}

// Fungsi rekursif untuk menghapus node dan subtree-nya
bool delete_comment_by_id_rec(CommentAddress r, Id nilai, bool valid) {
  if (r == NULL)
    return false;

  if ((r->info.id == nilai) || !valid) {
    CommentAddress child = r->first_child;
    while (child != NULL) {
      CommentAddress next = child->next_sibling;
      delete_comment_by_id_rec(child, nilai, false);
      child = next;
    }

    // Bebaskan memori untuk node saat ini
    if (r->info.content != NULL) {
      free(r->info.content);
    }
    free(r);
    return true;
  } else {

    CommentAddress child = r->first_child;
    while (child != NULL) {
      CommentAddress next = child->next_sibling;
      return delete_comment_by_id_rec(child, nilai, true);
      child = next;
    }
    return false;
  }
  // Hapus semua anak terlebih dahulu
}

#endif
