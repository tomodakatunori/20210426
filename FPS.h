#pragma once

//ヘッダファイルの読み込み]
#include"DxLib.h"

//マクロ定義
#define GAME_FPS		60	//FPSの数値
#define GAME_FPS_MAX	240	//FPSの最大値
#define GAME_FPS_MIN	60	//FPSの最小値

//FPS構造体
struct FPS
{
	BOOL IsInitFlg = FALSE;	//最初の測定フラグ
	LONGLONG StartTime = 0;	//測定開始時間
	LONGLONG NowTime = 0;	//現在の時刻
	LONGLONG OldTime = 0;	//以前の時刻

	int Value = GAME_FPS;	//FPSの値

	float DeltaTime = 0.000001;	//０が5個、経過時間
	int Count = 1;				//現在のフレーム数
	float DrawValue = 0.0f;		//計算結果を描画
	int SampleRate = GAME_FPS;	//平均を取るサンプル値
};

//プロトタイプ宣言
VOID FPSUpdate(VOID);	//fps値を更新
VOID FPSDraw(VOID);		//fps値を描画
VOID FPSWeit(VOID);		//fps値を待つ