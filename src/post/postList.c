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
  if (post_is_empty(p)) {
    printf("NULL\n");
  } else {

    printf("%s -> ", (*p).info.title);
    post_tampil_list((*p).next);
  }
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

void post_delete_by_id(PostAddress *p, Id nilai, Post *X) {
  PostAddress target = post_search_by_id(*p, nilai);
  post_delete_by_address(p, target, X);
}

void post_deallocation(PostAddress *p) {
  while (!post_is_empty(*p)) {
    Post i;
    post_del_awal(p, &i);
  }
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
