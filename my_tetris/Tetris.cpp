#include "Tetris.h"
#include<time.h>
#include<cstdlib>
#include<conio.h>
#include"Block.h"
#include<fstream>
#include<iostream>

#include<mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

//const int SPEED_NORMAL = 500;//ms
const int SPEED_NORMAL[MAX_LEVEL] = { 500,300,150,100,80 };
const int SPEED_QUICK = 30;

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}


}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat",0,0,0);

	delay = SPEED_NORMAL[0];

	//�����������
	srand(time(NULL));

	//������Ϸ����
	initgraph(938,896);

	//���ر���ͼƬ
	loadimage(&imgBg, "res/bg2.png");

	loadimage(&imgWin, "res/win.png");
	loadimage(&imgOver, "res/over.png");

	//���³�ʼ����Ϸ��������
	char data[20][10];
	for (int i = 0; i < rows; i++) {
		for (int j=0; j < cols;j++)
		{
			map[i][j] = 0;
		}
	}

	score = 0;
	lineCount = 0;
	level = 1;

	//��ʼ����߷�
	ifstream file;
	file.open(RECORDER_FILE);
	if (!file.is_open()) {
		cout << RECORDER_FILE << "��ʧ��" <<endl;
		highestScore = 100;
	}
	else
	{
		file >> highestScore;
	}
	file.close();//�ر��ļ�

	gameOver = false;
}


void Tetris::play()
{
	init();

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;



	int timer = 0;
	while (true) {
		//�����û�������
		keyEvent();

		timer += getDelay();
		if (timer > delay) {
			timer = 0;
			drop();
			//��Ⱦ��Ϸ����
			update = true;
			
		}
		if (update) {
			update = false;
			//������Ϸ����
			updateWindow();

			//������Ϸ����
			clearLine();
		}
		if (gameOver) {
			
			//�������
			saveScore();

			//������Ϸ��������
			displayOver();

			system("pause");
			init();

		}
		
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;//-128~127  //0..255
	bool rotateFlag = false;
	int dx = 0;

	if (_kbhit()) {
		ch = _getch();
		
		//������·��򰴼��������Զ����������ַ�
		//����������Ϸ���� ���Ⱥ󷵻أ�224 72
		// ����������·���� ���Ⱥ󷵻أ�224 80
		//��������������� ���Ⱥ󷵻أ�224 75
		//��������������� ���Ⱥ󷵻أ�224 77
		if (ch == 224) {
			ch = _getch();
			switch (ch) {
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;

			}
			
		}
	}
	if (rotateFlag == true) {
		//to do ��ʵ�� ��ת
		rotate();
		update = true;
	}
	if (dx != 0) {
		//�����ƶ�
		moveLeftRight(dx);
		update = true;

	}
	//update = true;
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg);//���Ʊ���ͼƬ

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0) continue;

			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}


	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(700, 115);

	drawScore();//���Ʒ���


	EndBatchDraw();
}

//��һ�ε���ֱ�ӷ���0
//���ؾ�����һ�ε����Ž��˶���ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blackInMap(map)) {
		//��������顰�̻���
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;

		//�����Ϸ�Ƿ����
		checkOver();
	 }
	delay = SPEED_NORMAL[level -1];

}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1;//�洢���ݵ�����
	for (int i = rows - 1; i >= 0; i--) {
		//����i���Ƿ�����
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//һ��ɨ��һ��洢
		}
		if (count < cols) {
			//��������
			k--; 
		}
		else {
			//����cout =cols
			lines++;
		}
		

	}
	if (lines > 0) {
		//����÷�
		int addScore[4] = { 10,30,60,80 };
		score += addScore[lines - 1];


		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
	
		//ÿһ�ٷ�Ϊһ������
		level = (score + 99) / 100;

		if (level > MAX_LEVEL) {
			gameOver = true;
		}
		

		lineCount += lines;

		
	}
	
}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);

	if (!curBlock->blackInMap(map)) {
		*curBlock = bakBlock;
	}
	//for(int i=0;i<4;i++)
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) {
		return;
	}

	bakBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blackInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180));

	LOGFONT f;
	gettextstyle(&f);//��ȡ��ǰ�ַ�
	f.lfHeight = 60;//����
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);//���屳��͸��Ч��


	outtextxy(670, 727, scoreText);

	//���������˶�����
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	//���Ƶ�ǰ�ǵڼ���
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	
	outtextxy(224-30, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	gameOver =( curBlock->blackInMap(map) == false);
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;

		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);

	//ʤ����������ʧ�ܽ���
	if (level <= MAX_LEVEL) {
		putimage(263, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(263, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
