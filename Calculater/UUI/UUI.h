#ifndef UNLIMITED_UNSIGNED_INTEGER_H
#define UNLIMITED_UNSIGNED_INTEGER_H

#include <Windows.h>

#include <vector>

#include <UUICalculator.h>

class UUI
{
public:
	UUI(size_t size, ...)
	{
		m_digits.resize(size);

		va_list arguments;
		va_start(arguments, size);
	
		for (int i = 0; i < size; ++i)
		{
			m_digits[i] = va_arg(arguments, BYTE);
		}

		va_end(arguments);

		std::reverse(m_digits.begin(), m_digits.end());
	}

	~UUI()
	{

	}

	std::vector<BYTE> DigitValuesRef()
	{
		return m_digits;
	}

	void OutPut()
	{
		for (int i = static_cast<int>(m_digits.size()) - 1; i >= 0; --i)
		{
			std::cout << static_cast<int>(m_digits[i]);
		}

		std::cout << std::endl;
	};

	UUI& Pow(UINT y)
	{
		UUI powTmp(*this);
		
		if (y == 0)
		{
			m_digits.clear();
			m_digits.shrink_to_fit();
			m_digits.push_back(1);
		}

		for (UINT i = 0; i < y - 1 && y != 0; ++i)
		{
			(*this) *= powTmp;
		}

		return *this;
	}

	UUI& operator+=(const UUI& rhs)
	{
		UUICalculator::Add(&m_digits, m_digits, rhs.m_digits);

		return *this;
	}

	UUI& operator*=(const UUI& rhs)
	{
		UUICalculator::Multiply(&m_digits, m_digits, rhs.m_digits);

		return *this;
	}

	using DigitValues = std::vector<BYTE>;

private:
	DigitValues m_digits;
};

#endif // !UNLIMITED_UNSIGNED_INTEGER_H
