#include<iostream>
#include<iomanip>
#include<string>
#include<conio.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int MAX_STAGE_HEIGHT = 10;
const int MAX_STAGE_WIDTH = 10;
bool Game_Clear = false;

struct Map
{
	int stage_width;//ステージの最大幅
	int stage_height;//ステージの最大高さ
	int Dat[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];
};
// 0　床
// 1　壁
// 2　荷物
// 3　ゴール(何も乗ってない状態)
// 4　人
// 5　人 on the ゴール
// 6　荷物 on the ゴール
// 7　壁の外のゲームに使わない領域
 const string OBJS[] = /*const char OBJS[] =*/{ " ","#","$",".","@","+","*"," " };

 //オブジェクトの名前
enum OBJNAME
{
	Floor,Wall,Lugg,Goal,Human,Human_on_goal,Lugg_on_goal
};

struct Point
{
	int x, y;
};

//入力方向
enum direction 
{
	UP,LEFT,DOWN,RIGHT,NONE
};

//入力方向の座標の移動
Point dirVector[5] =
{
	{0,-1},{-1,0},{0,+1},{+1,0},{0,0}
};

void UpdatePlay();
void DrawPlay();
void MoveObject(direction _dir, Map& _map);
bool CheckClear(Map& _map);

//サンプルステージの座標　オブジェクトを番号で指定
Map sampleStage = {
    5,5,
 {
	 /*{1,1,1,1,1,1,1,1,1,1},
	 {1,0,0,2,3,3,0,0,0,1},
	 {1,0,4,2,0,0,0,0,0,1},
	 {1,0,0,0,0,1,1,0,0,1},
	 {1,0,0,0,0,2,0,0,0,1},
	 {1,0,0,3,0,0,0,0,0,1},
	 {1,0,0,0,0,0,1,1,1,1},
	 {1,0,0,0,0,0,0,0,0,1},
	 {1,0,1,0,0,0,0,0,0,1},
	 {1,1,1,1,1,1,1,1,1,1}*/
	 {1,1,1,1,7,7,7,7,7,7},
	 {1,3,0,1,1,7,7,7,7,7},
	 {1,0,2,0,1,7,7,7,7,7},
	 {1,0,4,0,1,7,7,7,7,7},
	 {1,1,1,1,1,7,7,7,7,7},
	 {7,7,7,7,7,7,7,7,7,7},
	 {7,7,7,7,7,7,7,7,7,7},
	 {7,7,7,7,7,7,7,7,7,7},
	 {7,7,7,7,7,7,7,7,7,7}
	
 }
};


//ステージの描画
//_map　Mapの情報を参照渡し
 void DrawStage(Map& _map)
 {
	 for (int j = 0; j < _map.stage_height; j++)
	 {
		 for (int i = 0; i < _map.stage_width; i++)
		 {
			 cout << OBJS[_map.Dat[j][i]];
		 }
		 cout << endl;
	 }
 }

 //プレイヤー位置の取得
 //_map　Mapの情報を参照渡し
 Point GetPlayerPos(Map& _map)
 {
	 for (int j = 0; j < _map.stage_height; j++)
	 {
		 for (int i = 0; i < _map.stage_width; i++)
		 {
			 if (_map.Dat[j][i] == OBJNAME::Human || _map.Dat[j][i] == OBJNAME::Human_on_goal)
				 return { i,j };
		 }
	 }
	 return{ -1,-1 };
 }
 //オブジェクトをマップに設置
 //_obj 指定されたオブジェクトの名前[0~6]
 //_pos 指定された座標
 //_map　Mapの情報を参照渡し
 void SetObjectMap(OBJNAME _obj, Point _pos, Map& _map)
 {
	 //for (int j = 0; j < _map.stage_height; j++)
	 //{
		// for (int i = 0; i < _map.stage_width; i++)
		// {
		//	 if (_pos.x == i)//_posで指定された座標を見つけたら
		//		 if(_pos.y == j)
		//		    _map.Dat[j][i] = _obj;//指定されたオブジェクトに変更(_map.Dat[i][j]に対応する要素を_objにセット)
		// }
	 //}
	 if ((_pos.x >= 0) && (_pos.x <= _map.stage_width) && (_pos.y >= 0) && (_pos.x <= _map.stage_height))
	 {
		_map.Dat[_pos.y][_pos.x] = _obj;
	 }
 }
//取得した入力に応じた方向を返す
 //_c 取得した入力wasd
 direction GetDirection(char _c)
 {
	 switch (_c)
	 {
	 case 'a':
	 case 'A':
		 return LEFT;
		 break;

	 case 's':
	 case 'S':
		 return DOWN;
		 break;

	 case 'd':
	 case 'D':
		 return RIGHT;
		 break;

	 case 'w':
	 case 'W':
		 return UP;
		 break;
	 default:
		 return NONE;
		 break;
	 }
 }
 
 //その座標に対応するオブジェクト名を取得
 //_pos 指定された座標
 //_map　Mapの情報を参照渡し
 //return 取得したオブジェクト名をOBJNAME型で返す
 OBJNAME GetObjectNum(Point _pos, Map& _map)
 {
	 return (OBJNAME(_map.Dat[_pos.y][_pos.x]));
 }

 //オブジェクト移動用関数
 //_dir 入力方向
 //_map　Mapの情報を参照渡し
 void MoveObject(direction _dir, Map& _map)
 {

	 Point pp = GetPlayerPos(_map);//プレイヤーの位置を取得(現在地は人か人on the Goalしかない)
	 Point nextPos = { pp.x + dirVector[_dir].x, pp.y + dirVector[_dir].y};//移動先の予測位置
	 Point nextnextPos = { nextPos.x + dirVector[_dir].x, nextPos.y + dirVector[_dir].y };
	 OBJNAME crr = GetObjectNum(pp,_map);//ppのオブジェクト名を取得(判別)
	 OBJNAME next = GetObjectNum(nextPos, _map);/*OBJNAME(_map.Dat[nextPos.x][nextPos.y]);*///nextPosのオブジェクト名を取得(判別)
	 OBJNAME nextnext = GetObjectNum(nextnextPos, _map);

	 switch (next)//隣のオブジェクトが
	 {
		 case Floor://隣が床なら
			 SetObjectMap(OBJNAME::Human, nextPos,_map);//隣を人にする
			 if (crr == OBJNAME::Human_on_goal)//現在値に自分が人 on the ゴールだったら
				SetObjectMap(OBJNAME::Goal,pp,_map);//元の位置にゴールを置く
			 else
				SetObjectMap(OBJNAME::Floor,pp ,_map);//元の位置に床を設置
			 break;
		 case Wall://隣が壁なら
			 break;//何もしない
		 case Goal://隣がゴールなら
			 SetObjectMap(OBJNAME::Human_on_goal, nextPos,_map);//移動先を人 on the ゴールにする
			 if (crr == OBJNAME::Human_on_goal)//現在値が人 on the ゴールなら（2つ続いている状態）
				 SetObjectMap(OBJNAME::Goal,pp,_map);//現在値をゴールに
			 else
				 SetObjectMap(OBJNAME::Floor, pp, _map);//現在値を床にする
			 break;

		 case Lugg:
			 switch (nextnext)
			 {
				case Floor:
				 SetObjectMap(OBJNAME::Lugg, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//元の位置が人 on the ゴールなら
					 SetObjectMap(OBJNAME::Goal, pp, _map);//そこを空のゴールにする
				 else 
					 SetObjectMap(OBJNAME::Floor, pp, _map);

				 break;
			 
				case Goal:
				 SetObjectMap(OBJNAME::Lugg_on_goal,nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//元の位置が人 on the ゴールなら
					 SetObjectMap(OBJNAME::Goal, pp, _map);//そこを空のゴールにする
				 else 
					 SetObjectMap(OBJNAME::Floor, pp, _map);
				 break;
				case Wall:
				 break;
				default:
				 break;//次の位置が荷物や壁なら何もしない
			 }
			 break;
		 case Lugg_on_goal:
			 switch (nextnext)
			 {
			 case Floor:
				 SetObjectMap(OBJNAME::Lugg, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human_on_goal, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//元の位置が人 on the ゴールなら
					 SetObjectMap(OBJNAME::Goal, pp, _map);//そこを空のゴールにする
				 else
					 SetObjectMap(OBJNAME::Floor, pp, _map);
				 break;

			 case Goal:
				 SetObjectMap(OBJNAME::Lugg_on_goal, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human_on_goal, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//元の位置が人 on the ゴールなら
					 SetObjectMap(OBJNAME::Goal, pp, _map);//そこを空のゴールにする
				 else
					 SetObjectMap(OBJNAME::Floor, pp, _map);
				 break;
			 default:
				 break;
			 }
		default:
		 break;
	 }
 }
//int stageDate[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH] =
//{
//	{1,1,1,1,7,7,7,7,7,7},
//	{1,3,0,1,1,7,7,7,7,7},
//	{1,0,2,0,1,7,7,7,7,7},
//	{1,0,4,0,1,7,7,7,7,7},
//	{1,1,1,1,1,7,7,7,7,7},
//	{7,7,7,7,7,7,7,7,7,7},
//	{7,7,7,7,7,7,7,7,7,7},
//	{7,7,7,7,7,7,7,7,7,7},
//	{7,7,7,7,7,7,7,7,7,7},
//	{7,7,7,7,7,7,7,7,7,7}
//
//};
 bool CheckClear(Map& _map)
 {
	 for (int j = 0; j < _map.stage_height; j++)
	 {
		 for (int i = 0; i < _map.stage_width; i++)
		 {
			 if (_map.Dat[j][i] == OBJNAME::Lugg)
				 return false;
		 };
	 }
	 return true;
 }
 void DrawPlay()
 {
	 system("cls");
	 DrawStage(sampleStage);
	 if (CheckClear(sampleStage))
	 {
		 cout << "Clear!!" << endl;
	 }
	 else
	 {
		cout << "Input wasd to Move Costco M@n" << endl;
	 }
	 
 }
 void UpdatePlay()
 {
	 //int c = _getch();
	 //Point crrP = GetPlayerPos(sampleStage);//現在位置を取得
	 //direction nextdir = GetDirection(c);
	 //Point nextP = {crrP.x + dirVector[nextdir].x,crrP.y + dirVector[nextdir].y};//wasdの入力に応じて座標を変更
	 //if (c == 'd' || c == 'D')
	 //{
		// nextP = { crrP.x + 1, crrP.y + 0 };
	 //}
	 //else if (c == 'a' || c == 'A')
	 //{
		// nextP = { crrP.x - 1, crrP.y + 0 };
	 //}
	 //else if (c == 'w' || c == 'W')
	 //{
		// nextP = { crrP.x + 0, crrP.y - 1 };
	 //}
	 //else if (c == 's' || c == 'S')
	 //{
		// nextP = { crrP.x + 0, crrP.y + 1 };
	 //}
	 //else
	 //{
		// nextP = { crrP.x + 0, crrP.y + 0 };
	 //}*/
	 //SetObjectMap(OBJNAME::Floor, crrP, sampleStage);//元いた場所からプレイヤーを削除して床をセット
	 //SetObjectMap(OBJNAME::Human, nextP, sampleStage);//計算した位置にプレイヤーをセット

	 int c = _getch();//入力を取得
	 direction nextdir = GetDirection(c);
	 MoveObject(nextdir,sampleStage);
 }
 int main(){
	 
	 while (true)
	 {
		 DrawPlay();
		 UpdatePlay();
	 }
	 return 0;
 }