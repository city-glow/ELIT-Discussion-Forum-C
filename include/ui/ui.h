#ifndef UI_H
#define UI_H

#include "../array/item.h"
#include "../board/boardList.h"
#include "../comment/commentTreeList.h"
#include "../post/postList.h"
#include "../user/userList.h"
#include "../vote/voteList.h"

void ui_clear_screen();
void ui_pause();
int ui_show_main_menu();
void ui_search_post();
void ui_create_post();
void print_vote(bool has_voted, Vote my_vote, int vote_sum);
void ui_show_trending_posts();
int ui_show_dashboard(User user);
void handle_dashboard(BoardList *board_list, PostList *post_list,
                      UserList *user_list, VoteList *vote_list,
                      CommentTreeList *comment_tree_list, const User *user);
int ui_show_post(Post post, User poster, Board board, int vote_sum,
                 bool has_voted, Vote my_vote, User logged_user);
void get_vote_result(VoteList vote_list, int *vote_sum, Id current_user_id,
                     Id target_id, VoteTargetType target_type, Id *my_vote_id,
                     bool *has_voted);
void handle_post_page(Id post_id, PostList *post_list, VoteList *vote_list,
                      User logged_user, UserList *user_list,
                      BoardList *board_list,
                      CommentTreeList *comment_tree_list);
void handle_single_comment_page(Id comment_id, PostList *post_list,
                                VoteList *vote_list, User logged_user,
                                UserList *user_list,
                                CommentTreeList *comment_tree_list);
void handle_comment_selection(Item selected_item, UserList *user_list,
                              CommentTreeList *comment_list,
                              VoteList *vote_list, PostList *post_list,
                              User logged_user);
void handle_posts_page(PostList *post_list, VoteList *vote_list,
                       User logged_user, UserList *user_list,
                       BoardList *board_list,
                       CommentTreeList *comment_tree_list, Id board_id,
                       Id user_id);
void handle_boards_page(PostList *post_list, VoteList *vote_list,
                        User logged_user, UserList *user_list,
                        BoardList *board_list,
                        CommentTreeList *comment_tree_list, Id user_id);

#endif
