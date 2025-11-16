#define _CRT_SECURE_NO_WARNINGS 

#define FPS (1000 / 60.0f)
#define ROW 8
#define COL 6
#define SPEED 3
#define COLOR_WALL GREEN//
#define WIN_COUNT 5
#define PI 3.1415

#include <easyx.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

SOCKET slink;

struct Bullet
{
	int use;
	int B_time[2];
	float rad;
	float place[2];
};

struct Tank
{
	int alive;
	float rad;
	float place[2];
	struct Bullet bullet[5];
};

struct Key
{
	int R;
	int forward;
	int backward;
	int left;
	int right;
	int fire;
	int image[3];
};

int Start()
{
	int starttime, flashtime;
	IMAGE tank[2];
	IMAGE BK;
	loadimage(&tank[0], _T("./res/Tank_Trouble1.png"), 200, 200);
	loadimage(&tank[1], _T("./res/Tank_Trouble2.png"), 200, 200);
	loadimage(&BK, _T("./res/BK.png"), 1000, 700);
	mciSendString(L"open ./res/kada.mp3 alias music1", 0, 0, 0);
	TCHAR s0[5] = _T("双人模式");
	TCHAR s1[5] = _T("结束游戏");
	TCHAR s2[5] = _T("坦克动荡");
	TCHAR s3[5] = _T("联机模式");
	ExMessage message;
	starttime = clock();
	setbkmode(TRANSPARENT);
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setfillcolor(RGB(127, 127, 127));
	while (TRUE)
	{
		flashtime = clock() - starttime;
		if (flashtime >= FPS)
		{
			cleardevice();
			putimage(0, 0, &BK);
			putimage(390, 100, &tank[1],SRCAND);
			putimage(390, 100, &tank[0], SRCPAINT);
			rectangle(390, 400, 590, 450);
			rectangle(390, 470, 590, 520);
			rectangle(390, 540, 590, 590);
			if (message.x >= 390 && message.x <= 590)
			{
				if (message.y >= 400 && message.y <= 450)
				{
					solidrectangle(390, 400, 590, 450);
					if (message.lbutton)
					{
						mciSendString(L"play music1", 0, 0, 0);
						return 0;
					}
				}
				if (message.y >= 470 && message.y <= 520)
				{
					solidrectangle(390, 470, 590, 520);
					if (message.lbutton)
					{
						mciSendString(L"play music1", 0, 0, 0);
						return 1;
					}
				}
				if (message.y >= 540 && message.y <= 590)
				{
					solidrectangle(390, 540, 590, 590);
					if (message.lbutton)
					{
						mciSendString(L"play music1", 0, 0, 0);
						Sleep(500);
						return 3;
					}
				}
			}
			settextstyle(16, 0, _T("宋体"));
			outtextxy(460, 416, s0);
			outtextxy(460, 486, s3);
			outtextxy(460, 556, s1);
			settextstyle(32, 0, _T("宋体"));
			outtextxy(427, 310, s2);
			FlushBatchDraw();
			starttime = clock();
		}
		peekmessage(&message, EX_MOUSE);
	}
}

int NETLINK_S()
{
	SOCKET sServer;
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)return 0;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(8888);
	ser.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sServer, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)
	{
		closesocket(sServer);
		return 0;
	}
	if (listen(sServer, 1) == SOCKET_ERROR)
	{
		closesocket(sServer);
		return 0;
	}
	slink = accept(sServer, (SOCKADDR*)&remoteAddr, &nAddrlen);
	if (slink == INVALID_SOCKET)
	{
		closesocket(sServer);
		return 0;
	}
	closesocket(sServer);
	return 1;
}

int NETLINK_C(char s[16])
{
	sockaddr_in serAddr;
	slink = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slink == INVALID_SOCKET)return 0;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr(s);
	if (connect(slink, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(slink);
		return 0;
	}
	return 1;
}

int NETLINK(int mode,char s[16])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)return 0;
	if (mode == 1)return NETLINK_S();
	if (mode == 2)return NETLINK_C(s);
}

int Start_Link(int mode)
{
	if (mode == 0)return 0;
	if (mode == 3)return 3;
	int result;
	int starttime, flashtime;
	IMAGE tank[2];
	IMAGE BK;
	loadimage(&tank[0], _T("./res/Tank_Trouble1.png"), 200, 200);
	loadimage(&tank[1], _T("./res/Tank_Trouble2.png"), 200, 200);
	loadimage(&BK, _T("./res/BK.png"), 1000, 700);
	Sleep(500);
	mciSendString(L"close music1", 0, 0, 0);
	mciSendString(L"open ./res/kada.mp3 alias music2", 0, 0, 0);
	TCHAR s0[5] = _T("我是主机");
	TCHAR s1[5] = _T("返回主页");
	TCHAR s2[5] = _T("联机模式");
	TCHAR s3[5] = _T("我是客机");
	TCHAR s4[9] = _T("等待连接中...");
	TCHAR s5[5] = _T("连接成功");
	TCHAR s6[5] = _T("连接失败");
	ExMessage message;
	starttime = clock();
	setbkmode(TRANSPARENT);
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setfillcolor(RGB(127, 127, 127));
	while (TRUE)
	{
		flashtime = clock() - starttime;
		if (flashtime >= FPS)
		{
			cleardevice();
			putimage(0, 0, &BK);
			putimage(390, 100, &tank[1], SRCAND);
			putimage(390, 100, &tank[0], SRCPAINT);
			rectangle(390, 400, 590, 450);
			rectangle(390, 470, 590, 520);
			rectangle(390, 540, 590, 590);
			if (message.x >= 390 && message.x <= 590)
			{
				if (message.y >= 400 && message.y <= 450)
				{
					solidrectangle(390, 400, 590, 450);
					if (message.lbutton)
					{
						mciSendString(L"play music2", 0, 0, 0);
						mode = 1;
						break;
					}
				}
				if (message.y >= 470 && message.y <= 520)
				{
					solidrectangle(390, 470, 590, 520);
					if (message.lbutton)
					{
						mciSendString(L"play music2", 0, 0, 0);
						mode = 2;
						break;
					}
				}
				if (message.y >= 540 && message.y <= 590)
				{
					solidrectangle(390, 540, 590, 590);
					if (message.lbutton)
					{
						mciSendString(L"play music2", 0, 0, 0);
						return 4;
					}
				}
			}
			settextstyle(16, 0, _T("宋体"));
			outtextxy(460, 416, s0);
			outtextxy(460, 486, s3);
			outtextxy(460, 556, s1);
			settextstyle(32, 0, _T("宋体"));
			outtextxy(427, 310, s2);
			FlushBatchDraw();
			starttime = clock();
		}
		peekmessage(&message, EX_MOUSE);
	}
	if (mode == 1)
	{
		cleardevice();
		settextstyle(64, 0, _T("宋体"));
		outtextxy(300, 300, s4);
		FlushBatchDraw();
		result = NETLINK(mode,NULL);
		if (result == 1)
		{
			cleardevice();
			settextstyle(64, 0, _T("宋体"));
			outtextxy(320, 300, s5);
			FlushBatchDraw();
			Sleep(1000);
			return 1;
		}
		else
		{
			cleardevice();
			settextstyle(64, 0, _T("宋体"));
			outtextxy(320, 300, s6);
			FlushBatchDraw();
			Sleep(5000);
			return 3;
		}
	}
	if (mode == 2)
	{
		cleardevice();
		settextstyle(64, 0, _T("宋体"));
		outtextxy(300, 300, s4);
		FlushBatchDraw();
		TCHAR s0[16];
		char s[16];
		InputBox(s0, 16, L"请输入主机的ip地址",0,0,0,0,false);
		wcstombs(s, s0, MAX_PATH);
		result = NETLINK(mode,s);
		if (result == 1)
		{
			cleardevice();
			settextstyle(64, 0, _T("宋体"));
			outtextxy(320, 300, s5);
			FlushBatchDraw();
			Sleep(1000);
			return 2;
		}
		else
		{
			cleardevice();
			settextstyle(64, 0, _T("宋体"));
			outtextxy(320, 300, s6);
			FlushBatchDraw();
			Sleep(1000);
			return 3;
		}
	}
}

void Clean(struct Key key[2])
{
	int i, j;
	for (i = 0;i < 2;i++)
	{
		key[i].R = 0;
		key[i].backward = 0;
		key[i].fire = 0;
		key[i].forward = 0;
		key[i].left = 0;
		key[i].right = 0;
		for (j = 0;j < 3;j++)key[i].image[j] = 0;
	}
}

void Rand(struct Tank tank[2],int MAP[ROW][COL])
{
	int x, y, i, j, k, success = 1;
	int length, width;
	int place;
	while (success)
	{
		x = rand() % ROW;
		y = rand() % COL;
		if (MAP[x][y] == 0)
		{
			tank[0].place[0] = x;
			tank[0].place[1] = y;
			tank[0].rad = 0;
			tank[0].alive = 1;
			success = 0;
		}
	}success = 1;
	while (success)
	{
		x = rand() % ROW;
		y = rand() % COL;
		if (MAP[x][y] == 0)
		{
			if (x != tank[0].place[0] || y != tank[0].place[1])
			{
				tank[1].place[0] = x;
				tank[1].place[1] = y;
				tank[1].rad = 0;
				tank[1].alive = 1;
				success = 0;
			}
		}
	}
	length = 800 / ROW / 7 * 5;
	width = 600 / COL / 7 * 5;
	x = (800 / ROW - length) / 2;
	y = (600 / COL - width) / 2;
	for (i = 0;i < 2;i++)
	{
		tank[i].place[0] = x + 800 / ROW * tank[i].place[0];
		tank[i].place[1] = y + 600 / COL * tank[i].place[1];
		for (j = 0;j < 5;j++)tank[i].bullet[j].use = 0;
	}
	for (i = 0;i < 8;i++)
	{
		for (j = 0;j < 6;j++)
		{
			MAP[i][j] = 1111;
		}
	}
	for (i = 0;i < 48;i++)
	{
		x = i % 8;
		y = i / 8;
		for (j = 0;j < 2;j++)
		{
			place = rand() % 4;
			switch (place)
			{
			case 0:
				if (y > 0)
				{
					if (MAP[x][y] % 10 / 1 == 1)MAP[x][y] = MAP[x][y] - 1;
					if (MAP[x][y - 1] % 1000 / 100 == 1)MAP[x][y - 1] = MAP[x][y - 1] - 100;
				}break;
			case 1:
				if (x > 0)
				{
					if (MAP[x][y] % 100 / 10 == 1)MAP[x][y] = MAP[x][y] - 10;
					if (MAP[x - 1][y] % 10000 / 1000 == 1)MAP[x - 1][y] = MAP[x - 1][y] - 1000;
				}break;
			case 2:
				if (y < 5)
				{
					if (MAP[x][y] % 1000 / 100 == 1)MAP[x][y] = MAP[x][y] - 100;
					if (MAP[x][y + 1] % 10 / 1 == 1)MAP[x][y + 1] = MAP[x][y + 1] - 1;
				}break;
			case 3:
				if (x < 7)
				{
					if (MAP[x][y] % 10000 / 1000 == 1)MAP[x][y] = MAP[x][y] - 1000;
					if (MAP[x + 1][y] % 100 / 10 == 1)MAP[x + 1][y] = MAP[x + 1][y] - 10;
				}break;
			}
		}
	}
}

void Move_Tank(struct Tank tank[2],struct Key key[2])
{
	int i,j;
	IMAGE image[3];
	TCHAR s1[2] = _T("P");
	TCHAR s2[2];
	TCHAR s3[7] = _T("请求重置地图");
	loadimage(&image[0], _T("./res/motou.jpg"), 60,60);
	loadimage(&image[1], _T("./res/otu.jpg"), 60, 60);
	loadimage(&image[2], _T("./res/xiaoku.jpg"), 60, 60);
	for(i = 0;i < 2;i++)
	{
		if (tank[i].alive == 1)
		{
			if (key[i].fire)
			{
				for (j = 0;j < 5;j++)
				{
					if (tank[i].bullet[j].use == 0)
					{
						tank[i].bullet[j].use = 1;
						tank[i].bullet[j].B_time[0] = clock();
						tank[i].bullet[j].rad = tank[i].rad;
						tank[i].bullet[j].place[0] = tank[i].place[0] + 800 / ROW / 3 + 25 * cos(tank[i].rad);
						tank[i].bullet[j].place[1] = tank[i].place[1] + 600 / COL / 3 - 25 * sin(tank[i].rad);
						break;
					}
				}
			}
			if (key[i].forward)
			{
				tank[i].place[0] = tank[i].place[0] + SPEED * cos(tank[i].rad);
				tank[i].place[1] = tank[i].place[1] - SPEED * sin(tank[i].rad);
			}
			if (key[i].left)
				tank[i].rad = tank[i].rad + 0.05;
			if (key[i].backward)
			{
				tank[i].place[0] = tank[i].place[0] - SPEED * cos(tank[i].rad);
				tank[i].place[1] = tank[i].place[1] + SPEED * sin(tank[i].rad);
			}
			if (key[i].right)
				tank[i].rad = tank[i].rad - 0.05;
			if (key[i].R)
			{
				_stprintf(s2, _T("%d"), i + 1);
				if (i)settextcolor(RED);
				else settextcolor(BLUE);
				settextstyle(24, 0, _T("宋体"));
				outtextxy(820, 290, s1);
				outtextxy(835, 290, s2);
				outtextxy(850, 290, s3);
			}
		}
		for (j = 0;j < 3;j++)if (key[i].image[j])putimage(870, 190 + 170 * i, &image[j]);
	}
}

void Move_Bullet(struct Tank tank[2])
{
	int i,j;
	for (j = 0;j < 2;j++)
	{
		for (i = 0;i < 5;i++)
		{
			if (tank[j].bullet[i].use)
			{
				tank[j].bullet[i].place[0] = tank[j].bullet[i].place[0] + SPEED * cos(tank[j].bullet[i].rad);
				tank[j].bullet[i].place[1] = tank[j].bullet[i].place[1] - SPEED * sin(tank[j].bullet[i].rad);
			}
		}
	}
}

void Show_Tank(struct Tank tank[2])
{
	int i, length, width;
	IMAGE tank_image[2][2];
	length = 800 / ROW / 7 * 5;
	width = 600 / COL / 7 * 5;
	loadimage(&tank_image[0][0], _T("./res/TANK1.1.png"), length, width);
	loadimage(&tank_image[1][0], _T("./res/TANK2.1.png"), length, width);
	loadimage(&tank_image[0][1], _T("./res/TANK.2.png"), length, width);
	loadimage(&tank_image[1][1], _T("./res/TANK.2.png"), length, width);
	rotateimage(&tank_image[0][0], &tank_image[0][0], tank[0].rad, WHITE);
	rotateimage(&tank_image[0][1], &tank_image[0][1], tank[0].rad, WHITE);
	rotateimage(&tank_image[1][0], &tank_image[1][0], tank[1].rad, WHITE);
	rotateimage(&tank_image[1][1], &tank_image[1][1], tank[1].rad, WHITE);
	for (i = 0;i < 2;i++)
	{
		if (tank[i].alive == 1)
		{
			putimage(tank[i].place[0], tank[i].place[1], &tank_image[i][1], SRCAND);
			putimage(tank[i].place[0], tank[i].place[1], &tank_image[i][0], SRCPAINT);
		}
	}
}

void Show_Wall(int MAP[ROW][COL],int count[2])
{
	int i, j, k;
	TCHAR s1[6] = _T("P1得分:");
	TCHAR s2[6] = _T("P2得分:");
	TCHAR s3[3];
	TCHAR s4[3]; 
	IMAGE tank[2];
	loadimage(&tank[0], _T("./res/TANK1.png"), 80, 60);
	loadimage(&tank[1], _T("./res/TANK2.png"), 80, 60);
	_stprintf(s3, _T("%d"), count[0]);
	_stprintf(s4, _T("%d"), count[1]);
	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 10);
	setlinecolor(COLOR_WALL);
	for (i = 0;i < ROW;i++)
	{
		for (j = 0;j < COL;j++)
		{
			for (k = 0;k < 4;k++)
			{
				switch (k)
				{
				case 0:
					if (MAP[i][j] % (int)pow(10, k + 1) / (int)pow(10, k) == 1)
						line(800 / ROW * i, 600 / COL * j, 800 / ROW * (i + 1), 600 / COL * j);
					break;
				case 1:
					if (MAP[i][j] % (int)pow(10, k + 1) / (int)pow(10, k) == 1)
						line(800 / ROW * i, 600 / COL * j, 800 / ROW * i, 600 / COL * (j + 1));
					break;
				case 2:
					if (MAP[i][j] % (int)pow(10, k + 1) / (int)pow(10, k) == 1)
						line(800 / ROW * i, 600 / COL * (j + 1), 800 / ROW * (i + 1), 600 / COL * (j + 1));
					break;
				case 3:
					if (MAP[i][j] % (int)pow(10, k + 1) / (int)pow(10, k) == 1)
						line(800 / ROW * (i + 1), 600 / COL * j, 800 / ROW * (i + 1), 600 / COL * (j + 1));
					break;
				}
			}
		}
	}
	settextstyle(16, 0, _T("宋体"));
	settextcolor(BLUE);
	outtextxy(860, 160, s1);
	outtextxy(930, 160, s3);
	putimage(856, 80, &tank[0]);
	settextcolor(RED);
	outtextxy(860, 520, s2);
	outtextxy(930, 520, s4);
	putimage(856, 440, &tank[1]);
}

void Show_Bullet(struct Tank tank[2])
{
	int i, j;
	setfillcolor(BLACK);
	for (i = 0;i < 2;i++)
	{
		for (j = 0;j < 5;j++)
		{
			if (tank[i].bullet[j].use)
				solidcircle(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1], 5);
		}
	}
}

void Judge(struct Tank tank[2], struct Key key[2])
{
	int i, j, k, fail[2];
	float x[4],y[4],dx, dy;
	COLORREF color[5];
	color[0] = COLOR_WALL;
	color[1] = RGB(237, 28, 36);
	color[2] = RGB(136, 0, 21);
	color[3] = RGB(0, 162, 232);
	color[4] = RGB(63, 72, 204);
	for (i = 0; i < 2;i++)
	{
		for (j = 0;j < 5;j++)
		{
			if (tank[i].bullet[j].use)
			{
				if (getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1]) == COLOR_WALL)
				{
					if (getpixel(tank[i].bullet[j].place[0] - 2, tank[i].bullet[j].place[1]) != COLOR_WALL
						|| getpixel(tank[i].bullet[j].place[0] + 2, tank[i].bullet[j].place[1]) != COLOR_WALL)
						tank[i].bullet[j].rad = PI - tank[i].bullet[j].rad;
					else if (getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1] - 2) != COLOR_WALL
						|| getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1] + 2) != COLOR_WALL)
						tank[i].bullet[j].rad = -tank[i].bullet[j].rad;
					else tank[i].bullet[j].rad = PI / 2 - tank[i].bullet[j].rad;
				}
				if (getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1]) == RGB(0,162,232)
					|| getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1]) == RGB(63, 72, 204))
				{
					tank[0].alive = 0;
				}
				if (getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1]) == RGB(237, 28, 36)
					|| getpixel(tank[i].bullet[j].place[0], tank[i].bullet[j].place[1]) == RGB(136, 0, 21))
				{
					tank[1].alive = 0;
				}
			}
		}
	}
	fail[0] = 0;
	fail[1] = 0;
	for (i = 0;i < 2;i++)
	{
		x[0] = tank[i].place[0] + 800 / ROW / 3.0 + 30 * cos(tank[i].rad + 0.625);
		x[1] = tank[i].place[0] + 800 / ROW / 3.0 + 30 * cos(tank[i].rad + PI - 0.625);
		x[2] = tank[i].place[0] + 800 / ROW / 3.0 + 30 * cos(tank[i].rad + PI + 0.625);
		x[3] = tank[i].place[0] + 800 / ROW / 3.0 + 30 * cos(tank[i].rad - 0.625);
		y[0] = tank[i].place[1] + 600 / COL / 3.0 - 30 * sin(tank[i].rad + 0.625);
		y[1] = tank[i].place[1] + 600 / COL / 3.0 - 30 * sin(tank[i].rad + PI - 0.625);
		y[2] = tank[i].place[1] + 600 / COL / 3.0 - 30 * sin(tank[i].rad + PI + 0.625);
		y[3] = tank[i].place[1] + 600 / COL / 3.0 - 30 * sin(tank[i].rad - 0.625);
		for (j = 0;j < 3;j++)
		{
			dx = x[j + 1] - x[j];
		    dy = y[j + 1] - y[j];
			for (k = 0;k < 9;k++)if (getpixel(x[j] + k * dx / 8, y[j] + k * dy / 8) == color[0])fail[i] = 1;
		}
		dx = x[3] - x[0];
		dy = y[3] - y[0];
		for (k = 0;k < 9;k++)if (getpixel(x[0] + k * dx / 8, y[0] + k * dy / 8) == color[0])fail[i] = 1;
		if (fail[i])
		{
			if (key[i].left)
				tank[i].rad = tank[i].rad - 0.05;
			if (key[i].right)
				tank[i].rad = tank[i].rad + 0.05;
			if (key[i].forward)
			{
				tank[i].place[0] = tank[i].place[0] - SPEED * cos(tank[i].rad);
				tank[i].place[1] = tank[i].place[1] + SPEED * sin(tank[i].rad);
			}
			if (key[i].backward)
			{
				tank[i].place[0] = tank[i].place[0] + SPEED * cos(tank[i].rad);
				tank[i].place[1] = tank[i].place[1] - SPEED * sin(tank[i].rad);
			}
		}
	}
}

void Synchro(struct Key key[2], int mode)
{
	int date_send[8];
	char date_recv[255];
	int date[8];
	int ret,i;
	date_send[0] = key[mode - 1].forward;
	date_send[1] = key[mode - 1].backward;
	date_send[2] = key[mode - 1].left;
	date_send[3] = key[mode - 1].right;
	date_send[4] = key[mode - 1].image[0];
	date_send[5] = key[mode - 1].image[1];
	date_send[6] = key[mode - 1].image[2];
	date_send[7] = key[mode - 1].R;
	for (i = 0;i < 8;i++)
	{
		if(mode == 2)send(slink, (const char*)(&date_send[i]), sizeof(date_send[i]), 0);
		ret = recv(slink, date_recv, 255, 0);
		if (ret > 0)
		{
			date[i] = *((int*)(date_recv));
			date_recv[ret] = 0x00;
		}
		if (mode == 1)send(slink, (const char*)(&date_send[i]), sizeof(date_send[i]), 0);
	}
	key[2 - mode].forward = date[0];
	key[2 - mode].backward = date[1];
	key[2 - mode].left = date[2];
	key[2 - mode].right = date[3];
	key[2 - mode].image[0] = date[4];
	key[2 - mode].image[1] = date[5];
	key[2 - mode].image[2] = date[6];
	key[2 - mode].R = date[7];
}

int Game(int mode)
{
	if (mode != 3)
	{
		int NUM = 0, game = 1;
		int date_send[5] = { 0 };
		char date_recv[5][255];
		int date[5], ret[5];
		int starttime[3], flashtime[3], i, j, num;
		int result = 1, MAP[ROW][COL] = { 0 };
		int T_time[3][2], length, width;
		struct Tank tank[2];
		int count[2] = { 0 };
		int start[3] = { 0 };
		int Result[3] = { 0 };
		struct Key key[2];
		length = 800 / ROW;
		width = 600 / COL;
		mciSendString(L"open ./res/烟花闪烁的夜晚.mp3 alias music", 0, 0, 0);
		mciSendString(L"play music", 0, 0, 0);
		IMAGE tank_image_B[2][2];
		loadimage(&tank_image_B[0][0], _T("./res/TANK_BOOM1.1.png"), length, width);
		loadimage(&tank_image_B[1][0], _T("./res/TANK_BOOM2.1.png"), length, width);
		loadimage(&tank_image_B[0][1], _T("./res/TANK_BOOM.2.png"), length, width);
		loadimage(&tank_image_B[1][1], _T("./res/TANK_BOOM.2.png"), length, width);
		while (game)
		{
			if (count[0] == WIN_COUNT)return 1;
			if (count[1] == WIN_COUNT)return 2;
			Clean(key);
			for (i = 0;i < 3;i++)starttime[i] = clock();
			for (i = 0;i < 2;i++)for (j = 0;j < 5;j++)tank[i].bullet[j].B_time[0] = clock();
			for (i = 0;i < 3;i++)
			{
				start[i] = 0;
				Result[i] = 0;
			}
			srand((unsigned int)time(NULL));
			Rand(tank, MAP);
			while (result)
			{
				for (i = 0;i < 3;i++)flashtime[i] = clock() - starttime[i];
				if (mode == 0)//双人模式
				{
					if (flashtime[1] >= 200)
					{
						if (GetAsyncKeyState(' ') && 0x8000)key[0].fire = 1;
						if (GetAsyncKeyState(VK_RETURN) && 0x8000)key[1].fire = 1;
						if (GetAsyncKeyState('R') && 0x8000)Result[2] = 1;
						starttime[1] = clock();
					}
					if (flashtime[2] >= 20)
					{
						if (GetAsyncKeyState('W') && 0x8000)key[0].forward = 1;
						if (GetAsyncKeyState('A') && 0x8000)key[0].left = 1;
						if (GetAsyncKeyState('S') && 0x8000)key[0].backward = 1;
						if (GetAsyncKeyState('D') && 0x8000)key[0].right = 1;
						if (GetAsyncKeyState(38) && 0x8000)key[1].forward = 1;
						if (GetAsyncKeyState(37) && 0x8000)key[1].left = 1;
						if (GetAsyncKeyState(40) && 0x8000)key[1].backward = 1;
						if (GetAsyncKeyState(39) && 0x8000)key[1].right = 1;
						if (GetAsyncKeyState('1') && 0x8000)key[0].image[0] = 1;
						if (GetAsyncKeyState('2') && 0x8000)key[0].image[1] = 1;
						if (GetAsyncKeyState('3') && 0x8000)key[0].image[2] = 1;
						if (GetAsyncKeyState(VK_NUMPAD1) && 0x8000)key[1].image[0] = 1;
						if (GetAsyncKeyState(VK_NUMPAD2) && 0x8000)key[1].image[1] = 1;
						if (GetAsyncKeyState(VK_NUMPAD3) && 0x8000)key[1].image[2] = 1;
						starttime[2] = clock();
					}
				}
				else//联机模式
				{
					if (flashtime[1] >= 200)
					{
						if (GetAsyncKeyState(' ') && 0x8000)key[mode - 1].fire = 1;
						starttime[1] = clock();
					}
					if (flashtime[2] >= 20)
					{
						if (GetAsyncKeyState('W') && 0x8000)key[mode - 1].forward = 1;
						if (GetAsyncKeyState('A') && 0x8000)key[mode - 1].left = 1;
						if (GetAsyncKeyState('S') && 0x8000)key[mode - 1].backward = 1;
						if (GetAsyncKeyState('D') && 0x8000)key[mode - 1].right = 1;
						if (GetAsyncKeyState('1') && 0x8000)key[mode - 1].image[0] = 1;
						if (GetAsyncKeyState('2') && 0x8000)key[mode - 1].image[1] = 1;
						if (GetAsyncKeyState('3') && 0x8000)key[mode - 1].image[2] = 1;
						if (GetAsyncKeyState('R') && 0x8000)key[mode - 1].R = 1;
						starttime[2] = clock();
					}
				}
				if (flashtime[0] >= FPS)
				{
					num = 0;
					cleardevice();
					Show_Tank(tank);
					for (i = 0;i < 2;i++)
					{
						if (tank[i].alive == 0)
						{
							T_time[i][0] = clock();
							start[i] = 1;
							tank[i].alive = -1;
						}
						if (start[i])
						{
							T_time[i][1] = clock() - T_time[i][0];
							if (T_time[i][1] <= 2000)
							{
								putimage(tank[i].place[0], tank[i].place[1], &tank_image_B[i][1], SRCAND);
								putimage(tank[i].place[0], tank[i].place[1], &tank_image_B[i][0], SRCPAINT);
							}
							else start[i] = 0;
						}
					}
					Show_Wall(MAP, count);
					if (mode != 0)
					{
						Synchro(key, mode);
						if (key[0].R == 1 && key[1].R == 1)
						{
							Result[2] = 1;
							Sleep(1000);
						}
					}
					Move_Tank(tank, key);
					Move_Bullet(tank);
					Judge(tank, key);
					for (i = 0;i < 2;i++)if (tank[i].alive == 1)num++;
					if (num <= 1 && start[2] == 0)
					{
						start[2] = 1;
						T_time[2][0] = clock();
					}
					if (start[2])
					{
						T_time[2][1] = clock() - T_time[2][0];
						if (T_time[2][1] >= 3000)
						{
							if (tank[0].alive == 1)Result[0] = 1;
							else if (tank[1].alive == 1)Result[1] = 1;
							else Result[2] = 1;
						}
					}
					Show_Bullet(tank);
					Clean(key);
					FlushBatchDraw();
					starttime[0] = clock();
				}
				for (i = 0;i < 2;i++)for (j = 0;j < 5;j++)
				{
					tank[i].bullet[j].B_time[1] = clock() - tank[i].bullet[j].B_time[0];
					if (tank[i].bullet[j].B_time[1] >= 8000)tank[i].bullet[j].use = 0;
				}
				for (i = 0;i < 2;i++)if (Result[i])count[i]++;
				if (Result[0] == 1 || Result[1] == 1 || Result[2] == 1)break;
			}
		}
	}
	return 0;
}

int End(int mode,int result)
{
	int starttime, flashtime, number = 0;
	mciSendString(L"close music", 0, 0, 0);
	mciSendString(L"close music1", 0, 0, 0);
	mciSendString(L"close music2", 0, 0, 0);
	mciSendString(L"open ./res/kada.mp3 alias music1", 0, 0, 0);
	TCHAR s0[5] = _T("再来一次");
	TCHAR s1[5] = _T("结束游戏");
	TCHAR s2[5] = _T("获得胜利");
	TCHAR s3[2] = _T("P");
	TCHAR s4[2];
	_stprintf(s4, _T("%d"), result);
	IMAGE BK;
	loadimage(&BK, _T("./res/BK.png"), 1000, 700);
	ExMessage message;
	starttime = clock();
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(127,127,127));
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 1);
	settextcolor(BLACK);
	while (mode != 3)
	{
		flashtime = clock() - starttime;
		if (flashtime >= FPS)
		{
			cleardevice();
			putimage(0, 0, &BK);
			rectangle(390, 400, 590, 450);
			rectangle(390, 500, 590, 550);
			if (message.x >= 390 && message.x <= 590)
			{
				if (message.y >= 400 && message.y <= 450)
				{
					solidrectangle(390, 400, 590, 450);
					if (message.lbutton)
					{
						mciSendString(L"play music1", 0, 0, 0);
						return 1;
					}
				}
				if (message.y >= 500 && message.y <= 550)
				{
					solidrectangle(390, 500, 590, 550);
					if (message.lbutton)
					{
						mciSendString(L"play music1", 0, 0, 0);
						Sleep(500);
						mciSendString(L"close music1", 0, 0, 0);
						return 0;
					}
				}
			}
			settextstyle(16, 0, _T("宋体"));
			outtextxy(460, 416, s0);
			outtextxy(460, 516, s1);
			settextstyle(32, 0, _T("宋体"));
			outtextxy(450, 220, s2);
			outtextxy(410, 220, s3);
			outtextxy(430, 220, s4);
			FlushBatchDraw();
			starttime = clock();
		}
		peekmessage(&message, EX_MOUSE);
	}
	return 0;
}

int main()//代码美观优化
{
	int mode,result,end = 1,start = 1;
	initgraph(1000, 700);
	BeginBatchDraw();
	setbkcolor(WHITE);
	while (start)
	{
		start = 0;
		mode = Start();
		mode = Start_Link(mode);
		if (mode == 4)
		{
			Sleep(500);
			mciSendString(L"close music2", 0, 0, 0);
			start = 1;
		}
	}
	while (end)
	{
		result = Game(mode);
		end = End(mode, result);
	}
	EndBatchDraw();
	closesocket(slink);
	WSACleanup();
	return 0;
}