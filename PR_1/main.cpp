#include <iostream>;
using namespace std;

void coutp(unsigned int x)
{
	int n = sizeof(int) * 8;
	unsigned maska = (1 << (n - 1));
	for (int i = 1; i <= n; i++)
	{
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1;
	}
}

void ex1()
{
	int x16 = 0x111;
	const int mask = 0x1600;
	int result = x16 | mask;
	cout << "result = " << result << endl;
}

void ex2()
{
	int x;
	cout << "x = ";
	cin >> x;
	const int mask = 0x28002;
	int result = x & mask;
	cout << "result = " << result << endl;
}

void ex3()
{
	int x;
	cout << "x = ";
	cin >> x;
	int mask = 10;
	int result = x << mask;
	cout << "result = " << result << endl;
}

void ex4()
{
	int x;
	cout << "x = ";
	cin >> x;
	int mask = 10;
	int result = x >> mask;
	cout << "result = " << result << endl;
}

void ex5()
{
	int x;
	int mask = 1;
	cout << "x = ";
	cin >> x;
	mask <<= x;
	coutp(mask);
}

int main()
{
	ex1();
	ex2();
	ex3();
	ex4();
	ex5();
}
