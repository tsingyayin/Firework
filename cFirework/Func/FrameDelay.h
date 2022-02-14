#pragma once
#include <QtCore>
#include <cmath>

class FrameDelay :public QObject
{
public:
	static int gcd(int m, int n) {
		if (m % n == 0) { return n; }
		else { return gcd(n, m % n); }
	}
	static QList<int> func(int CurrentVideoRate) {
		//计算给定帧率的延迟数列
		//算法思想是，用1000毫秒除以帧率Rate，取得整数部分延迟Type1_PD
		//再计算出取整数部分延迟后，会有空出多少毫秒Type2_N
		//如此一来，1000 = Type1_PD * (Rate - Type2_N) + (Type1_PD + 1) * Type2_N.
		//即 1000 = Type1_PD * Rate +Type2_N
		//实际上，会对第一类延迟的个数和第二类延迟的个数取最简比后再组列表。如帧率30时输出(16 ,17 ,17)
		int Rate = CurrentVideoRate;
		int Type1_PD = 1000 / Rate;
		int Type2_N = 1000 - Type1_PD * Rate;
		int Type1_N = Rate - Type2_N;
		int GCD = gcd(Type2_N, Type1_N);
		int Type2_LN = Type2_N / GCD;
		int Type1_LN = Type1_N / GCD;
		QList<int> DelayList;
		for (int i = 0; i < Type1_LN; i++) {
			DelayList.append(Type1_PD);
		}
		for (int i = 0; i < Type2_LN; i++) {
			DelayList.append(Type1_PD + 1);
		}
		return DelayList;
	}
};