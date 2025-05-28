#ifndef USER_C
#define USER_C
#include "../../include/user/user.h"
#include <string.h>

void create_user(User *user, char *username, char *hashed_password, char *profile_picture) {
    strcpy(user->username, username);
    strcpy(user->hashed_password, hashed_password);
    strcpy(user->profile_picture, profile_picture);
    user->followings = NULL;
    user->notifications = NULL;
}

#endif
