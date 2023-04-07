#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include "headers.h"

#include "ViewCamera.h"
#include "WindowManager.h"

class InputProcessor{
private:

    WindowManager* window;

    bool god = false;

    WindowManager::BTN_STATE lastStateRightBtn = WindowManager::BTN_STATE::RELEASE;
    double mouseX, mouseY;
    std::unordered_set<std::string> input;

public:
    InputProcessor(WindowManager* window);
    virtual ~InputProcessor(){}

    bool process(ViewCamera *cam, float delta);

    bool* getGodReference();
};

#endif