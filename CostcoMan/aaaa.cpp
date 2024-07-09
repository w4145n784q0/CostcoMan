#include<iostream>
#include<thread>
#include<string>
using namespace std;

int main()
{
	string str = "_________________0";
	string mleft;
	while (true)
	{
		mleft = str.back();
		str.pop_back();
		str = mleft + str;
		system("cls");
		cout << str << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}



}