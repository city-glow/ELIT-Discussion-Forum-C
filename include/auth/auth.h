#ifndef AUTH_H
#define AUTH_H

#include "../user/userList.h"  // agar bisa akses UserList dan User

bool register_user(UserList *list, char *username, char *password);
bool login(UserList list, char *username, char *password, User *logged_user);
bool is_username_valid(char *username, UserList list);
#endif
