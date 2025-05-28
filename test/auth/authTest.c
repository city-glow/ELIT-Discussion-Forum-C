#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/user/userList.h"
#include "../../include/auth/auth.h"

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    printf("\nTekan ENTER untuk melanjutkan...");
    getchar();
}

void menu() {
    printf("========== MENU UTAMA ==========\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Tampilkan Semua User\n");
    printf("0. Keluar\n");
    printf("================================\n");
    printf("Pilihan: ");
}

void user_dashboard(User user) {
    printf("\n--- Selamat datang, %s! (ID: %d) ---\n", user.username, user.id);
    printf("1. Logout\n");
    printf("================================\n");
}

int main() {
    UserList list;
    create_list(&list);

    int pilihan;
    User logged_user;
    bool is_logged_in = false;

    do {
        clear_screen();
        if (!is_logged_in) {
            menu();
            scanf("%d", &pilihan);
            getchar();  // flush newline

            if (pilihan == 1) {
                char username[50], password[100], profile_picture[100];

                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;

                printf("Masukkan password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;

                printf("Masukkan nama file foto profil: ");
                fgets(profile_picture, sizeof(profile_picture), stdin);
                profile_picture[strcspn(profile_picture, "\n")] = 0;

                register_user(&list, username, password, profile_picture);
                pause();
            } else if (pilihan == 2) {
                char username[50], password[100];

                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;

                printf("Masukkan password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;

                if (login(list, username, password, &logged_user)) {
                    is_logged_in = true;
                }
                pause();
            } else if (pilihan == 3) {
                tampil_list(list.first);
                pause();
            }
        } else {
            clear_screen();
            user_dashboard(logged_user);

            int dashboard_choice;
            printf("Pilihan: ");
            scanf("%d", &dashboard_choice);
            getchar();

            if (dashboard_choice == 1) {
                is_logged_in = false;
                printf("Logout berhasil.\n");
                pause();
            }
        }

    } while (pilihan != 0);

    printf("Terima kasih telah menggunakan aplikasi ini.\n");
    deallocation(&list.first);
    return 0;
}
