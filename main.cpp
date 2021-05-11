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

// �v���O������ WinMain ����n�܂�܂�
//windous�̃v���O���~���O��winAPI�œ����Ă���
//DxLib��Directx�Ƃ����Q�[���v���O�������ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, 
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
	//������̓E�B���h�E�̏������̏�ɏ����ׂ�

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

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0){break;}	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0){break;} //��ʂ̏��o

		//�L�[�{�[�h�̓��͂̍X�V
		AllKeyUpdate();
		
		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��

	}

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;		//�\�t�g�̏I�� 
}