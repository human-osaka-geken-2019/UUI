#ifndef UNLIMITE_UNSIGNED_INTEGER_CALCULATORH
#define UNLIMITE_UNSIGNED_INTEGER_CALCULATORH

#include <Windows.h>

#include <functional>

#include "UUI.h"

/// <summary>
/// UnlimitedUnsignedIntの計算系
/// </summary>
class UUICalculator
{
public:
	/// <summary>
	/// 同位桁同士の足し算
	/// </summary>
	/// <param name="pSum">結果</param>
	/// <param name="pCarry">桁上げの値</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	/// <returns></returns>
	static inline BYTE AddHalf(BYTE* pSum, BYTE* pCarry, BYTE lhs, BYTE rhs)
	{
		BYTE addNum = lhs + rhs;

		*pCarry = addNum / (DIGIT_VALUE_MAX + 1);

		return *pSum = addNum % (DIGIT_VALUE_MAX + 1);
	}

	/// <summary>
	/// 桁上げを行う同位桁同士の足し算
	/// </summary>
	/// <param name="pLhs">左辺全て</param>
	/// <param name="rhs">右辺の計算を行う値</param>
	/// <param name="digitNum">現在の桁数</param>
	static inline void AddFull(UUI* pLhs, BYTE rhs, int digitNum)
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
	};

	static void Interporate(UUI* pInterpolated, const UUI& origin, size_t interpolationSize)
	{
		*pInterpolated = origin;

		//新たに追加されたコンテナだけ0が入る
		pInterpolated->resize(interpolationSize, 0);
	}

	static void Add(UUI* pResult, const UUI& lhs, const UUI& rhs)
	{
		//桁が大きいほうに合わせるため
		size_t interpolationSize = max(lhs.size(), rhs.size());

		UUI lhsInterpolated;
		Interporate(&lhsInterpolated, lhs, interpolationSize);

		UUI rhsInterpolated;
		Interporate(&rhsInterpolated, rhs, interpolationSize);

		for (int i = 0; i < static_cast<int>(interpolationSize); ++i)
		{
			AddFull(&lhsInterpolated, rhsInterpolated[i], i);
		}

		*pResult = lhsInterpolated;
	}
	
	/// <summary>
	/// 同位桁同士の掛け算
	/// </summary>
	/// <param name="pResult">結果</param>
	/// <param name="pCarry">桁上げの値</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	/// <returns></returns>
	static inline BYTE MultiplyHalf(BYTE* pResult, BYTE* pCarry, BYTE lhs, BYTE rhs)
	{
		BYTE multiNum = lhs * rhs;

		*pCarry = multiNum / (DIGIT_VALUE_MAX + 1);

		return *pResult = multiNum % (DIGIT_VALUE_MAX + 1);
	}

	/// <summary>
	/// サイズの拡張
	/// </summary>
	/// <param name="pInterpolated">補間されるもの</param>
	/// <param name="interpolationSize">増やした際の全体でのサイズ</param>
	static void SizeUp(UUI* pInterpolated, size_t interpolationSize)
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
	static void SubstituteDigitNum(UUI* pDigits, BYTE digitValue, int digitNum)
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
	static inline void MultiplyHalfAndKeepResultAndCarry(UUI* pResultBuff, UUI* pCarryBuff, 
		BYTE lhs, BYTE rhs, int digitNumMultiplys, int digitNumMultiplied)
	{
		int resultDigitsNum = digitNumMultiplys + digitNumMultiplied;

		//繰り上がった数なのでさらに+1
		int carryDigitsNum = resultDigitsNum + 1;

		BYTE result = 0;
		BYTE carry = 0;

		MultiplyHalf(&result, &carry, lhs, rhs);

		SubstituteDigitNum(pResultBuff, result, resultDigitsNum);
		SubstituteDigitNum(pCarryBuff, carry, carryDigitsNum);
	};
	
	/// <summary>
	/// 一桁全てを右辺に掛け合わせる
	/// </summary>
	/// <param name="pResult">結果を入れるもの</param>
	/// <param name="digitNumMultiply">掛ける側の要素番号</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	static inline void MultiplyFull(UUI* pResult, int digitNumMultiplys, const UUI& lhs, const UUI& rhs)
	{
		UUI carryBuffer;
		UUI resultBuffer;

		int multipliedSize = static_cast<int>(rhs.size());

		for (int i = 0; i < multipliedSize; ++i)
		{
			MultiplyHalfAndKeepResultAndCarry(&resultBuffer, &carryBuffer, lhs[i], rhs[digitNumMultiplys], digitNumMultiplys, i);
		}

		UUI resultComponent;
		Add(&resultComponent, resultBuffer, carryBuffer);

		Add(pResult, *pResult, resultComponent);
	};

	/// <summary>
	/// 掛け算
	/// </summary>
	/// <param name="pResult">結果を入れるもの</param>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	static void Multiply(UUI* pResult, const UUI& lhs, const UUI& rhs)
	{
		//桁が大きいほうに合わせるため
		size_t interpolationSize = max(lhs.size(), rhs.size());

		UUI lhsInterpolated;
		Interporate(&lhsInterpolated, lhs, interpolationSize);

		UUI rhsInterpolated;
		Interporate(&rhsInterpolated, rhs, interpolationSize);

		for (int i = 0; i < static_cast<int>(interpolationSize); ++i)
		{
			MultiplyFull(pResult, i, lhsInterpolated, rhsInterpolated);
		}
	}

	static const BYTE DIGIT_VALUE_MAX = 9;

private:
	UUICalculator()
	{

	}

	~UUICalculator()
	{

	}
};

#endif // !UNLIMITE_UNSIGNED_INTEGER_CALCULATORH

///// <summary>
///// キャリー先読みなしの加算器
///// </summary>
///// <param name="pResult">計算結果を保管する</param>
///// <param name="lhs">足し合わせる値1</param>
///// <param name="rhs">足し合わせる値2</param>
//static void Calc(UUI* pResult, const UUI& lhs, const UUI& rhs)
//{
//	//桁が大きいほうに合わせるため
//	size_t interpolationSize = max(lhs.size(), rhs.size());
//
//	UUI& rResult = *(std::move(pResult));
//	rResult.resize(interpolationSize);
//
//	UUI lhsInterpolatedBits;
//	UUI rhsInterpolatedBits;
//	InterpolateBits(2, interpolationSize, &lhsInterpolatedBits, &lhs, &rhsInterpolatedBits, &rhs);
//
//	//桁上がり
//	BYTE carry = 0b00000000;
//	BYTE prevCarry = 0b00000000;
//
//	//計算結果
//	BYTE sum = 0b00000000;
//
//	for (int si = 0; si < interpolationSize; ++si)
//	{
//		for (int bi = 0; bi < CHAR_BIT; ++bi)
//		{
//			AddBit(si, bi, &sum, &carry, &prevCarry, interpolationSize, pResult, lhsInterpolatedBits, rhsInterpolatedBits);
//		}
//	}
//}
//
//static inline void AddBit(const size_t& sizeItr, int bitItr, BYTE* pSum, BYTE* pCarry, BYTE* pPrevCarry,
//	const size_t& interpolationSize, UUI* pResult, const UUI& lhsInterpolatedBits, const UUI& rhsInterpolatedBits)
//{
//	BYTE lhsBit = lhsBit = MaskLSB(lhsInterpolatedBits[sizeItr] >> bitItr);
//	BYTE rhsBit = rhsBit = MaskLSB(rhsInterpolatedBits[sizeItr] >> bitItr);
//
//	if (bitItr == 0)
//	{
//		AddHalf(pSum, pCarry, lhsBit, rhsBit);
//
//		pResult->at(sizeItr) |= (*pSum);
//
//		return;
//	}
//
//	AddFull(pSum, pCarry, lhsBit, rhsBit, *pCarry);
//
//	pResult->at(sizeItr) |= (*pSum) << bitItr;
//
//	if (*pCarry != 0 && *pPrevCarry == 1 &&
//		sizeItr == interpolationSize - 1 && bitItr == CHAR_BIT - 1 && lhsBit == rhsBit)
//	{
//		IncreaseSize(pResult, lhsBit, rhsBit);
//
//		return;
//	}
//
//	*pPrevCarry = *pCarry;
//}
//
//static inline void IncreaseSize(UUI* pResult, BYTE lhsBit, BYTE rhsBit)
//{
//	BYTE increaseByte = (lhsBit == 0) ? 0b00000000 : 0b11111111;
//
//	pResult->push_back(increaseByte);
//
//	return;
//}
//
//static inline BYTE AddFull(BYTE* pSum, BYTE* pCarry, BYTE lhs, BYTE rhs, BYTE xCarry)
//{
//	BYTE carry = 0b00000000;
//	BYTE sum = 0b00000000;
//	AddHalf(&sum, &carry, lhs, rhs);
//
//	BYTE carry2 = 0b00000000;
//	AddHalf(pSum, &carry2, sum, xCarry);
//
//	*pCarry = carry | carry2;
//
//	return *pSum;
//}
//
//static inline BYTE AddHalf(BYTE* pSum, BYTE* pCarry, BYTE lhs, BYTE rhs)
//{
//	*pCarry = lhs & rhs;
//
//	return *pSum = lhs ^ rhs;
//}
//
//static inline BYTE MaskLSB(BYTE origin)
//{
//	return origin & 0b00000001;
//}
//
//static inline void InterpolateBits(int size, size_t interpolationSize, ...)
//{
//	va_list pUnlimitedIntList;
//	va_start(pUnlimitedIntList, interpolationSize);
//
//	while ((--size) >= 0)
//	{
//		UUI* pResult = va_arg(pUnlimitedIntList, UUI*);
//
//		InterpolateBits(pResult, va_arg(pUnlimitedIntList, UUI*), interpolationSize);
//	}
//
//	va_end(pUnlimitedIntList);
//}
//
//static inline void InterpolateBits(UUI* pResult, const UUI& origin, size_t size)
//{
//	size_t originSize = origin.size();
//	size_t sizeDiff = size - originSize;
//
//	if (sizeDiff < 0) return;
//
//	UUI& rResult = *(std::move(pResult));
//	rResult = origin;
//
//	if (originSize == size) return;
//
//	bool isMinus = rResult[rResult.size() - 1] >> (CHAR_BIT - 1);
//
//	for (int i = 0; i < sizeDiff; ++i)
//	{
//		rResult.push_back((isMinus) ? 0b00000001 : 0b00000000);
//	}
//}
//
//static inline void InterpolateBits(UUI* pResult, const UUI* pOrigin, size_t size)
//{
//	size_t originSize = pOrigin->size();
//	size_t sizeDiff = size - originSize;
//
//	if (sizeDiff < 0) return;
//
//	UUI& rResult = *(std::move(pResult));
//	rResult = *pOrigin;
//
//	if (originSize == size) return;
//
//	bool isMinus = rResult[rResult.size() - 1] >> (CHAR_BIT - 1);
//
//	for (int i = 0; i < sizeDiff; ++i)
//	{
//		rResult.push_back((isMinus) ? 0b00000001 : 0b00000000);
//	}
//}
