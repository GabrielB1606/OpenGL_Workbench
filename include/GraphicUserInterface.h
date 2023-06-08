#ifndef GUI_H
#define GUI_H

#include "headers.h"
#include <noc_file_dialog.h>

#include "WindowManager.h"
#include "World.h"
#include "InputProcessor.h"
#include <stb_particle_system/stb_particle_system.h>
#include "Texture.h"

class GraphicUserInterface{
private:
    WindowManager* windowManager = nullptr;
    
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    void* io;

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    static const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;


    size_t selectedModel = -1;
    size_t selectedMesh = -1;
    int selectedLight = -1;

   glm::vec3 vec3f = glm::vec3(0.f, 1.f, 2.f);

   const char* pointLightLabels[3]= {"Point Light #1", "Point Light #2" , "Point Light #3"};

public:
    GraphicUserInterface( WindowManager* windowManager, int glMajVersion, int glMinVersion);
    ~GraphicUserInterface();

    void draw(World* world, ViewCamera* mainCamera, InputProcessor* input, ImVec4* clearColor);
    void drawParticleSystem(ParticleSystem* ps);
    void drawMeshesTree(std::vector<BasicMesh*> *tree);
    bool drawDragVec3(glm::vec3 *v, std::string name, float v_min = -FLT_MAX, float v_max = +FLT_MAX, float v_speed = 0.05f, std::array<std::string, 3> labels = {"x", "y", "z"});
    bool drawDragVec2(glm::vec2 *v, std::string name, float v_min = -FLT_MAX, float v_max = +FLT_MAX, float v_speed = 0.05f, std::array<std::string, 2> labels = {"x", "y"});
    void render();
};


#endif