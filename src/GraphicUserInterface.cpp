#include "GraphicUserInterface.h"

GraphicUserInterface::GraphicUserInterface( WindowManager* windowManager, int glMajVersion, int glMinVersion) : windowManager(windowManager){
    // Setup Dear ImGui context
	const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;

    ((ImGuiIO&)io).ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ((ImGuiIO&)io).ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ((ImGuiIO&)io).ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (((ImGuiIO&)io).ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL( this->windowManager->getWindow(), true);
    // ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Load Fonts
    {
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);
    }

}

GraphicUserInterface::~GraphicUserInterface(){
        // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GraphicUserInterface::draw(World* world, ViewCamera* mainCamera, InputProcessor* input, ImVec4* clearColor){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::Separator();

            ImGui::Checkbox("God Mode", input->getGodReference());

            ImGui::DragFloat("Sensitivity", mainCamera->getSensitivyReference(), 0.5f, 0.f, FLT_MAX);
            ImGui::DragFloat("Movement Speed", mainCamera->getSpeedReference(), 0.5f, 0.f, FLT_MAX);

            ImGui::Separator();

            ImGui::Checkbox("Show Skybox", world->getShowSkyboxReference());
            if( !*world->getShowSkyboxReference() )
                ImGui::ColorEdit3("clear color", (float*)clearColor); // Edit 3 floats representing a color

            ImGui::Separator();
            
            ImGui::Text("Floor");

            ImGui::PushButtonRepeat(true);
            ImGui::Text( "Rotate: " );
            ImGui::SameLine(0.f, 10.f);

            ImGui::Text( "x:" );
            ImGui::SameLine(0.f, 0.f);
            if (ImGui::Button("-##rotate x- floor")){
                world->getFloor()->rotate(glm::vec3(-5.f, 0.f, 0.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::SameLine();
            if (ImGui::Button("+##rotate x+ floor")){
                world->getFloor()->rotate(glm::vec3(5.f, 0.f, 0.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::SameLine(0.f, 10.f);

            ImGui::Text( "y:" );
            ImGui::SameLine(0.f, 0.f);
            if (ImGui::Button("-##rotate y- floor")){
                world->getFloor()->rotate(glm::vec3(0.f, -5.f, 0.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::SameLine();
            if (ImGui::Button("+##rotate y+ floor")){
                world->getFloor()->rotate(glm::vec3(0.f, 5.f, 0.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::SameLine(0.f, 10.f);

            ImGui::Text( "z:" );
            ImGui::SameLine(0.f, 0.f);
            if (ImGui::Button("-##rotate z- floor")){
                world->getFloor()->rotate(glm::vec3(0.f, 0.f, -5.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::SameLine();
            if (ImGui::Button("+##rotate z+ floor")){
                world->getFloor()->rotate(glm::vec3(0.f, 0.f, 5.f));
                world->getFloor()->calculateReflectionMatrix();
            }
            ImGui::PopButtonRepeat();

            if (drawDragVec3( world->getFloor()->getTranslationReference().get(), "Transalate"))
                world->getFloor()->calculateReflectionMatrix();

            glm::vec2 floorScale = glm::vec2( world->getFloor()->getScale().x, world->getFloor()->getScale().z );
            if (drawDragVec2( &floorScale, "Scale", 0.045f)){
                world->getFloor()->setScale( glm::vec3(floorScale.x, 1.f, floorScale.y) );
                world->getFloor()->calculateReflectionMatrix();
            }

            ImGui::Separator();

            if( ImGui::Button("Load OBJ") )                         // Load OBJ from GUI
                world->loadMesh(
                    noc_file_dialog_open(
                        NOC_FILE_DIALOG_OPEN,
                        "obj\0*.obj\0",
                        std::filesystem::current_path().string().c_str(),
                        NULL
                    )
                );

            drawMeshesTree( world->getMeshesVectorReference() );

            if( selectedModel < world->getMeshes().size() ){

                // float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
                ImGui::PushButtonRepeat(true);
                ImGui::Text( "Rotate: " );
                ImGui::SameLine(0.f, 10.f);

                ImGui::Text( "x:" );
                ImGui::SameLine(0.f, 0.f);
                if (ImGui::Button("-##rotate x-"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(-5.f, 0.f, 0.f));
                ImGui::SameLine();
                if (ImGui::Button("+##rotate x+"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(5.f, 0.f, 0.f));
                ImGui::SameLine(0.f, 10.f);

                ImGui::Text( "y:" );
                ImGui::SameLine(0.f, 0.f);
                if (ImGui::Button("-##rotate y-"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(0.f, -5.f, 0.f));
                ImGui::SameLine();
                if (ImGui::Button("+##rotate y+"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(0.f, 5.f, 0.f));
                ImGui::SameLine(0.f, 10.f);

                ImGui::Text( "z:" );
                ImGui::SameLine(0.f, 0.f);
                if (ImGui::Button("-##rotate z-"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(0.f, 0.f, -5.f));
                ImGui::SameLine();
                if (ImGui::Button("+##rotate z+"))
                    world->getMesh(selectedModel)->rotate(glm::vec3(0.f, 0.f, 5.f));
                ImGui::PopButtonRepeat();

                if (drawDragVec3( world->getMesh(selectedModel)->getTranslationReference().get(), "Transalate"))
                    world->getMesh(selectedModel)->calculateModelMatrix();

                if (drawDragVec3( world->getMesh(selectedModel)->getScaleReference().get(), "Scale", 0.045f))
                    world->getMesh(selectedModel)->calculateModelMatrix();
                
                if( ImGui::Button("Remove Mesh") ){
                    world->removeMesh( selectedModel );
                    selectedModel = -1;
                }

            }

            ImGui::Separator();

            ImGui::Text("Point Lights");

            if( ImGui::Button("Add Light") && world->getLights().size()<3 )
                world->addLight( new Light() );

            ImGui::ListBox(
                "##Point Lights List",
                &selectedLight,
                pointLightLabels,
                static_cast<int>(world->getLights().size())
            );


            if( (size_t)selectedLight < world->getLights().size() ){
                
                drawDragVec3(world->getLight(selectedLight)->getPositionReference().get(), "Position");
                drawDragVec3(world->getLight(selectedLight)->getAttentionReference(), "Attenuation", 0.f, +FLT_MAX, 0.025f, {"x²", "x", "c"});
                ImGui::Text( "Intensity: " );
                ImGui::SameLine();
                ImGui::DragFloat("##Intensity Drag", world->getLight(selectedLight)->getIntensityReference(), 0.025f, 0.f, 1.f);
                ImGui::ColorEdit3("Light color", glm::value_ptr( *world->getLight(selectedLight)->getColorReference() ));
                
                if( ImGui::Button("Remove Light") ){
                    world->removeLight( selectedLight );
                    selectedLight = -1;
                }

            }

        }

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
       
        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

}

void GraphicUserInterface::drawMeshesTree(std::vector<BasicMesh*> *tree){

    for (size_t node_index = 0; node_index < tree->size(); node_index++) {
        
        const auto& node = (*tree)[node_index];

        ImGuiTreeNodeFlags nodeFlags = treeFlags;

        if( selectedModel == node_index )
            nodeFlags |=  ImGuiTreeNodeFlags_Selected;

        if (ImGui::TreeNodeEx((void*)(intptr_t)node_index, nodeFlags, node[0].getName().c_str() )) {
            
            if (ImGui::IsItemClicked())
                selectedModel = node_index;

            for (size_t i = 0; i < node->getSubMeshesSize() ; i++) {

                // bool is_selected = this->selectedModel == node_index && this->selectedMesh == static_cast<int>(i - 1);
                bool is_selected = this->selectedModel == node_index;
                if (ImGui::Selectable( node->getSubMeshName(i).c_str() , is_selected, ImGuiSelectableFlags_Disabled)) {
                    // selectedModel = node_index;
                    selectedMesh = static_cast<int>(i - 1);
                }
                if (is_selected && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) {
                    // Do something when the Delete key is pressed on a selected item
                }
            }
            ImGui::TreePop();
        }
    }

}

bool GraphicUserInterface::drawDragVec3(glm::vec3 *v, std::string name, float v_min, float v_max, float v_speed, std::array<std::string, 3> labels){
    bool ans = false;

    ImGui::PushItemWidth(60);

    ImGui::Text( (name + ": ").c_str() );
    ImGui::SameLine();
    ans |= ImGui::DragFloat(("##" + name + "X").c_str(), &(v->x ), v_speed, v_min, v_max, (labels[0]+": %.2f").c_str(), ImGuiSliderFlags_None);
    
    ImGui::SameLine();
    ans |= ImGui::DragFloat(("##" + name + "Y").c_str(), &( v->y ), v_speed, v_min, v_max, (labels[1]+": %.2f").c_str(), ImGuiSliderFlags_None);
    
    ImGui::SameLine();
    ans |= ImGui::DragFloat(("##" + name + "Z").c_str(), &( v->z ), v_speed, v_min, v_max, (labels[2]+": %.2f").c_str(), ImGuiSliderFlags_None);
    
    ImGui::PushItemWidth(0);

    return ans;
}

bool GraphicUserInterface::drawDragVec2(glm::vec2 *v, std::string name, float v_min, float v_max, float v_speed, std::array<std::string, 2> labels){
    bool ans = false;

    ImGui::PushItemWidth(60);

    ImGui::Text( (name + ": ").c_str() );
    ImGui::SameLine();
    ans |= ImGui::DragFloat(("##" + name + "X").c_str(), &(v->x ), v_speed, v_min, v_max, (labels[0]+": %.2f").c_str(), ImGuiSliderFlags_None);
    
    ImGui::SameLine();
    ans |= ImGui::DragFloat(("##" + name + "Y").c_str(), &( v->y ), v_speed, v_min, v_max, (labels[1]+": %.2f").c_str(), ImGuiSliderFlags_None);
    
    ImGui::PushItemWidth(0);

    return ans;
}

void GraphicUserInterface::render(){

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    this->windowManager->getFramebufferSize( &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (((ImGuiIO&)io).ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
