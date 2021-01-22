#include <stdio.h>
#include <Windows.h>

#define CIN GetStdHandle(STD_INPUT_HANDLE)
#define COUT GetStdHandle(STD_OUTPUT_HANDLE)
#define WHITE 7
#define RED 4
#define BLUE 1
#define YELLOW 6

char background[13][13] = {
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
int turnCount = 0;

void drawBackground();
void mouseEvent();
void inputMouseSetting();
void getBackgroundPosition(int* posX, int* posY);
void drawOorX(int player, int posX, int posY);
void getOorXarrayPostion(int* posX, int* posY);
void setOorXarray(int player, int posX, int posY);
int checkOorX();
void winnerLog(int winner);
void setColor(int color);
int allReadyOorXDraw(int x, int y);

int main() {
	drawBackground();

	while (1) {
		mouseEvent();
	}
}

void drawBackground() {
	system("cls");

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			if (background[i][j] != ' ') {
				if (background[i][j] == '|' || background[i][j] == '-') {
					setColor(WHITE);
				}
				else {
					int x = j, y = i;

					getBackgroundPosition(&x, &y);
					getOorXarrayPostion(&x, &y);

					if (OorX[y][x] == 1) {
						setColor(RED);
					}
					else if (OorX[y][x] == 2) {
						setColor(BLUE);
					}
				}
			}

			putchar(background[i][j]);
		}
		putchar('\n');
	}

	if (player) {
		setColor(BLUE);
	}
	else setColor(RED);

	printf("\nplayer%d 차례 입니다.\n", player + 1);
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

			if (posX < 13 && posY < 13) {
				getBackgroundPosition(&posX, &posY);
				drawOorX(player + 1, posX, posY);
				getOorXarrayPostion(&posX, &posY);
				if (!allReadyOorXDraw(posX, posY)) {
					setOorXarray(player + 1, posX, posY);

					player = !player;

					drawBackground();

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

void getBackgroundPosition(int* posX, int* posY) {
	while (background[--(*posY)][*posX] != '-');
	(*posY)++;
	while (background[(*posY)][--(*posX)] != '|');
	(*posX)++;
}

void drawOorX(int player, int posX, int posY) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int y = posY + i;
			int x = posX + j;

			if (player == 1)
				background[y][x] = X[i][j];
			if (player == 2)
				background[y][x] = O[i][j];
		}
	}
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

void setOorXarray(int player, int posX, int posY) {
	OorX[posY][posX] = player;
	turnCount++;
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

void winnerLog(int winner) {
	system("cls");

	if (winner == 1) {
		setColor(RED);
		printf("player1이 이겼습니다 ! !\n\n");
	}
	else if (winner == 2) {
		setColor(BLUE);
		printf("player2가 이겼습니다 ! !\n\n");
	}
	else if (winner == 0) {
		setColor(YELLOW);
		printf("무승부 입니다 ! !\n\n");
	}

	exit(0);
}

void setColor(int color) {
	SetConsoleTextAttribute(COUT, color);
}

int allReadyOorXDraw(int x, int y) {
	if (OorX[y][x] == 0)
		return 0;
	else return 1;
}