#ifndef VOTELIST_C
#define VOTELIST_C
#include "../../include/vote/voteList.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include <stdlib.h>

void get_vote_result(VoteList vote_list, int *vote_sum, Id current_user_id,
                     Id target_id, VoteTargetType target_type, Id *my_vote_id,
                     bool *has_voted) {
  int upvotes = 0;
  int downvotes = 0;
  *my_vote_id = 0;
  *has_voted = false;

  VoteAddress current = vote_list.first;

  while (current != NULL) {
    if (current->info.target_id == target_id &&
        current->info.target_type == target_type) {

      if (current->info.is_upvote) {
        upvotes++;
      } else {
        downvotes++;
      }

      if (current->info.user_id == current_user_id) {
        *my_vote_id = current->info.id;
        *has_voted = true;
      }
    }

    current = current->next;
  }

  *vote_sum = upvotes - downvotes;
}

bool vote_is_empty(VoteAddress p) { return (p == NULL); }

void vote_create_list(VoteList *p) {
  p->first = NULL;
  p->id_max = 0;
}

void vote_create_node(VoteAddress *p) {
  *p = (VoteAddress)malloc(sizeof(VoteElmtList));
}

void vote_isi_node(VoteAddress *p, Vote nilai) {
  if (*p != NULL) {
    (**p).info = nilai;
    (**p).next = NULL;
  }
}

void vote_tampil_list(VoteAddress p) {
  if (vote_is_empty(p)) {
    printf("NULL\n");
  } else {

    printf("[%d, %d, %d]-> ", (*p).info.user_id, p->info.target_id, p->info.target_type);
    vote_tampil_list((*p).next);
  }
}

void vote_insert_awal(VoteAddress *p, VoteAddress PNew) {
  (*PNew).next = *p;
  *p = PNew;
}

void vote_insert_akhir(VoteAddress *p, VoteAddress PNew) {
  if (vote_is_empty(*p)) {
    *p = PNew;
  } else {
    VoteAddress temp = *p;
    while (!vote_is_empty((*temp).next)) {
      temp = (*temp).next;
    }
    (*temp).next = PNew;
  }
}

VoteAddress vote_search_by_id(VoteAddress p, Id nilai) {
  while (!vote_is_empty(p)) {
    if (p->info.id == nilai) {
      return p;
    }
    p = (*p).next;
  }
  return NULL;
}
void vote_insert(VoteList *p, VoteAddress PNew) {
  (*p).id_max += 1;
  PNew->info.id = p->id_max;
  vote_insert_akhir(&(p->first), PNew);
}

void vote_insert_after(VoteAddress *pBef, VoteAddress PNew) {
  (*PNew).next = (**pBef).next;
  (**pBef).next = PNew;
}

void vote_del_awal(VoteAddress *p, Vote *X) {
  if (!vote_is_empty(*p)) {
    *X = (**p).info;
    VoteAddress temp = *p;
    *p = (**p).next;
    (*temp).next = NULL;
    free(temp);
  }
}

void vote_del_akhir(VoteAddress *p, Vote *X) {
  if (!vote_is_empty(*p)) {
    if (vote_is_empty((**p).next)) {
      printf("yellow!");
      *X = (**p).info;
      free(*p);
      *p = NULL;
    } else {
      VoteAddress prev;
      VoteAddress last = *p;
      while (!vote_is_empty((*last).next)) {
        prev = last;
        last = (*last).next;
      }
      *X = (*last).info;
      free(last);
      (*prev).next = NULL;
    }
  }
}

void vote_del_after(VoteAddress *pBef, Vote *X) {
  VoteAddress temp = (**pBef).next;
  (**pBef).next = (*temp).next;
  *X = (*temp).info;
  (*temp).next = NULL;
  free(temp);
}

void vote_delete_by_address(VoteAddress *p, VoteAddress pDel, Vote *X) {
  if (vote_is_empty(*p) || vote_is_empty(pDel))
    return;

  if (*p == pDel) {
    vote_del_awal(p, X);
  } else {
    VoteAddress temp = *p;
    while (temp->next != NULL && temp->next != pDel) {
      temp = temp->next;
    }

    if (temp->next == pDel) {
      vote_del_after(&temp, X);
    }
  }
}

void vote_delete_by_id(VoteAddress *p, Id nilai, Vote *X) {
  VoteAddress target = vote_search_by_id(*p, nilai);
  vote_delete_by_address(p, target, X);
}

void vote_deallocation(VoteAddress *p) {
  while (!vote_is_empty(*p)) {
    Vote i;
    vote_del_awal(p, &i);
  }
}

int vote_count(VoteAddress p) {
  if (vote_is_empty(p)) {
    return 0;
  } else {
    return 1 + vote_count((*p).next);
  }
}

VoteAddress vote_balik_list(VoteAddress p) {
  if (!vote_is_empty(p)) {
    VoteAddress prev = NULL;
    VoteAddress this = p;
    VoteAddress next = (*p).next;
    while (!vote_is_empty(next)) {
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


void save_vote_list(VoteList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    // Save id_max
    fwrite(&(list->id_max), sizeof(Id), 1, file);

    // Save votes
    VoteAddress current = list->first;
    while (current != NULL) {
        fwrite(&(current->info), sizeof(Vote), 1, file);
        current = current->next;
    }

    fclose(file);
}

void load_vote_list(VoteList *list, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open vote file for writing");
        return;
    }

    vote_create_list(list);

    // Load id_max
    fread(&(list->id_max), sizeof(Id), 1, file);

    Vote temp_vote;
    while (fread(&temp_vote, sizeof(Vote), 1, file) == 1) {
        VoteAddress new_node;
        vote_create_node(&new_node);
        vote_isi_node(&new_node, temp_vote);
        vote_insert_akhir(&(list->first), new_node);
    }

    fclose(file);
}
#endif
