#pragma once
#include<vector>
#include<graphics.h>
#include"Block.h"
using namespace std;
class Tetris
{
public:
	//���캯��
	Tetris(int rows, int cols, int left, int top, int  blockSize); 
	void init();//��ʼ��
	void play();//��ʼ��Ϸ


private:
	void keyEvent();
	void updateWindow();

	//���ؾ�����һ�ε��øú���������˶���ʱ�䣨ms��
	//��һ�ε��øú��� ����0
	int getDelay();
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();//��ת
	void drawScore();//���Ƶ�ǰ����
	void checkOver();//�����Ϸ�Ƿ����
	void saveScore();//������߷�
	void displayOver();//������Ϸ��������


private:
	int delay;
	bool update;//�Ƿ����


	//0:�հ�λ�ã�û���κη���
	//5�������ֶ���˹����
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;
	Block* nextBlock;//Ԥ�淽��
	Block  bakBlock;//��ǰ���齵������У�����������һ���˷�λ�õģ�
	
	int score; //��ǰ����
	int highestScore;//��߷�
	int level;//�ؿ�
	int lineCount;//��ǰ���˶�������
	bool gameOver;//��Ϸ�Ƿ����

	IMAGE imgOver;
	IMAGE imgWin;
};

