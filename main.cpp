//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"//DxLib���g���̂ɕK�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"//�Q�[���^�C�g��
#define GAME_WIDTH 1280//�Q�[����ʂ̕�
#define GAME_HEIGTH 720//�Q�[����ʂ̍���
#define GAME_COLOR 36//�Q�[���̐F��
#define GAME_ICON_ID 333//�Q�[���̃A�C�R��

// �v���O������ WinMain ����n�܂�܂�
//windous�̃v���O���~���O��winAPI�œ����Ă���
//DxLib��Directx�Ƃ����Q�[���v���O�������ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);//�E�B���h�E�̉𑜓x
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);
	SetBackgroundColor(255, 122, 122);
	SetWindowIconID(GAME_ICON_ID);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�������[�v
	while (1)
	{
		//�Ȃɂ�����L�[�������ꂽ
		if (CheckHitKeyAll() != 0)//0�̂Ƃ��A�Ȃɂ��L�[�������ꂽ
		{
			break;//�������[�v�𔲂���	
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)//-1�̂Ƃ��A�G���[��E�B���h�E�����ꂽ
		{
			break;//�������[�v�𔲂���
		}
	}

	DxLib_End();// �c�w���C�u�����g�p�̏I������

	return 0;// �\�t�g�̏I�� 
}