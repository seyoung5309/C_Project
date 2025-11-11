#include <stdio.h>
#include <stdlib.h>

void Setboard(char [15][15]); // 보드 판 설정 
void prt(char [15][15]); // 현황 출력 
void stone(char [15][15], int); // 돌 두기  
int win_check(char [15][15]); // 승리 조건 확인  
int game_over(char [15][15], int); // 게임 오버 시 

int main(void) {

	int check = 0; 
    char board[15][15];
    Setboard(board);
    int color = 1;
    
	while (1) {
		stone(board, color);
		system("cls");
		check = win_check(board);
		if (check == 1) {
			system("cls");
			Setboard(board);
		} else if (check == 2) {
			printf("게임을 종료합니다.\n");
			break;
		}
		color *= -1;
	}
    return 0;
}

// 보드 판 설정 
void Setboard(char (*board)[15]) {
	system("cls");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            board[i][j] = '#';
        }
    }
}

// 현황 출력 
void prt(char (*board)[15]) {
	for (int i = 1; i <= 16; i++) {
		printf("%d", i-1);
		if (i < 10) {
			printf("  ");
		} else {
			printf(" ");
		}
	}
	printf("\n");
    for (int i = 0; i < 15; i++) {
    	printf("%d ", i+1);
    	if (i < 9) {
    		printf(" ");
		}
        for (int j = 0; j < 15; j++) {
            printf("%c  ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// 돌 두기 
void stone(char (*board)[15], int c) {
	int x, y;
	int count = 1; 
	
	prt(board);
	while (1) {
		printf("%s의 위치를 입력하세요(x좌표 y좌표): ", c == 1 ? "백 돌(O)" : "흑 돌(X)");
		scanf("%d %d", &y, &x);
	
		if (x < 1 || x > 15) {
			printf("옳바른 범위가 아닙니다. 다시 입력하세요.(3번 실패 시, 다음 턴:%d)\n\n", count);
			if (count == 3) {
				break;
			} else {
				count++;
				continue;
			}
		} else if (y < 1 || y > 15) {
			printf("옳바른 범위가 아닙니다. 다시 입력하세요.(3번 실패 시, 다음 턴:%d)\n\n", count);
			if (count == 3) {
				break;
			} else {
				count++;
				continue;
			}
		}
	
		if (board[x-1][y-1] == '#') {
			board[x-1][y-1] = (c == 1) ? 'O' : 'X';
			break;
		} else {
			printf("이미 돌이 존재합니다. 다시 입력하세요.(3번 실패 시, 다음 턴:%d)\n\n", count);	
			if (count == 3) {
				break;
			} else {
				count++;
			}
		}
	}
}

// 승리 조건 확인 
int win_check(char (*board)[15]) {
	int color = 0; 
	// 가로 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] == 'O' && board[i][j+1] == 'O' && board[i][j+2] == 'O' && board[i][j+3] == 'O' && board[i][j+4] == 'O') {
				color = 1; 
				break;
			} else if (board[i][j] == 'X' && board[i][j+1] == 'X' && board[i][j+2] == 'X' && board[i][j+3] == 'X' && board[i][j+4] == 'X') {
				color = 2; 
				break; 	
			}
		}
	}
	// 세로 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 10; j++) {
			if (board[j][i] == 'O' && board[j+1][i] == 'O' && board[j+2][i] == 'O' && board[j+3][i] == 'O' && board[j+4][i] == 'O') {
				color = 1; 
				break;
			} else if (board[j][i] == 'X' && board[j+1][i] == 'X' && board[j+2][i] == 'X' && board[j+3][i] == 'X' && board[j+4][i] == 'X') {
				color = 2; 
				break; 	
			}
		}
	}
	// 대각선 좌상향 
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++)
		if (board[i][j] == 'O' && board[i+1][j+1] == 'O' && board[i+2][j+2] == 'O' && board[i+3][j+3] == 'O' && board[i+4][j+4] == 'O') {
			color = 1;
			break;
		} else if (board[i][j] == 'X' && board[i+1][j+1] == 'X' && board[i+2][j+2] == 'X' && board[i+3][j+3] == 'X' && board[i+4][j+4] == 'X') {
			color = 2; 
			break;
		}
	}
	// 대각선 우상향	
	for (int i = 14; i >= 4; i--) {
		for (int j = 0; j < 11; j++) {
			if (board[i][j] == 'O' && board[i-1][j+1] == 'O' && board[i-2][j+2] == 'O' && board[i-3][j+3] == 'O' && board[i-4][j+4] == 'O') {
				color = 1;
				break;
			} else if (board[i][j] == 'X' && board[i-1][j+1] == 'X' && board[i-2][j+2] == 'X' && board[i-3][j+3] == 'X' && board[i-4][j+4] == 'X') {
				color = 2; 
				break;
			}
		}
	}
	// 전체 확인 - 다 채워졌는지. 
	int full = 0; 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] == '#') {
				full = 1; 
				break;
			}
		}
		if (full == 1) {
			break;
		} 
	}
	if (full == 0) {
		printf("전부 채워졌지만 승부가 나지 않았습니다...\n");
		printf("게임을 계속하시겠습니까?(다시 하기(1) | 종료 (2)): ");
		scanf("%d", &full);
		if (full == 1) {
			return 1;
		} else if (full == 2) {
			return 2; 
		}
	}
	full = 0; 
	// 게임 오버 시  
	if (color != 0) {
		int end = game_over(board, color);
		if (end == 1) {
			return 1; 
		} else if (end == 2) {
			return 2; 
		}
	}
	return 0; 
}

// 게임 오버 시 
int game_over(char (*board)[15], int color) {
	prt(board);
	if (color == 1) {
		printf("백 돌이 이겼다!\n");
	} else if (color == 2) {
		printf("흑 돌이 이겼다!\n");
	}
	printf("게임을 계속하시겠습니까?(다시 하기(1) | 종료 (2)): ");
	scanf("%d", &color);
	
	return color;
}