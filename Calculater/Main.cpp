#include <conio.h>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <random>

#include "UUI.h"

using uui::UUI;

int main()
{
	auto OutPutString = [](const char* pString)
	{
		std::cout << pString << std::flush;
	};

	auto OutPutFixedFloat = [](double value, int precisionPlace)
	{
		std::cout << std::fixed;
		std::cout << std::setprecision(precisionPlace) << value << std::flush;
		std::cout << std::defaultfloat;
	};

	OutPutString("Q2\n\n");

	OutPutString("誕生日 : ");
	UUI myBirth(8, 1, 9, 9, 9, 0, 5, 0, 4);
	myBirth.OutPut();

	OutPutString("\n誕生日の6乗 : ");
	UUI myBirthPowed6;
	myBirthPowed6.Pow(myBirth, 6);
	myBirthPowed6.OutPut();

	OutPutString("\nQ3\n\n");

	double K = 1.9990504;
	double time_sec = 3.2;

	//移動距離
	double movement = 0.0;

	//小数点以下を破棄した時間
	int flooredTime_sec = static_cast<int>(time_sec);

	//1.0秒経過毎に速度がK倍されるため
	for (int i = 0; i <= flooredTime_sec; ++i)
	{
		//i秒経過時の速度、初速度は1.0
		double velocity = pow(K, i);

		//0秒の場合移動を行っていないので0.0を返す
		movement += ((i == 0) ? 0.0 : velocity);

		//time_secの小数部分摘出
		double decimalPartOfTime_sec = time_sec - flooredTime_sec;

		if (i != flooredTime_sec) continue;

		OutPutFixedFloat(time_sec, 1);
		OutPutString("秒後の速度 : ");
		OutPutFixedFloat(velocity, 2);

		//time_secの小数部分がない場合decimalPartOfTime_secが0.0になるため問題ない
		movement += velocity * decimalPartOfTime_sec;

		OutPutString("\n\n距離 : ");
		OutPutFixedFloat(movement, 2);
	}

	//終了するのを止めるため
	_getch();

	return 0;
}
