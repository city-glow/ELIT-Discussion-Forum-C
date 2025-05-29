#ifndef UI_H
#define UI_H

#include "../../include/user/userList.h"

void ui_clear_screen();
void ui_pause();
int ui_show_main_menu();
void ui_show_dashboard(User user);
void ui_search_post();
void ui_create_post();
void ui_show_trending_posts();

#endif
