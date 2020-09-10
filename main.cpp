#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Model.h"
#include "PlayerModel.h"


int ProcessLoop();

void CameraRotationKey(float &cameraHAngle, float &cameraVAngle);
void PlayerMoveKey(float &angle, float &cameraHAngle, VECTOR &moveVector, int &animIndexToBeAttached);
void CalPlayerPosition(int animIndexToBeAttached, float cameraHAngle, VECTOR* position, VECTOR moveVector);
void MV1DrawPositionedModel(int playerModelHandle, float angle, VECTOR position);
void SetCameraPosition(VECTOR position, float cameraVAngle, float cameraHAngle);
void AnimChanger(int animIndexToBeAttached, int *animIndexAttached, int *attachIndex, int playerModelHandle, float *playTime, float *totalTime);
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
#define CAMERA_ANGLE_SPEED 2.0f
//�J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT 15.0f
//�J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE 100.0f
// �t�B�[���h���C����`���͈�
#define LINE_AREA_SIZE			2400.0f
// ���C���̐�
#define LINE_NUM			24

//�G�̐�
#define GHOST_NUMBER  15
//�G�̓�������
#define GHOST_MOVESPEED 9.0f
//�G�̏o��������
#define GHOST_DISTANCE 400.0f
//�G�̏o���Ԋu����
#define GHOST_TIME 100
//�G�̓��ߓx����
#define GHOST_TRANSPARENT_RATE 0.45f






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	static int nowMode; //���݂̉��(�V�[��)

	
	//�w�i�֘A
	int bgModelHandle1, bgModelHandle2, bgModelHandle3, bgModelHandle4, bgModelHandle5;
	//�J�����֌W
	float  cameraHAngle, cameraVAngle;
	//�G�֘A
	bool ghostNumberFlag = FALSE;
	int ghostCounter, ghostModelHandle[GHOST_NUMBER];
	float ghostTotalTime, ghostPositionX, ghostPositionZ;
	float ghostAngle[GHOST_NUMBER], ghostPlayTime[GHOST_NUMBER];
	VECTOR ghostPosition[GHOST_NUMBER], ghostMoveVector[GHOST_NUMBER], ghostToPlayer[GHOST_NUMBER];
	//��
	int debagModeFlag;
	char key[256]; //�L�[�̉�����Ԃ��i�[ 
	char oldKey[256]; //�O�̃t���[���̃L�[�̉�����Ԃ��i�[

	

	//SetBackgroundColor(50, 155, 255)
	ChangeWindowMode(TRUE), SetWindowSizeChangeEnableFlag(TRUE), SetBackgroundColor(0, 0, 0), SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1000, 750, 32);
	if (DxLib_Init() == -1)	{return -1;}


	//�v���C���[�̃��f�������[�h
	PlayerModel playerModel(MV1LoadModel("dat/Lat���~�N/Lat���~�NVer2.3_Normal�G�b�W������p.pmd"));

	


	//�o�b�N�O���E���h�̃��f�������[�h
	bgModelHandle1 = MV1LoadModel("dat/�A�m�}������/�f�b�h�}�X�^�[�̕���/�f�b�h�}�X�^�[�̕���.x");
	//bgModelHandle1 = MV1LoadModel("dat/jimenB6/Jimen_B601.x");
	bgModelHandle2 = MV1LoadModel("dat/packaged�p�X�e�[�W1�E�n�܂�̏ꏊ/packaged�p���ʂ̋�.x");
	MV1SetScale(bgModelHandle1, VGet(30.0f, 30.0f, 30.0f));
	MV1SetScale(bgModelHandle2, VGet(30.0f, 30.0f, 30.0f));
	
	



	// �J�����̌�����������
	cameraHAngle = 0.0f;
	cameraVAngle = -3.0f;
	SetCameraNearFar(15.0f, 3500.0f);



	//�G�̃��f�������[�h
	ghostModelHandle[0] = MV1LoadModel("dat/�~�N�O�o���̂�����/�~�N������/�~�N������Ver1.0.pmd");
	MV1SetOpacityRate(ghostModelHandle[0], GHOST_TRANSPARENT_RATE); //�G���f���̔�������
	MV1SetScale(ghostModelHandle[0], VGet(5.0f, 5.0f, 5.0f));
	ghostPositionX = 0.0f;
	ghostPositionZ =1000.0f;
	ghostPosition[0] = VGet(ghostPositionX, 0.0f, ghostPositionZ);
	ghostAngle[0] = atan2(ghostPositionX, ghostPositionZ);
	

	//z�o�b�t�@��L����
	SetUseZBufferFlag(TRUE);
	SetWriteZBuffer3D(TRUE);
	MV1SetUseZBuffer(bgModelHandle2,TRUE);
	MV1SetWriteZBuffer(bgModelHandle2, TRUE);

	
	//���̑�������
	debagModeFlag = 0;
	ghostCounter = 1;
	
	int loopCounter = 0;
	//���C�����[�v
	while (ProcessLoop() == 0) {
		loopCounter++;
		if (loopCounter == 30) {
			loopCounter == 0;
		}else
			loopCounter++;
		
		

		//�O�̃t���[���ł̉�����Ԃ�ۑ��A���t���[���̉�����Ԃ��擾
		memcpy(oldKey, key, sizeof(key));
		GetHitKeyStateAll(key);

		//�w�i���f����`��
		MV1DrawModel(bgModelHandle1);
		MV1DrawModel(bgModelHandle2);
		


		//�v���C���[�ƃJ�����̈ړ��A�`��
		{
			// Q,R�L�[�F�J�����̐����p�x��ύX, W,E�L�[�F�J�����̐����p�x��ύX
			CameraRotationKey(cameraHAngle, cameraVAngle);

			// �ړ��x�N�g����������
			playerModel.moveVector = VGet(0.0f, 0.0f, 0.0f);

			//�����L�[�F1�t���b�v�ł̃L�����N�^�[�̈ړ��x�N�g���ƌ�����ݒ�, �A�j���[�V�����ԍ��𑖂�ɐݒ�
			playerModel.moveKey(cameraHAngle);

			//�v���C���[�̈ʒu�ƌ������Z�o
			playerModel.calPosition(cameraHAngle);

			//�v���C���[�̈ʒu�ƌ������Z�b�g��A�`��
			playerModel.drawModel();
			MV1DrawModel(playerModel.modelHandle);


			// �J�����̈ʒu�ƌ������Z�o�A�Z�b�g
			SetCameraPosition(playerModel.position, cameraVAngle, cameraHAngle);

			//�K�v�Ȃ�A�j���[�V������ύX�A���Ԃ�i�߂�
			playerModel.animChange();
		}
			

		//��莞�Ԃ��ƂɓG�̐��𑝂₷
		if (loopCounter%GHOST_TIME == 0) {
			do {
				
				ghostPositionX = rand() % 2401 - 1200;
				ghostPositionZ = rand() % 2401 - 1200;
				ghostPosition[ghostCounter] = VGet(ghostPositionX, 0.0f, ghostPositionZ);
				ghostToPlayer[ghostCounter] = VSub(playerModel.position, ghostPosition[ghostCounter]);
			} while (VSize(ghostToPlayer[ghostCounter]) <GHOST_DISTANCE  );

			if (ghostNumberFlag == FALSE) {
				ghostModelHandle[ghostCounter] = MV1DuplicateModel(ghostModelHandle[0]);
				MV1SetOpacityRate(ghostModelHandle[ghostCounter], GHOST_TRANSPARENT_RATE); //�G���f���̔�������
				MV1SetScale(ghostModelHandle[ghostCounter], VGet(4.0f, 4.0f, 4.0f));
			}
			ghostCounter++;
			if (ghostCounter == GHOST_NUMBER) {
				ghostNumberFlag = TRUE;
				ghostCounter = 0;
			}
		}
		

		//���[�v���ƂɑS�G�l�~�[���ړ��A�ĕ`��
		
			
			for (int d = 0; d < (ghostNumberFlag? GHOST_NUMBER : ghostCounter); d++) {
				// �R�c���f���ɐV�����ʒu���W���Z�b�g
				ghostToPlayer[d] = VSub(playerModel.position, ghostPosition[d]);

				if (VSize(ghostToPlayer[d]) < 13) {
					SetFontSize(100);
					SetFontThickness(10);
					//ChangeFont("�l�r ����");                    
					//ChangeFontType(DX_FONTTYPE_ANTIALIASING);     //�A���`�G�C���A�X�t�H���g�ɕύX
					DrawFormatString(30, 550, GetColor(255, 0, 0), "GAME OVER");
					//DrawFormatString(500, 550, GetColor(255, 0, 0), "SCORE %d", loopCounter);
					
				}

				ghostMoveVector[d] = VGet(VNorm(ghostToPlayer[d]).x * GHOST_MOVESPEED, 0.0f, VNorm(ghostToPlayer[d]).z*GHOST_MOVESPEED);
				ghostPosition[d] = VAdd(ghostPosition[d], ghostMoveVector[d]);
				MV1SetPosition(ghostModelHandle[d], ghostPosition[d]);


				//�������v���C���[�̂ق���
				ghostAngle[d] = atan2(ghostPosition[d].x - playerModel.position.x, ghostPosition[d].z - playerModel.position.z);
				MV1SetRotationXYZ(ghostModelHandle[d], VGet(0.0f, ghostAngle[d], 0.0f));

				//�`��
				MV1DrawModel(ghostModelHandle[d]);
			}
		

		

		//L�L�[�Ńf�o�b�O���[�h(�i�q�Ƃ��ʒu��������)
		if (key[KEY_INPUT_D] == 1 && oldKey[KEY_INPUT_D]==0) {
			if (debagModeFlag == 0) debagModeFlag = 1;
			else debagModeFlag = 0;
		}
		if (debagModeFlag == 1) DrawLattice();


		//SetFontSize(130);
		//SetFontThickness(15);
		//DrawFormatString(100, 0, GetColor(255, 0, 0), "SCORE %d", loopCounter);

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


// Q,R�L�[�F�J�����̐����p�x��ύX, W,E�L�[�F�J�����̐����p�x��ύX

void CameraRotationKey(float &cameraHAngle, float &cameraVAngle) {
	if (CheckHitKey(KEY_INPUT_R) == 1) {
		cameraHAngle -= CAMERA_ANGLE_SPEED;
		if (cameraHAngle <= -180) {
			cameraHAngle += 360;
		}
	}
	if (CheckHitKey(KEY_INPUT_Q) == 1) {
		cameraHAngle += CAMERA_ANGLE_SPEED;
		if (cameraHAngle >= 180) {
			cameraHAngle -= 360;
		}
	}
	if (CheckHitKey(KEY_INPUT_E) == 1) {
		cameraVAngle -= CAMERA_ANGLE_SPEED;
		if (cameraVAngle <= -5.0f) {
			cameraVAngle = -5.0f;
		}
	}
	if (CheckHitKey(KEY_INPUT_W) == 1) {
		cameraVAngle += CAMERA_ANGLE_SPEED;
		if (cameraVAngle >= 89.0f) {
			cameraVAngle = 89.0f;
		}
	}
}









// �J�����̈ʒu�ƌ������Z�o�A�Z�b�g
void SetCameraPosition(VECTOR position, float cameraVAngle, float cameraHAngle) {

	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;
	float sinParam, cosParam;


	// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
	CameraLookAtPosition = position;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// �J�����̈ʒu���J�����̐����p�x�Ɛ����p�x����Z�o
	// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
	sinParam = sin(cameraVAngle / 180.0f * DX_PI_F);
	cosParam = cos(cameraVAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0.0f;
	TempPosition1.y = sinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -cosParam * CAMERA_LOOK_AT_DISTANCE;

	// ���ɐ����p�x�𔽉f�����ʒu���Z�o
	sinParam = sin(cameraHAngle / 180.0f * DX_PI_F);
	cosParam = cos(cameraHAngle / 180.0f * DX_PI_F);
	TempPosition2.x = cosParam * TempPosition1.x - sinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = sinParam * TempPosition1.x + cosParam * TempPosition1.z;

	// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

	// �J�����̐ݒ�ɔ��f����
	SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
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