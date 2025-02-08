#pragma once
#include "Coreminimal.h"
#include "AssetsManager.h"
#include "input.h"

constexpr float screenWidth = 1920.0f;
constexpr float screenHeight = 1080.0f;



class GameEngine;

class Main
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void ToggleFullScreen(void);

	void ChengeWindowSize(int Width, int height);



	Main();
	~Main();

	HWND GetWindowHangle(void);
	HINSTANCE* GetInstanceHandle(void);
	//void SetWindowType();
	int Run(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd);



	long GetMousePosX(void);
	long GetMousePosY(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);

	void Exit(void);

protected:
	void Awake(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
private:
	HWND hWnd;
	HINSTANCE* hInstance;
	int fps;
	POINT mousePos;	//マウスのキャッシュ位置

	float deltaTime;
	float beforeTime;

	LPRECT windowRect;

	int countFPS;

	GameEngine* pGameEngine;

};

