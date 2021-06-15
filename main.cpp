//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���̊�{
#include "keyboard.h"	//�L�[�{�[�h���g���̂ɕK�v
#include"FPS.h"			//FPS�̏���

//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��
	char path[255];		//�摜�̏ꏊ�i�p�X�j
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	RECT coll;				//�����蔻��̗̈�(�l�p)
	BOOL IsDraw = FALSE;	//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTOR
{
    IMAGE img;
	int speed = 5;		//�ړ����x
	RECT coll;				//�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;		//X�ʒu
	int y;		//Y�ʒu
	int width;	//��
	int height;	//����

	int Volume = 255;	//�{�����[���@�Q�T�T�`�O
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//���y�̃{�����[���i�O�`�Q�T�T�j
	int playType = -1;
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//���S��ǂݍ���
IMAGE Titlelogo;
IMAGE Titleenter;
IMAGE EndClear;

//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//���ʉ�
AUDIO playerSE;

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

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL onCollRect(RECT a, RECT b);	//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^�̓ǂݍ���
VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����

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

	//�ŏ��̃V�[���̓^�C�g����ʂ�����
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���.
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s������
		DxLib_End;	//Dxlib�I��
		return -1;	//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ̏���

		//�L�[�{�[�h�̓��͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

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

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWeit();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��

	}

	//�I��鎞�̏���
	DeleteGraph(playMovie.handle);		//���������������폜

	DeleteGraph(player.img.handle);		//�摜������������폜
	DeleteGraph(Goal.img.handle);		//�摜������������폜

	DeleteGraph(Titlelogo.handle);		//���S������������폜
	DeleteGraph(Titleenter.handle);		//���S������������폜
	DeleteGraph(EndClear.handle);		//���S������������폜

	DeleteSoundMem(TitleBGM.handle);	//�^�C�g��BGM������������폜
	DeleteSoundMem(PlayBGM.handle);		//�v���CBGM������������폜
	DeleteSoundMem(EndBGM.handle);		//�G���hBGM������������폜

	DeleteSoundMem(playerSE.handle);		//SE������������폜

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;		//�\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE�A�ǂݍ��߂Ȃ������Ȃ�FARSE</returns>
BOOL GameLoad()
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\movie\\playmovie.mp4");	//�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);			//�摜�̓ǂݍ���

	//���悪�ǂݍ��߂Ȃ������Ƃ�
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			playMovie.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//����̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(player.img.path, ".\\image\\player.png");	//�p�X�̃R�s�[
	player.img.handle = LoadGraph(player.img.path);	//�摜�̓ǂݍ���
	if (player.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			player.img.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.img.handle, &player.img.width, &player.img.height);

	//����̃{�����[��
	playMovie.Volume = 255;

	//�S�[���̉摜��ǂݍ���
	strcpyDx(Goal.img.path, ".\\image\\Goal.png");	//�p�X�̃R�s�[
	Goal.img.handle = LoadGraph(Goal.img.path);	//�摜�̓ǂݍ���
	if (Goal.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			Goal.img.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(Goal.img.handle, &Goal.img.width, &Goal.img.height);

	//���y�̓ǂݍ���
	strcatDx(TitleBGM.path, ".\\Audio\\������  �s�A�m30.mp3");
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);
	//�ǂݍ��߂Ȃ��������̃G���[
	if (TitleBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			TitleBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		
		return FALSE;	//�ǂݍ��ݎ��s
	}

	TitleBGM.playType = DX_PLAYTYPE_LOOP;
	TitleBGM.Volume = 255;

	//���y�̓ǂݍ���
	strcatDx(PlayBGM.path, ".\\Audio\\������  ����31.mp3");
	PlayBGM.handle = LoadSoundMem(PlayBGM.path);
	//�ǂݍ��߂Ȃ��������̃G���[
	if (PlayBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			PlayBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	PlayBGM.playType = DX_PLAYTYPE_LOOP;
	PlayBGM.Volume = 255;

	//���y�̓ǂݍ���
	strcatDx(EndBGM.path, ".\\Audio\\������  �t�@���^�W�[14.mp3");
	EndBGM.handle = LoadSoundMem(EndBGM.path);
	//�ǂݍ��߂Ȃ��������̃G���[
	if (EndBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			EndBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	EndBGM.playType = DX_PLAYTYPE_LOOP;
	EndBGM.Volume = 255;

	//���y�̓ǂݍ���
	strcatDx(playerSE.path, ".\\Audio\\������  �n01.mp3");
	playerSE.handle = LoadSoundMem(playerSE.path);
	//�ǂݍ��߂Ȃ��������̃G���[
	if (playerSE.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			playerSE.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	playerSE.playType = DX_PLAYTYPE_BACK;
	playerSE.Volume = 255;

	//�^�C�g�����S�̓ǂݍ���
	strcpyDx(Titlelogo.path, ".\\image\\�^�C�g�����S.png");	//�p�X�̃R�s�[
	Titlelogo.handle = LoadGraph(Titlelogo.path);	//�摜�̓ǂݍ���
	if (Titlelogo.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			Titlelogo.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�G���^�[���S�̓ǂݍ���
	strcpyDx(Titleenter.path, ".\\image\pushenter.png");	//�p�X�̃R�s�[
	Titleenter.handle = LoadGraph(Titleenter.path);	//�摜�̓ǂݍ���
	if (Titleenter.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			Titleenter.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",			//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�G���h���S�̓ǂݍ���
	strcpyDx(EndClear.path, ".\\image\end.png");	//�p�X�̃R�s�[
	EndClear.handle = LoadGraph(EndClear.path);	//�摜�̓ǂݍ���
	if (EndClear.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			EndClear.path,					//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",			//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	return TRUE;	//���ׂēǂݍ��߂�
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[��������
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;	//������
	player.img.y = GAME_HEIGTH / 2 - player.img.height / 2;	//������
	player.speed = 500;		//�ړ����x
	player.img.IsDraw = TRUE;	//�`��ł���

	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X

		//�S�[����������
	Goal.img.x = 0;
	Goal.img.y = GAME_HEIGTH / 2;
	Goal.speed = 500;		//�ړ����x
	Goal.img.IsDraw = TRUE;		//�`��ł���

	//�����蔻����X�V����
	CollUpdate(&Goal);	//�v���C���[�̓����蔻��̃A�h���X
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
		//��ʂ�؂�ւ����Ƃ����y���~�߂�
		StopSoundMem(TitleBGM.handle);

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
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
	
	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V
	CollUpdate(&Goal);

	//�v���C���[���S�[���ɓ��������Ƃ���
	if (onCollRect(player.coll, Goal.coll) == true)
	{
		//��ʂ�؂�ւ����Ƃ����y���~�߂�
		StopSoundMem(PlayBGM.handle);

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//�����������I��
		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i����

	//�������悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//����̍Đ�
	}

	//����̕`��
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGTH, playMovie.handle, TRUE);

	//�v���C���[��`��
	if (player.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255,0,0),FALSE);
		}

	}

	//�S�[����`��
	if (Goal.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}

	}

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
		//��ʂ�؂�ւ����Ƃ����y���~�߂�
		StopSoundMem(EndBGM.handle);

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
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

/// <summary>
/// �����蔻��̗̈�X�V(�v���C���[)
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 70;
	chara->coll.top = chara->img.y + 80;
	chara->coll.right = chara->img.x + chara->img.width - 70;
	chara->coll.bottom = chara->img.y + chara->img.height - 150;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					
	chara->coll.top = chara->img.y;						
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TRUE�A������Ȃ�������FALSE</returns>
BOOL onCollRect(RECT a, RECT b)
{
	if (a.left < b.right &&		//a�̍���X���W < b�̉E��X���W����
		a.right > b.left &&		//a�̉E��X���W > b�̍���X���W����
		a.top < b.bottom &&		//a�̏��Y���W < b�̉���Y���W����
		a.bottom > b.top		//a�̉���Y���W > b�̏��Y���W
		)
	{
		//�������Ă��鎞
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ���
		return FALSE;
	}
}