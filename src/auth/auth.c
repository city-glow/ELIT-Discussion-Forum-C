#include "../../include/auth/auth.h"
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_username_valid(char *username) {
  if (username == NULL) {
    return false;
  }

  for (int i = 0; username[i] != '\0'; i++) {
    if (username[i] == ' ') {
      return false;
    }
  } 

  return true;
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

bool register_user(UserList *list, char *username, char *password) {

  if (!is_username_valid(username)) {
    printf("Username tidak boleh pakai spasi!.\n");
    return false;
  }

  if (is_username_taken(*list, username)) {
    printf("Username '%s' sudah digunakan.\n", username);
    return false;
  }

  // Hash the password
  unsigned char hash[SHA256_DIGEST_LENGTH];
#ifdef _WIN32
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  SHA256_Update(&ctx, password, strlen(password));
  SHA256_Final(hash, &ctx);
#else
  SHA256((unsigned char *)password, strlen(password), hash);
#endif


  // Convert the binary hash to hex string
  char hashed_password[65]; // 64 chars + null terminator
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    sprintf(&hashed_password[i * 2], "%02x", hash[i]);

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
  // Hash the input password
  unsigned char hash[SHA256_DIGEST_LENGTH];
#ifdef _WIN32
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  SHA256_Update(&ctx, password, strlen1(password));
  SHA256_Final(hash, &ctx);
#else
  SHA256((unsigned char *)password, strlen(password), hash);
#endif

  // Convert hash to hexadecimal string
  char hashed_input[65]; // 64 chars + null terminator
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    sprintf(&hashed_input[i * 2], "%02x", hash[i]);

  // Search for user and compare hashed passwords
  UserAddress curr = list.first;
  while (curr != NULL) {
    if (strcmp(curr->info.username, username) == 0 &&
        strcmp(curr->info.hashed_password, hashed_input) == 0) {
      *logged_user = curr->info;
      printf("Login berhasil. Selamat datang, %s!\n", username);
      return true;
    }
    curr = curr->next;
  }

  printf("Login gagal. Username atau password salah.\n");
  return false;
}
