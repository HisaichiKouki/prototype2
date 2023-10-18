#include <Novice.h>
#include "Vector2.h"

const char kWindowTitle[] = "LC1A_20_ヒサイチ_コウキ";

typedef struct intVector2
{
	int x;
	int y;
}intVector2;

typedef enum ChipType
{
	kChipTypeNone,
	kChipTypeMiddle,
	kChipTypeStrtEnd,

}ChipType;

typedef enum MoveType
{
	kMoveTypeNONE,
	kMoveTypeUP,
	kMoveTypeRIGHT,
	kMoveTypeDOWN,
	kMoveTypeLEFT,

}MoveType;


typedef struct Player
{
	intVector2 mapPos;
	Vector2 screenPos;
	Vector2 radius;

	int moveDirection;
	bool moveFlag;
	bool death;
}Player;



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	const int mapSizeX = 10;
	const int mapSizeY = 10;

	int mapReset[mapSizeY][mapSizeX] =
	{
		{0,2,1,2,0,0,0,0,0,0},//0
		{2,0,0,0,2,0,0,0,0,0},//0
		{2,0,0,0,1,0,0,0,0,0},//1
		{0,2,1,2,1,2,1,2,0,0},//2
		{0,2,2,0,2,1,1,1,2,0},//3
		{0,2,1,2,0,2,1,1,2,0},//4
		{0,0,0,0,0,0,0,0,0,0},//5
		{0,0,0,0,0,0,0,0,0,0},//6
		{0,0,0,0,0,0,0,0,0,0},//7
		{0,0,0,0,0,0,0,0,0,0},//8

	};


	int map[mapSizeY][mapSizeX]{};


	int blockSize = 32;
	int blockRadius = blockSize / 4;


	Player player{};

	player.mapPos = { 0,0 };
	player.radius = { 8,8 };
	player.moveDirection = kMoveTypeNONE;
	player.moveFlag = false;

	int timer = 0;

	bool ResetF = true;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///                                                            ///
		/// --------------------↓更新処理ここから-------------------- ///
		///                                                            ///       

		if (keys[DIK_R]&&!preKeys[DIK_R])
		{
			ResetF = true;
		}

		if (ResetF)
		{
			player.death = false;
			player.mapPos = { 0,5 };
			player.moveDirection = kMoveTypeNONE;
			timer = 0;


			for (int y = 0; y < mapSizeY; y++)
			{
				for (int x = 0; x < mapSizeX; x++)
				{
					map[y][x] = mapReset[y][x];
				}
			}

			ResetF = false;
		}

		if (keys[DIK_UP] && preKeys[DIK_UP])
		{
			player.moveDirection = kMoveTypeUP;

		}
		else if (keys[DIK_DOWN] && !preKeys[DIK_DOWN])
		{
			player.moveDirection = kMoveTypeDOWN;
		}
		else if (keys[DIK_RIGHT] && !preKeys[DIK_RIGHT])
		{
			player.moveDirection = kMoveTypeRIGHT;
		}
		else if (keys[DIK_LEFT] && !preKeys[DIK_LEFT])
		{
			player.moveDirection = kMoveTypeLEFT;
		}

		if (timer % 5 == 0)
		{
			if (player.moveDirection == kMoveTypeUP)
			{//前が上移動

				//範囲内なら動く
				if (player.mapPos.y > 0)
				{
					if (!player.moveFlag && map[player.mapPos.y][player.mapPos.x] == kChipTypeStrtEnd)
					{
						//導線のおわりの右に出る
						player.mapPos.y -= 1;
						//導線のおわりに別の導線があるとしぬ
						if (map[player.mapPos.y][player.mapPos.x] == kChipTypeMiddle)player.death = true;
						player.moveDirection = kMoveTypeNONE;

						map[player.mapPos.y + 1][player.mapPos.x] = kChipTypeNone;//前フレに踏んだ導線のおわりを消す
					}
					else if (map[player.mapPos.y - 1][player.mapPos.x] == kChipTypeStrtEnd)
					{
						//はじめとおわり
						player.mapPos.y -= 1;
						player.moveDirection = kMoveTypeUP;
						player.moveFlag = !player.moveFlag;
						if (player.moveFlag)
						{
							map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
						}
					}
					else if (map[player.mapPos.y - 1][player.mapPos.x] == kChipTypeMiddle)
					{
						//導線の移動

						player.mapPos.y -= 1;
						player.moveDirection = kMoveTypeUP;
						map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
					}
					else if (map[player.mapPos.y - 1][player.mapPos.x] == kChipTypeNone)//移動先に何もない時
					{
						player.moveDirection = kMoveTypeNONE;

					}
				}
				else
				{
					player.moveFlag = false;
					player.moveDirection = kMoveTypeNONE;

				}
			}
			else if (player.moveDirection == kMoveTypeDOWN)
			{
				if (player.mapPos.y < mapSizeY - 1)
				{
					//前が下移動
					if (!player.moveFlag && map[player.mapPos.y][player.mapPos.x] == kChipTypeStrtEnd)
					{
						//導線のおわりの下に出る
						player.mapPos.y += 1;
						//導線のおわりに別の導線があるとしぬ
						if (map[player.mapPos.y][player.mapPos.x] == kChipTypeMiddle)player.death = true;
						player.moveDirection = kMoveTypeNONE;
						map[player.mapPos.y - 1][player.mapPos.x] = kChipTypeNone;//前フレに踏んだ導線のおわりを消す
					}
					else if (map[player.mapPos.y + 1][player.mapPos.x] == kChipTypeStrtEnd)
					{
						//はじめとおわり
						player.mapPos.y += 1;
						player.moveDirection = kMoveTypeDOWN;
						player.moveFlag = !player.moveFlag;
						if (player.moveFlag)
						{
							map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
						}
					}
					else if (map[player.mapPos.y + 1][player.mapPos.x] == kChipTypeMiddle)
					{
						//導線の移動

						player.mapPos.y += 1;
						player.moveDirection = kMoveTypeDOWN;
						map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
					}
					else if (map[player.mapPos.y + 1][player.mapPos.x] == kChipTypeNone)//移動先に何もない時
					{
						player.moveDirection = kMoveTypeNONE;

					}
				}
				else
				{
					player.moveFlag = false;
					player.moveDirection = kMoveTypeNONE;

				}
			}
			else if (player.moveDirection == kMoveTypeRIGHT)
			{
				if (player.mapPos.x < mapSizeX - 1)
				{
					//前が右移動
					if (!player.moveFlag && map[player.mapPos.y][player.mapPos.x] == kChipTypeStrtEnd)//前フレで導線のおわりにいた時
					{

						//導線のおわりの右に出る
						player.mapPos.x += 1;
						//導線のおわりに別の導線があるとしぬ
						if (map[player.mapPos.y][player.mapPos.x] == kChipTypeMiddle)player.death = true;
						player.moveDirection = kMoveTypeNONE;
						map[player.mapPos.y][player.mapPos.x - 1] = kChipTypeNone;//前フレに踏んだ導線のおわりを消す
					}
					else if (map[player.mapPos.y][player.mapPos.x + 1] == kChipTypeStrtEnd)
					{
						//はじめとおわり
						player.mapPos.x += 1;
						player.moveDirection = kMoveTypeRIGHT;
						player.moveFlag = !player.moveFlag;
						if (player.moveFlag)
						{
							map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
						}
					}
					else if (map[player.mapPos.y][player.mapPos.x + 1] == kChipTypeMiddle)
					{
						//導線の移動

						player.mapPos.x += 1;
						player.moveDirection = kMoveTypeRIGHT;
						map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
					}
					else if (map[player.mapPos.y][player.mapPos.x + 1] == kChipTypeNone)//移動先に何もない時
					{
						player.moveDirection = kMoveTypeNONE;

					}
				}
				else
				{
					player.moveFlag = false;
					player.moveDirection = kMoveTypeNONE;

				}
			}
			else if (player.moveDirection == kMoveTypeLEFT)
			{
				if (player.mapPos.x > 0)
				{
					//前が下移動
					if (!player.moveFlag && map[player.mapPos.y][player.mapPos.x] == kChipTypeStrtEnd)
					{
						//導線のおわりの左に出る
						player.mapPos.x -= 1;
						//導線のおわりに別の導線があるとしぬ
						if (map[player.mapPos.y][player.mapPos.x] == kChipTypeMiddle)player.death = true;
						player.moveDirection = kMoveTypeNONE;
						map[player.mapPos.y][player.mapPos.x + 1] = kChipTypeNone;//前フレに踏んだ導線のおわりを消す
					}
					else if (map[player.mapPos.y][player.mapPos.x - 1] == kChipTypeStrtEnd)
					{
						//はじめとおわり
						player.mapPos.x -= 1;
						player.moveFlag = !player.moveFlag;
						if (player.moveFlag)
						{
							map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
						}
					}
					else if (map[player.mapPos.y][player.mapPos.x - 1] == kChipTypeMiddle)
					{
						//導線の移動

						player.mapPos.x -= 1;
						player.moveDirection = kMoveTypeLEFT;
						map[player.mapPos.y][player.mapPos.x] = kChipTypeNone;//踏んだら消す
					}
					else if (map[player.mapPos.y][player.mapPos.x - 1] == kChipTypeNone)//移動先に何もない時
					{
						player.moveDirection = kMoveTypeNONE;

					}
				}
				else
				{
					player.moveFlag = false;
					player.moveDirection = kMoveTypeNONE;

				}
			}
		}

		timer++;
		///                                                            ///
		/// --------------------↑更新処理ここまで-------------------- ///
		///                                                            ///       

		///                                                            ///
		/// --------------------↓描画処理ここから-------------------- ///
		///                                                            ///

		for (int y = 0; y < mapSizeY; y++)
		{
			for (int x = 0; x < mapSizeX; x++)
			{
				if (map[y][x] == kChipTypeMiddle)
				{
					Novice::DrawEllipse(100 + x * blockSize, 100 + y * blockSize, blockRadius, blockRadius, 0, BLACK, kFillModeSolid);

				}
				if (map[y][x] == kChipTypeStrtEnd)
				{
					Novice::DrawEllipse(100 + x * blockSize, 100 + y * blockSize, blockRadius, blockRadius, 0, RED, kFillModeSolid);

				}

			}
		}


		Novice::DrawEllipse(100 + player.mapPos.x * blockSize, 100 + player.mapPos.y * blockSize, blockRadius, blockRadius, 0, BLUE, kFillModeSolid);

		Novice::ScreenPrintf(0, 500, "MoveType=%d player.move=%d player.death=%d pl.MapPos[%d][%d]", player.moveDirection, player.moveFlag, player.death, player.mapPos.y, player.mapPos.x);
		///                                                            ///
		/// --------------------↑描画処理ここまで-------------------- ///
		///                                                            ///       



		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
