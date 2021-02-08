#pragma warning(disable : 4996)
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define CIN GetStdHandle(STD_INPUT_HANDLE)
#define COUT GetStdHandle(STD_OUTPUT_HANDLE)
#define WHITE 7
#define RED 4
#define BLUE 1
#define YELLOW 6

enum {
	GAME_START = 0,
	GAME_EXIT
};

enum {
	NONE = 0,
	PALYER_1,
	PALYER_2
};

typedef struct score {
	int player1;
	int player2;
} Score;

const background[13][13] = {
	{'-','-','-','-','-','-','-','-','-','-','-','-','-',},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'-','-','-','-','-','-','-','-','-','-','-','-','-',},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'-','-','-','-','-','-','-','-','-','-','-','-','-',},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
	{'-','-','-','-','-','-','-','-','-','-','-','-','-',},
};
const O[3][3] =
{
	{' ', '*', ' '},
	{'*', ' ', '*'},
	{' ', '*', ' '},
};
const X[3][3] =
{
	{'*', ' ', '*'},
	{' ', '*', ' '},
	{'*', ' ', '*'},
};
int OorX[3][3] = { 0, };
int player = 0;
int gameFlag = 0;
int gameMenu = 0;
int turnCount = 0;
Score score = { 0, 0 };

void gameScreen();

/* 게임 시작 화면 관련 함수*/
void gameStartScreen();
void printGameStartScreen();
void inputGameMenu();
void selectedGameMenu();

/* 게임 플레이 화면 관련 함수 */
void gamePlayingScreen();
void printGameBackground();
void setColor(int color);
void getBackgroundPosition(int* posX, int* posY);
void getOorXarrayPostion(int* posX, int* posY);
void mouseEvent();
void inputMouseSetting();
void drawOorX(int player, int posX, int posY);
int allReadyOorXDraw(int x, int y);
void setOorXarray(int player, int posX, int posY);
void winnerLog(int winner);
int checkOorX();
void gotoMouseXY(int x, int y);
void printPlayerTurn();

void main() {
	while (1)
	{
		gameScreen();
	}
}

void gameScreen() {
	if (!gameFlag) { // 게임 시작 화면 호출
		gameStartScreen();
	}
	else { // 게임 플레이 화면 호출
		gamePlayingScreen();
	}
}

void gameStartScreen() {
	printGameStartScreen();
	inputGameMenu();
}

void printGameStartScreen() {
	system("cls");

	printf("[현재 스코어]\n");
	printf("player1 : %d vs player2 : %d\n\n", score.player1, score.player2);

	printf("[게임 메뉴]\n");
	printf("%s 게임하기\n", gameMenu == GAME_START ? "▶" : "▷");
	printf("%s 게임종료", gameMenu == GAME_EXIT ? "▶" : "▷");
}

void inputGameMenu() {
	char key = getch();

	if (key == 'w' || key == 'W') {
		gameMenu = GAME_START;
	}
	else if (key == 's' || key == 'S') {
		gameMenu = GAME_EXIT;
	}
	else if (key == '\r') {
		selectedGameMenu();
	}	
}

void selectedGameMenu() {
	switch (gameMenu)
	{
	case GAME_START:
		gameFlag = 1;
		break;
	case GAME_EXIT:
		exit(0);
		break;
	}
}

void gamePlayingScreen() {
	player = 0; 
	turnCount = 0;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			OorX[i][j] = 0;
		}
	}

	printGameBackground();

	while (gameFlag != 0) {
		mouseEvent();
	}
}

void printGameBackground() {
	system("cls");

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			putchar(background[i][j]);
		}
		putchar('\n');
	}

	printPlayerTurn();
}

void setColor(int color) {
	SetConsoleTextAttribute(COUT, color);
}

void getBackgroundPosition(int* posX, int* posY) {
	while (background[--(*posY)][*posX] != '-');
	(*posY)++;
	while (background[(*posY)][--(*posX)] != '|');
	(*posX)++;
}

void getOorXarrayPostion(int* posX, int* posY) {
	int x = (*posX), y = (*posY);
	x--; y--;

	while (x > 3) {
		x -= 3;
	}
	while (y > 3) {
		y -= 3;
	}

	(*posX) = x; (*posY) = y;
}

int allReadyOorXDraw(int x, int y) {
	if (OorX[y][x] == 0)
		return 0;
	else return 1;
}

void mouseEvent() {

	INPUT_RECORD record;
	DWORD read;
	int posX, posY;
	int winner = -1;

	inputMouseSetting();

	ReadConsoleInput(CIN, &record, 1, &read);

	if (record.EventType == MOUSE_EVENT) {
		if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED && !(record.Event.MouseEvent.dwEventFlags & MOUSE_MOVED) && !(record.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK)) {
			int posX = record.Event.MouseEvent.dwMousePosition.X;
			int posY = record.Event.MouseEvent.dwMousePosition.Y;

			if (background[posX][posY] == ' ') {
				getBackgroundPosition(&posX, &posY);
				drawOorX(player + 1, posX, posY);
				getOorXarrayPostion(&posX, &posY);
				if (!allReadyOorXDraw(posX, posY)) {
					setOorXarray(player + 1, posX, posY);

					player = !player;
					
					printPlayerTurn();

					winner = checkOorX();

					if (winner != -1) {
						winnerLog(winner);
					}
				}
			}
		}
	}
}

void inputMouseSetting() {
	DWORD mode;

	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
}


void drawOorX(int player, int posX, int posY) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int y = posY + i;
			int x = posX + j;

			gotoMouseXY(x, y);

			if (player == PALYER_1) {
				setColor(RED);
				printf("%c", X[i][j]);
			}
			if (player == PALYER_2) {
				setColor(BLUE);
				printf("%c", O[i][j]);
			}
		}
	}
}

void setOorXarray(int player, int posX, int posY) {
	OorX[posY][posX] = player;
	turnCount++;
}

void winnerLog(int winner) {
	system("cls");

	if (winner == PALYER_1) {
		setColor(RED);
		printf("player1이 이겼습니다 ! !\n\n");
		score.player1++;
	}
	else if (winner == PALYER_2) {
		setColor(BLUE);
		printf("player2가 이겼습니다 ! !\n\n");
		score.player2++;
	}
	else if (winner == NONE) {
		setColor(YELLOW);
		printf("무승부 입니다 ! !\n\n");
	}

	printf("Enter키를 눌러 메인화면으로 돌아가주세요...");

	if (getch() == '\r') {
		gameFlag = 0;
		system("cls");
		setColor(WHITE);
	}
}

int checkOorX() {
	int i, j;
	int countL = 0, countR = 0, countX = 0, countY = 0;

	/* 대각선 확인 */
	for (i = 0; i < 2; i++) {
		if (OorX[i][i] != 0 || OorX[i][2 - i] != 0) {
			if (OorX[i][i] == OorX[i + 1][i + 1])
				countL++;
			else countL = 0;
		}
		if (OorX[i][2 - i] != 0) {
			if (OorX[i][2 - i] == OorX[i + 1][2 - (i + 1)])
				countR++;
			else countR = 0;
		}
	}

	if (countL == 2 || countR == 2) {
		return OorX[1][1];
	}

	/* 줄 확인 */
	for (i = 0; i < 3; i++) {
		countX = 0; countY = 0;
		for (j = 0; j < 2; j++) {
			if (OorX[i][j] != 0) {
				if (OorX[i][j] == OorX[i][j + 1]) {
					countX++;
				}
			}
			if (OorX[j][i] != 0) {
				if (OorX[j][i] == OorX[j + 1][i]) {
					countY++;
				}
			}
		}

		if (countX == 2) {
			return OorX[i][j];
		}
		else if (countY == 2) {
			return OorX[j][i];
		}
	}

	/* 무승부 확인 */
	if (turnCount == 9) {
		return 0;
	}

	return -1;
}

void gotoMouseXY(int x, int y) {
	COORD cur;
	cur.X = x;
	cur.Y = y;

	SetConsoleCursorPosition(COUT, cur);
}

void printPlayerTurn() {
	gotoMouseXY(0, 14);

	if (player) {
		setColor(BLUE);
	}
	else setColor(RED);

	printf("\nplayer%d 차례 입니다.\n", player + 1);
}