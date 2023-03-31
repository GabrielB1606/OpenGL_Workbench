#ifndef GUI_H
#define GUI_H

#include "headers.h"
#include <noc_file_dialog.h>

#include "WindowManager.h"
#include "World.h"

class GraphicUserInterface{
private:
    WindowManager* windowManager = nullptr;
    
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    void* io;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
    GraphicUserInterface( WindowManager* windowManager, int glMajVersion, int glMinVersion);
    ~GraphicUserInterface();

    void draw(World* world, ImVec4* clearColor);
    void render();
};


#endif