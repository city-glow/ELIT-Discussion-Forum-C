#ifndef POSTLIST_C
#define POSTLIST_C
#include "../../include/post/postList.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool post_is_empty(PostAddress p) { return (p == NULL); }

void post_create_list(PostList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void post_create_node(PostAddress *p) {
  *p = (PostAddress)malloc(sizeof(PostElmtList));
}

void post_isi_node(PostAddress *p, Post nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (*p)->info.content = strdup(nilai.content);
    (**p).next = NULL;
  }
}

void post_tampil_list(PostAddress p) {
  while (p != NULL) {
    if (p->info.approved) {
      printf("%s -> ", p->info.title);
    }
    p = p->next;
  }
  printf("NULL\n");
}

void post_insert_awal(PostAddress *p, PostAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void post_insert_akhir(PostAddress *p, PostAddress PNew) {
  if (post_is_empty(*p)) {
    *p = PNew;
  } else {
    PostAddress temp = *p;
    while (!post_is_empty((*temp).next)) {
      temp = (*temp).next;
    }
    (*temp).next = PNew;
  }
}

PostAddress post_search_by_id(PostAddress p, Id nilai) {
  while (!post_is_empty(p)) {
    if (p->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}

void post_insert(PostList *p, PostAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  post_insert_akhir(&(p->first), PNew);
}

void post_insert_after(PostAddress *pBef, PostAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void post_del_awal(PostAddress *p, Post *X) {
  if (!post_is_empty(*p)) {
    *X = (**p).info;
    X->content = strdup((*p)->info.content);
    PostAddress temp = *p;

    *p = (**p).next;
    (*temp).next = NULL;
    free(temp->info.content);
    free(temp);
  }
}

void post_del_akhir(PostAddress *p, Post *X) {
  if (!post_is_empty(*p)) {
    if (post_is_empty((**p).next)) {
      printf("yellow!");
      *X = (**p).info;

      X->content = strdup((*p)->info.content);
      free((*p)->info.content);
      free(*p);
      *p = NULL;
    } else {
      PostAddress prev;
      PostAddress last = *p;
      while (!post_is_empty((*last).next)) {
        prev = last;
        last = (*last).next;
      }
      *X = (*last).info;
      X->content = strdup((*p)->info.content);
      free(last->info.content);
      free(last);
      (*prev).next = NULL;
    }
  }
}

void post_del_after(PostAddress *pBef, Post *X) {
  PostAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp->info.content);
  free(temp);
}

void post_delete_by_address(PostAddress *p, PostAddress pDel, Post *X) {
  if (post_is_empty(*p) || post_is_empty(pDel))
    return;

  if (*p == pDel) {
    post_del_awal(p, X);
  } else {
    PostAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      post_del_after(&temp, X);
    }
  }
}

void post_delete_by_id(PostAddress *p, Id nilai, Post *X, VoteList *vote_list,
                       CommentTreeList *comment_tree_list) {
  PostAddress target = post_search_by_id(*p, nilai);
  post_delete_by_address(p, target, X);
  vote_delete_all_by_target(&(*vote_list).first, X->id, VOTE_TARGET_POST);
  CommentTree Y;
  comment_delete_all_post_id_with_votes(&(comment_tree_list->first), X->id, &Y, vote_list);
}

void post_deallocation(PostAddress *p) {
  while (!post_is_empty(*p)) {
    Post i;
    post_del_awal(p, &i);
  }
}

// Delete all posts by board id and save after deletion
void post_delete_all_by_board_id(PostList *list, Id board_id, VoteList *vote_list, CommentTreeList *comment_tree_list) {
  PostAddress *curr = &(list->first);
  while (*curr != NULL) {
    if ((*curr)->info.board_id == board_id) {
      Post X;
      PostAddress to_delete = *curr;
      *curr = (*curr)->next;
      post_delete_by_address(curr, to_delete, &X);
      vote_delete_all_by_target(&vote_list->first, X.id, VOTE_TARGET_POST);
      CommentTree Y;
      comment_delete_all_post_id_with_votes(&(comment_tree_list->first), X.id, &Y, vote_list);
      save_post_list(list, "../storage/posts.dat");
    } else {
      curr = &((*curr)->next);
    }
  }
  save_post_list(list, "../storage/posts.dat");
}

int post_count(PostAddress p) {
  if (post_is_empty(p)) {
    return 0;
  } else {
    return 1 + post_count((*p).next);
  }
}

PostAddress post_balik_list(PostAddress p) {
  if (!post_is_empty(p)) {
    PostAddress prev = NULL;
    PostAddress this = p;
    PostAddress next = (*p).next;
    while (!post_is_empty(next)) {
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
#endif

// =====================
// Save/Load Functions
// =====================

void save_post_list(PostList *list, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    printf("Failed to open %s for writing\n", filename);
    return;
  }
  // Save id_max
  fwrite(&(list->id_max), sizeof(Id), 1, file);

  // Count posts
  int count = 0;
  PostAddress current = list->first;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  fwrite(&count, sizeof(int), 1, file);

  // Write each post
  current = list->first;
  while (current != NULL) {
    // Write struct fields except content pointer
    fwrite(&(current->info.id), sizeof(Id), 1, file);
    fwrite(&(current->info.user_id), sizeof(Id), 1, file);
    fwrite(&(current->info.board_id), sizeof(Id), 1, file);
    fwrite(&(current->info.edited), sizeof(bool), 1, file);
    fwrite(&(current->info.approved), sizeof(bool), 1, file);
    fwrite(current->info.title, sizeof(char), MAX_TITLE + 1, file);

    // Write content string length and content
    int content_len = current->info.content ? strlen(current->info.content) : 0;
    fwrite(&content_len, sizeof(int), 1, file);
    if (content_len > 0) {
      fwrite(current->info.content, sizeof(char), content_len, file);
    }
    current = current->next;
  }
  fclose(file);
}

void load_post_list(PostList *list, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Failed to open %s for reading\n", filename);
    return;
  }
  post_create_list(list);

  // Load id_max
  fread(&(list->id_max), sizeof(Id), 1, file);

  int count = 0;
  fread(&count, sizeof(int), 1, file);

  for (int i = 0; i < count; ++i) {
    Post temp;
    fread(&(temp.id), sizeof(Id), 1, file);
    fread(&(temp.user_id), sizeof(Id), 1, file);
    fread(&(temp.board_id), sizeof(Id), 1, file);
    fread(&(temp.edited), sizeof(bool), 1, file);
    fread(&(temp.approved), sizeof(bool), 1, file);
    fread(temp.title, sizeof(char), MAX_TITLE + 1, file);

    int content_len = 0;
    fread(&content_len, sizeof(int), 1, file);
    if (content_len > 0) {
      temp.content = (char *)malloc(content_len + 1);
      fread(temp.content, sizeof(char), content_len, file);
      temp.content[content_len] = '\0';
    } else {
      temp.content = strdup("");
    }

    PostAddress new_node;
    post_create_node(&new_node);
    post_isi_node(&new_node, temp);
    post_insert_akhir(&(list->first), new_node);

    // Free temp.content because post_isi_node duplicates it
    free(temp.content);
  }
  fclose(file);
}
