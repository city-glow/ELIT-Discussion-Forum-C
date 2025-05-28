#ifndef AUTH_H
#define AUTH_H

#include "../user/userList.h"  // agar bisa akses UserList dan User

bool register_user(UserList *list, char *username, char *password, char *profile_picture);
bool login(UserList list, char *username, char *password, User *logged_user);

#endif
