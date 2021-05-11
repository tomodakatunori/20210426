//キーボードのソースファイル

//ヘッダーファイルの読み込み
#include "keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数

/// <summary>
/// キーの入力状態を更新する
/// </summary>
VOID ALLKeyUpdate(VOID)
{
	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		keyboard.OldKeyState[i] = keyboard.AllKeyState[i];
	}

	//すべてのキーの入力状態を得る
	GetHitKeyStateAll(keyboard.TempKeyState);

	// 押されているキーの時間を更新する
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//キーコードを押しているとき
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;
		}
		else
		{
			keyboard.AllKeyState[i] = 0;
		}
	}

	return;
}

/// <summary>
/// キーを押しているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>押していたらTRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーを上げているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>上げていたらTRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	if (
		keyboard.OldKeyState[KEY_INPUT_] != 0	//以前は押してた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//現在は押してない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーをクリックしたか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	if (
		keyboard.OldKeyState[KEY_INPUT_] != 0	//以前は押してた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//現在は押してない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/// <summary>
/// キーを押し続けているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <param name="MillitTime">キーを押し続けているミリ秒</param>
/// <returns></returns>
BOOL KeyDownKeep(int KEY_INPUT, int MillitTime)
{
	//1秒は1000ミリ秒
	float MillSec = 1000.0f;

	//押し続けている時間は、ミリ秒×FPS値
	int UpdateTime = (MillitTime / MillSec) * 60;

	if (keyboard.AllKeyState[ KEY_INPUT ] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}