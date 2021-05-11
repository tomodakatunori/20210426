//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うのに必要

#include "keyboard.h"	//キーボードを使うのに必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"//ゲームタイトル
#define GAME_WIDTH 1280		//ゲーム画面の幅
#define GAME_HEIGTH 720		//ゲーム画面の高さ
#define GAME_COLOR 32		//ゲームの色域
#define GAME_ICON_ID 333	//ゲームのアイコン
#define GAME_WINDOWBER 0	//ウィンドウバーの種類

// プログラムは WinMain から始まります
//windousのプログラミング＝winAPIで動いている
//DxLibはDirectxというゲームプログラムを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを表示しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//ウィンドウの解像度
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);				//ウィンドウの大きさ
	SetBackgroundColor(255, 255, 255);					//背景の色
	SetWindowIconID(GAME_ICON_ID);						//ウィンドウのアイコン
	SetWindowStyleMode(GAME_WINDOWBER);					//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期の有効
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする	
	//こいつらはウィンドウの初期化の上に書くべき

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//ダブルバッファリングの有効化
	SetDrawScreen(DX_SCREEN_BACK);
	
	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGTH / 2;
	//円の半径
	int radius = 50;

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0){break;}	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0){break;} //画面の商経

		//キーボードの入力の更新
		AllKeyUpdate();
		
		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();	//ダブルバッファリングした画面を描画

	}

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;		//ソフトの終了 
}