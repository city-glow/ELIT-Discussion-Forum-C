#ifndef USER_C
#define USER_C
#include "../../include/user/user.h"
#include <string.h>
#include <stdio.h>

void create_user(User *user, char *username, char *hashed_password) {
    strcpy(user->username, username);
    strcpy(user->hashed_password, hashed_password);
    user->followings = NULL;
    //user->notifications = NULL;
}

void print_user(User user) {
    printf("+------------------------------+");
    printf("| %s", user.username);
    printf("+------------------------------+");
}

#endif
