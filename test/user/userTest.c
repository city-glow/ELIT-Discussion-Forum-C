#include "../../include/user/userList.h"
#include <stdio.h>

int main() {
    UserList userList;
    create_list(&userList);

    // User 1
    User user1;
    create_user(&user1, "alice", "hashed_pw1", "pic1.jpg");
    UserAddress node1;
    create_node(&node1);
    isi_node(&node1, user1);
    insert(&userList, node1);

    // User 2
    User user2;
    create_user(&user2, "bob", "hashed_pw2", "pic2.jpg");
    UserAddress node2;
    create_node(&node2);
    isi_node(&node2, user2);
    insert(&userList, node2);

    // User 3
    User user3;
    create_user(&user3, "charlie", "hashed_pw3", "pic3.jpg");
    UserAddress node3;
    create_node(&node3);
    isi_node(&node3, user3);
    insert(&userList, node3);

    printf("List awal:\n");
    tampil_list(userList.first);

    // Cari user dengan ID 2
    printf("\nMencari user dengan ID 2:\n");
    UserAddress found = search_by_id(userList.first, 2);
    if (found != NULL) {
        printf("Ditemukan: %s\n", found->info.username);
    } else {
        printf("User tidak ditemukan\n");
    }

    // Hapus node pertama
    printf("\nMenghapus node pertama:\n");
    User deleted_user;
    del_awal(&userList.first, &deleted_user);
    printf("Dihapus: %s\n", deleted_user.username);
    tampil_list(userList.first);

    // Balik list
    printf("\nMembalik list:\n");
    userList.first = balik_list(userList.first);
    tampil_list(userList.first);

    // Hapus semua node
    printf("\nDealokasi semua node:\n");
    deallocation(&userList.first);
    tampil_list(userList.first);

    return 0;
}
