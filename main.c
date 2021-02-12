#pragma warning(disable : 4996) // 에러 C4996을 무시
#include <stdio.h> 
#include <conio.h> // 콘솔 입출력 함수를 제공
#include <Windows.h> // 윈도우 용의 수 많은 함수를 제공

#define CIN GetStdHandle(STD_INPUT_HANDLE) // 표준 입력 디바이스
#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // 표준 출력 디바이스

/* 콘솔 색상 코드 */
#define WHITE 7 
#define RED 4
#define BLUE 1
#define YELLOW 6

const background[13][13] =
{
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

enum { // 게임 시작, 종료 관련 enum 
	GAME_START = 0,
	GAME_EXIT
};

enum { // 게임 플레이어 관련 enum
	NONE = 0,
	PALYER_1,
	PALYER_2
};

typedef struct score { // 게임 스코어 관련 구조체
	int player1;
	int player2;
} Score;

int OorX[3][3] = { 0, }; // O와 X의 위치를 저장하는 변수
int player = 0; // 현재 플레이어를 저장하는 변수
int gameFlag = 0; // 게임 시작 여부 관련 변수
int gameMenu = 0; // 선택된 메뉴 관련 변수
int turnCount = 0; // 게임 턴 횟수 변수
Score score = { 0, 0 }; // 게임 스코어 관련 변수

void gameScreen(); // 해당 함수는 게임 메뉴 화면과 게임 플레이 화면을 선택하여 보여주는 함수

void gameStartScreen(); // 게임 메뉴 화면 출력과 메뉴 선택 함수를 호출하는 함수
void printGameStartScreen(); // 게임 메뉴 화면 출력하는 함수
void inputGameMenu(); // 키보드 입력을 처리 하는 함수
void gotoMouseXY(int x, int y); // 마우스 포인터 이동을 해주는 함수
void selectedGameMenu(); // 선택된 메뉴 이벤트를 처리 하는 함수

void gamePlayingScreen(); // 게임 플레이에 쓰이는 변수 초기화와 게임 백그라운드 출력과 게임 진행에 관한 함수를 호출 하는 함수
void printGameBackground(); // 게임 백그라운드를 출력하는 함수
void printPlayerTurn(); // 현재 플레이어 차례를 출력하는 함수
void setColor(int color); // 콘솔 색상을 변경하는 함수
void inputMouseSetting(); // 콘솔 입력을 마우스 입력으로 바꾸는 함수

void gameEvent(); // 게임 마우스 입력 이벤트 처리와 게임 진행 이벤트를 처리 하는 함수

void getBackgroundPosition(int* posX, int* posY); // 클릭된 칸 왼쪽 상단 좌표 값을 얻기 위한 함수
void getOorXarrayPosition(int* posX, int* posY); // 얻은 좌표 값을 OorX 배열의 위치로 변환하는 함수
int allReadyOorXDraw(int x, int y); // 클릭된 위치에 O 또는 X가 그려져 있는지 확인하는 함수
void drawOorX(int player, int posX, int posY); // O 또는 X를 그리는 함수
void setOorXarray(int player, int posX, int posY); // OorX 배열에 해당 플레이어의 값을 추가하고, 턴을 증가 시키는 함수
int checkOorX(); // O, X 또는 턴을 확인하여 게임의 종료 여부를 반환 하는 함수
void winnerLog(int winner); // 게임 종료 로그를 출력 하는 함수

void main() {
	while (1) {
		gameScreen();
	}
}

void gameScreen() {
	if (!gameFlag) {
		gameStartScreen();
	}
	else {
		gamePlayingScreen();
	}
}

void gameStartScreen() {
	printGameStartScreen();

	while (gameFlag != 1) {
		inputGameMenu();
	}
}

void printGameStartScreen() {
	printf("[현재 스코어]\n");
	printf("player1 : %d vs player2 : %d\n\n", score.player1, score.player2);

	printf("[게임 메뉴]\n");
	printf("▶게임하기\n");
	printf("▷게임종료");
}

void gotoMouseXY(int x, int y) {
	COORD cur;
	cur.X = x;
	cur.Y = y;

	SetConsoleCursorPosition(COUT, cur);
}

void inputGameMenu() {
	char key = getch(); // getch()는 키보드의 하나 키를 입력 받을 수 있게 하는 함수

	if (key == 'w' || key == 'W') {
		gotoMouseXY(0, 4);
		printf("▶\n\r▷");
		gameMenu = GAME_START;
	}
	else if (key == 's' || key == 'S') {
		gotoMouseXY(0, 4);
		printf("▷\n\r▶");
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
		exit(0); // 프로그램을 종료 하는 함수
		break;
	}
}

void gamePlayingScreen() {
	/* 게임 플레이와 관련된 변수는 초기화를 한다 */
	player = 0;
	turnCount = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			OorX[i][j] = 0;
		}
	}

	printGameBackground(); // 게임 백그라운드를 출력한다
	inputMouseSetting(); // 마우스로 콘솔 입력을 바꾸기 위한 함수

	while (gameFlag != 0) {
		gameEvent(); // 마우스 클릭 이벤트와 게임 진행에 관한 이벤트를 처리한다
	}
}

void printGameBackground() {
	system("cls"); // 콘솔 화면을 초기화 한다

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			putchar(background[i][j]);
		}
		putchar('\n');
	}

	printPlayerTurn(); // 현재 플레이어 차례를 보여준다
}

void printPlayerTurn() {
	gotoMouseXY(0, 14);

	if (player) {
		setColor(BLUE);
	}
	else setColor(RED);

	printf("\nplayer%d 차례 입니다.\n", player + 1);
}

void setColor(int color) {
	SetConsoleTextAttribute(COUT, color);
}

void inputMouseSetting() {
	DWORD mode;

	GetConsoleMode(CIN, &mode); // 현재 콘솔 입력 모드를 가져온다.
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT); // 마우스 입력을 허용한다.
}

void gameEvent() {
	INPUT_RECORD record; // 콘솔 입력 버퍼의 입력 이벤트 구조체
	DWORD read; // unsigned long ( 데이터 기본 처리 단위인 WORD의 2배 )
	int posX, posY; // 마우스가 클릭된 좌표를 저장
	int winner; // 게임 무승부, 이긴 플레이어를 저장

	ReadConsoleInput(CIN, &record, 1, &read); // 콘솔 입력 이벤트 읽기

	if (record.EventType == MOUSE_EVENT) {
		if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED
			&& !(record.Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
			&& !(record.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK)) {
			int posX = record.Event.MouseEvent.dwMousePosition.X;
			int posY = record.Event.MouseEvent.dwMousePosition.Y;

			if (background[posX][posY] == ' ') { // 클릭한 곳이 공백인지 확인한다.
				getBackgroundPosition(&posX, &posY);
				if (!allReadyOorXDraw(posX, posY)) {
					drawOorX(player + 1, posX, posY);
					setOorXarray(player + 1, posX, posY);

					player = !player; // player을 not 취해서 player 교체를 한다.

					printPlayerTurn();
				}

				winner = checkOorX();

				if (winner != -1) {
					winnerLog(winner);
				}
			}
		}
	}
}

void getBackgroundPosition(int* posX, int* posY) {
	while (background[--(*posY)][*posX] != '-');
	(*posY)++;
	while (background[(*posY)][--(*posX)] != '|');
	(*posX)++;
}

void getOorXarrayPosition(int* posX, int* posY) {
	int x = (*posX), y = (*posY);
	x--; y--;

	while (x > 2) {
		x -= 3;
	}
	while (y > 2) {
		y -= 3;
	}

	(*posX) = x; (*posY) = y;
}

int allReadyOorXDraw(int x, int y) {
	getOorXarrayPosition(&x, &y);
	if (OorX[y][x] == 0)
		return 0; // O 또는 X가 없음
	else return 1; // O 또는 X가 있음
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
	getOorXarrayPosition(&posX, &posY);
	OorX[posY][posX] = player;
	turnCount++;
}

int checkOorX() {
	int i, j;
	int countA = 0, countB = 0;

	/* 대각선 확인 */
	for (i = 0; i < 2; i++) {
		if (OorX[i][i] != 0 || OorX[i][2 - i] != 0) {
			if (OorX[i][i] == OorX[i + 1][i + 1])
				countA++;
		}
		if (OorX[i][2 - i] != 0) {
			if (OorX[i][2 - i] == OorX[i + 1][2 - (i + 1)])
				countB++;
		}
	}

	if (countA == 2 || countB == 2) {
		return OorX[1][1];
	}

	/* 축 확인 */
	for (i = 0; i < 3; i++) {
		countA = 0; countB = 0;
		for (j = 0; j < 2; j++) {
			if (OorX[i][j] != 0) {
				if (OorX[i][j] == OorX[i][j + 1]) {
					countA++;
				}
			}
			if (OorX[j][i] != 0) {
				if (OorX[j][i] == OorX[j + 1][i]) {
					countB++;
				}
			}
		}

		if (countA == 2) {
			return OorX[i][j];
		}
		else if (countB == 2) {
			return OorX[j][i];
		}
	}

	/* 무승부 확인 */
	if (turnCount == 9) {
		return 0;
	}

	return -1;
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

	system("pause"); // 콘솔을 일시중단 시킨다.

	gameFlag = 0;
	system("cls");
	setColor(WHITE);
}