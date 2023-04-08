/*
1.创建项目
2.先导入素材（找美工）
3.c++开发
4.设计c++项目的模块
	就是设计类:
		Block：表示方块
		Tetris：表示俄罗斯方块游戏
5.设计各个模块的主要接口
6.启动游戏
	

*/

#include"Tetris.h"

int main()
{

	Tetris game(20, 10, 263, 133, 36);
	game.play();
	return 0;  
}