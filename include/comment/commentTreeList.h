#ifndef COMMENTTREELIST_H
#define COMMENTTREELIST_H
#include "commentTree.h"
#include "../vote/voteList.h"
#include <stdbool.h>

typedef struct CommentTreeTElmtList *CommentTreeAddress;
typedef struct CommentTreeTElmtList {
  CommentTree info;
  CommentTreeAddress next;
} CommentTreeElmtList;
typedef struct {
  CommentTreeAddress first;
  Id id_max;
} CommentTreeList;

bool comment_tree_list_is_empty(CommentTreeAddress p);
void comment_tree_list_create_node(CommentTreeAddress *p);
void comment_tree_list_create_list(CommentTreeList *p);
void comment_tree_list_isi_node(CommentTreeAddress *p, CommentTree nilai);
void comment_tree_list_tampil_list(CommentTreeAddress p, UserList user_list, VoteList vote_list, User logged_user);
// void comment_tree_list_tampil_list(CommentTreeAddress p);
// void comment_tree_list_insert(CommentTreeList *p, CommentTreeAddress PNew);
void comment_tree_list_insert(CommentTreeList *p, CommentAddress PNew);
CommentAddress comment_tree_list_search_by_id(CommentTreeAddress p, Id nilai);
CommentTreeAddress comment_tree_list_search_by_post_id(CommentTreeAddress p, Id nilai);
// void CommentTree_delete_by_address(CommentTreeAddress *p, CommentTreeAddress pDel, CommentTree *X);
void comment_tree_list_delete_by_id(CommentTreeAddress *p, Id nilai, CommentTree *X);
void comment_tree_list_delete_comment_by_id(CommentTreeList *p, Id nilai,
                                            Comment *X, VoteList *vote_list);
void comment_tree_list_deallocation(CommentTreeAddress *p);
int comment_tree_list_count(CommentTreeAddress p);
void comment_delete_all_post_id(CommentTreeAddress *p, Id nilai,
                                CommentTree *X);
CommentTreeAddress comment_tree_list_balik_list(CommentTreeAddress p);
CommentTreeAddress comment_tree_list_search_by_root_id(CommentTreeAddress p,
                                                       Id nilai);
void comment_delete_all_post_id_with_votes(CommentTreeAddress *p, Id nilai,
                                           CommentTree *X,
                                           VoteList *vote_list);

void save_comment_tree_list(CommentTreeList *list, const char *filename);
void load_comment_tree_list(CommentTreeList *list, const char *filename);
#endif
