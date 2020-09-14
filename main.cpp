#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "PlayerModel.h"
#include "EnemyModel.h"
#include "CameraControler.h"



int ProcessLoop();

void CameraRotationKey(float &cameraHAngle, float &cameraVAngle);

void SetCameraPosition(VECTOR position, float cameraVAngle, float cameraHAngle);
void DrawLattice();


typedef enum {
	menu,config,  game, score
}playMode;

//1�t���b�v������̈ړ���
#define MOVESPEED 10.0f;
// �ҋ@�A�j���[�V�����̔ԍ�
#define ANIM_NEUTRAL 0
//����A�j���[�V�����̔ԍ�
#define ANIM_RUN 1

//�P�t���b�v������̃J�����̉�]�p�x
#define CAMERA_ANGLE_SPEED 3.0f
//�J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT 30.0f
//�J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE 100.0f
// �t�B�[���h���C����`���͈�
#define LINE_AREA_SIZE			2400.0f
// ���C���̐�
#define LINE_NUM			24

//�G�̐�
#define GHOST_NUMBER  20
//�G�̓�������
#define GHOST_MOVESPEED 9.0f
//�G�̏o��������
#define GHOST_DISTANCE 400.0f
//�G�̏o���Ԋu����
#define GHOST_TIME 60
//�G�̓��ߓx����
#define GHOST_TRANSPARENT_RATE 0.7f






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	srand((unsigned int)time(NULL)); //�^�������̃V�[�h�l��������
	

	

	

	//�G�֘A
	bool ghostNumberFlag = FALSE;
	int ghostCounter = 0;

	std::vector<EnemyModel> enemyModels;

	
	//��
	int debagModeFlag = 0;
	char key[256]; //�L�[�̉�����Ԃ��i�[ 
	char oldKey[256]; //�O�̃t���[���̃L�[�̉�����Ԃ��i�[

	



	

	//SetBackgroundColor(50, 155, 255)
	ChangeWindowMode(TRUE), SetWindowSizeChangeEnableFlag(TRUE), SetBackgroundColor(0, 0, 0), SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1000, 750, 32);
	if (DxLib_Init() == -1)	{return -1;}

	CameraControler camera;


	//�v���C���[�̃��f�������[�h
	PlayerModel playerModel(MV1LoadModel("dat/Lat���~�N/Lat���~�NVer2.3_Normal�G�b�W������p.pmd"));


	


	//�o�b�N�O���E���h�̃��f�������[�h
	int bgModelHandle1 = MV1LoadModel("dat/�A�m�}������/�f�b�h�}�X�^�[�̕���/�f�b�h�}�X�^�[�̕���.x");
	int bgModelHandle2 = MV1LoadModel("dat/packaged�p�X�e�[�W1�E�n�܂�̏ꏊ/packaged�p���ʂ̋�.x");
	MV1SetScale(bgModelHandle1, VGet(30.0f, 30.0f, 30.0f));
	MV1SetScale(bgModelHandle2, VGet(30.0f, 30.0f, 30.0f));
	
	//�G�̃��f�������[�h����vector�Ɋi�[
	enemyModels.push_back(EnemyModel(MV1LoadModel("dat/�~�N�O�o���̂�����/�~�N������/�~�N������Ver1.0.pmd")));
	for (int i = 1; i < GHOST_NUMBER; i++) {
		enemyModels.push_back(EnemyModel(MV1DuplicateModel(enemyModels[0].modelHandle)));
	}
	ghostCounter++;

	

	//z�o�b�t�@��L����
	//SetUseZBufferFlag(TRUE);
	//SetWriteZBuffer3D(TRUE);
	//MV1SetUseZBuffer(bgModelHandle2,TRUE);
	//MV1SetWriteZBuffer(bgModelHandle2, TRUE);



	
	int loopCounter = 0;
	//���C�����[�v
	while (ProcessLoop() == 0) {

		loopCounter++;
		if (loopCounter == 30) {
			loopCounter = 0;
		}else
			loopCounter++;
		
		

		//�O�̃t���[���ł̉�����Ԃ�ۑ��A���t���[���̉�����Ԃ��擾
		memcpy(oldKey, key, sizeof(key));
		GetHitKeyStateAll(key);

		//�w�i���f����`��
		MV1DrawModel(bgModelHandle1);
		MV1DrawModel(bgModelHandle2);
		


		//�v���C���[�ƃJ�����̈ړ��A�`��
		playerModel.moveVector = VGet(0.0f, 0.0f, 0.0f);// �ړ��x�N�g����������
		playerModel.moveKey(camera.hAngle);//�����L�[�F1�t���b�v�ł̃L�����N�^�[�̈ړ��x�N�g���ƌ�����ݒ�, �A�j���[�V�����ԍ��𑖂�ɐݒ�
		playerModel.calPosition(camera.hAngle);//�v���C���[�̈ʒu�ƌ������Z�o
		playerModel.drawModel();//�v���C���[�̈ʒu�ƌ������Z�b�g��A�`��
		playerModel.animChange();//�K�v�Ȃ�A�j���[�V������ύX�A���Ԃ�i�߂�
		camera.setCameraPosition(playerModel.position);// �J�����̈ʒu�ƌ������Z�o�A�Z�b�g
		camera.cameraRotationKey();// Q,R�L�[�F�J�����̐����p�x��ύX, W,E�L�[�F�J�����̐����p�x��ύX


			

		//��莞�Ԃ��ƂɓG�̐��𑝂₷
		if (loopCounter%GHOST_TIME == 0) {
			do {
				enemyModels[ghostCounter].position = VGet(rand() % 2401 - 1200, 0.0f, rand() % 2401 - 1200);
			} while (VSize(VSub(playerModel.position, enemyModels[ghostCounter].position)) < GHOST_DISTANCE); //�͂��߂���߂���������Ȃ���

	
			ghostCounter++;
			if (ghostCounter == GHOST_NUMBER) {
				ghostNumberFlag = TRUE;
				ghostCounter = 0; //���̃��[�v�ōĕ`�悷�郂�f���̔ԍ����w�肷�邽�߂ɁA0����GHOST_NUMBER�̊ԂŃ��[�v
			}
		}

		//���[�v���ƂɑS�G�l�~�[���ړ��A�ĕ`��A�v���C���[�ɐڐG���Ă�����Q�[���I�[�o�[

			for (int d = 0; d < (ghostNumberFlag ? GHOST_NUMBER : ghostCounter); d++) {
				// �R�c���f���ɐV�����ʒu���W���Z�b�g
				enemyModels[d].enemyToPlayer = VSub(playerModel.position, enemyModels[d].position);

				if (VSize(enemyModels[d].enemyToPlayer) < 13) {
					SetFontSize(100);
					SetFontThickness(10);
					DrawFormatString(30, 550, GetColor(255, 0, 0), "GAME OVER");
				}
				//�ʒu��ύX�A�������v���C���[�̂ق���
				enemyModels[d].renewPosition(playerModel.position);
	
				//�`��
				enemyModels[d].drawModel();
			}
		

	
		//L�L�[�Ńf�o�b�O���[�h(�i�q�Ƃ��ʒu��������)
		if (key[KEY_INPUT_D] == 1 && oldKey[KEY_INPUT_D]==0) {
			if (debagModeFlag == 0) debagModeFlag = 1;
			else debagModeFlag = 0;
		}
		if (debagModeFlag == 1) DrawLattice();


	}

	
	DxLib_End();				

	return 0;				
}




//���C�����[�v�̏������ŌĂԊ֐�
int ProcessLoop() {
	if (ScreenFlip() != 0)return -1;
	if (ProcessMessage() != 0)return -1;
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)return -1;
	if (clsDx() != 0)return -1;
	if (ClearDrawScreen() != 0)return -1;
	return 0;
}




// �n�ʂɊi�q�ƍ��W����`��A����`��(�f�o�b�O�p)
void DrawLattice() {

	int i, j;
	VECTOR Pos1;
	VECTOR Pos2;
	VECTOR Pos3;


	DrawLine3D(VGet(0, 1000, 0), VGet(0, -1000, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(1000, 0, 0.3f), VGet(-1000, 0, 0.3f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.3f, 0, 1000), VGet(0.3f, 0, -1000), GetColor(255, 0, 0));


	Pos1 = VGet(100, 0, 0);
	Pos2 = VGet(80, 5, 0);
	Pos3 = VGet(80, -5, 0);
	DrawTriangle3D(Pos1, Pos2, Pos3, GetColor(0, 255, 0), TRUE);


	Pos1 = VGet(0, 0, 100);
	Pos2 = VGet(0, 5, 80);
	Pos3 = VGet(0, -5, 80);
	DrawTriangle3D(Pos1, Pos2, Pos3, GetColor(0, 200, 255), TRUE);




	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, LINE_AREA_SIZE / 2.0f);
	for (i = 0; i <= LINE_NUM; i++) {

		if (i == 15) {
			Pos1.x += LINE_AREA_SIZE / LINE_NUM;
			Pos2.x += LINE_AREA_SIZE / LINE_NUM;
			continue;
		}


		DrawLine3D(Pos1, Pos2, GetColor(0, 200, 255));
		Pos1.x += LINE_AREA_SIZE / LINE_NUM;
		Pos2.x += LINE_AREA_SIZE / LINE_NUM;
	}

	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	Pos2 = VGet(LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	for (i = 0; i < LINE_NUM; i++) {

		if (i == 15) {
			Pos1.z += LINE_AREA_SIZE / LINE_NUM;
			Pos2.z += LINE_AREA_SIZE / LINE_NUM;
			continue;
		}

		DrawLine3D(Pos1, Pos2, GetColor(0, 255, 200));
		Pos1.z += LINE_AREA_SIZE / LINE_NUM;
		Pos2.z += LINE_AREA_SIZE / LINE_NUM;
	}


	//���̕`��
	DrawSphere3D(VGet(LINE_AREA_SIZE / 2.0f, 3.0f, LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(LINE_AREA_SIZE / 2.0f, 3.0f, -LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(-LINE_AREA_SIZE / 2.0f, 3.0f, -LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(-LINE_AREA_SIZE / 2.0f, 3.0f, LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);


}