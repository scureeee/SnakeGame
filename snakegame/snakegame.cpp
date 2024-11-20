#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

/*マクロの定義*/
//フィールドの横幅と立幅
#define WIDTH 15
#define HEIGHT 10

//スネークの頭の初期位置
#define STARTX (WIDTH/2)
#define STARTY (HEIGHT/2)

//スネークの最大長
#define MAXLENGTH ((WIDTH - 2) * (HEIGHT - 2))
//スネークの最大長の1/3の長さ
#define LENGTH_LEV1 (MAXLENGTH / 3)
//スネークの最大長の2/3の長さ
#define LENGTH_LEV2 (LENGTH_LEV1 * 2)

//キー未入力時のスネーク進行速度 (TIME_〇lev[msec] * LOOPNUM
#define TIME_LEV1 50
#define TIME_LEV2 30
#define TIME_LEV3 10
#define LOOPNUM 10
/*マクロの定義*/


/*定数定義*/
//キー入力をenumの定数にするための定義
enum direction {
	left,
	up,
	right,
	down,
	non
};
/*定数定義*/


/*構造体の型定義*/
//スネークの元となる構造体
typedef struct snakePosition
{
	int x, y;
	enum direction nextDirection;
}snake;

//餌の元となる構造体
typedef struct snakeFood
{
	int x, y;
}food;
/*構造体の型定義*/


/*グローバル変数*/
//フィールドを2次元配列で定義
int field[HEIGHT][WIDTH];

//スネークを定義
snake snake1[MAXLENGTH];

//餌を定義
food food1;

//スネークの初期の長さを定義
int snakeLength = 2;

/*グローバル変数宣言*/


/*プロトタイプ宣言*/
int init();
int viewField();
int countSec();
enum direction getKey();
int shiftSnake();
int moveSnake(enum direction key);
int makeFood();
int judge();
/*プロトタイプ宣言*/


int main(void)
{
	enum direction key; //enum型の宣言
	int loopCount = 0;

	init(); //初期化処理(壁、スネーク、餌を作る)
	viewField();//描画処理

	while (1) {  //無限にループ
		countSec();//時間をカウントする
		key = getKey(); //キー入力を受け付ける
		if (loopCount == LOOPNUM || key != non) { //時間経過かキー入力があれば更新する
			moveSnake(key);
			if (judge() < 0) break;
			viewField();
			loopCount = 0;
		}
		loopCount += 1;
	}
	return 0;
}

int init()
{
	//フィールドの初期値
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1)) field[i][j] = 1;
			else field[i][j] = 0;
		}
	}


	//スネークの初期の長さ
	snakeLength = 2;


	//スネークの初期化
	memset(snake1, 0, sizeof(snake1));
	snake1[0].x = STARTX;
	snake1[0].y = STARTY;
	snake1[0].nextDirection = left;

	snake1[1].x = STARTX + 1;
	snake1[1].y = STARTY;

	//餌を作る
	makeFood();

	return 0;
}

int viewField()
{
	//フィールドの初期化
	system("cls");  //コンソールクリア
	

	//数値でフィールド上にあるものを判別してる
	//壁をフィールドに反映
	for (int i = 0; i < HEIGHT; i++) { //iをHEIGHTに届くまでループさせる
		for (int j = 0; j < WIDTH; j++) {
			if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1)) field[i][j] = 1;
			//壁は1　　空白は0
			else field[i][j] = 0;
		}
	}

	//スネークをフィールドに反映
	for (int i = 0; i < snakeLength; i++) {
		field[(HEIGHT - 1) - snake1[i].y][snake1[i].x] = 2;
	}
	field[(HEIGHT - 1) - food1.y][food1.x] = 3;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (field[i][j] == 1)printf("壁");
			else if (field[i][j] == 2)printf("蛇");
			else if (field[i][j] == 3)printf("餌");
			else printf("　");
			if (j == (WIDTH - 1)) printf("\n");
		}
	}
	return 0;
}

int countSec()
{
	//時間(mSec)をカウントする
	double startTime = 0.0;
	double endTime = 0.0;
	double totalTime = 0.0;

	int level = 0;

	//レベルをチェック
	if (snakeLength < LENGTH_LEV1) level = TIME_LEV1;
	else if (snakeLength >= LENGTH_LEV1 && snakeLength < LENGTH_LEV2) level = TIME_LEV2;
	else level = TIME_LEV3;

	startTime = clock(); //clock関数により時間計算が始まる
	while (1) {
		endTime = clock();
		totalTime = endTime - startTime;
		if (totalTime > TIME_LEV1)break;
	}
	return 0;
}

enum direction getKey()
{
	int key = 0;

	if (_kbhit()) //キー入力があるかチェック
	{
		key = _getch(); //キーコードを取得して変数に代入
	}

	if (key == 0x4B)  /* ←キー */
		return left;   /* 左に移動 */
	else if (key == 0x48)  /* ←キー */
		return up;   /* 左に移動 */
	else if (key == 0x4D)  /* ←キー */
		return right;   /* 右に移動 */
	else if (key == 0x50)  /* ←キー */
		return down;  /* 下に移動 */
	else
		return non;
	

	return non;
}

int shiftSnake()
{
	if (snakeLength == 1) return -1;

	for (int i = 1; i < snakeLength; i++)
	{
		//餌を食べて重なった部分はずらさない
		if (snake1[snakeLength - i].x == snake1[snakeLength - (i + 1)].x
			&& snake1[snakeLength - i].y == snake1[snakeLength - (i + 1)].y) {
			continue;
		}
		snake1[snakeLength - i].x = snake1[snakeLength - (i + 1)].x;
		snake1[snakeLength - i].y = snake1[snakeLength - (i + 1)].y;
	}

	return 0;
}

int moveSnake(enum direction key)
{
	switch (key)
	{
	case left:
			//首がある方向のキー入力だった場合は受け付けない
			if (snake1[0].x == (snake1[1].x + 1))
			{
				shiftSnake(); //首から下を前にシフトする
				snake1[0].x += 1; //頭を動かす
				break;
			}
			shiftSnake();
			snake1[0].x -= 1;
			snake1[0].nextDirection = left;
			break;
	case up:
		if (snake1[0].y == (snake1[1].y - 1)) 
		{
			shiftSnake();
			snake1[0].y -= 1;
			break;
		}
		shiftSnake();
		snake1[0].y += 1;
		snake1[0].nextDirection = up;
		break;
	case right:
		if (snake1[0].x == (snake1[1].x - 1)) 
		{
			shiftSnake();
			snake1[0].x -= 1;
			break;
		}
		shiftSnake();
		snake1[0].x += 1;
		snake1[0].nextDirection = right;
		break;
	case down:
		if (snake1[0].y == (snake1[1].y + 1))
		{
			shiftSnake();
			snake1[0].y += 1;
			break;
		}
		shiftSnake();
		snake1[0].y -= 1;
		snake1[0].nextDirection = down;
		break;
	case non:
		//キー入力がなかった場合は頭の向いている方向へ動かす
		switch (snake1[0].nextDirection)
		{
			case left:
				shiftSnake();
				snake1[0].x -= 1; // 頭を左に動かす
				snake1[0].nextDirection = left; // 頭が次に動く方向を左に設定
				break;
			case up:
				shiftSnake();
				snake1[0].y += 1;
				snake1[0].nextDirection = up;
				break;
			case right:
				shiftSnake();
				snake1[0].x += 1;
				snake1[0].nextDirection = right;
				break;
			case down:
				shiftSnake();
				snake1[0].y -= 1;
				snake1[0].nextDirection = down;
				break;
		}
		break;
	}
	return 0;
}

//ヘビを進めた後に、壁か自分の体、もしくは餌に当たったかの判定
int judge()
{
	//壁が体に当たったらアウト
	if (snake1[0].x == 0 || snake1[0].x == (WIDTH - 1) || snake1[0].y == 0 || snake1[0].y == (HEIGHT - 1))
	{
		return -1;
	}

	//頭が体に当たったらアウト
	if (snakeLength < 5);
	else if (snakeLength >= 5)
	{
		for (int i = 5; i < snakeLength; i++)
		{
			if (snake1[0].x == snake1[i].x && snake1[0].y == snake1[i].y)
				return -1;
		}
	}

	//餌を食べた
	if (snake1[0].x == food1.x && snake1[0].y == food1.y)
	{
		//餌を移動する
		makeFood();
		
		//スネークを伸ばす
		snakeLength += 1;
		snake1[snakeLength - 1].x = snake1[snakeLength - 2].x;
		snake1[snakeLength - 1].y = snake1[snakeLength - 2].y;

		return 0;
	}

	return 0;
}

int makeFood()
{
	int x, y;

	//餌の初期化
	food1.x = 0;
	food1.y = 0;

	while (1)
	{

		srand((unsigned int)time(NULL));  //現在時刻の情報で初期化
		x = 1 + (rand() % (WIDTH - 2));
		y = 1 + (rand() % (HEIGHT - 2));
		break;
	}

	food1.x = x;
	food1.y = y;

	return 0;
}