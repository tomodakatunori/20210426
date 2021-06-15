//ヘッダファイルの読み込み
#include "game.h"		//ゲームの基本
#include "keyboard.h"	//キーボードを使うのに必要
#include"FPS.h"			//FPSの処理

//構造体の定義

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル
	char path[255];		//画像の場所（パス）
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	RECT coll;				//当たり判定の領域(四角)
	BOOL IsDraw = FALSE;	//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTOR
{
    IMAGE img;
	int speed = 5;		//移動速度
	RECT coll;				//当たり判定の領域(四角)
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;		//X位置
	int y;		//Y位置
	int width;	//幅
	int height;	//高さ

	int Volume = 255;	//ボリューム　２５５～０
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;		//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//音楽のボリューム（０～２５５）
	int playType = -1;
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームシーン
GAME_SCENE OldGameScene;	//前回のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//プレイ背景の動画
MOVIE playMovie;

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR Goal;

//ロゴを読み込む
IMAGE Titlelogo;
IMAGE Titleenter;
IMAGE EndClear;

//音楽
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//効果音
AUDIO playerSE;

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

BOOL onCollRect(RECT a, RECT b);	//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームのデータの読み込み
VOID GameInit(VOID);	//ゲームのデータの初期化

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

	//ゲームの読み込み.
	if (!GameLoad())
	{
		//データの読み込みに失敗した時
		DxLib_End;	//Dxlib終了
		return -1;	//異状終了
	}

	//ゲームの初期化
	GameInit();

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
	DeleteGraph(playMovie.handle);		//動画をメモリから削除

	DeleteGraph(player.img.handle);		//画像をメモリから削除
	DeleteGraph(Goal.img.handle);		//画像をメモリから削除

	DeleteGraph(Titlelogo.handle);		//ロゴをメモリから削除
	DeleteGraph(Titleenter.handle);		//ロゴをメモリから削除
	DeleteGraph(EndClear.handle);		//ロゴをメモリから削除

	DeleteSoundMem(TitleBGM.handle);	//タイトルBGMをメモリから削除
	DeleteSoundMem(PlayBGM.handle);		//プレイBGMをメモリから削除
	DeleteSoundMem(EndBGM.handle);		//エンドBGMをメモリから削除

	DeleteSoundMem(playerSE.handle);		//SEをメモリから削除

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;		//ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE、読み込めなかったならFARSE</returns>
BOOL GameLoad()
{
	//プレイ動画の背景を読み込む
	strcpyDx(playMovie.path, ".\\movie\\playmovie.mp4");	//パスのコピー
	playMovie.handle = LoadGraph(playMovie.path);			//画像の読み込み

	//動画が読み込めなかったとき
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			playMovie.path,			//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//動画の幅と高さを取得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//プレイヤーの画像を読み込み
	strcpyDx(player.img.path, ".\\image\\player.png");	//パスのコピー
	player.img.handle = LoadGraph(player.img.path);	//画像の読み込み
	if (player.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			player.img.path,			//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(player.img.handle, &player.img.width, &player.img.height);

	//動画のボリューム
	playMovie.Volume = 255;

	//ゴールの画像を読み込み
	strcpyDx(Goal.img.path, ".\\image\\Goal.png");	//パスのコピー
	Goal.img.handle = LoadGraph(Goal.img.path);	//画像の読み込み
	if (Goal.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			Goal.img.path,				//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(Goal.img.handle, &Goal.img.width, &Goal.img.height);

	//音楽の読み込む
	strcatDx(TitleBGM.path, ".\\Audio\\魔王魂  ピアノ30.mp3");
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);
	//読み込めなかった時のエラー
	if (TitleBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			TitleBGM.path,				//メッセージ本文
			"音楽読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);
		
		return FALSE;	//読み込み失敗
	}

	TitleBGM.playType = DX_PLAYTYPE_LOOP;
	TitleBGM.Volume = 255;

	//音楽の読み込む
	strcatDx(PlayBGM.path, ".\\Audio\\魔王魂  民族31.mp3");
	PlayBGM.handle = LoadSoundMem(PlayBGM.path);
	//読み込めなかった時のエラー
	if (PlayBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			PlayBGM.path,				//メッセージ本文
			"音楽読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	PlayBGM.playType = DX_PLAYTYPE_LOOP;
	PlayBGM.Volume = 255;

	//音楽の読み込む
	strcatDx(EndBGM.path, ".\\Audio\\魔王魂  ファンタジー14.mp3");
	EndBGM.handle = LoadSoundMem(EndBGM.path);
	//読み込めなかった時のエラー
	if (EndBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			EndBGM.path,				//メッセージ本文
			"音楽読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	EndBGM.playType = DX_PLAYTYPE_LOOP;
	EndBGM.Volume = 255;

	//音楽の読み込む
	strcatDx(playerSE.path, ".\\Audio\\魔王魂  馬01.mp3");
	playerSE.handle = LoadSoundMem(playerSE.path);
	//読み込めなかった時のエラー
	if (playerSE.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			playerSE.path,				//メッセージ本文
			"音楽読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	playerSE.playType = DX_PLAYTYPE_BACK;
	playerSE.Volume = 255;

	//タイトルロゴの読み込み
	strcpyDx(Titlelogo.path, ".\\image\\タイトルロゴ.png");	//パスのコピー
	Titlelogo.handle = LoadGraph(Titlelogo.path);	//画像の読み込み
	if (Titlelogo.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			Titlelogo.path,			//メッセージ本文
			"画像読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//エンターロゴの読み込み
	strcpyDx(Titleenter.path, ".\\image\pushenter.png");	//パスのコピー
	Titleenter.handle = LoadGraph(Titleenter.path);	//画像の読み込み
	if (Titleenter.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			Titleenter.path,				//メッセージ本文
			"画像読み込みエラー",			//メッセージタイトル
			MB_OK							//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//エンドロゴの読み込み
	strcpyDx(EndClear.path, ".\\image\end.png");	//パスのコピー
	EndClear.handle = LoadGraph(EndClear.path);	//画像の読み込み
	if (EndClear.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			EndClear.path,					//メッセージ本文
			"画像読み込みエラー",			//メッセージタイトル
			MB_OK							//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	return TRUE;	//すべて読み込めた
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;	//中央寄せ
	player.img.y = GAME_HEIGTH / 2 - player.img.height / 2;	//中央寄せ
	player.speed = 500;		//移動速度
	player.img.IsDraw = TRUE;	//描画できる

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス

		//ゴールを初期化
	Goal.img.x = 0;
	Goal.img.y = GAME_HEIGTH / 2;
	Goal.speed = 500;		//移動速度
	Goal.img.IsDraw = TRUE;		//描画できる

	//当たり判定を更新する
	CollUpdate(&Goal);	//プレイヤーの当たり判定のアドレス
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
		//画面を切り替えたとき音楽を止める
		StopSoundMem(TitleBGM.handle);

		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
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
	
	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;

		//動くときの効果音
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//動くときの効果音
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;

		//動くときの効果音
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;

		//動くときの効果音
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);

	//ゴールの当たり判定を更新
	CollUpdate(&Goal);

	//プレイヤーがゴールに当たったときは
	if (onCollRect(player.coll, Goal.coll) == true)
	{
		//画面を切り替えたとき音楽を止める
		StopSoundMem(PlayBGM.handle);

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		//処理を強制終了
		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//背景動画

	//もし動画が再生されていないとき
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);		//動画の再生
	}

	//動画の描画
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGTH, playMovie.handle, TRUE);

	//プレイヤーを描写
	if (player.img.IsDraw == TRUE)
	{
		//画像を描写
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描写
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255,0,0),FALSE);
		}

	}

	//ゴールを描画
	if (Goal.img.IsDraw == TRUE)
	{
		//画像を描写
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);

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
		//画面を切り替えたとき音楽を止める
		StopSoundMem(EndBGM.handle);

		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
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
	chara->coll.left = chara->img.x + 70;
	chara->coll.top = chara->img.y + 80;
	chara->coll.right = chara->img.x + chara->img.width - 70;
	chara->coll.bottom = chara->img.y + chara->img.height - 150;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					
	chara->coll.top = chara->img.y;						
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>当たったらTRUE、当たらなかったらFALSE</returns>
BOOL onCollRect(RECT a, RECT b)
{
	if (a.left < b.right &&		//aの左辺X座標 < bの右辺X座標かつ
		a.right > b.left &&		//aの右辺X座標 > bの左辺X座標かつ
		a.top < b.bottom &&		//aの上辺Y座標 < bの下辺Y座標かつ
		a.bottom > b.top		//aの下辺Y座標 > bの上辺Y座標
		)
	{
		//当たっている時
		return TRUE;
	}
	else
	{
		//当たっていない時
		return FALSE;
	}
}