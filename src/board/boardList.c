#ifndef BOARDLIST_C
#define BOARDLIST_C
#include "../../include/board/boardList.h"
#include "../../include/moderateThis/moderateThisQueue.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool board_is_empty(BoardAddress p) { return (p == NULL); }

void board_create_list(BoardList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void board_create_node(BoardAddress *p) {
  *p = (BoardAddress)malloc(sizeof(BoardElmtList));
}

void board_isi_node(BoardAddress *p, Board nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (**p).next = NULL;
  }
}

void board_tampil_list(BoardAddress p) {
  if (board_is_empty(p)) {
    printf("NULL\n");
  } else {

    printf("%s -> ", (*p).info.title);
    board_tampil_list((*p).next);
  }
}

void board_insert_awal(BoardAddress *p, BoardAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void board_insert_akhir(BoardAddress *p, BoardAddress PNew) {
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

BoardAddress board_search_by_id(BoardAddress p, Id nilai) {
  while (!board_is_empty(p)) {
    if (p->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}

BoardAddress board_search_by_title(BoardAddress p, char *title) {
  while (!board_is_empty(p)) {
    if (strcmp(p->info.title, title) == 0) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}

void board_insert(BoardList *p, BoardAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  board_insert_akhir(&(p->first), PNew);
  save_board_list(p, "../storage/boards.dat");
}

int board_insert_and_get_id(BoardList *p, BoardAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  board_insert_akhir(&(p->first), PNew);
  return PNew->info.id;
  save_board_list(p, "../storage/boards.dat");
}

void board_insert_after(BoardAddress *pBef, BoardAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void board_del_awal(BoardAddress *p, Board *X) {
  if (!board_is_empty(*p)) {
    *X = (**p).info;
    BoardAddress temp = *p;
    *p = (**p).next;
    (*temp).next = NULL;
    free(temp);
  }
}

void board_del_akhir(BoardAddress *p, Board *X) {
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

void board_del_after(BoardAddress *pBef, Board *X) {
  BoardAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp);
}

void board_delete_by_address(BoardAddress *p, BoardAddress pDel, Board *X) {
  if (board_is_empty(*p) || board_is_empty(pDel))
    return;

  if (*p == pDel) {
    board_del_awal(p, X);
  } else {
    BoardAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      board_del_after(&temp, X);
    }
  }
}

void board_delete_by_id(BoardList *p, Id nilai, Board *X, PostList *list, VoteList *vote_list, CommentTreeList *comment_tree_list) {
  BoardAddress target = board_search_by_id(p->first, nilai);
  board_delete_by_address(&p->first, target, X);
  save_board_list(p, "../storage/boards.dat");
  post_delete_all_by_board_id(list, X->id, vote_list, comment_tree_list);
}

void board_deallocation(BoardAddress *p) {
  while (!board_is_empty(*p)) {
    Board i;
    board_del_awal(p, &i);
  }
}

int board_count(BoardAddress p) {
  if (board_is_empty(p)) {
    return 0;
  } else {
    return 1 + board_count((*p).next);
  }
}

BoardAddress board_balik_list(BoardAddress p) {
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

bool get_board_by_id(BoardList p, Id nilai, Board *get) {
  BoardAddress found = board_search_by_id(p.first, nilai);
  if (!board_is_empty(found)) {
    *get = found->info;
  }
  return !board_is_empty(found);
}

// Helper to save a ModerateQueue
static void save_moderate_queue(FILE *file, const ModerateQueue *queue) {
    ModerateQueueAddress curr = queue->front;
    int count = 0;
    // Count elements
    while (curr) {
        count++;
        curr = curr->next;
    }
    fwrite(&count, sizeof(int), 1, file);
    curr = queue->front;
    while (curr) {
        fwrite(&(curr->info.request_id), sizeof(Id), 1, file);
        curr = curr->next;
    }
}

// Helper to load a ModerateQueue
static void load_moderate_queue(FILE *file, ModerateQueue *queue) {
    int count = 0;
    fread(&count, sizeof(int), 1, file);
    moderate_queue_create(queue);
    int i = 0;
    for (i = 0; i < count; ++i) {
        Id request_id;
        fread(&request_id, sizeof(Id), 1, file);
        ModerateRequest request;
        request.request_id = request_id;
        // Other fields can be initialized as needed or loaded if stored
        moderate_queue_enqueue(queue, request);
    }
}

bool save_board_list(const BoardList *boardList, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return false;
    fwrite(&(boardList->id_max), sizeof(Id), 1, file);
    BoardAddress curr = boardList->first;
    int count = 0;
    // Count boards
    while (curr) { count++; curr = curr->next; }
    fwrite(&count, sizeof(int), 1, file);
    curr = boardList->first;
    while (curr) {
        fwrite(&(curr->info.id), sizeof(Id), 1, file);
        fwrite(&(curr->info.owner_id), sizeof(Id), 1, file);
        fwrite(curr->info.title, sizeof(char), MAX_TITLE + 1, file);
        save_moderate_queue(file, &(curr->info.queue));
        curr = curr->next;
    }
    fclose(file);
    return true;
}

bool load_board_list(BoardList *boardList, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return false;
    board_create_list(boardList);
    fread(&(boardList->id_max), sizeof(Id), 1, file);
    int count = 0;
    fread(&count, sizeof(int), 1, file);
    int i = 0;
    for (i = 0; i < count; ++i) {
        Board temp;
        fread(&(temp.id), sizeof(Id), 1, file);
        fread(&(temp.owner_id), sizeof(Id), 1, file);
        fread(temp.title, sizeof(char), MAX_TITLE + 1, file);
        load_moderate_queue(file, &(temp.queue));
        BoardAddress newNode;
        board_create_node(&newNode);
        board_isi_node(&newNode, temp);
        board_insert_akhir(&(boardList->first), newNode);
    }
    fclose(file);
    return true;
}
#endif
