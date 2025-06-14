#ifndef COMMENTTREELIST_C
#define COMMENTTREELIST_C
#include "../../include/comment/commentTreeList.h"
#include "../../include/comment/commentTree.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool comment_tree_list_is_empty(CommentTreeAddress p) { return (p == NULL); }

void comment_tree_list_create_list(CommentTreeList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void comment_tree_list_create_node(CommentTreeAddress *p) {
  *p = (CommentTreeAddress)malloc(sizeof(CommentTreeElmtList));
  if (*p == NULL) {
        perror("malloc failed");
        exit(1);
    }
}

void comment_tree_list_isi_node(CommentTreeAddress *p, CommentTree nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (**p).next = NULL;
  }
}

void comment_tree_list_tampil_list(CommentTreeAddress p, UserList user_list,
                                   VoteList vote_list, User logged_user) {
  if (comment_tree_list_is_empty(p)) {
    printf("NULL\n");
  } else {
    comment_tree_print_tree(p->info, user_list, vote_list, logged_user);
    comment_tree_list_tampil_list((*p).next, user_list, vote_list, logged_user);
  }
}

void comment_tree_list_insert_awal(CommentTreeAddress *p,
                                   CommentTreeAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void comment_tree_list_insert_akhir(CommentTreeAddress *p,
                                    CommentTreeAddress PNew) {

if (p == NULL || PNew == NULL) {
        fprintf(stderr, "FATAL: insert_akhir got null pointer\n");
        return;
    }
  if (comment_tree_list_is_empty(*p)) {
    *p = PNew;
  } else {
    CommentTreeAddress temp = *p;
    while (temp->next != NULL) {
      temp = (*temp).next;
    }
    (*temp).next = PNew;
  }
}

CommentTreeAddress comment_tree_list_search_by_post_id(CommentTreeAddress p,
                                                       Id nilai) {
  while (!comment_tree_list_is_empty(p)) {
    if (p->info.post_id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}

CommentTreeAddress comment_tree_list_search_by_root_id(CommentTreeAddress p,
                                                       Id nilai) {
  while (!comment_tree_list_is_empty(p)) {
    if (p->info.root->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}

CommentAddress comment_tree_list_search_by_id(CommentTreeAddress p, Id nilai) {
  if (nilai <= 0)
    return NULL;
  while (!comment_tree_list_is_empty(p)) {
    CommentAddress result = comment_tree_search_by_id(p->info, nilai);
    if (result != NULL) {
      return result; // Found the ID
    }
    p = p->next; // Move to next tree in the list
  }
  return NULL; // Not found
}

void comment_tree_list_insert(CommentTreeList *p, CommentAddress PNew) {
  // Assign unique ID
  p->id_max += 1;
  PNew->info.id = p->id_max;

  // Case 1: Top-level comment (no reply)
  if (PNew->info.reply_to == -1) {
    CommentTreeAddress np;
    comment_tree_list_create_node(&np); // Allocate node for new tree
    create_comment_tree(&np->info);     // Initialize the tree
    np->next = NULL; //THIS FIXES IT LOLLLL
    

    // Insert the comment node into the new tree
    if (!create_and_insert_comment_node(&np->info, PNew->info)) {
      // Handle failure (e.g. memory allocation)
      return;
    }

    // Insert new CommentTree node into the list
    comment_tree_list_insert_akhir(&(p->first), np);
  }
  // Case 2: Reply to existing comment
  else {
    CommentTreeAddress curr = p->first;
    bool inserted = false;

    // Search through all trees for the parent comment
    while (curr != NULL && !inserted) {
      inserted = create_and_insert_comment_node(&curr->info, PNew->info);
      curr = curr->next;
    }

    // Optional: Handle case when parent ID is not found in any tree
    if (!inserted) {
      printf("node not inserted");
      printf("enter to contnieu");
      getchar();
      // You could log a warning or handle the orphaned comment here
    }
  }
}

void comment_tree_list_insert_after(CommentTreeAddress *pBef,
                                    CommentTreeAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void comment_tree_list_del_awal(CommentTreeAddress *p, CommentTree *X) {
  if (!comment_tree_list_is_empty(*p)) {
    *X = (**p).info;
    CommentTreeAddress temp = *p;
    *p = (**p).next;
    (*temp).next = NULL;
    free(temp);
  }
}

void comment_tree_list_del_akhir(CommentTreeAddress *p, CommentTree *X) {
  if (!comment_tree_list_is_empty(*p)) {
    if (comment_tree_list_is_empty((**p).next)) {
      printf("yellow!");
      *X = (**p).info;
      free(*p);
      *p = NULL;
    } else {
      CommentTreeAddress prev;
      CommentTreeAddress last = *p;
      while (!comment_tree_list_is_empty((*last).next)) {
        prev = last;
        last = (*last).next;
      }
      *X = (*last).info;
      free(last);
      (*prev).next = NULL;
    }
  }
}

void comment_tree_list_del_after(CommentTreeAddress *pBef, CommentTree *X) {
  CommentTreeAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp);
}

void comment_tree_list_delete_by_address(CommentTreeAddress *p,
                                         CommentTreeAddress pDel,
                                         CommentTree *X) {
  if (comment_tree_list_is_empty(*p) || comment_tree_list_is_empty(pDel))
    return;

  if (*p == pDel) {
    comment_tree_list_del_awal(p, X);
  } else {
    CommentTreeAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      comment_tree_list_del_after(&temp, X);
    }
  }
}

void comment_tree_list_delete_by_post_id(CommentTreeAddress *p, Id nilai,
                                         CommentTree *X) {
  CommentTreeAddress target = comment_tree_list_search_by_post_id(*p, nilai);
  comment_tree_list_delete_by_address(p, target, X);
}

// #include "../../include/vote/voteList.h"
// ...

// Helper to traverse and delete votes for all comments in a tree
static void delete_votes_in_comment_tree(CommentAddress node,
                                         VoteList *vote_list) {
  if (!node)
    return;
  // Delete all votes for this comment
  vote_delete_all_by_target(&vote_list->first, node->info.id,
                            VOTE_TARGET_COMMENT);
  // Traverse children
  CommentAddress child = node->first_child;
  while (child) {
    delete_votes_in_comment_tree(child, vote_list);
    child = child->next_sibling;
  }
}

void comment_delete_all_post_id_with_votes(CommentTreeAddress *p, Id nilai,
                                           CommentTree *X,
                                           VoteList *vote_list) {
  CommentTreeAddress target = comment_tree_list_search_by_post_id(*p, nilai);
  while (target != NULL) {
    // Delete votes for all comments in this tree
    if (target->info.root) {
      delete_votes_in_comment_tree(target->info.root, vote_list);
    }
    comment_tree_list_delete_by_address(p, target, X);
    target = comment_tree_list_search_by_post_id(*p, nilai);
  }
}

// Old function kept for compatibility
void comment_delete_all_post_id(CommentTreeAddress *p, Id nilai,
                                CommentTree *X) {
  CommentTreeAddress target = comment_tree_list_search_by_post_id(*p, nilai);
  while (target != NULL) {
    comment_tree_list_delete_by_address(p, target, X);
    target = comment_tree_list_search_by_post_id(*p, nilai);
  }
}

void comment_tree_list_delete_comment_by_id(CommentTreeList *p, Id nilai,
                                            Comment *X) {
  if (p == NULL || p->first == NULL)
    return;

  CommentTreeAddress tree = p->first;
  while (tree != NULL) {
    CommentAddress root = tree->info.root;

    // Special case: the root of this tree is the node to delete
    if (root != NULL && root->info.id == nilai) {
      if (X != NULL) {
        *X = root->info;
      }

      delete_comment_by_id_rec(root, nilai, false);
      tree->info.root = NULL; // Delete root only, not the whole list
      return;
    }

    // Search and delete within the tree
    if (delete_comment_by_id_rec(tree->info.root, nilai, true)) {
      return; // Found and deleted
    }

    tree = tree->next;
  }
}

void comment_tree_list_deallocation(CommentTreeAddress *p) {
  while (!comment_tree_list_is_empty(*p)) {
    CommentTree i;
    comment_tree_list_del_awal(p, &i);
  }
}

int comment_tree_list_count(CommentTreeAddress p) {
  if (comment_tree_list_is_empty(p)) {
    return 0;
  } else {
    return 1 + comment_tree_list_count(p->next);
  }
}

CommentTreeAddress comment_tree_list_balik_list(CommentTreeAddress p) {
  if (!comment_tree_list_is_empty(p)) {
    CommentTreeAddress prev = NULL;
    CommentTreeAddress this = p;
    CommentTreeAddress next = (*p).next;
    while (!comment_tree_list_is_empty(next)) {
      (*this).next = prev;
      prev = this;
      this = next;
      next = (*this).next;
    }
    (*this).next = prev;
    p = this;
  }
  return p;
}

// bool get_comment_tree_list_by_id(CommentTreeList p, Id nilai,
//                                  CommentTree *get) {
//   CommentTreeAddress found = comment_tree_list_search_by_id(p.first, nilai);
//   if (!comment_tree_list_is_empty(found)) {
//     *get = found->info;
//   }
//   return !comment_tree_list_is_empty(found);
// }

// =====================
// Save/Load Functions
// =====================

static void save_comment_node(FILE *file, CommentAddress node) {
  if (!node)
    return;
  // Save comment fields
  fwrite(&(node->info.id), sizeof(Id), 1, file);
  fwrite(&(node->info.user_id), sizeof(Id), 1, file);
  fwrite(&(node->info.post_id), sizeof(Id), 1, file);
  fwrite(&(node->info.reply_to), sizeof(Id), 1, file);

  int content_len = node->info.content ? strlen(node->info.content) : 0;
  fwrite(&content_len, sizeof(int), 1, file);
  if (content_len > 0) {
    fwrite(node->info.content, sizeof(char), content_len, file);
  }

  // Save children count
  int child_count = 0;
  CommentAddress child = node->first_child;
  while (child) {
    child_count++;
    child = child->next_sibling;
  }
  fwrite(&child_count, sizeof(int), 1, file);

  // Save each child recursively
  child = node->first_child;
  while (child) {
    save_comment_node(file, child);
    child = child->next_sibling;
  }
}

void save_comment_tree_list(CommentTreeList *list, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    printf("Failed to open %s for writing\n", filename);
    return;
  }
  // Save id_max
  fwrite(&(list->id_max), sizeof(Id), 1, file);

  // Count trees
  int tree_count = 0;
  CommentTreeAddress curr = list->first;
  while (curr) {
    tree_count++;
    curr = curr->next;
  }
  fwrite(&tree_count, sizeof(int), 1, file);

  // Save each tree
  curr = list->first;
  while (curr) {
    // Save post_id
    fwrite(&(curr->info.post_id), sizeof(Id), 1, file);
    // Save tree existence (root != NULL)
    int has_root = curr->info.root != NULL ? 1 : 0;
    fwrite(&has_root, sizeof(int), 1, file);
    if (has_root) {
      save_comment_node(file, curr->info.root);
    }
    curr = curr->next;
  }
  fclose(file);
}

static CommentAddress load_comment_node(FILE *file) {
  Comment temp;
  fread(&(temp.id), sizeof(Id), 1, file);
  fread(&(temp.user_id), sizeof(Id), 1, file);
  fread(&(temp.post_id), sizeof(Id), 1, file);
  fread(&(temp.reply_to), sizeof(Id), 1, file);

  int content_len = 0;
  fread(&content_len, sizeof(int), 1, file);
  if (content_len > 0) {
    temp.content = (char *)malloc(content_len + 1);
    fread(temp.content, sizeof(char), content_len, file);
    temp.content[content_len] = '\0';
  } else {
    temp.content = strdup("");
  }

  CommentAddress node = (CommentAddress)malloc(sizeof(CommentElmtList));
  node->info = temp;
  node->first_child = NULL;
  node->next_sibling = NULL;
  node->parent = NULL;

  int child_count = 0;
  fread(&child_count, sizeof(int), 1, file);

  CommentAddress prev_child = NULL;
  for (int i = 0; i < child_count; ++i) {
    CommentAddress child = load_comment_node(file);
    child->parent = node;
    if (!node->first_child) {
      node->first_child = child;
    } else {
      prev_child->next_sibling = child;
    }
    prev_child = child;
  }
  return node;
}

void load_comment_tree_list(CommentTreeList *list, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Failed to open %s for reading\n", filename);
    return;
  }
  comment_tree_list_create_list(list);

  fread(&(list->id_max), sizeof(Id), 1, file);
  int tree_count = 0;
  fread(&tree_count, sizeof(int), 1, file);

  for (int i = 0; i < tree_count; ++i) {
    CommentTree temp_tree;
    fread(&(temp_tree.post_id), sizeof(Id), 1, file);
    int has_root = 0;
    fread(&has_root, sizeof(int), 1, file);
    if (has_root) {
      temp_tree.root = load_comment_node(file);
    } else {
      temp_tree.root = NULL;
    }
    CommentTreeAddress new_node;
    comment_tree_list_create_node(&new_node);
    comment_tree_list_isi_node(&new_node, temp_tree);
    comment_tree_list_insert_akhir(&(list->first), new_node);
  }
  fclose(file);
}
#endif