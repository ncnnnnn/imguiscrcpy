#include "Application.h"
#include <iostream>
#include <string>
#include <imgui_internal.h> 
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include <iostream>
#include "windows.h"
#include <deque>
#include <string>
#include <thread>


#include "adb_api.h"
namespace App
{


	void RenderUI()
	{
		//p_open不需要，改成nullptr
		bool* p_open = nullptr;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		//window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			//不关闭Docking
			//ShowDockingDisabledMessage();
		}

#if 0
		if (ImGui::BeginMenuBar())
		{
			//菜单做一些汉化
			if (ImGui::BeginMenu("选项（Options）"))
			{
				ImGui::MenuItem("全屏（Fullscreen）", NULL, &opt_fullscreen);
				ImGui::MenuItem("填充（Padding）", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("标志：不分割（Flag: NoSplit）", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("标志：不调整大小（Flag: NoResize）", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("标志：不停靠在中心节点（Flag: NoDockingInCentralNode）", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("标志：自动隐藏选项卡栏（Flag: AutoHideTabBar）", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("标志：中心节点筛选器（Flag: PassthruCentralNode）", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();
				//不关闭菜单
			   /* if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;*/
				ImGui::EndMenu();
			}
			//增加主题切换
			if (ImGui::BeginMenu("主题（Other）"))
			{
				if (ImGui::MenuItem("暗黑（Dark）")) { ImGui::StyleColorsDark(); }
				if (ImGui::MenuItem("明亮（Light）")) { ImGui::StyleColorsLight(); }
				if (ImGui::MenuItem("经典（Classic）")) { ImGui::StyleColorsClassic(); }

				ImGui::EndMenu();
			}
			//HelpMarker 不需要
			ImGui::EndMenuBar();
		}
#endif

		/**添加自己的窗口**/
		//ShowTreeView();

		ShowMainView();

		ImGui::End();

	}


	void HideTabBar()
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
	}

	//一级索引
	int FirstIdx = 0;
	//二级索引
	int SecondIdx = 0;
	//导航页面
	void ShowTreeView()
	{
		HideTabBar();
		ImGui::Begin("TreeView");
		if (ImGui::TreeNode("功能选项"))
		{
			for (int i = 0; i < 5; i++)
			{
				if (i == 0)
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);

				if (ImGui::TreeNode((void*)(intptr_t)i, "功能 %d", i))
				{
					ImGui::Text("图标");
					ImGui::SameLine();
					if (ImGui::SmallButton("按钮1")) { FirstIdx = i; SecondIdx = 0; }
					ImGui::Text("图标");
					ImGui::SameLine();
					if (ImGui::SmallButton("按钮2")) { FirstIdx = i; SecondIdx = 1; }
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("其它选项"))
		{
			ImGui::Text("图标");
			ImGui::SameLine();
			if (ImGui::SmallButton("按钮")) {}
			ImGui::TreePop();
		}
		ImGui::End();
	}
	//内容页面
	void ShowMainView()
	{
		HideTabBar();
		// 清除之前的内容
		ImGui::Begin("MainView");

		ImGui::Text("设备列表：");

		if (0)
		{
			{
				ImGui::Text("请拖放文件到这里");
				//一个表格示例
				static ImGuiTableFlags flags =
					ImGuiTableFlags_SizingFixedFit |
					ImGuiTableFlags_RowBg |
					ImGuiTableFlags_Borders |
					ImGuiTableFlags_Resizable |
					ImGuiTableFlags_Reorderable |
					ImGuiTableFlags_Hideable;
				if (ImGui::BeginTable("table0", 3, flags))
				{
					ImGui::TableSetupColumn("算法", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("编码", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("输出", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					ImGui::EndTable();
				}
			}
		}

		if (0)
		{
			switch (FirstIdx)
			{
			case 0:
				ShowPageView0();
				break;
			case 1:
				ShowPageView1();
				break;
			case 2:
				ShowPageView2();
				break;
			case 3:
				ShowPageView3();
				break;
			case 4:
				ShowPageView4();
				break;
			default:
				break;
			}
		}

		ImGui::End();
	}

	const GUID kAdbInterfaceId = ANDROID_USB_CLASS_ID;
	int interface_count = 0;
#define SPINT_ACTIVE  0x00000001
#define SPINT_DEFAULT 0x00000002
#define SPINT_REMOVED 0x00000004
	int adbGetDev() {

		// Enumerate interfaces
		ADBAPIHANDLE enum_handle =
			AdbEnumInterfaces(kAdbInterfaceId, true, true, true);
		if (NULL == enum_handle) {
			printf("\nEnum interfaces failure:");
			printf("\nUnable to enumerate ADB interfaces: %u", GetLastError());
			return false;
		}

		// Unite interface info structure and buffer big enough to contain the
		// largest structure.
		union {
			AdbInterfaceInfo interface_info;
			char buf[4096];
		};
		unsigned long buf_size = sizeof(buf);

		// Enumerate (and count) interfaces, printing information for each found
		// interface.
		interface_count = 0;
		while (AdbNextInterface(enum_handle, &interface_info, &buf_size)) {
			interface_count++;
			printf("\nFound interface %ws:", interface_info.device_name);
			if (interface_info.flags & SPINT_ACTIVE)
				printf(" ACTIVE");
			if (interface_info.flags & SPINT_DEFAULT)
				printf(" DEFAULT");
			if (interface_info.flags & SPINT_REMOVED)
				printf(" REMOVED");

			buf_size = sizeof(buf);
		}

		bool ret = true;
		if (GetLastError() != ERROR_NO_MORE_ITEMS) {
			printf("\n--- AdbNextInterface failure %u", GetLastError());
			ret = false;
		}

		if (!AdbCloseHandle(enum_handle)) {
			printf("\n--- AdbCloseHandle failure %u", GetLastError());
			ret = false;
		}


		return 0;
	}



	std::wstring StringToWString(const std::string& str);
	std::string WStringToString(const std::wstring& wstr);
	std::string ExeCmd(std::wstring CommandLine);
	//https://www.cnblogs.com/YangARTuan/p/13929617.html
	int getDevices()
	{
		for (int i = 0; i < 10; i++)
		{
			//std::string searchDevices = "adb devices";
			//std::string out_content = ExeCmd(StringToWString(searchDevices));
			//std::cout << out_content.c_str() << std::endl;
			//Sleep(50);

			FILE* fp = NULL;
			char debug_buf[1024] = { 0 };
			if ((fp = _popen("adb devices", "r")) != NULL)
			{
				//::ShowWindow(::GetConsoleWindow(), SW_HIDE);//hide console window
				while (fgets(debug_buf, 255, fp) != NULL)
				{

					if (std::string(debug_buf) != "\n")
					{
						printf("read buffer = %s\n", debug_buf);
					}
					Sleep(50);

				}
				_pclose(fp);
				fp = NULL;
			}
		}
		std::cout << "Hello World!\n";
		return 0;
	}


	std::string ExeCmd(std::wstring CommandLine)
	{
		STARTUPINFO si;
		SECURITY_ATTRIBUTES sa;
		PROCESS_INFORMATION pi;
		HANDLE /*g_hChildStd_IN_Rd,*/ g_hChildStd_OUT_Wr, g_hChildStd_OUT_Rd/*, g_hChildStd_IN_Wr*/;  //pipe handles
		char buf[1024];           //i/o buffer

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		//if (CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &sa, 0))   //create stdin pipe
		//{
		if (CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0))  //create stdout pipe
		{

			//set startupinfo for the spawned process
			/*The dwFlags member tells CreateProcess how to make the process.
			STARTF_USESTDHANDLES: validates the hStd* members.
			STARTF_USESHOWWINDOW: validates the wShowWindow member*/
			GetStartupInfo(&si);

			si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			//set the new handles for the child process
			si.hStdOutput = g_hChildStd_OUT_Wr;
			si.hStdError = g_hChildStd_OUT_Wr;
			//si.hStdInput = g_hChildStd_IN_Rd;

			//spawn the child process
			if (CreateProcess(NULL, (LPWSTR)CommandLine.c_str(), NULL, NULL, TRUE, NULL/*CREATE_NEW_CONSOLE*/,
				NULL, NULL, &si, &pi))
			{
				unsigned long bread;   //bytes read
				unsigned long avail;   //bytes available
				memset(buf, 0, sizeof(buf));

				for (;;)
				{
					PeekNamedPipe(g_hChildStd_OUT_Rd, buf, 1023, &bread, &avail, NULL);
					//check to see if there is any data to read from stdout
					if (bread != 0)
					{
						if (ReadFile(g_hChildStd_OUT_Rd, buf, 1023, &bread, NULL))
						{
							break;
						}

					}
				}

				//clean up all handles
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				//CloseHandle(g_hChildStd_IN_Rd);
				CloseHandle(g_hChildStd_OUT_Wr);
				CloseHandle(g_hChildStd_OUT_Rd);
				//CloseHandle(g_hChildStd_IN_Wr);
				return std::string(buf);
			}
			else
			{
				//CloseHandle(g_hChildStd_IN_Rd);
				CloseHandle(g_hChildStd_OUT_Wr);
				CloseHandle(g_hChildStd_OUT_Rd);
				//CloseHandle(g_hChildStd_IN_Wr);
				return std::string("create child process fail, error code: 2");
			}
		}
		else
		{
			//CloseHandle(g_hChildStd_IN_Rd);
			//CloseHandle(g_hChildStd_IN_Wr);
			return std::string("create stdout pipe fail, error code: 1");
		}
		//}
		//return std::string("create stdin pipe fail, error code: 0");
	}


	std::wstring StringToWString(const std::string& str)
	{
		int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* wide = new wchar_t[num];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
		std::wstring w_str(wide);
		delete[] wide;
		return w_str;
	}

	std::string WStringToString(const std::wstring& wstr)
	{
		std::string str;
		int nLen = (int)wstr.length();
		str.resize(nLen, ' ');
		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
		if (nResult == 0)
		{
			return "";
		}
		return str;
	}

	// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
	// Debug program: F5 or Debug > Start Debugging menu

	// Tips for Getting Started: 
	//   1. Use the Solution Explorer window to add/manage files
	//   2. Use the Team Explorer window to connect to source control
	//   3. Use the Output window to see build output and other messages
	//   4. Use the Error List window to view errors
	//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
	//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
	void ShowPageView0()
	{
		ImGui::Text("功能%d -> 按钮%d -> 页面0", FirstIdx, SecondIdx);
		//一个表格示例
		static ImGuiTableFlags flags =
			ImGuiTableFlags_SizingFixedFit |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_Borders |
			ImGuiTableFlags_Resizable |
			ImGuiTableFlags_Reorderable |
			ImGuiTableFlags_Hideable;
		if (ImGui::BeginTable("table0", 3, flags))
		{
			ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();
			for (int row = 0; row < 5; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 3; column++)
				{
					ImGui::TableSetColumnIndex(column);
					ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
				}
			}
			ImGui::EndTable();
		}
	}
	void ShowPageView1()
	{
		ImGui::Text("功能%d -> 按钮%d -> 页面1", FirstIdx, SecondIdx);
		//选项卡示例
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Avocado"))
			{
				ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Broccoli"))
			{
				ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Cucumber"))
			{
				ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	void ShowPageView2() { ImGui::Text("功能%d -> 按钮%d -> 页面2", FirstIdx, SecondIdx); }
	void ShowPageView3() { ImGui::Text("功能%d -> 按钮%d -> 页面3", FirstIdx, SecondIdx); }
	void ShowPageView4() { ImGui::Text("功能%d -> 按钮%d -> 页面4", FirstIdx, SecondIdx); }
}
