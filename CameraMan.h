#pragma once
namespace fb {
	FB_DECLARE_SMART_PTR(IInputInjector);
	FB_DECLARE_SMART_PTR(Camera);
}
FB_DECLARE_SMART_PTR(CameraMan);
class CameraMan {
	float mRadius = 10.f;
	float mTheta = fb::HALF_PI;
	float mPhi = 0.f;
	fb::Vec3 mOrigin = fb::Vec3::ZERO;
	bool mDirty = true;
	fb::CameraPtr mEngineCam;

public:
	CameraMan(fb::CameraPtr engineCam);
	void ProcessInput(fb::IInputInjectorPtr injector);
	void Update(float dt);
};

extern CameraManPtr gCameraMan;