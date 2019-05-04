#include <conio.h>
#include <iostream>
#include <random>
#include <iomanip>

#include "UUI.h"

int main()
{
	UUI myBirth(8, 1, 9, 9, 9, 0, 5, 0, 4);

	myBirth.OutPut();

	myBirth.Pow(6);

	myBirth.OutPut();

	double K = 1.9990504;

	double time = 3.2;

	double velocity = std::pow(K, time);
	double movement = velocity * time;

	std::cout << std::fixed;
	std::cout << std::setprecision(2) << velocity << std::endl;
	std::cout << std::setprecision(2) << movement << std::endl;
	std::cout << std::defaultfloat;

	/*UUI ans;
	Adder::Calc(&ans, a, b);

	auto CoutBits = [&](const UUI& unlimitedInt)
	{
		for (int di = static_cast<int>(unlimitedInt.size() - 1); di >= 0; --di)
		{
			for (int bi = CHAR_BIT - 1; bi >= 0; --bi)
			{
				std::cout << static_cast<int>(Adder::MaskLSB(unlimitedInt[di] >> bi));
			}
		}

		std::cout << std::endl;
	};

	CoutBits(a);
	CoutBits(b);
	CoutBits(ans);*/

	//終了するのを止めるため
	_getch();

	return 0;
}


