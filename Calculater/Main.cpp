#include <conio.h>
#include <iostream>
#include <random>

#include "UUI.h"
#include "UUICalculator.h"

int main()
{
	UUI a = { 1 };
	UUI b = { 4, 0, 5, 0, 9, 9, 9, 1 };

	UUI c;

	/*auto CreateRandom = [&](UUI* pUnlimitedInt)
	{
		static std::random_device randDevice;
		static std::minstd_rand randGenerator(randDevice());
		static std::uniform_int_distribution<> byteRangeRand(0, UCHAR_MAX);
		static std::uniform_int_distribution<> digitRangeRand(0, 9);

		for (int i = 1; i >= 0; --i)
		{
			pUnlimitedInt->push_back(digitRangeRand(randGenerator));
		}
	};

	CreateRandom(&a);
	CreateRandom(&b);*/

	auto OutPutUnlimitedInt = [&](UUI& unlimitedInt)
	{
		for (int i = static_cast<int>(unlimitedInt.size()) - 1; i >= 0; --i)
		{
			std::cout << static_cast<int>(unlimitedInt[i]);
		}

		std::cout << std::endl;
	};

	//OutPutUnlimitedInt(a);
	//OutPutUnlimitedInt(b);

	for (int i = 0; i < 6; ++i)
	{
		UUICalculator::Multiply(&a, a, b);

		OutPutUnlimitedInt(a);
	}

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


