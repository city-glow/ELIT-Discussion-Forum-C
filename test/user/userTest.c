#include "../../include/user/userList.h"
#include <stdio.h>

int main() {
    UserList userList;
    user_create_list(&userList);

    // User 1
    User user1;
    create_user(&user1, "alice", "hashed_pw1");
    UserAddress node1;
    user_create_node(&node1);
    user_isi_node(&node1, user1);
    user_insert(&userList, node1);

    // User 2
    User user2;
    create_user(&user2, "bob", "hashed_pw2");
    UserAddress node2;
    user_create_node(&node2);
    user_isi_node(&node2, user2);
    user_insert(&userList, node2);

    // User 3
    User user3;
    create_user(&user3, "charlie", "hashed_pw3");
    UserAddress node3;
    user_create_node(&node3);
    user_isi_node(&node3, user3);
    user_insert(&userList, node3);

    printf("List awal:\n");
    user_tampil_list(userList.first);

    // Cari user dengan ID 2
    printf("\nMencari user dengan ID 2:\n");
    UserAddress found = user_search_by_id(userList.first, 2);
    if (found != NULL) {
        printf("Ditemukan: %s\n", found->info.username);
    } else {
        printf("User tidak ditemukan\n");
    }

    // Hapus node pertama
    printf("\nMenghapus node dengan id 2:\n");
    User deleted_user;
    user_delete_by_id(&userList.first, 2, &deleted_user);
    printf("Dihapus: %s\n", deleted_user.username);
    user_tampil_list(userList.first);

    // Balik list
    printf("\nMembalik list:\n");
    userList.first = user_balik_list(userList.first);
    user_tampil_list(userList.first);

    // Hapus semua node
    printf("\nDealokasi semua node:\n");
    user_deallocation(&userList.first);
    user_tampil_list(userList.first);

    return 0;
}
