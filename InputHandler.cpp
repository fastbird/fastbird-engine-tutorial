#include "stdafx.h"
#include "InputHandler.h"
#include "CameraMan.h"
using namespace fb;

InputHandlerPtr gInputHandler;
void InputHandler::ConsumeInput(fb::IInputInjectorPtr injector) {
	if (gCameraMan) {
		gCameraMan->ProcessInput(injector);
	}
}