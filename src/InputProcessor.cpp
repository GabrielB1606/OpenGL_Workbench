#include "InputProcessor.h"

InputProcessor::InputProcessor(WindowManager *window){
    this->window = window;
}

bool InputProcessor::process(ViewCamera *cam, float delta){

    bool ans = false;

    // poll events from the window
    input = this->window->pollEvents();

    // mouse input processing
	WindowManager::BTN_STATE rightMouse = window->getMouseButtonState( WindowManager::MOUSE_BUTTON::RIGHT );

	if( rightMouse == WindowManager::BTN_STATE::PRESS ){
		double currMouseX, currMouseY;
		window->getCursorPos(&currMouseX, &currMouseY);
		
		if( lastStateRightBtn == WindowManager::BTN_STATE::PRESS){
			cam->rotate( -delta*glm::vec3( currMouseX - mouseX, mouseY - currMouseY, 0.f ) );
            ans = true;
        }
		
		mouseX = currMouseX;
		mouseY = currMouseY;
	}

	lastStateRightBtn = rightMouse;

    // keyboard input processing
	if( input.find("ESCAPE") != input.end() )
		window->close();
	
	if( input.find("W") != input.end() ){
		
		if(!god)
			cam->walk(glm::vec3(0.f, 0.f, delta));
		else
			cam->move(glm::vec3(0.f, 0.f, delta));

        ans = true;
    }
	
	if( input.find("A") != input.end() ){

		if(!god)
			cam->walk(glm::vec3(delta, 0.f, 0.f));
		else
			cam->move(glm::vec3(delta, 0.f, 0.f));

        ans = true;
    }
	
	if( input.find("S") != input.end() ){

		if(!god)
			cam->walk(glm::vec3(0.f, 0.f, -delta));
		else
			cam->move(glm::vec3(0.f, 0.f, -delta));

        ans = true;
    }
	
	if( input.find("D") != input.end() ){

		if(!god)
			cam->walk(glm::vec3(-delta, 0.f, 0.f));
		else
			cam->move(glm::vec3(-delta, 0.f, 0.f));

        ans = true;
    }

    return ans;
}
