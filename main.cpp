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

//1フリップあたりの移動量
#define MOVESPEED 10.0f;
// 待機アニメーションの番号
#define ANIM_NEUTRAL 0
//走りアニメーションの番号
#define ANIM_RUN 1

//１フリップ当たりのカメラの回転角度
#define CAMERA_ANGLE_SPEED 3.0f
//カメラの注視点の高さ
#define CAMERA_LOOK_AT_HEIGHT 30.0f
//カメラと注視点の距離
#define CAMERA_LOOK_AT_DISTANCE 100.0f
// フィールドラインを描く範囲
#define LINE_AREA_SIZE			2400.0f
// ラインの数
#define LINE_NUM			24

//敵の数
#define GHOST_NUMBER  20
//敵の動く速さ
#define GHOST_MOVESPEED 9.0f
//敵の出現時距離
#define GHOST_DISTANCE 400.0f
//敵の出現間隔時間
#define GHOST_TIME 60
//敵の透過度合い
#define GHOST_TRANSPARENT_RATE 0.7f






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	srand((unsigned int)time(NULL)); //疑似乱数のシード値を時刻に
	

	

	

	//敵関連
	bool ghostNumberFlag = FALSE;
	int ghostCounter = 0;

	std::vector<EnemyModel> enemyModels;

	
	//他
	int debagModeFlag = 0;
	char key[256]; //キーの押下状態を格納 
	char oldKey[256]; //前のフレームのキーの押下状態を格納

	



	

	//SetBackgroundColor(50, 155, 255)
	ChangeWindowMode(TRUE), SetWindowSizeChangeEnableFlag(TRUE), SetBackgroundColor(0, 0, 0), SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1000, 750, 32);
	if (DxLib_Init() == -1)	{return -1;}

	CameraControler camera;


	//プレイヤーのモデルをロード
	PlayerModel playerModel(MV1LoadModel("dat/Lat式ミク/Lat式ミクVer2.3_Normalエッジ無し専用.pmd"));


	


	//バックグラウンドのモデルをロード
	int bgModelHandle1 = MV1LoadModel("dat/アノマロさん/デッドマスターの部屋/デッドマスターの部屋.x");
	int bgModelHandle2 = MV1LoadModel("dat/packaged用ステージ1・始まりの場所/packaged用共通の空.x");
	MV1SetScale(bgModelHandle1, VGet(30.0f, 30.0f, 30.0f));
	MV1SetScale(bgModelHandle2, VGet(30.0f, 30.0f, 30.0f));
	
	//敵のモデルをロードしてvectorに格納
	enemyModels.push_back(EnemyModel(MV1LoadModel("dat/ミク三姉妹のお化け/ミクお化け/ミクお化けVer1.0.pmd")));
	for (int i = 1; i < GHOST_NUMBER; i++) {
		enemyModels.push_back(EnemyModel(MV1DuplicateModel(enemyModels[0].modelHandle)));
	}
	ghostCounter++;

	

	//zバッファを有効に
	//SetUseZBufferFlag(TRUE);
	//SetWriteZBuffer3D(TRUE);
	//MV1SetUseZBuffer(bgModelHandle2,TRUE);
	//MV1SetWriteZBuffer(bgModelHandle2, TRUE);



	
	int loopCounter = 0;
	//メインループ
	while (ProcessLoop() == 0) {

		loopCounter++;
		if (loopCounter == 30) {
			loopCounter = 0;
		}else
			loopCounter++;
		
		

		//前のフレームでの押下状態を保存、現フレームの押下状態を取得
		memcpy(oldKey, key, sizeof(key));
		GetHitKeyStateAll(key);

		//背景モデルを描画
		MV1DrawModel(bgModelHandle1);
		MV1DrawModel(bgModelHandle2);
		


		//プレイヤーとカメラの移動、描画
		playerModel.moveVector = VGet(0.0f, 0.0f, 0.0f);// 移動ベクトルを初期化
		playerModel.moveKey(camera.hAngle);//方向キー：1フリップでのキャラクターの移動ベクトルと向きを設定, アニメーション番号を走りに設定
		playerModel.calPosition(camera.hAngle);//プレイヤーの位置と向きを算出
		playerModel.drawModel();//プレイヤーの位置と向きをセット後、描画
		playerModel.animChange();//必要ならアニメーションを変更、時間を進める
		camera.setCameraPosition(playerModel.position);// カメラの位置と向きを算出、セット
		camera.cameraRotationKey();// Q,Rキー：カメラの水平角度を変更, W,Eキー：カメラの垂直角度を変更


			

		//一定時間ごとに敵の数を増やす
		if (loopCounter%GHOST_TIME == 0) {
			do {
				enemyModels[ghostCounter].position = VGet(rand() % 2401 - 1200, 0.0f, rand() % 2401 - 1200);
			} while (VSize(VSub(playerModel.position, enemyModels[ghostCounter].position)) < GHOST_DISTANCE); //はじめから近すぎたらやりなおし

	
			ghostCounter++;
			if (ghostCounter == GHOST_NUMBER) {
				ghostNumberFlag = TRUE;
				ghostCounter = 0; //↓のループで再描画するモデルの番号を指定するために、0からGHOST_NUMBERの間でループ
			}
		}

		//ループごとに全エネミーを移動、再描画、プレイヤーに接触していたらゲームオーバー

			for (int d = 0; d < (ghostNumberFlag ? GHOST_NUMBER : ghostCounter); d++) {
				// ３Ｄモデルに新しい位置座標をセット
				enemyModels[d].enemyToPlayer = VSub(playerModel.position, enemyModels[d].position);

				if (VSize(enemyModels[d].enemyToPlayer) < 13) {
					SetFontSize(100);
					SetFontThickness(10);
					DrawFormatString(30, 550, GetColor(255, 0, 0), "GAME OVER");
				}
				//位置を変更、向きをプレイヤーのほうに
				enemyModels[d].renewPosition(playerModel.position);
	
				//描画
				enemyModels[d].drawModel();
			}
		

	
		//Lキーでデバッグモード(格子とか位置示すもの)
		if (key[KEY_INPUT_D] == 1 && oldKey[KEY_INPUT_D]==0) {
			if (debagModeFlag == 0) debagModeFlag = 1;
			else debagModeFlag = 0;
		}
		if (debagModeFlag == 1) DrawLattice();


	}

	
	DxLib_End();				

	return 0;				
}




//メインループの条件式で呼ぶ関数
int ProcessLoop() {
	if (ScreenFlip() != 0)return -1;
	if (ProcessMessage() != 0)return -1;
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)return -1;
	if (clsDx() != 0)return -1;
	if (ClearDrawScreen() != 0)return -1;
	return 0;
}




// 地面に格子と座標軸を描画、球を描画(デバッグ用)
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


	//球の描画
	DrawSphere3D(VGet(LINE_AREA_SIZE / 2.0f, 3.0f, LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(LINE_AREA_SIZE / 2.0f, 3.0f, -LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(-LINE_AREA_SIZE / 2.0f, 3.0f, -LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawSphere3D(VGet(-LINE_AREA_SIZE / 2.0f, 3.0f, LINE_AREA_SIZE / 2.0f), 20.0f, 128, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);


}