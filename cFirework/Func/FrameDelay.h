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
		//�������֡�ʵ��ӳ�����
		//�㷨˼���ǣ���1000�������֡��Rate��ȡ�����������ӳ�Type1_PD
		//�ټ����ȡ���������ӳٺ󣬻��пճ����ٺ���Type2_N
		//���һ����1000 = Type1_PD * (Rate - Type2_N) + (Type1_PD + 1) * Type2_N.
		//�� 1000 = Type1_PD * Rate +Type2_N
		//ʵ���ϣ���Ե�һ���ӳٵĸ����͵ڶ����ӳٵĸ���ȡ���Ⱥ������б���֡��30ʱ���(16 ,17 ,17)
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