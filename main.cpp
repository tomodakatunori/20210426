//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g���̂ɕK�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"//�Q�[���^�C�g��
#define GAME_WIDTH 1280	//�Q�[����ʂ̕�
#define GAME_HEIGTH 720	//�Q�[����ʂ̍���
#define GAME_COLOR 32	//�Q�[���̐F��
#define GAME_ICON_ID 333	//�Q�[���̃A�C�R��
#define GAME_WINDOWBER 0 //�E�B���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�܂�
//windous�̃v���O���~���O��winAPI�œ����Ă���
//DxLib��Directx�Ƃ����Q�[���v���O�������ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);	//Log.txt��\�����Ȃ�
	ChangeWindowMode(TRUE);	//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);	//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//�E�B���h�E�̉𑜓x
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);	//�E�B���h�E�̑傫��
	SetBackgroundColor(255, 255, 255);	//�w�i�̐F
	SetWindowIconID(GAME_ICON_ID);	//�E�B���h�E�̃A�C�R��
	SetWindowStyleMode(GAME_WINDOWBER);	//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);	//�f�B�X�v���C�̐��������̗L��
	SetAlwaysRunFlag(TRUE);	//�E�B���h�E�������ƃA�N�e�B�u�ɂ���	
	//������̓E�B���h�E�̏������̏�ɏ����ׂ�

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�l�p�̈ʒu
	int X = GAME_WIDTH/2;	//���̒��S
	int Y = GAME_HEIGTH/2;	//�c�̒��S

	//�l�p�̑傫��
	int width = 32;//��
	int height = 32;//�c

	//�~�̔��a
	int radius = 100;

	//���x
	int speed = 1;
	int XSpeed = 2;
	int YSpeed = 2;

	//�_�u���o�b�t�@�����O�̗L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		//�Ȃɂ�����L�[�������ꂽ
		if (CheckHitKeyAll() != 0)	//0�̂Ƃ��A�Ȃɂ��L�[�������ꂽ
		{
			break;	//�������[�v�𔲂���	
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E�����ꂽ
		{
			break;	//�������[�v�𔲂���
		}

		//��ʂ���������
		if (ClearDrawScreen() != 0){break;}

		//�l�p��`��
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255,0,0),//�F���擾
		//	TRUE//�h��Ԃ�
		//);

		//�~��`��
		DrawCircle(
			X, Y, radius,
			GetColor(122, 122, 0),
			FALSE , 5
		);

		X += XSpeed;
		//�΂ߏ�ɓ�����
		Y += YSpeed;

		//�[�ɗ�����ړ���������𔽓]������
		if ( X - radius < 0 || X + radius > GAME_WIDTH)	//��ʂ̉�����o���Ƃ�
		{
			//-1�Ȃ�+1�ɁA+1�Ȃ��-1�ɂ���
			XSpeed = -XSpeed;	//�ړ��̌����𔽓]������
			//�ǂɓ��������瑬���Ȃ�
			if (XSpeed > 0) { XSpeed += 2; }
			else if (XSpeed < 0) { XSpeed -= 2; }
		}

		if (Y - radius< 0 || Y + radius > GAME_HEIGTH)	//��ʂ̏c����o���Ƃ�
		{
			//-1�Ȃ�+1�ɁA+1�Ȃ��-1�ɂ���
			YSpeed = -YSpeed;	//�ړ��̌����𔽓]������
			//�ǂɓ��������瑬���Ȃ�
			if (YSpeed > 0) { YSpeed += 2; }
			else if (YSpeed < 0) { YSpeed -= 2; }
		}

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��

	}

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;	//�\�t�g�̏I�� 
}