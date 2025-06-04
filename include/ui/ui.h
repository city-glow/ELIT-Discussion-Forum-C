#ifndef UI_H
#define UI_H

#include "../board/boardList.h"
#include "../post/postList.h"
#include "../user/userList.h"
#include "../vote/voteList.h"

void ui_clear_screen();
void ui_pause();
int ui_show_main_menu();
void ui_search_post();
void ui_create_post();
void ui_show_trending_posts();
int ui_show_dashboard(User user);
void handle_dashboard(BoardList *board_list, PostList *post_list,
                      UserList *user_list, VoteList *vote_list,
                      const User *user);
int ui_show_post(Id post_id, PostList *post_list, VoteList *vote_list,
                 User user, UserList *user_list, BoardList *board_list);
void get_vote_result(VoteList vote_list, int *vote_sum, Id current_user_id,
                     Id target_id, VoteTargetType target_type, Id *my_vote_id,
                     bool *has_voted);

#endif
