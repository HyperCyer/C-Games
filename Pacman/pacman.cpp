/*
				   _ooOoo_
				  o8888888o
				  88" . "88
				  (| -_- |)
				  O\  =  /O
			   ____/`---'\____
			 .'  \\|     |//  `.
			/  \\|||  :  |||//  \
		   /  _||||| -:- |||||-  \
		   |   | \\\  -  /// |   |
		   | \_|  ''\---/''  |   |
		   \  .-\__  `-`  ___/-. /
		 ___`. .'  /--.--\  `. . __
	  ."" '<  `.___\_<|>_/___.'  >'"".
	 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
	 \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
				   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		 佛祖保佑       永无BUG
*/
// pacman.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "pacman.h"
#include "GObject.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#define MAXLENGTH 735
#define MAXWIDTH 780
#define MAX_LOADSTRING 100
#define P_ROW 9
#define P_ARRAY 13
using namespace std;

FILE *stream;



// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
PacMan* p;
Enemy* e1;
Enemy* e2;
Enemy* e3;
Enemy* e4;
Stage_1* sMap1;
int s_n = 0;//关卡数
void RestGObject();

//释放动态内存函数模板
template<class T>
void Realese(T t)
{
	if (t != NULL)
		delete t;
}

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。
	//调用控制台
	/*AllocConsole();
	freopen_s(&stream, "CONOUT$", "w+t", stdout);
	freopen_s(&stream, "CONIN$", "r+t", stdin);*/

	p = new PacMan(P_ROW, P_ARRAY);
	e1 = new RedEn(E_ROW, E_ARRAY);
	e2 = new RedEn(E_ROW, E_ARRAY);
	e3 = new BlueEn(E_ROW, E_ARRAY);
	e4 = new YellowEn(E_ROW, E_ARRAY);
	GMap* MAPDATA[3] = { new Stage_1(),new Stage_2(),new Stage_3() };
	GObject::pStage = MAPDATA[0];
	sMap1 = new Stage_1();
	Enemy::player = p;
	srand((unsigned)time(NULL)); //随机数处理

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	HWND hWnd;
	if (!InitInstance(hInstance, nCmdShow, hWnd))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

	MSG msg;

	// 主消息循环:
	DWORD t = 0;
	while (Enemy::player->exit == false && s_n < 3)
	{
		if (p->win()) {
			s_n++;
			HDC hdc = GetDC(hWnd);
			RestGObject();
			if (s_n < 3) {
				char str[20];
				sprintf_s(str, "恭喜你通过第%d关", s_n);
				MessageBoxA(hWnd, str, "吃豆子提示", MB_OK);
				RECT WhiteRect;
				WhiteRect.left = 0;
				WhiteRect.top = 0;
				WhiteRect.right = MAXLENGTH;
				WhiteRect.bottom = MAXWIDTH;
				FillRect(hdc, &WhiteRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage = MAPDATA[s_n];
				GObject::pStage->DrawMap(hdc);
				e1->tw = UP;
				e2->tw = UP;
				e3->tw = UP;
				e4->tw = UP;
			}
		}
		if (Enemy::player->godie) {
			Enemy::player->godie = false;
			MessageBoxA(hWnd, "GAME OVER", "逮到你啦", MB_OK);
			Enemy::player->exit = true;
		}


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			p->SetTwCommand(DOWN);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			p->SetTwCommand(LEFT);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			p->SetTwCommand(RIGHT);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			p->SetTwCommand(UP);
		}

		if (GetTickCount() - t >= 58) {
			HDC hdc = GetDC(hWnd);
			GObject::pStage->DrawPeas(hdc);
			p->getPosition();
			e1->DrawBlank(hdc);
			e2->DrawBlank(hdc);
			e3->DrawBlank(hdc);
			e4->DrawBlank(hdc);
			p->DrawBlank(hdc);
			e1->action();
			e2->action();
			e3->action();
			e4->action();
			p->action();
			p->Draw(hdc);
			e1->Draw(hdc);
			e2->Draw(hdc);
			e3->Draw(hdc);
			e4->Draw(hdc);
			t = GetTickCount();
			DeleteDC(hdc);
		}


	}
	Realese(e1);
	for (int i = 0; i < 3; i++)
	{
		Realese(MAPDATA[i]);
	}
	Realese(p);

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PACMAN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& hWnd)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, MAXLENGTH, MAXWIDTH, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		GObject::pStage->DrawMap(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void RestGObject() {
	p->SetPosition(P_ROW, P_ARRAY);
	e1->SetPosition(E_ROW, E_ARRAY);
	e2->SetPosition(E_ROW, E_ARRAY);
	e3->SetPosition(E_ROW, E_ARRAY);
	e4->SetPosition(E_ROW, E_ARRAY);
}