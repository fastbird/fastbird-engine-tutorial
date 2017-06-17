// mygame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mygame.h"
#include "FBEngineFacade/EngineFacade.h"
#include "FBEngineFacade/MeshFacade.h"
#include "FBTimer/Timer.h"
#include "FBRenderer/Camera.h"

#include "InputHandler.h"
#include "CameraMan.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace fb;

EngineFacadePtr gEngine;
HWindowId gWindowId;
MeshFacadePtr gCube;

void SetupCube() {
	gCube = MeshFacade::Create();
	gCube->CreateEmptyMeshObject();
	gCube->StartModification();
	std::vector<Vec3> positions = {
		// x face
		Vec3(1, -1, -1),
		Vec3(1, -1, 1),
		Vec3(1, 1, -1),
		Vec3(1, 1, -1),
		Vec3(1, -1, 1),
		Vec3(1, 1, 1),

		// -x face
		Vec3(-1, 1, -1),
		Vec3(-1, 1, 1),
		Vec3(-1, -1, -1),
		Vec3(-1, -1, -1),
		Vec3(-1, 1, 1),
		Vec3(-1, -1, 1),

		// y face
		Vec3(1, 1, -1),
		Vec3(1, 1, 1),
		Vec3(-1, 1, -1),

		Vec3(-1, 1, -1),
		Vec3(1, 1, 1),
		Vec3(-1, 1, 1),

		// -y face
		Vec3(-1, -1, -1),
		Vec3(-1, -1, 1),
		Vec3(1, -1, -1),
		Vec3(1, -1, -1),
		Vec3(-1, -1, 1),
		Vec3(1, -1, 1),

		// z face
		Vec3(-1, -1, 1),
		Vec3(-1, 1, 1),
		Vec3(1, -1, 1),
		Vec3(1, -1, 1),
		Vec3(-1, 1, 1),
		Vec3(1, 1, 1),

		// -z face
		Vec3(-1, 1, -1),
		Vec3(-1, -1, -1),
		Vec3(1, 1, -1),
		Vec3(1, 1, -1),
		Vec3(-1, -1, -1),
		Vec3(1, -1, -1)
	};
	gCube->SetPositions(0, &positions[0], positions.size());
	gCube->EndModification(false);
	gCube->AttachToCurrentScene();
}

void InitializeEngine() {
	gEngine = EngineFacade::Create();
	gWindowId = gEngine->CreateEngineWindow(-1, -1, 1280, 800, "mygame", "mygame powered by FBEngine", 0, 
		WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, 0, WndProc);
	gEngine->InitRenderer("FBRendererD3D11");
	gEngine->InitCanvas(gWindowId, 0, 0);

	gInputHandler = std::make_shared<InputHandler>();
	gEngine->RegisterInputConsumer(gInputHandler, IInputConsumer::Priority::Priority55_INTERACTION);
	//gCameraMan = std::make_shared<CameraMan>(gEngine->GetMainCamera());
	gEngine->GetMainCamera()->SetEnalbeInput(true);
}

void UpdateFrame() {	
	gpTimer->Tick();
	auto dt = gpTimer->GetDeltaTime();	
	gEngine->UpdateFileMonitor();
	gEngine->UpdateInput();

	if (gCameraMan)
		gCameraMan->Update(dt);

	gEngine->Update(dt);

	gEngine->EndInput();

	gEngine->Render();
}
std::vector<fb::MeshFacadePtr> gObjects;
void LoadObjects() {
	auto mesh = MeshFacade::Create();
	mesh = mesh->LoadMeshObject("data/objects/turtleship.dae");
	if (mesh) {
		gObjects.push_back(mesh);
		mesh->AttachToCurrentScene();
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

		InitializeEngine();
		LoadObjects();

		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				UpdateFrame();
			}
		}
		gEngine->PrepareQuit();

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;    
    }
		return gEngine->WinProc((HWindow)hWnd, message, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
