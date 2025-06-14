#ifndef USERLIST_H
#define USERLIST_H
#include "user.h"
#include <stdbool.h>

typedef struct UserTElmtList *UserAddress;
typedef struct UserTElmtList {
  User info;
  UserAddress next;
} UserElmtList;
typedef struct {
  UserAddress first;
  Id id_max;
} UserList;

bool user_is_empty(UserAddress p);
void user_create_node(UserAddress *p);
void user_create_list(UserList *p);
void user_isi_node(UserAddress *p, User nilai);
void user_tampil_list(UserAddress p);
void user_insert(UserList *p, UserAddress PNew);
UserAddress user_search_by_id(UserAddress p, Id nilai);
// void user_delete_by_address(UserAddress *p, UserAddress pDel, User *X);
void user_delete_by_id(UserAddress *p, Id nilai, User *X);
void user_deallocation(UserAddress *p);
int user_count(UserAddress p);
UserAddress user_balik_list(UserAddress p);
void save_user_list(UserList *list, const char *filename);
void load_user_list(UserList *list, const char *filename);

#endif
