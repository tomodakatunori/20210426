//FPSのソースファイル

//ヘッダファイルの読み込み
#include "FPS.h"

//グローバル変数
FPS  fps;	//FPSの管理
/// <summary>
/// FPSを計算し、値を更新する
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	//一番最初の処理
	if (FALSE == fps.IsInitFlg)
	{
		//測定開始時間をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//windousが起動してから経過した時間(マイクロ秒)

		fps.IsInitFlg = TRUE;	//フラグを立てる
	}

	//現在の時刻をマイクロ秒単位で取得
	fps.NowTime = GetNowHiPerformanceCount();

	//前回取得した時間からの経過時間秒（少数）に変換してからセット
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//今回取得しか時間を保存
	fps.OldTime = fps.NowTime;

	//１フレーム目～FPS測定値までは、カウントアップ
	if (fps.Count < fps.SampleRate)
	{
		//カウントを増やす
		fps.Count++;
	}
	else
	{
		//FPS設定値のフレームで、平均FPSを測定

		//現在の時刻から０フレーム目の時間を引き、FPSの数値で割る
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.StartTime) / (float)fps.SampleRate);

		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//Windowsが起動してから経過した時間（マイクロ秒）

		//カウント開始
		fps.Count = 1;
	}

	return;
}

/// <summary>
/// 平均FPS値を描画
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG ==TRUE)
	{
		//文字列を描画
		DrawFormatString(0, GAME_HEIGTH - 20, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}
	

	return;
}

/// <summary>
/// FPSで処理を待つ
/// </summary>
/// <param name=""></param>
VOID FPSWeit(VOID)
{
	//現在の時刻、最初の時間で、現在かかっている時刻を取得する
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//待つべきミリ秒数　(1秒／fps値＊現在のフレーム数)から、現在かかっている時刻を引く
	int waitTime = 1000000.0f / fps.Value * fps.Count - resultTime;

	//マイクロ秒からミリ秒に変換
	waitTime /= 1000.0f;

	//処理が早かったら、処理を待つ
	if (waitTime > 0)
	{
		WaitTimer(waitTime);	//引数ミリ秒待つ
	}

	//垂直同期をOFFになっているか
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS最大値でないとき
		if (fps.Value < GAME_FPS_MAX)
		{
			//1秒毎のFPS値よりも、待つ時間が小さいときは、もっとFPS値を上げてもいい
			//待つ時間　10ミリ　<= 1秒/60fps = 16.666ミリ　もう少し早くできる
			if (waitTime > 0
				&& waitTime <= 1000.0f / fps.Value)
			{
				fps.Count++;
			}
			else
			{
				//FPS値が追いついておらず、遅い時は、FPSを下げる
				if (fps.Value = GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}

	return;
}