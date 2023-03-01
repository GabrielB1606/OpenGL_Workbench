#ifndef GUI_H
#define GUI_H

#include "headers.h"
class WindowManager;

class GraphicUserInterface{
private:
    WindowManager* windowManager = nullptr;
    
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiIO& io = ImGui::GetIO();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
    GraphicUserInterface( WindowManager* windowManager, int glMajVersion, int glMinVersion);
    ~GraphicUserInterface();

    void draw();
    void render();
};


#endif