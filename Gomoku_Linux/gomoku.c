#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include <locale.h>

#define BOARD_SIZE 15
#define MAX_NAME 50

typedef struct {
    char player1[MAX_NAME];
    char player2[MAX_NAME];
    char winner[MAX_NAME];
    char date[30];
} GameRecord;

void init_game();
void end_game();
void get_player_names(char *p1, char *p2);
void draw_board(char board[BOARD_SIZE][BOARD_SIZE], int cursor_x, int cursor_y, char *p1, char *p2, int turn);
void draw_title();
int stone(char board[BOARD_SIZE][BOARD_SIZE], int *cursor_x, int *cursor_y, int color, char *p1, char *p2, int turn);
int win_check(char board[BOARD_SIZE][BOARD_SIZE]);
void save_game_record(char *p1, char *p2, int winner);
void show_records();
int game_over_screen(char board[BOARD_SIZE][BOARD_SIZE], int color, char *p1, char *p2);

int main(void) {
    char board[BOARD_SIZE][BOARD_SIZE];
    char player1[MAX_NAME], player2[MAX_NAME];
    int color = 1;
    int cursor_x = 7, cursor_y = 7;
    int turn = 0;
    
    init_game();
    
    // 타이틀 화면
    draw_title();
    getch();
    
    // 플레이어 이름 입력
    get_player_names(player1, player2);
    
    // 게임 시작
    while(1) {
        // 보드 초기화
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = '#';
            }
        }
        
        cursor_x = 7;
        cursor_y = 7;
        color = 1;
        turn = 0;
        
        // 게임 진행
        int game_result = 0;
        while(1) {
            int result = stone(board, &cursor_x, &cursor_y, color, player1, player2, turn);
            
            if(result == -1) { // ESC로 종료
                game_result = -1;
                break;
            }
            
            int check = win_check(board);
            
            if(check == 1 || check == 2) {
                int choice = game_over_screen(board, check, player1, player2);
                save_game_record(player1, player2, check);
                
                if(choice == 1) {
                    game_result = 1; // 다시 하기
                    break;
                } else {
                    game_result = -1; // 종료
                    break;
                }
            }
            
            // 보드가 다 찼는지 확인
            int full = 1;
            for(int i = 0; i < BOARD_SIZE; i++) {
                for(int j = 0; j < BOARD_SIZE; j++) {
                    if(board[i][j] == '#') {
                        full = 0;
                        break;
                    }
                }
                if(!full) break;
            }
            
            if(full) {
                clear();
                mvprintw(LINES/2, (COLS-40)/2, "무승부입니다!");
                mvprintw(LINES/2+2, (COLS-40)/2, "1: 다시 하기  2: 종료");
                refresh();
                
                int ch = getch();
                if(ch == '1') {
                    game_result = 1;
                    break;
                } else {
                    game_result = -1;
                    break;
                }
            }
            
            color *= -1;
            turn++;
        }
        
        if(game_result == -1) break;
    }
    
    end_game();
    return 0;
}

void init_game() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // 마우스 이벤트 활성화
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // 백돌
    init_pair(2, COLOR_BLACK, COLOR_WHITE);  // 흑돌
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // 커서
    init_pair(4, COLOR_CYAN, COLOR_BLACK);   // 타이틀
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // 정보
}

void end_game() {
    endwin();
}

void draw_title() {
    clear();
    attron(COLOR_PAIR(4) | A_BOLD);
    
    int start_y = LINES/2 - 5;
    mvprintw(start_y, (COLS-50)/2, "o-------------------------------------------o");
    mvprintw(start_y+1, (COLS-50)/2, "|                                           |");
    mvprintw(start_y+2, (COLS-50)/2, "|               Gomoku Game!                |");
    mvprintw(start_y+3, (COLS-50)/2, "|                                           |");    
    mvprintw(start_y+4, (COLS-50)/2, "|          This was made with C!            |");
    mvprintw(start_y+5, (COLS-50)/2, "|                                           |");
    mvprintw(start_y+6, (COLS-50)/2, "o-------------------------------------------o");
    
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    attron(COLOR_PAIR(5));
    mvprintw(start_y+8, (COLS-50)/2 + 8, "Press any key to start...");
    attroff(COLOR_PAIR(5));
    
    refresh();
}

void get_player_names(char *p1, char *p2) {
    clear();
    echo();
    curs_set(1);
    
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(LINES/2 - 3, (COLS-40)/2, "======= Player Name Input =======");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    attron(COLOR_PAIR(1));
    mvprintw(LINES/2, (COLS-40)/2, "White Stone(O) Player Name: ");
    attroff(COLOR_PAIR(1));
    getnstr(p1, MAX_NAME-1);
    
    attron(COLOR_PAIR(2));
    mvprintw(LINES/2 + 2, (COLS-40)/2, "Black Stone(X) Player Name: ");
    attroff(COLOR_PAIR(2));
    getnstr(p2, MAX_NAME-1);
    
    if(strlen(p1) == 0) strcpy(p1, "Player 1");
    if(strlen(p2) == 0) strcpy(p2, "Player 2");
    
    noecho();
    curs_set(0);
    
    clear();
    attron(COLOR_PAIR(5));
    mvprintw(LINES/2, (COLS-40)/2, "Starting the game...");
    attroff(COLOR_PAIR(5));
    refresh();
    napms(1000);
}

void draw_board(char board[BOARD_SIZE][BOARD_SIZE], int cursor_x, int cursor_y, char *p1, char *p2, int turn) {
    clear();
    
    // 타이틀
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(0, (COLS-20)/2, "Gomoku Game!");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    // 플레이어 정보
    int info_x = 2;
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, info_x, "White Stone(O): %s", p1);
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(3, info_x, "Black Stone(X): %s", p2);
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    attron(COLOR_PAIR(5));
    mvprintw(4, info_x, "Turn: %d", turn + 1);
    mvprintw(6, info_x, "Now: %s", (turn % 2 == 0) ? p1 : p2);
    attroff(COLOR_PAIR(5));
    
    // 조작 방법
    mvprintw(LINES-3, 2, "Arrow Keys/Mouse: Move | SPACE/Click: Land | ESC: Exit");
    
    // 보드 그리기
    int start_y = 8;
    int start_x = (COLS - BOARD_SIZE*3) / 2;
    
    // 상단 좌표
    mvprintw(start_y-1, start_x, "  ");
    for(int i = 0; i < BOARD_SIZE; i++) {
        mvprintw(start_y-1, start_x + 2 + i*3, "%2d", i);
    }
    
    for(int i = 0; i < BOARD_SIZE; i++) {
        // 좌측 좌표
        mvprintw(start_y + i, start_x, "%2d", i);
        
        for(int j = 0; j < BOARD_SIZE; j++) {
            int y = start_y + i;
            int x = start_x + 2 + j*3;
            
            // 돌 표시
            if(board[i][j] == 'O') {
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(y, x, "[o]");
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else if(board[i][j] == 'X') {
                attron(COLOR_PAIR(2) | A_BOLD);
                mvprintw(y, x, "[x]");
                attroff(COLOR_PAIR(2) | A_BOLD);
            } else {
                mvprintw(y, x, "[ ]");
            }
        }
    }
    
    refresh();
}

int stone(char board[BOARD_SIZE][BOARD_SIZE], int *cursor_x, int *cursor_y, int color, char *p1, char *p2, int turn) {
    MEVENT event;
    
    while(1) {
        draw_board(board, *cursor_x, *cursor_y, p1, p2, turn);
        
        int ch = getch();
        
        switch(ch) {
            case KEY_UP:
                if(*cursor_y > 0) (*cursor_y)--;
                break;
            case KEY_DOWN:
                if(*cursor_y < BOARD_SIZE-1) (*cursor_y)++;
                break;
            case KEY_LEFT:
                if(*cursor_x > 0) (*cursor_x)--;
                break;
            case KEY_RIGHT:
                if(*cursor_x < BOARD_SIZE-1) (*cursor_x)++;
                break;
            case ' ':
                if(board[*cursor_y][*cursor_x] == '#') {
                    board[*cursor_y][*cursor_x] = (color == 1) ? 'O' : 'X';
                    return 0;
                } else {
                    attron(COLOR_PAIR(2) | A_BOLD);
                    mvprintw(LINES-1, 2, "There is already a stone...");
                    attroff(COLOR_PAIR(2) | A_BOLD);
                    refresh();
                    napms(500);
                }
                break;
            case KEY_MOUSE:
                if(getmouse(&event) == OK) {
                    if(event.bstate & BUTTON1_CLICKED || event.bstate & BUTTON1_PRESSED) {
                        // 보드 영역 계산
                        int start_y = 8;
                        int start_x = (COLS - BOARD_SIZE*3) / 2;
                        
                        // 클릭 위치를 보드 좌표로 변환
                        int board_y = event.y - start_y;
                        int board_x = (event.x - start_x - 2) / 3;
                        
                        // 유효한 범위인지 확인
                        if(board_y >= 0 && board_y < BOARD_SIZE && 
                           board_x >= 0 && board_x < BOARD_SIZE) {
                            *cursor_x = board_x;
                            *cursor_y = board_y;
                            
                            // 빈 자리면 돌 놓기
                            if(board[*cursor_y][*cursor_x] == '#') {
                                board[*cursor_y][*cursor_x] = (color == 1) ? 'O' : 'X';
                                return 0;
                            } else {
                                attron(COLOR_PAIR(2) | A_BOLD);
                                mvprintw(LINES-1, 2, "There is already a stone...");
                                attroff(COLOR_PAIR(2) | A_BOLD);
                                refresh();
                                napms(500);
                            }
                        }
                    }
                }
                break;
            case 27: // ESC
                return -1;
        }
    }
}

int win_check(char board[BOARD_SIZE][BOARD_SIZE]) {
    // 가로 체크
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j <= BOARD_SIZE-5; j++) {
            if(board[i][j] != '#' &&
               board[i][j] == board[i][j+1] &&
               board[i][j] == board[i][j+2] &&
               board[i][j] == board[i][j+3] &&
               board[i][j] == board[i][j+4]) {
                return (board[i][j] == 'O') ? 1 : 2;
            }
        }
    }
    
    // 세로 체크
    for(int i = 0; i <= BOARD_SIZE-5; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] != '#' &&
               board[i][j] == board[i+1][j] &&
               board[i][j] == board[i+2][j] &&
               board[i][j] == board[i+3][j] &&
               board[i][j] == board[i+4][j]) {
                return (board[i][j] == 'O') ? 1 : 2;
            }
        }
    }
    
    // 대각선 \ 체크
    for(int i = 0; i <= BOARD_SIZE-5; i++) {
        for(int j = 0; j <= BOARD_SIZE-5; j++) {
            if(board[i][j] != '#' &&
               board[i][j] == board[i+1][j+1] &&
               board[i][j] == board[i+2][j+2] &&
               board[i][j] == board[i+3][j+3] &&
               board[i][j] == board[i+4][j+4]) {
                return (board[i][j] == 'O') ? 1 : 2;
            }
        }
    }
    
    // 대각선 / 체크
    for(int i = 4; i < BOARD_SIZE; i++) {
        for(int j = 0; j <= BOARD_SIZE-5; j++) {
            if(board[i][j] != '#' &&
               board[i][j] == board[i-1][j+1] &&
               board[i][j] == board[i-2][j+2] &&
               board[i][j] == board[i-3][j+3] &&
               board[i][j] == board[i-4][j+4]) {
                return (board[i][j] == 'O') ? 1 : 2;
            }
        }
    }
    
    return 0;
}

int game_over_screen(char board[BOARD_SIZE][BOARD_SIZE], int color, char *p1, char *p2) {
    clear();
    
    // 최종 보드 표시
    int start_y = 2;
    int start_x = (COLS - BOARD_SIZE*3) / 2;
    
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            int y = start_y + i;
            int x = start_x + j*3;
            
            if(board[i][j] == 'O') {
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(y, x, "[o]");
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else if(board[i][j] == 'X') {
                attron(COLOR_PAIR(2) | A_BOLD);
                mvprintw(y, x, "[x]");
                attroff(COLOR_PAIR(2) | A_BOLD);
            } else {
                mvprintw(y, x, "[ ]");
            }
        }
    }
    
    // 승리 메시지
    int msg_y = start_y + BOARD_SIZE + 2;
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(msg_y, (COLS-40)/2, "o-----------------------------------o");
    mvprintw(msg_y+1, (COLS-40)/2, "|                                   |");
    if(color == 1) {
        mvprintw(msg_y+2, (COLS-40)/2, "            %s Win!            ", p1);
    } else {
        mvprintw(msg_y+2, (COLS-40)/2, "            %s Win!            ", p2);
    }
    mvprintw(msg_y+3, (COLS-40)/2, "|                                   |");
    mvprintw(msg_y+4, (COLS-40)/2, "o-----------------------------------o");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    attron(COLOR_PAIR(5));
    mvprintw(msg_y+6, (COLS-40)/2, "1: Retry  2: Exit");
    attroff(COLOR_PAIR(5));
    
    refresh();
    
    while(1) {
        int ch = getch();
        if(ch == '1') return 1;
        if(ch == '2') return 2;
    }
}

void save_game_record(char *p1, char *p2, int winner) {
    FILE *fp = fopen("omok_records.txt", "a");
    if(fp == NULL) return;
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    fprintf(fp, "==========================================\n");
    fprintf(fp, "날짜: %04d-%02d-%02d %02d:%02d:%02d\n", 
            t->tm_year+1900, t->tm_mon+1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    fprintf(fp, "백돌(O): %s\n", p1);
    fprintf(fp, "흑돌(X): %s\n", p2);
    fprintf(fp, "승자: %s\n", (winner == 1) ? p1 : p2);
    fprintf(fp, "==========================================\n\n");
    
    fclose(fp);
}