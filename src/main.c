#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/ui/ui.h"
#include "../include/user/userList.h"
#include "../include/auth/auth.h"

int main() {
    UserList list;
    user_create_list(&list);

    int pilihan;
    User logged_user;
    bool is_logged_in = false;

    do {
        if (!is_logged_in) {
            pilihan = ui_show_main_menu();
            if (pilihan == 1) {
                char username[MAX_USERNAME + 1], password[100];

                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;

                printf("Masukkan password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;

                register_user(&list, username, password);
                ui_pause();

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
                ui_pause();

            } else if (pilihan == 3) {
                user_tampil_list(list.first);
                ui_pause();
            }
        } else {
            ui_show_dashboard(logged_user);
            int dashboard_choice;
            scanf("%d", &dashboard_choice);
            getchar(); // Bersihkan newline

            if (dashboard_choice == 0) {
                is_logged_in = false;
                printf("Logout berhasil.\n");
                ui_pause();
            }
        }
    } while (pilihan != 0);

    printf("Terima kasih telah menggunakan aplikasi ini.\n");
    user_deallocation(&list.first);
    return 0;
}
