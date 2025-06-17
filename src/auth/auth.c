#include "../../include/auth/auth.h"
#include "../../include/argon2/argon2.h"  // Argon2 library
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Helper: Generate random salt (for demonstration)
void generate_salt(char *salt, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand((unsigned int)time(NULL));
    size_t i = 0;
    for (i = 0; i < length; ++i) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

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

bool is_username_valid(char *username, UserList list) {
    if (username == NULL) {
        printf("Username tidak boleh kosong!!\n");
        return false;
    }

    if (strcmp(username, "") == 0) {
        printf("Username tidak boleh kosong!!\n");
        return false;
    }
	int i = 0;
    for (i = 0; username[i] != '\0'; i++) {
        if (username[i] == ' ') {
            printf("Username tidak boleh ada spasi!\n");
            return false;
        }
    }

    if (is_username_taken(list, username)) {
        printf("Username tidak tersedia!! (sudah terpakai)\n");
        return false;
    }

    return true;
}

bool register_user(UserList *list, char *username, char *password) {
    if (!is_username_valid(username, *list)) {
        printf("Username tidak boleh pakai spasi!.\n");
        return false;
    }

    if (is_username_taken(*list, username)) {
        printf("Username '%s' sudah digunakan.\n", username);
        return false;
    }

    // Generate salt (optional, Argon2 handles it internally)
    char salt[16]; // 16 bytes is standard for Argon2
    generate_salt(salt, sizeof(salt));

    // Hash the password using Argon2id
    char hashed_password[128]; // Output buffer
    uint32_t time_cost = 3;     // Iterations
    uint32_t memory_cost = 65536; // 64 MB
    uint32_t parallelism = 1;   // Threads

    int result = argon2id_hash_encoded(
        time_cost, memory_cost, parallelism,
        password, strlen(password),
        salt, sizeof(salt), 32,
        hashed_password, sizeof(hashed_password)
    );

    if (result != ARGON2_OK) {
        fprintf(stderr, "Argon2 hashing gagal: %s\n", argon2_error_message(result));
        return false;
    }

    // Create and insert the new user
    User new_user;
    create_user(&new_user, username, hashed_password);

    UserAddress new_node;
    user_create_node(&new_node);
    user_isi_node(&new_node, new_user);
    user_insert(list, new_node);

    printf("Registrasi berhasil untuk '%s'.\n", username);
    return true;
}

bool login(UserList list, char *username, char *password, User *logged_user) {
    // Search for user
    UserAddress curr = list.first;
    while (curr != NULL) {
        if (strcmp(curr->info.username, username) == 0) {
            // Verify password using Argon2
            int result = argon2id_verify(curr->info.hashed_password, password, strlen(password));
            if (result == ARGON2_OK) {
                *logged_user = curr->info;
                printf("Login berhasil. Selamat datang, %s!\n", username);
                return true;
            } else {
                printf("Password salah.\n");
                return false;
            }
        }
        curr = curr->next;
    }

    printf("Login gagal. Username tidak ditemukan.\n");
    return false;
}
