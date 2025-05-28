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

bool is_empty(UserAddress p);
void create_node(UserAddress *p);
void create_list(UserList *p);
void isi_node(UserAddress *p, User nilai);
void tampil_list(UserAddress p);
void insert(UserList *p, UserAddress PNew);
void insert_awal(UserAddress *p, UserAddress PNew);
void insert_akhir(UserAddress *p, UserAddress PNew);
UserAddress search_by_id(UserAddress p, Id nilai);
void insert_after(UserAddress *pBef, UserAddress PNew);
void del_awal(UserAddress *p, User *X);
void del_akhir(UserAddress *p, User *X);
void del_after(UserAddress *pBef, User *X);
void delete_by_address(UserAddress *p, UserAddress pDel, User *X);
void deallocation(UserAddress *p);
int count(UserAddress p);
UserAddress balik_list(UserAddress p);

#endif
