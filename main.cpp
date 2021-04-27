//ヘッダーファイルの読み込み
#include "DxLib.h"//DxLibを使うのに必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"//ゲームタイトル
#define GAME_WIDTH 1280//ゲーム画面の幅
#define GAME_HEIGTH 720//ゲーム画面の高さ
#define GAME_COLOR 36//ゲームの色域
#define GAME_ICON_ID 333//ゲームのアイコン

// プログラムは WinMain から始まります
//windousのプログラミング＝winAPIで動いている
//DxLibはDirectxというゲームプログラムを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ChangeWindowMode(TRUE);//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);//ウィンドウの解像度
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);
	SetBackgroundColor(255, 122, 122);
	SetWindowIconID(GAME_ICON_ID);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//無限ループ
	while (1)
	{
		//なにかしらキーが押された
		if (CheckHitKeyAll() != 0)//0のとき、なにかキーを押された
		{
			break;//無限ループを抜ける	
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)//-1のとき、エラーやウィンドウを閉じられた
		{
			break;//無限ループを抜ける
		}
	}

	DxLib_End();// ＤＸライブラリ使用の終了処理

	return 0;// ソフトの終了 
}