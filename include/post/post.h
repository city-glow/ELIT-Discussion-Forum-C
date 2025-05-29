#ifndef POST_H
#define USER_H
#include "../id/idList.h"
#include "../notif/notifStack.h"
#define MAX_USERNAME 100

typedef struct {
    Id id;
    char username[MAX_USERNAME + 1];
    char hashed_password[128];
    char profile_picture[256];
    IdList followings;
    NotifStack notifications;
} User;

void create_user(User *user, char *username, char *hashed_password, char *profile_picture);

#endif
