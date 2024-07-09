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
	int stage_width;//�X�e�[�W�̍ő啝
	int stage_height;//�X�e�[�W�̍ő卂��
	int Dat[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];
};
// 0�@��
// 1�@��
// 2�@�ו�
// 3�@�S�[��(��������ĂȂ����)
// 4�@�l
// 5�@�l on the �S�[��
// 6�@�ו� on the �S�[��
// 7�@�ǂ̊O�̃Q�[���Ɏg��Ȃ��̈�
 const string OBJS[] = /*const char OBJS[] =*/{ " ","#","$",".","@","+","*"," " };

 //�I�u�W�F�N�g�̖��O
enum OBJNAME
{
	Floor,Wall,Lugg,Goal,Human,Human_on_goal,Lugg_on_goal
};

struct Point
{
	int x, y;
};

//���͕���
enum direction 
{
	UP,LEFT,DOWN,RIGHT,NONE
};

//���͕����̍��W�̈ړ�
Point dirVector[5] =
{
	{0,-1},{-1,0},{0,+1},{+1,0},{0,0}
};

void UpdatePlay();
void DrawPlay();
void MoveObject(direction _dir, Map& _map);
bool CheckClear(Map& _map);

//�T���v���X�e�[�W�̍��W�@�I�u�W�F�N�g��ԍ��Ŏw��
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


//�X�e�[�W�̕`��
//_map�@Map�̏����Q�Ɠn��
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

 //�v���C���[�ʒu�̎擾
 //_map�@Map�̏����Q�Ɠn��
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
 //�I�u�W�F�N�g���}�b�v�ɐݒu
 //_obj �w�肳�ꂽ�I�u�W�F�N�g�̖��O[0~6]
 //_pos �w�肳�ꂽ���W
 //_map�@Map�̏����Q�Ɠn��
 void SetObjectMap(OBJNAME _obj, Point _pos, Map& _map)
 {
	 //for (int j = 0; j < _map.stage_height; j++)
	 //{
		// for (int i = 0; i < _map.stage_width; i++)
		// {
		//	 if (_pos.x == i)//_pos�Ŏw�肳�ꂽ���W����������
		//		 if(_pos.y == j)
		//		    _map.Dat[j][i] = _obj;//�w�肳�ꂽ�I�u�W�F�N�g�ɕύX(_map.Dat[i][j]�ɑΉ�����v�f��_obj�ɃZ�b�g)
		// }
	 //}
	 if ((_pos.x >= 0) && (_pos.x <= _map.stage_width) && (_pos.y >= 0) && (_pos.x <= _map.stage_height))
	 {
		_map.Dat[_pos.y][_pos.x] = _obj;
	 }
 }
//�擾�������͂ɉ�����������Ԃ�
 //_c �擾��������wasd
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
 
 //���̍��W�ɑΉ�����I�u�W�F�N�g�����擾
 //_pos �w�肳�ꂽ���W
 //_map�@Map�̏����Q�Ɠn��
 //return �擾�����I�u�W�F�N�g����OBJNAME�^�ŕԂ�
 OBJNAME GetObjectNum(Point _pos, Map& _map)
 {
	 return (OBJNAME(_map.Dat[_pos.y][_pos.x]));
 }

 //�I�u�W�F�N�g�ړ��p�֐�
 //_dir ���͕���
 //_map�@Map�̏����Q�Ɠn��
 void MoveObject(direction _dir, Map& _map)
 {

	 Point pp = GetPlayerPos(_map);//�v���C���[�̈ʒu���擾(���ݒn�͐l���lon the Goal�����Ȃ�)
	 Point nextPos = { pp.x + dirVector[_dir].x, pp.y + dirVector[_dir].y};//�ړ���̗\���ʒu
	 Point nextnextPos = { nextPos.x + dirVector[_dir].x, nextPos.y + dirVector[_dir].y };
	 OBJNAME crr = GetObjectNum(pp,_map);//pp�̃I�u�W�F�N�g�����擾(����)
	 OBJNAME next = GetObjectNum(nextPos, _map);/*OBJNAME(_map.Dat[nextPos.x][nextPos.y]);*///nextPos�̃I�u�W�F�N�g�����擾(����)
	 OBJNAME nextnext = GetObjectNum(nextnextPos, _map);

	 switch (next)//�ׂ̃I�u�W�F�N�g��
	 {
		 case Floor://�ׂ����Ȃ�
			 SetObjectMap(OBJNAME::Human, nextPos,_map);//�ׂ�l�ɂ���
			 if (crr == OBJNAME::Human_on_goal)//���ݒl�Ɏ������l on the �S�[����������
				SetObjectMap(OBJNAME::Goal,pp,_map);//���̈ʒu�ɃS�[����u��
			 else
				SetObjectMap(OBJNAME::Floor,pp ,_map);//���̈ʒu�ɏ���ݒu
			 break;
		 case Wall://�ׂ��ǂȂ�
			 break;//�������Ȃ�
		 case Goal://�ׂ��S�[���Ȃ�
			 SetObjectMap(OBJNAME::Human_on_goal, nextPos,_map);//�ړ����l on the �S�[���ɂ���
			 if (crr == OBJNAME::Human_on_goal)//���ݒl���l on the �S�[���Ȃ�i2�����Ă����ԁj
				 SetObjectMap(OBJNAME::Goal,pp,_map);//���ݒl���S�[����
			 else
				 SetObjectMap(OBJNAME::Floor, pp, _map);//���ݒl�����ɂ���
			 break;

		 case Lugg:
			 switch (nextnext)
			 {
				case Floor:
				 SetObjectMap(OBJNAME::Lugg, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//���̈ʒu���l on the �S�[���Ȃ�
					 SetObjectMap(OBJNAME::Goal, pp, _map);//��������̃S�[���ɂ���
				 else 
					 SetObjectMap(OBJNAME::Floor, pp, _map);

				 break;
			 
				case Goal:
				 SetObjectMap(OBJNAME::Lugg_on_goal,nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//���̈ʒu���l on the �S�[���Ȃ�
					 SetObjectMap(OBJNAME::Goal, pp, _map);//��������̃S�[���ɂ���
				 else 
					 SetObjectMap(OBJNAME::Floor, pp, _map);
				 break;
				case Wall:
				 break;
				default:
				 break;//���̈ʒu���ו���ǂȂ牽�����Ȃ�
			 }
			 break;
		 case Lugg_on_goal:
			 switch (nextnext)
			 {
			 case Floor:
				 SetObjectMap(OBJNAME::Lugg, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human_on_goal, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//���̈ʒu���l on the �S�[���Ȃ�
					 SetObjectMap(OBJNAME::Goal, pp, _map);//��������̃S�[���ɂ���
				 else
					 SetObjectMap(OBJNAME::Floor, pp, _map);
				 break;

			 case Goal:
				 SetObjectMap(OBJNAME::Lugg_on_goal, nextnextPos, _map);
				 SetObjectMap(OBJNAME::Human_on_goal, nextPos, _map);
				 if (crr == OBJNAME::Human_on_goal)//���̈ʒu���l on the �S�[���Ȃ�
					 SetObjectMap(OBJNAME::Goal, pp, _map);//��������̃S�[���ɂ���
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
	 //Point crrP = GetPlayerPos(sampleStage);//���݈ʒu���擾
	 //direction nextdir = GetDirection(c);
	 //Point nextP = {crrP.x + dirVector[nextdir].x,crrP.y + dirVector[nextdir].y};//wasd�̓��͂ɉ����č��W��ύX
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
	 //SetObjectMap(OBJNAME::Floor, crrP, sampleStage);//�������ꏊ����v���C���[���폜���ď����Z�b�g
	 //SetObjectMap(OBJNAME::Human, nextP, sampleStage);//�v�Z�����ʒu�Ƀv���C���[���Z�b�g

	 int c = _getch();//���͂��擾
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