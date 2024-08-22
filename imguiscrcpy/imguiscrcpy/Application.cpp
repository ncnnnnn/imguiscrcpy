#include "Application.h"
#include <iostream>
#include <string>
#include <imgui_internal.h>

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

        /**添加自己的窗口**/
        ShowTreeView();
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
        ImGui::End();
    }
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
