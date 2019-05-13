#include "UUI.h"

namespace uui
{
/// <summary>
/// vectorの最大の最大サイズまで桁を増やせる符号なし整数の計算を行うクラス
/// </summary>
class UUI::Calculator
{
public:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// コンストラクタ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Calculator()
	{

	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// デストラクタ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	~Calculator()
	{

	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 関数
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/// <summary>
	/// 足し算
	/// </summary>
	/// <param name="pResult">結果を入れるもの</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	void Add(DigitValues* pResult, const DigitValues& lhs, const DigitValues& rhs)const
	{
		//桁が大きいほうに合わせるため
		size_t interpolationSize = max(lhs.size(), rhs.size());

		DigitValues lhsInterpolated;
		Interporate(&lhsInterpolated, lhs, interpolationSize);

		DigitValues rhsInterpolated;
		Interporate(&rhsInterpolated, rhs, interpolationSize);

		for (int i = 0; i < static_cast<int>(interpolationSize); ++i)
		{
			AddFull(&lhsInterpolated, rhsInterpolated[i], i);
		}

		*pResult = lhsInterpolated;
	}

	/// <summary>
	/// 掛け算
	/// </summary>
	/// <param name="pResult">結果を入れるもの</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	void Multiply(DigitValues* pResult, const DigitValues& lhs, const DigitValues& rhs)const
	{
		//桁が大きいほうに合わせるため
		size_t interpolationSize = max(lhs.size(), rhs.size());

		DigitValues lhsInterpolated;
		Interporate(&lhsInterpolated, lhs, interpolationSize);

		DigitValues rhsInterpolated;
		Interporate(&rhsInterpolated, rhs, interpolationSize);

		DigitValues result;

		for (int i = 0; i < static_cast<int>(interpolationSize); ++i)
		{
			MultiplyFull(&result, i, lhsInterpolated, rhsInterpolated);
		}

		*pResult = result;
	}

private:
	BYTE DIGIT_VALUE_MAX = 9;

	Calculator(const Calculator& rhs) = delete;

	Calculator& operator=(const Calculator& rhs) = delete;

	/// <summary>
	/// 同位桁同士の足し算
	/// </summary>
	/// <param name="pSum">結果</param>
	/// <param name="pCarry">桁上げの値</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	void AddHalf(BYTE* pSum, BYTE* pCarry, BYTE lhs, BYTE rhs)const
	{
		BYTE addNum = lhs + rhs;

		*pCarry = addNum / (DIGIT_VALUE_MAX + 1);

		*pSum = addNum % (DIGIT_VALUE_MAX + 1);
	}

	/// <summary>
	/// 桁上げを行う同位桁同士の足し算
	/// </summary>
	/// <param name="pLhs">左辺全て</param>
	/// <param name="rhs">右辺の計算を行う値</param>
	/// <param name="digitNum">現在の桁数</param>
	void AddFull(DigitValues* pLhs, BYTE rhs, int digitNum)const
	{
		BYTE carry = 0;

		AddHalf(&(*pLhs)[digitNum], &carry, (*pLhs)[digitNum], rhs);

		if (carry == 0) return;

		//次のけたがない場合
		if (digitNum + 1 == pLhs->size())
		{
			pLhs->push_back(carry);

			return;
		}

		AddFull(pLhs, carry, digitNum + 1);
	}

	/// <summary>
	/// 桁の保管
	/// </summary>
	/// <param name="pInterpolated">保管されたものを入れる</param>
	/// <param name="origin">保管される前の値</param>
	/// <param name="interpolationSize">保管するサイズ</param>
	void Interporate(DigitValues* pInterpolated, const DigitValues& origin, size_t interpolationSize)const
	{
		*pInterpolated = origin;

		//新たに追加されたコンテナだけ0が入る
		pInterpolated->resize(interpolationSize, 0);
	}

	/// <summary>
	/// 同位桁同士の掛け算
	/// </summary>
	/// <param name="pResult">結果</param>
	/// <param name="pCarry">桁上げの値</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	void MultiplyHalf(BYTE* pResult, BYTE* pCarry, BYTE lhs, BYTE rhs)const
	{
		BYTE multiNum = lhs * rhs;

		*pCarry = multiNum / (DIGIT_VALUE_MAX + 1);

		*pResult = multiNum % (DIGIT_VALUE_MAX + 1);
	}

	/// <summary>
	/// サイズの拡張
	/// </summary>
	/// <param name="pInterpolated">補間されるもの</param>
	/// <param name="interpolationSize">増やした際の全体でのサイズ</param>
	void SizeUp(DigitValues* pInterpolated, size_t interpolationSize)const
	{
		if (pInterpolated->size() >= interpolationSize) return;

		//新たに追加されたコンテナだけ0が入る
		pInterpolated->resize(interpolationSize, 0);
	}

	/// <summary>
	/// 安全な一桁に対する代入を行う
	/// </summary>
	/// <param name="pDigits">代入されるもの</param>
	/// <param name="digitValue">代入される値</param>
	/// <param name="digitNum">代入する桁</param>
	void SubstituteDigitNum(DigitValues* pDigits, BYTE digitValue, int digitNum)const
	{
		if (digitValue == 0) return;

		//サイズを入れるので桁+1
		SizeUp(pDigits, digitNum + 1);

		(*pDigits)[digitNum] = digitValue;
	}

	/// <summary>
	/// 計算結果及び繰上りを保存する一桁の掛け算
	/// </summary>
	/// <param name="pResultBuff">掛けた時の繰上りなしの値をいれるもの</param>
	/// <param name="pCarryBuff">繰り上げを入れるもの</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	/// <param name="digitNum">掛ける側の桁</param>
	/// <param name="digitNumMultiplied">掛け算先の桁</param>
	void MultiplyHalfAndKeepResultAndCarry(DigitValues* pResultBuff, DigitValues* pCarryBuff,
		BYTE lhs, BYTE rhs, int digitNumMultiplys, int digitNumMultiplied)const
	{
		int resultDigitsNum = digitNumMultiplys + digitNumMultiplied;

		//繰り上がった数なのでさらに+1
		int carryDigitsNum = resultDigitsNum + 1;

		BYTE result = 0;
		BYTE carry = 0;

		MultiplyHalf(&result, &carry, lhs, rhs);

		SubstituteDigitNum(pResultBuff, result, resultDigitsNum);
		SubstituteDigitNum(pCarryBuff, carry, carryDigitsNum);
	}

	/// <summary>
	/// 一桁全てを右辺に掛け合わせる
	/// </summary>
	/// <param name="pResult">結果を入れるもの</param>
	/// <param name="digitNumMultiply">掛ける側の要素番号</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	void MultiplyFull(DigitValues* pResult, int digitNumMultiplys, const DigitValues& lhs, const DigitValues& rhs)const
	{
		DigitValues carryBuffer;
		DigitValues resultBuffer;

		int multipliedSize = static_cast<int>(rhs.size());

		for (int i = 0; i < multipliedSize; ++i)
		{
			MultiplyHalfAndKeepResultAndCarry(&resultBuffer, &carryBuffer, lhs[i], rhs[digitNumMultiplys], digitNumMultiplys, i);
		}

		DigitValues resultComponent;
		Add(&resultComponent, resultBuffer, carryBuffer);

		Add(pResult, *pResult, resultComponent);
	}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// コンストラクタ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UUI::UUI(size_t size, ...)
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

UUI::UUI(const UUI& value) :m_pCalculator(new Calculator())
{
	(*this) = value;
}

UUI::UUI() : m_pCalculator(new Calculator())
{
	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// デストラクタ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UUI::~UUI()
{
	m_pCalculator.release();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 関数
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void UUI::OutPut()const
{
	for (int i = static_cast<int>(m_digits.size()) - 1; i >= 0; --i)
	{
		std::cout << static_cast<int>(m_digits[i]);
	}

	std::cout << std::endl;
}

UUI& UUI::Pow(UUI& x, UINT y)
{
	m_digits = x.m_digits;

	if (y == 0)
	{
		m_digits.clear();
		m_digits.shrink_to_fit();
		m_digits.push_back(1);
	}

	for (UINT i = 0; i < y - 1; ++i)
	{
		m_pCalculator->Multiply(&m_digits, m_digits, x.m_digits);
	}

	return *this;
}

UUI& UUI::Pow(UINT y)
{
	return Pow(*this, y);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// オペレータ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UUI& UUI::operator=(const UUI& rhs)
{
	m_digits = rhs.m_digits;

	return *this;
}

UUI UUI::operator+(const UUI& rhs)const
{
	UUI result;
	m_pCalculator->Add(&result.m_digits, m_digits, rhs.m_digits);

	return result;
}

UUI& UUI::operator+=(const UUI& rhs)
{
	return (*this) = (*this) + rhs;
}

UUI UUI::operator*(const UUI& rhs)const
{
	UUI result;
	m_pCalculator->Multiply(&result.m_digits, m_digits, rhs.m_digits);

	return result;
}

UUI& UUI::operator*=(const UUI& rhs)
{
	return (*this) = (*this) * rhs;
}

BYTE& UUI::operator[](const size_t& index)
{
	return m_digits[index];
}
}// namespace uui
