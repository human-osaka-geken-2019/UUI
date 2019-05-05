#include <conio.h>
#include <iostream>
#include <random>
#include <iomanip>

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
	double time = 3.2;

	OutPutFixedFloat(time, 1);
	OutPutString("秒後の速度 : ");

	//1秒経過するごとに速度がK倍し、初速度が1.0のためK^timeはtime時点での速度となる
	//timeが0の時K^0となり速度は1.0になる
	double velocity = std::pow(K, time);
	OutPutFixedFloat(velocity, 2);

	OutPutString("\n\n距離 : ");
	double movement = velocity * time;
	OutPutFixedFloat(movement, 2);

	//終了するのを止めるため
	_getch();

	return 0;
}
