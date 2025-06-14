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
void get_vote_result(VoteList vote_list, int *vote_sum, Id current_user_id,
                     Id target_id, VoteTargetType target_type, Id *my_vote_id,
                     bool *has_voted);
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
void vote_delete_all_by_target(VoteAddress *p, Id target_id, VoteTargetType target_type);
void vote_delete_all_by_user(VoteAddress *p, Id user_id);

void save_vote_list(VoteList *list, const char *filename);
void load_vote_list(VoteList *list, const char *filename);

#endif
