#pragma once
#include "FBInputManager/IInputConsumer.h"

FB_DECLARE_SMART_PTR(InputHandler);
class InputHandler : public fb::IInputConsumer {
public:
	void ConsumeInput(fb::IInputInjectorPtr injector) OVERRIDE;
};

extern InputHandlerPtr gInputHandler;