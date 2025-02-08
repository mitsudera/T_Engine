#include "Main.h"
#include "GameEngine.h"
#include "GUI.h"

#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"Game"				// ウインドウのキャプション名


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Main* main = new Main;


	int result;
	result = main->Run(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	delete main;
	return result;
}

Main::Main()
{
}

Main::~Main()
{
}

HWND Main::GetWindowHangle(void)
{
	return this->hWnd;

}

HINSTANCE* Main::GetInstanceHandle(void)
{
	return this->hInstance;

}

int Main::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// インスタンスハンドルのポインターを保持
	this->hInstance = &hInstance;

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;


	// ウィンドウクラスの初期化
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		Main::WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};


	MSG			msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		(int)screenWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		(int)screenHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);



	// DirectXの初期化(ウィンドウを作成してから行う)
	Awake();
	//if (FAILED(Init(hInstance, hWnd, mode)))
	//{
	//	return -1;
	//}

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;


	// 経過時間
	this->deltaTime = 0;
	this->beforeTime = (float)timeGetTime();

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000.0f)	// 1秒ごとに実行
			{
				countFPS = dwFrameCount;
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / 60))	// 1/60秒ごとに実行
			{

				this->deltaTime = (float)(dwCurrentTime - dwExecLastTime);

				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

				Update();			// 更新処理
				Draw();				// 描画処理
				dwFrameCount++;
			}
		}
	}
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();
	return (int)msg.wParam;
}

float Main::GetDeltaTime(void)
{
	return this->deltaTime / 1000.0f;
}

void Main::Exit(void)
{
	// Post a quit message to end the message loop
	PostQuitMessage(0);
}



void Main::Awake(void)
{
	//マウス位置の初期化
	GetCursorPos(&this->mousePos);

	srand((unsigned)time(NULL));
	pGameEngine = new GameEngine(this);

	pGameEngine->Awake();
	

}

void Main::Update(void)
{
	pGameEngine->Update();
}

void Main::Draw(void)
{
	pGameEngine->Draw();
}

void Main::Uninit(void)
{
	pGameEngine->Uninit();
}

LRESULT Main::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (GUI::GuiWndPro(hWnd, message, wParam, lParam) == TRUE)
	{
		return 1;
	}


	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


// フルスクリーン切り替え
void Main::ToggleFullScreen(void) 
{
	static WINDOWPLACEMENT wpPrev = { sizeof(wpPrev) };
	LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (lStyle & WS_OVERLAPPEDWINDOW) {
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(hWnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLong(hWnd, GWL_STYLE, lStyle &~ WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else {
		SetWindowLong(hWnd, GWL_STYLE, lStyle| WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}


void Main::ChengeWindowSize(int width,int height) 
{

	// 現在のウィンドウスタイルを取得
	LONG style = GetWindowLong(hWnd, GWL_STYLE);

	// ウィンドウの枠の幅と高さを考慮して、クライアント領域のサイズを設定
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, style, FALSE);

	// ウィンドウのサイズと位置を変更
	SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);

}



