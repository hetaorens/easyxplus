#include<stdio.h>
//#include"groups_.h"
#include <graphics.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include<vector>
#pragma comment(lib,"Imm32.lib")
#include<iostream>
using namespace std;

typedef vector< pair<pair<RECT, string>, pair<RECT, string>>> text_elmt;	 //first pair is text  second pair is tanc

void GetIMEString(HWND hWnd, string& str)
{
	HIMC hIMC = ImmGetContext(hWnd);//获取HIMC 
	if (hIMC)
	{
		//这里先说明一下，以输入“中国”为例 
		//切换到中文输入法后，输入“zhongguo”，这个字符串称作IME组成字符串 
		//而在输入法列表中选择的字符串“中国”则称作IME结果字符串 
		static bool flag = false;//输入完成标记：在输入中时，IME组成字符串不为空，置true；输入完成后，IME组成字符串为空，置false 
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0); //获取IME组成输入的字符串的长度 
		if (dwSize > 0)//如果IME组成字符串不为空，且没有错误（此时dwSize为负值），则置输入完成标记为true 
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag) //如果IME组成字符串为空，并且标记为true，则获取IME结果字符串 
		{
			int iSize; //IME结果字符串的大小 
			LPSTR pszMultiByte = NULL;//IME结果字符串指针 
			int ChineseSimpleAcp = 936;//宽字节转换时中文的编码 
			WCHAR* lpWideStr = NULL;//宽字节字符数组 
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//获取IME结果字符串的大小 
			if (dwSize > 0) //如果IME结果字符串不为空，且没有错误 
			{
				dwSize += sizeof(WCHAR);//大小要加上NULL结束符 
				//为获取IME结果字符串分配空间 
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize); //清空结果空间 
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//获取IME结果字符串，这里获取的是宽字节 
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//计算将IME结果字符串转换为ASCII标准字节后的大小 
				//为转换分配空间 
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//宽字节转换 
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//添加到string中 
				//释放空间 
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
			}
			flag = false;
		}
		ImmReleaseContext(hWnd, hIMC);//释放HIMC 
	}
}

int button_judge(int x, int y, text_elmt text_obj)
{
	int cur = 1;
	for (auto const& ph : text_obj)
	{
		if (x > ph.first.first.left && x < ph.first.first.right && y<ph.first.first.bottom && y > ph.first.first.top) return cur;
		++cur;
	}
	return 0;
}

int drawing_interface(text_elmt text_obj, vector<string> backimg, bool mode = false)
{

	if (mode == true)
	{
		initgraph(800, 500);
		setbkcolor(WHITE);
	}
	cleardevice();
	//setbkcolor(WHITE);
	BeginBatchDraw();//开始批量绘图

	//底图			  
	setbkmode(TRANSPARENT);//设置背景模式
	IMAGE img;
	loadimage(&img, backimg[0].c_str(), 800, 500, true);
	putimage(0, 0, &img);
	//loadimage(&img, backimg[1].c_str(), 100, 100, true);
	//putimage(0, 0, &img);
	//-------------------------------------


	//settextstyle(40, 0, "宋体");
	//settextcolor(WHITE);
	//outtextxy(250, 50, "南京邮电大学大银行");
	//------------------------------------
	settextcolor(BLACK);

	//settextstyle(30, 0, "宋体", 0, 0, 1000, false, false, false);
	//setfillcolor(RGB(250, 250, 250));
	//setbkmode(TRANSPARENT);//设置背景模式
	for (auto& ph : text_obj)
	{
		settextstyle(30, 0, "微软雅黑");
		RECT temp1 = (ph.first.first);
		RECT temp2 = (ph.second.first);
		if (ph.second.second == ph.first.second)
		{
			//setfillstyle(BS_SOLID);
			//IMAGE img;/*
			//loadimage(&img, _T("bank.bmp")) ;
			//setfillstyle(BS_DIBPATTERN, NULL, &img); 
			//setfillstyle((BYTE*)"\x3e\x41\x80\x80\x80\x80\x80\x41");
			clearroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);

			//setfillstyle(BS_SOLID);
			setfillcolor(RGB(176, 196, 222));
			//cout << getfillcolor() << endl;
			//IMAGE img;
			//loadimage(&img, _T("bank.bmp")) ;
			//setfillstyle(BS_DIBPATTERN, NULL, &img); 
			//setfillstyle((BYTE*)"\x3e\x41\x80\x80\x80\x80\x80\x41");
			fillroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);
			drawtext(ph.first.second.c_str(), &temp1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else	if (ph.second.second == "")
		{
			clearroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);
			fillroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);
			drawtext(ph.first.second.c_str(), &temp1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else	if (ph.second.second == "1")
		{
			//clearroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);
			//fillroundrect(temp2.left, temp2.top, temp2.right, temp2.bottom, 20, 20);

			drawtext(ph.first.second.c_str(), &temp1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			drawtext(ph.first.second.c_str(), &temp1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

	}

	EndBatchDraw();//结束批量绘图
	return 1;
}

int click_interface(text_elmt text_obj, vector<string> backimg, int (*func)(text_elmt text_obj, vector<string> backimg, bool mode))
{

	(*func)(text_obj, backimg, true);
	MOUSEMSG m;//鼠标指针
	int i, event = 0;
	while (true)
	{
		m = GetMouseMsg();//获取一条鼠标消息

		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			setrop2(R2_XORPEN);
			setlinecolor(LIGHTCYAN);//线条颜色为亮青色
			setlinestyle(PS_SOLID, 3);//设置画线样式为实现，10磅
			setfillcolor(WHITE);//填充颜色为白色
			if (button_judge(m.x, m.y, text_obj) != 0)
			{
				if (event != button_judge(m.x, m.y, text_obj))
				{
					event = button_judge(m.x, m.y, text_obj);//记录这一次触发的按钮
					//graph_system(event);
					//fillrectangle(r[event - 1][0], r[event - 1][1], r[event - 1][2], r[event - 1][3]);//有框填充矩形（X1,Y1,X2,Y2）
					fillrectangle(text_obj[event - 1].first.first.left, text_obj[event - 1].first.first.top, text_obj[event - 1].first.first.right, text_obj[event - 1].first.first.bottom);
				}
			}
			else
			{
				if (event != 0)//上次触发的按钮未被修正为原来的颜色
				{
					//fillrectangle(r[event - 1][0], r[event - 1][1], r[event - 1][2], r[event - 1][3]);//两次同或为原来颜色
					fillrectangle(text_obj[event - 1].first.first.left, text_obj[event - 1].first.first.top, text_obj[event - 1].first.first.right, text_obj[event - 1].first.first.bottom);
					//graph_system(event);
					event = 0;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			setrop2(R2_NOTXORPEN);//二元光栅——NOT(屏幕颜色 XOR 当前颜色)
			for (i = 0; i <= 10; i++)
			{
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色
				circle(m.x, m.y, 2 * i);
				Sleep(10);//停顿10ms
				circle(m.x, m.y, 2 * i);//抹去刚刚画的圆
			}
			int chioce = button_judge(m.x, m.y, text_obj);
			if (chioce != 0)
			{
				flushmessage();
				return 	chioce;
			}
			else {
				flushmessage();//单击事件后清空鼠标消息
		   //printf("\r\n(%d,%d)",m.x,m.y);//打印鼠标坐标，方便调试时确定区域
				break;
			}
			break;
			//FlushMouseMsgBuff();//清空鼠标消息缓存区
			flushmessage();
		}
	}
}

vector<string> input_interface(text_elmt text_obj, vector<string> backimg, int (*func)(text_elmt text_obj, vector<string> backimg, bool mode))
{

	(*func)(text_obj, backimg, true);
	HWND hWnd = GetHWnd();
	BeginBatchDraw(); // 设置批绘图模式 

	int input_num = text_obj.size();
	vector<string> return_data;
	vector<string> str_v;
	for (int i = 0; i < input_num; ++i)
	{
		str_v.push_back("");
	}

	for (int i = 0; i < input_num; ++i)
	{

		string str = "";
		while (true)
		{

			if (_kbhit()) //如果是ASCII输入 
			{
				char c = _getch();
				if (c == '\b')
				{
					if (str.length() > 0)
					{
						if (str.at(str.length() - 1) & 0x8000)
							str.erase(str.end() - 1);
						str.erase(str.end() - 1);
						(*func)(text_obj, backimg, false);
						str_v[i] = str;
						int cur = 0;
						for (auto& ph : text_obj)
						{
							settextstyle(30, 0, "微软雅黑");
							outtextxy(ph.second.first.left, ph.second.first.top, str_v[cur].c_str());
							cur++;
						}

					}
				}
				else if (c == 27) {}
				else if (c == 13)
				{
					str_v[i] = str;
					break;
				}
				else {

					str += c;
					str_v[i] = str;
				}
			}
			else //除此之外，检测是否有IME输入，如果有，则将输入结果添加到string中 
			{
				GetIMEString(hWnd, str);
				str_v[i] = str;
			}
			if (str.length() > 20)
			{
				//str = "";

				str = "";
				str_v[i] = str;
				(*func)(text_obj, backimg, false);
				//outtextxy(310, 210, str.c_str());
				int cur = 0;
				for (auto& ph : text_obj)
				{
					settextstyle(30, 0, "微软雅黑");
					outtextxy(ph.second.first.left, ph.second.first.top, str_v[cur].c_str());
					cur++;
				}
				continue;
			}
			int cur = 0;
			for (auto& ph : text_obj)
			{
				settextstyle(30, 0, "微软雅黑");
				outtextxy(ph.second.first.left, ph.second.first.top, str_v[cur].c_str());
				cur++;
			}
			//cleardevice();
			FlushBatchDraw();

			Sleep(10);


		}
	}
	closegraph();
	return str_v;

}

int user_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("查询");
	text.push_back("取款");
	text.push_back("转账");
	text.push_back("存款");
	text.push_back("改密");
	text.push_back("历史");
	text.push_back("退出");
	int point[7][4] = { { -10,100,120,150 } , { 680,100,810,150 } ,{ -10,200,120,250 }, { 680,200,810,250 } , {-10,300,120,350 } , { 680,300,810,350 }, { 680,400,810,450 } };
	for (int i = 0; i < 7; i++)
	{
		RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
		s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss, text[i].c_str())));
	}
	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	choice = click_interface(s, img, drawing_interface);
	return choice;

}
vector<string> user_login_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("银行卡卡号：");
	text.push_back("银行卡密码：");
	int point[2][4] = { { 220,200,380,250 } , { 220,280,380,330 } };

	RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	RECT ss2 = { point[0][0] + 140, point[0][1] + 10, point[0][2] + 240, point[0][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));
	ss1 = { point[1][0], point[1][1], point[1][2], point[1][3] };
	ss2 = { point[1][0] + 140, point[1][1] + 10, point[1][2] + 70, point[1][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[1].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;

}
vector<string> user_Withdrawals_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	//text.push_back("银行卡卡号：");
	text.push_back("请输入取款金额：");
	int point[1][4] = { { 270,180,450,230 } };
	//int point[1][4] = { { 180,180,360,230 } };
	RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	RECT ss2 = { point[0][0] + 140, point[0][1] + 5, point[0][2] + 240, point[0][3] - 5 };
	//s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));*/
	ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	ss2 = { point[0][0] + 170, point[0][1] + 10, point[0][2] + 75, point[0][3] - 5 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;

}
vector<string> user_deposit_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	//text.push_back("银行卡卡号：");
	text.push_back("请输入存款金额：");
	int point[1][4] = { { 270,180,450,230 } };
	//int point[1][4] = { { 180,180,360,230 } };
	RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	RECT ss2 = { point[0][0] + 140, point[0][1] + 5, point[0][2] + 240, point[0][3] - 5 };
	//s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));*/
	ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	ss2 = { point[0][0] + 170, point[0][1] + 10, point[0][2] + 75, point[0][3] - 5 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;

}
vector<string> user_transfer_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back(" 对方卡号：");
	text.push_back(" 转账金额：");
	int point[2][4] = { { 220,200,380,250 } , { 220,280,380,330 } };

	RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	RECT ss2 = { point[0][0] + 140, point[0][1] + 10, point[0][2] + 240, point[0][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));
	ss1 = { point[1][0], point[1][1], point[1][2], point[1][3] };
	ss2 = { point[1][0] + 140, point[1][1] + 10, point[1][2] + 70, point[1][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[1].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
vector<string> user_changepd_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("请输入老密码：");
	text.push_back("请输入新密码：");
	int point[2][4] = { { 220,200,380,250 } , { 220,280,380,330 } };

	RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	RECT ss2 = { point[0][0] + 150, point[0][1] + 10, point[0][2] + 70, point[0][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));
	ss1 = { point[1][0], point[1][1], point[1][2], point[1][3] };
	ss2 = { point[1][0] + 150, point[1][1] + 10, point[1][2] + 70, point[1][3] - 10 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[1].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
int query_ui(vector<string> data)
{
	//int choice = 0;
	//text_elmt s;
	//vector<string> text;
 //
	//text.push_back("退出");
	//int point[1][4] = {   { 680,400,810,450 } };
	//for (int i = 0; i < 1; i++)
	//{
	//	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	//	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss, text[i].c_str())));
	//}
	//vector<string >img;
	//img.push_back("atm.bmp");
	//vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	//return choice;
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("余额 : " + data[0]);
	text.push_back("今日余额 : " + data[1]);

	//text.push_back("退出");
	int point[7][4] = { { 250,150,550,200 } ,{ 250,300,550,350 } };
	for (int i = 0; i < 2; i++)
	{
		RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
		s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss, "1")));
	}
	vector<string >img;
	img.push_back("atm.bmp");
	//vector<string> return_data;
	drawing_interface(s, img, true);
	//choice = click_interface(s, img, drawing_interface);
	_getch();
	return choice;

	//choice = click_interface(s, img, drawing_interface);
	//return_data = input_interface(s, img, drawing_interface);

}
int admin_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("开户");
	text.push_back("销户");
	text.push_back("查询");
	text.push_back("修改");

	text.push_back("退出");
	int point[7][4] = { { -10,100,120,150 } , { 680,100,810,150 } ,{ -10,200,120,250 }, { 680,200,810,250 } , {-10,300,120,350 } , { 680,300,810,350 }, { 680,400,810,450 } };
	for (int i = 0; i < 5; i++)
	{
		RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
		s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss, text[i].c_str())));
	}
	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	choice = click_interface(s, img, drawing_interface);
	return choice;
}
vector<string> admin_open_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("卡号");
	text.push_back("密码");
	text.push_back("姓名");
	text.push_back("金额");
	int point[4][4] = { { 220,100,380,150 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	for (int i = 0; i < 4; i++)
	{
		if (i == 1 || i == 3)
		{
			RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
			RECT ss2 = { point[i][0] + 140, point[i][1] + 10, point[i][2] + 60, point[i][3] - 10 };
			s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));
		}
		else if (i == 2)
		{
			RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
			RECT ss2 = { point[i][0] + 140, point[i][1] + 10, point[i][2] + 100, point[i][3] - 10 };
			s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));
		}
		else {
			RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
			RECT ss2 = { point[i][0] + 140, point[i][1] + 10, point[i][2] + 240, point[i][3] - 10 };
			s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));
		}

	}
	//RECT ss1 = { point[0][0], point[0][1], point[0][2], point[0][3] };
	//RECT ss2 = { point[0][0] + 140, point[0][1] + 10, point[0][2] + 240, point[0][3] - 10 };
	//s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[0].c_str()), pair<RECT, string>(ss2, "")));
	//ss1 = { point[1][0], point[1][1], point[1][2], point[1][3] };
	//ss2 = { point[1][0] + 140, point[1][1] + 10, point[1][2] + 70, point[1][3] - 10 };
	//s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss1, text[1].c_str()), pair<RECT, string>(ss2, "")));

	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
vector<string> admin_del_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back(" 注销卡号");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
vector<string> admin_query_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back(" 查询卡号");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
int change_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("修改账户姓名");
	text.push_back("修改账户密码");
	text.push_back("修改账户余额");
	text.push_back("退出");
	int point[7][4] = { { 320,100,480,150 } ,{ 320,200,480,250 }, { 320,300,480,350 }, { 320,400,480,450 } };
	for (int i = 0; i < 4; i++)
	{
		RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
		s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss, text[i].c_str())));
	}
	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	choice = click_interface(s, img, drawing_interface);
	return choice;
}
vector<string> admin_change_ui()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back(" 更正卡号");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
vector<string> admin_change_ui1()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("请输入新名称");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
vector<string> admin_change_ui2()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("请输入新密码");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;

}
vector<string> admin_change_ui3()
{
	int choice = 0;
	text_elmt s;
	vector<string> text;
	text.push_back("请设置余额");

	int point[4][4] = { { 150,200,280,260 } , { 220,180,380,230 }, { 220,260,380,310 }, { 220,340,380,390 } };
	int i = 0;
	RECT ss = { point[i][0], point[i][1], point[i][2], point[i][3] };
	RECT ss2 = { point[i][0] + 140, point[i][1] + 15, point[i][2] + 240, point[i][3] - 15 };
	s.push_back(pair<pair<RECT, string>, pair<RECT, string>>(pair<RECT, string>(ss, text[i].c_str()), pair<RECT, string>(ss2, "")));



	vector<string >img;
	img.push_back("atm.bmp");
	vector<string> return_data;

	//choice = click_interface(s, img, drawing_interface);
	return_data = input_interface(s, img, drawing_interface);
	return return_data;
}
//int main() {
//	//user_login_ui();
//
//	//cout << user_ui();
//	//vector<string> return_data = user_Withdrawals_ui();
//	//vector<string> return_data;		
//	//return_data.push_back("100");
//	//return_data.push_back("20000");
//	//int flag = 1;
//	//while (flag)
//
//	//{
//	//	int choice = user_ui();
//	//	switch (choice)
//	//	{
//	//	case 1:
//	//		return_data.push_back("100");
//	//		return_data.push_back("20000");
//	//		 query_ui(return_data);
//	//		break;
//	//	case 2:
//	//		user_Withdrawals_ui();
//	//		break;
//	//	case 3:
//	//		user_transfer_ui();
//	//			break;
//	//		case 4:
//	//			user_deposit_ui();
//	//				break;
//	//			case 5:
//	//				user_changepd_ui();
//	//				break;
//	//			case 6:
//	//			
//	//				  query_ui(return_data);
//	//				break;
//	//			case 7:
//	//				flag = 0;
//
//	//	break;
//	//	}
//	//}
//	//admin_open_ui();
//	//admin_del_ui();
//	admin_change_ui();
//	int x = 0,flag=1;
//	while (flag)
//	{
//		x=change_ui();
//		switch (x)
//		{
//		case 1:
//			admin_change_ui1();
//	 
//			break;
//		case 2:
//			admin_change_ui2();
//			break;
//		case 3:
//			admin_change_ui3();
//			break;
//		case 4:
//			flag = 0;
//			break;
//		}
//	}
//	
//	//admin_ui();
//	//for (auto const& ph : return_data)
//	//{
//	//	cout << ph.c_str() << endl;
//	//}
//										
//	return 0;
//}
