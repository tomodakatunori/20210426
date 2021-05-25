//ヘッダファイルの読み込み
#include "game.h"		//ゲームの基本
#include "keyboard.h"	//キーボードを使うのに必要
#include"FPS.h"			//FPSの処理

//構造体の定義

//キャラクタの構造体
struct CHARACTOR
{
	int handle = -1;	//画像のハンドル
	char path[255];		//画像の場所（パス）
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ
	int speed = 5;		//移動速度

	RECT coll;				//当たり判定の領域(四角)
	BOOL IsDraw = FALSE;	//画像が描画できるか
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームシーン
GAME_SCENE OldGameScene;	//前回のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR Goal;

//画面の切り替え
BOOL IsfadeOut = FALSE;	//フェードアウト
BOOL IsfadeIn = FALSE;	//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;					//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレームに変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウント
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードインのカウント
int fadeInCntMax = 0;				//フェードインのカウンタMAX

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);			//当たり判定

// プログラムは WinMain から始まります
//windousのプログラミング＝winAPIで動いている
//DxLibはDirectxというゲームプログラムを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
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

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//ダブルバッファリングの有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンはタイトル画面だから
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//プレイヤーの画像を読み込み
	strcpyDx(player.path, ".\\image\\player.png");	//パスのコピー
	player.handle = LoadGraph(player.path);	//画像の読み込み
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			player.path,			//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
			);
		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス

	//プレイヤーを初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;	//中央寄せ
	player.y = GAME_HEIGTH / 2 - player.height / 2;	//中央寄せ
	player.speed = 500;		//移動速度
	player.IsDraw = TRUE;	//描画できる

	//ゴールの画像を読み込み
	strcpyDx(Goal.path, ".\\image\\Goal.jpg");	//パスのコピー
	Goal.handle = LoadGraph(Goal.path);	//画像の読み込み
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			Goal.path,				//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);
		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//当たり判定を更新する
	CollUpdate(&Goal);	//プレイヤーの当たり判定のアドレス

	//ゴールを初期化
	Goal.x = GAME_WIDTH - Goal.width;	
	Goal.y = 0;
	Goal.speed = 500;		//移動速度
	Goal.IsDraw = TRUE;		//描画できる

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面の消去

		//キーボードの入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシ－ンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとの処理
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();	//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWeit();

		ScreenFlip();	//ダブルバッファリングした画面を描画

	}

	//終わる時の処理
	DeleteGraph(player.handle);	//画像をメモリから削除
	DeleteGraph(Goal.handle);	//画像をメモリから削除

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;		//ソフトの終了 
}

/// <summary>
/// シーンを切り替える
/// </summary>
/// <param name="">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//シーンに切り替える
	IsfadeIn = FALSE;				//フェードインしない
	IsfadeOut = TRUE;				//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}



	//当たり判定を更新する
	CollUpdatePlayer(&player);

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	if (player.IsDraw == TRUE)
	{
		//画像を描写
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描写
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255,0,0),FALSE);
		}

	}

	//ゴールを描画
	if (Goal.IsDraw == TRUE)
	{
		//画像を描写
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描写
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}

	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}


	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsfadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeInCnt = fadeInCntInit;	//カウント初期化
			IsfadeIn = FALSE;			//フェードイン処理完了
		}
	}

	//フェードアウト
	if (IsfadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeOutCnt = fadeOutCntInit;	//カウント初期化
			IsfadeOut = FALSE;			//フェードイン処理完了
		}
	}

	//切り替え処理終了？
	if (IsfadeIn == FALSE && IsfadeOut == FALSE)
	{
		//フェードインしてない、フェードアウトもしていない時
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	case GAME_SCENE_CHANGE:
		break;
	default:
		break;
	}

	//フェードイン
	if (IsfadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsfadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描写
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGTH,GetColor(0,0,0),TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;					
	chara->coll.top = chara->y;						
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}