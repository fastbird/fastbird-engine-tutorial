#include "stdafx.h"
#include "CameraMan.h"
#include "FBInputManager/IInputInjector.h"
#include "FBRenderer/Camera.h"

using namespace fb;
CameraManPtr gCameraMan;
static float MouseSensitivity = 0.005f;

CameraMan::CameraMan(fb::CameraPtr engineCam)
	: mEngineCam(engineCam)
{

}
void CameraMan::ProcessInput(fb::IInputInjectorPtr injector) {
	Vec2I xy = injector->GetDpiDependentDeltaXY();
	if (xy.x != 0.f) {
		mPhi += xy.x * MouseSensitivity;
		mDirty = true;
	}

	if (xy.y != 0.f) {
		mTheta -= xy.y * MouseSensitivity;
		mDirty = true;
	}	
}

void CameraMan::Update(float dt) {
	if (!mEngineCam)
		return;
	
	if (!mDirty)	
		return;

	mDirty = false;

	mEngineCam->SetSphericalCoord(mOrigin, mRadius, mTheta, mPhi);
}