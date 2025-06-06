#ifndef VOTELIST_H
#define VOTELIST_H
#include "vote.h"
#include <stdbool.h>

typedef struct VoteTElmtList *VoteAddress;
typedef struct VoteTElmtList {
  Vote info;
  VoteAddress next;
} VoteElmtList;
typedef struct {
  VoteAddress first;
  Id id_max;
} VoteList;

bool vote_is_empty(VoteAddress p);
void vote_create_node(VoteAddress *p);
void vote_create_list(VoteList *p);
void vote_isi_node(VoteAddress *p, Vote nilai);
void vote_tampil_list(VoteAddress p);
void vote_insert(VoteList *p, VoteAddress PNew);
VoteAddress vote_search_by_id(VoteAddress p, Id nilai);
// void vote_delete_by_address(VoteAddress *p, VoteAddress pDel, Vote *X);
void vote_delete_by_id(VoteAddress *p, Id nilai, Vote *X);
void vote_deallocation(VoteAddress *p);
int vote_count(VoteAddress p);
VoteAddress vote_balik_list(VoteAddress p);

#endif
