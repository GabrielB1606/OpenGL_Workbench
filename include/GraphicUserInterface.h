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
    static const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;


    size_t selectedModel = -1;
    size_t selectedMesh = -1;
    size_t selectedLight = -1;

   glm::vec3 vec3f = glm::vec3(0.f, 1.f, 2.f);

public:
    GraphicUserInterface( WindowManager* windowManager, int glMajVersion, int glMinVersion);
    ~GraphicUserInterface();

    void draw(World* world, ImVec4* clearColor);
    void drawMeshesTree(std::vector<BasicMesh*> *tree);
    void drawDragVec3(glm::vec3 *v, std::string name, float v_min = -FLT_MAX, float v_max = +FLT_MAX, float v_speed = 0.05f);
    void render();
};


#endif