#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/auth/auth.h"
#include <stdio.h>

// Fungsi untuk mengecek apakah username sudah dipakai
bool is_username_taken(UserList list, char *username) {
    UserAddress curr = list.first;
    while (curr != NULL) {
        if (strcmp(curr->info.username, username) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool register_user(UserList *list, char *username, char *password, char *profile_picture) {
    if (is_username_taken(*list, username)) {
        printf("Username '%s' sudah digunakan.\n", username);
        return false;
    }

    User new_user;
    create_user(&new_user, username, password, profile_picture);

    UserAddress new_node;
    create_node(&new_node);
    isi_node(&new_node, new_user);
    insert(list, new_node);

    printf("Registrasi berhasil untuk '%s'.\n", username);
    return true;
}

bool login(UserList list, char *username, char *password, User *logged_user) {
    UserAddress curr = list.first;
    while (curr != NULL) {
        if (strcmp(curr->info.username, username) == 0 &&
            strcmp(curr->info.hashed_password, password) == 0) {
            *logged_user = curr->info;
            printf("Login berhasil. Selamat datang, %s!\n", username);
            return true;
        }
        curr = curr->next;
    }

    printf("Login gagal. Username atau password salah.\n");
    return false;
}
