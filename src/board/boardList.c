#ifndef BOARDLIST_C
#define BOARDLIST_C
#include "../../include/board/boardList.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>

bool board_is_empty(BoardAddress p) { return (p == NULL); }

void Board_create_list(BoardList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void Board_create_node(BoardAddress *p) {
  *p = (BoardAddress)malloc(sizeof(BoardElmtList));
}

void Board_isi_node(BoardAddress *p, Board nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (**p).next = NULL;
  }
}

void Board_tampil_list(BoardAddress p) {
  if (board_is_empty(p)) {
    printf("NULL\n");
  } else {

    printf("%s -> ", (*p).info.title);
    Board_tampil_list((*p).next);
  }
}

void insert_awal(BoardAddress *p, BoardAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void insert_akhir(BoardAddress *p, BoardAddress PNew) {
  if (board_is_empty(*p)) {
    *p = PNew;
  } else {
    BoardAddress temp = *p;
    while (!board_is_empty((*temp).next)) {
      temp = (*temp).next;
    }
    (*temp).next = PNew;
  }
}

BoardAddress Board_search_by_id(BoardAddress p, Id nilai) {
  while (!board_is_empty(p)) {
    if (p->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}
void Board_insert(BoardList *p, BoardAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  insert_akhir(&(p->first), PNew);
}

void insert_after(BoardAddress *pBef, BoardAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void del_awal(BoardAddress *p, Board *X) {
  if (!board_is_empty(*p)) {
    *X = (**p).info;
    BoardAddress temp = *p;
    *p = (**p).next;
    (*temp).next = NULL;
    free(temp);
  }
}

void del_akhir(BoardAddress *p, Board *X) {
  if (!board_is_empty(*p)) {
    if (board_is_empty((**p).next)) {
      printf("yellow!");
      *X = (**p).info;
      free(*p);
      *p = NULL;
    } else {
      BoardAddress prev;
      BoardAddress last = *p;
      while (!board_is_empty((*last).next)) {
        prev = last;
        last = (*last).next;
      }
      *X = (*last).info;
      free(last);
      (*prev).next = NULL;
    }
  }
}

void del_after(BoardAddress *pBef, Board *X) {
  BoardAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp);
}

void delete_by_address(BoardAddress *p, BoardAddress pDel, Board *X) {
  if (board_is_empty(*p) || board_is_empty(pDel))
    return;

  if (*p == pDel) {
    del_awal(p, X);
  } else {
    BoardAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      del_after(&temp, X);
    }
  }
}

void Board_delete_by_id(BoardAddress *p, Id nilai, Board *X) {
  BoardAddress target = Board_search_by_id(*p, nilai);
  delete_by_address(p, target, X);
}

void Board_deallocation(BoardAddress *p) {
  while (!board_is_empty(*p)) {
    Board i;
    del_awal(p, &i);
  }
}

int Board_count(BoardAddress p) {
  if (board_is_empty(p)) {
    return 0;
  } else {
    return 1 + Board_count((*p).next);
  }
}

BoardAddress Board_balik_list(BoardAddress p) {
  if (!board_is_empty(p)) {
    BoardAddress prev = NULL;
    BoardAddress this = p;
    BoardAddress next = (*p).next;
    while (!board_is_empty(next)) {
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
