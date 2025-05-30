#ifndef USERLIST_C
#define USERLIST_C
#include "../../include/user/userList.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>

bool user_is_empty(UserAddress p) { return (p == NULL); }

void user_create_list(UserList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void user_create_node(UserAddress *p) {
  *p = (UserAddress)malloc(sizeof(UserElmtList));
}

void user_isi_node(UserAddress *p, User nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (**p).next = NULL;
  }
}

void user_tampil_list(UserAddress p) {
  if (user_is_empty(p)) {
    printf("NULL\n");
  } else {

    printf("%s -> ", (*p).info.username);
    user_tampil_list((*p).next);
  }
}

void user_insert_awal(UserAddress *p, UserAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void user_insert_akhir(UserAddress *p, UserAddress PNew) {
  if (user_is_empty(*p)) {
    *p = PNew;
  } else {
    UserAddress temp = *p;
    while (!user_is_empty((*temp).next)) {
      temp = (*temp).next;
    }
    (*temp).next = PNew;
  }
}

UserAddress user_search_by_id(UserAddress p, Id nilai) {
  while (!user_is_empty(p)) {
    if (p->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}
void user_insert(UserList *p, UserAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  user_insert_akhir(&(p->first), PNew);
}

void user_insert_after(UserAddress *pBef, UserAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void user_del_awal(UserAddress *p, User *X) {
  if (!user_is_empty(*p)) {
    *X = (**p).info;
    UserAddress temp = *p;
    *p = (**p).next;
    (*temp).next = NULL;
    free(temp);
  }
}

void user_del_akhir(UserAddress *p, User *X) {
  if (!user_is_empty(*p)) {
    if (user_is_empty((**p).next)) {
      printf("yellow!");
      *X = (**p).info;
      free(*p);
      *p = NULL;
    } else {
      UserAddress prev;
      UserAddress last = *p;
      while (!user_is_empty((*last).next)) {
        prev = last;
        last = (*last).next;
      }
      *X = (*last).info;
      free(last);
      (*prev).next = NULL;
    }
  }
}

void user_del_after(UserAddress *pBef, User *X) {
  UserAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp);
}

void user_delete_by_address(UserAddress *p, UserAddress pDel, User *X) {
  if (user_is_empty(*p) || user_is_empty(pDel))
    return;

  if (*p == pDel) {
    user_del_awal(p, X);
  } else {
    UserAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      user_del_after(&temp, X);
    }
  }
}

void user_delete_by_id(UserAddress *p, Id nilai, User *X) {
  UserAddress target = user_search_by_id(*p, nilai);
  user_delete_by_address(p, target, X);
}

void user_deallocation(UserAddress *p) {
  while (!user_is_empty(*p)) {
    User i;
    user_del_awal(p, &i);
  }
}

int user_count(UserAddress p) {
  if (user_is_empty(p)) {
    return 0;
  } else {
    return 1 + user_count((*p).next);
  }
}

UserAddress user_balik_list(UserAddress p) {
  if (!user_is_empty(p)) {
    UserAddress prev = NULL;
    UserAddress this = p;
    UserAddress next = (*p).next;
    while (!user_is_empty(next)) {
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
