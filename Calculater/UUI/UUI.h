#ifndef UNLIMITED_UNSIGNED_INTEGER_H
#define UNLIMITED_UNSIGNED_INTEGER_H

#include <Windows.h>

#include <iostream>
#include <vector>
#include <memory>

namespace uui
{
/// <summary>
/// vectorの最大の最大サイズまで桁を増やせる符号なし整数
/// </summary>
class UUI
{
public:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// コンストラクタ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/// <summary>
	/// 引数で渡した値を代入する
	/// </summary>
	/// <param name="size">桁の数</param>
	/// <param name="">値(一桁ずつ)</param>
	UUI(size_t size, ...);

	/// <summary>
	/// m_digitsのコピー
	/// </summary>
	/// <param name="value">コピー元</param>
	UUI(const UUI& value);

	UUI();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// デストラクタ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	~UUI();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 関数
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/// <summary>
	/// 数値の表示
	/// </summary>
	void OutPut()const;

	/// <summary>
	/// 乗算した値を自身に代入する
	/// </summary>
	/// <param name="x">乗算したい値</param>
	/// <param name="y">何乗するか</param>
	/// <returns>自身の参照</returns>
	UUI& Pow(UUI& x, UINT y);

	/// <summary>
	/// 自身を乗算する
	/// </summary>
	/// <param name="y">何乗するか</param>
	/// <returns></returns>
	UUI& Pow(UINT y);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// オペレータ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/// <summary>
	/// m_digitsを代入する
	/// </summary>
	/// <param name="rhs">代入する値</param>
	/// <returns>自身の参照</returns>
	UUI& operator=(const UUI& rhs);

	/// <summary>
	/// 引数との合計を返す
	/// </summary>
	/// <param name="rhs">足し合わせる値</param>
	/// <returns>合計値</returns>
	UUI operator+(const UUI& rhs)const;

	/// <summary>
	/// 引数との合計を自身に代入する
	/// </summary>
	/// <param name="rhs">足し合わせる値</param>
	/// <returns>自身の参照</returns>
	UUI& operator+=(const UUI& rhs);

	/// <summary>
	/// 引数を掛け合わせた値を返す
	/// </summary>
	/// <param name="rhs">掛け合わせる値</param>
	/// <returns>掛け合わせた値</returns>
	UUI operator*(const UUI& rhs)const;

	/// <summary>
	/// 引数を掛け合わせた値を自身に代入する
	/// </summary>
	/// <param name="rhs">掛け合わせる値</param>
	/// <returns>自身の参照</returns>
	UUI& operator*=(const UUI& rhs);

private:
	using DigitValues = std::vector<BYTE>;

	class Calculator;
	std::unique_ptr<Calculator> m_pCalculator;

	DigitValues m_digits;
};
}// namespace uui

#endif // !UNLIMITED_UNSIGNED_INTEGER_H
