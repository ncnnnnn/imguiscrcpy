#pragma once
#include "imgui.h"

//https://www.cnblogs.com/timefiles/p/17657802.html
//ImGui界面优化：使用图标字体、隐藏主窗口标题栏
//https://www.cnblogs.com/timefiles/p/17632348.html
//使用C++界面框架ImGUI开发一个简单程序
//二次元攻城狮的博客

namespace App
{
	//主UI函数，放停靠空间的代码
	void RenderUI();
	//隐藏窗口的TabBar
	void HideTabBar();
	//导航页面
	void ShowTreeView();
	//内容页面
	void ShowMainView();
	/*
	* 内容页面0-5
	*/
	void ShowPageView0();
	void ShowPageView1();
	void ShowPageView2();
	void ShowPageView3();
	void ShowPageView4();

	int getDevices();
}

