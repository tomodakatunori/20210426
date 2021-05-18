//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g���̂ɕK�v

#include "keyboard.h"	//�L�[�{�[�h���g���̂ɕK�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"//�Q�[���^�C�g��
#define GAME_WIDTH 1280		//�Q�[����ʂ̕�
#define GAME_HEIGTH 720		//�Q�[����ʂ̍���
#define GAME_COLOR 32		//�Q�[���̐F��
#define GAME_ICON_ID 333	//�Q�[���̃A�C�R��
#define GAME_WINDOWBER 0	//�E�B���h�E�o�[�̎��

//�񋓌^
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
	//�Q�[���̃V�[��
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;		//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//��ʂ̐؂�ւ�
BOOL IsfadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsfadeIn = FALSE;	//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;					//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���g
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���g
int fadeInCntMax = 0;				//�t�F�[�h�C���̃J�E���^MAX

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

// �v���O������ WinMain ����n�܂�܂�
//windous�̃v���O���~���O��winAPI�œ����Ă���
//DxLib��Directx�Ƃ����Q�[���v���O�������ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt��\�����Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//�E�B���h�E�̉𑜓x
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);				//�E�B���h�E�̑傫��
	SetBackgroundColor(255, 255, 255);					//�w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�E�B���h�E�̃A�C�R��
	SetWindowStyleMode(GAME_WINDOWBER);					//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐��������̗L��
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���	

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�̗L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�~�̒��S�_
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGTH / 2;
	//�~�̔��a
	int radius = 50;

	//�ŏ��̃V�[���̓^�C�g����ʂ�����
	GameScene = GAME_SCENE_TITLE;

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; } //��ʂ̏���

		//�L�[�{�[�h�̓��͂̍X�V
		AllKeyUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�|�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃ̏���
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();	//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//�L�[����
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			Y--;	//��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			Y++;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			X--;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			X++;	//�E�Ɉړ�
		}


		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��

	}

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;		//�\�t�g�̏I�� 
}

/// <summary>
/// �V�[����؂�ւ���
/// </summary>
/// <param name="">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//�V�[���ɐ؂�ւ���
	IsfadeIn = FALSE;				//�t�F�[�h�C�����Ȃ�
	IsfadeOut = TRUE;				//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}


	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsfadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeInCnt = fadeInCntInit;	//�J�E���g������
			IsfadeIn = FALSE;			//�t�F�[�h�C����������
		}
	}

	//�t�F�[�h�A�E�g
	if (IsfadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeOutCnt = fadeOutCntInit;	//�J�E���g������
			IsfadeOut = FALSE;			//�t�F�[�h�C����������
		}
	}

	//�؂�ւ������I���H
	if (IsfadeIn == FALSE && IsfadeOut == FALSE)
	{
		//�t�F�[�h�C�����ĂȂ��A�t�F�[�h�A�E�g�����Ă��Ȃ���
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	case GAME_SCENE_CHANGE:
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsfadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsfadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGTH,GetColor(0,0,0),TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}