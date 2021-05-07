//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うのに必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"//ゲームタイトル
#define GAME_WIDTH 1280	//ゲーム画面の幅
#define GAME_HEIGTH 720	//ゲーム画面の高さ
#define GAME_COLOR 32	//ゲームの色域
#define GAME_ICON_ID 333	//ゲームのアイコン
#define GAME_WINDOWBER 0 //ウィンドウバーの種類

// プログラムは WinMain から始まります
//windousのプログラミング＝winAPIで動いている
//DxLibはDirectxというゲームプログラムを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);	//Log.txtを表示しない
	ChangeWindowMode(TRUE);	//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);	//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//ウィンドウの解像度
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);	//ウィンドウの大きさ
	SetBackgroundColor(255, 255, 255);	//背景の色
	SetWindowIconID(GAME_ICON_ID);	//ウィンドウのアイコン
	SetWindowStyleMode(GAME_WINDOWBER);	//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);	//ディスプレイの垂直同期の有効
	SetAlwaysRunFlag(TRUE);	//ウィンドウをずっとアクティブにする	
	//こいつらはウィンドウの初期化の上に書くべき

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//四角の位置
	int X = GAME_WIDTH/2;	//横の中心
	int Y = GAME_HEIGTH/2;	//縦の中心

	//四角の大きさ
	int width = 32;//横
	int height = 32;//縦

	//円の半径
	int radius = 100;

	//速度
	int speed = 1;
	int XSpeed = 2;
	int YSpeed = 2;

	//ダブルバッファリングの有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1)
	{
		//なにかしらキーが押された
		if (CheckHitKeyAll() != 0)	//0のとき、なにかキーを押された
		{
			break;	//無限ループを抜ける	
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウを閉じられた
		{
			break;	//無限ループを抜ける
		}

		//画面を消去する
		if (ClearDrawScreen() != 0){break;}

		//四角を描写
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255,0,0),//色を取得
		//	TRUE//塗りつぶし
		//);

		//円を描画
		DrawCircle(
			X, Y, radius,
			GetColor(122, 122, 0),
			FALSE , 5
		);

		X += XSpeed;
		//斜め上に動かす
		Y += YSpeed;

		//端に来たら移動する向きを反転させる
		if ( X - radius < 0 || X + radius > GAME_WIDTH)	//画面の横から出たとき
		{
			//-1なら+1に、+1ならば-1にする
			XSpeed = -XSpeed;	//移動の向きを反転させる
			//壁に当たったら速くなる
			if (XSpeed > 0) { XSpeed += 2; }
			else if (XSpeed < 0) { XSpeed -= 2; }
		}

		if (Y - radius< 0 || Y + radius > GAME_HEIGTH)	//画面の縦から出たとき
		{
			//-1なら+1に、+1ならば-1にする
			YSpeed = -YSpeed;	//移動の向きを反転させる
			//壁に当たったら速くなる
			if (YSpeed > 0) { YSpeed += 2; }
			else if (YSpeed < 0) { YSpeed -= 2; }
		}

		ScreenFlip();	//ダブルバッファリングした画面を描画

	}

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;	//ソフトの終了 
}