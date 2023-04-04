#include "ThreadPolicy.h"
#include <stdio.h>
#include <thread>
#include <Windows.h>


template<typename Policy = SingleThreadPolicy>
class Test : public Policy
{
	using Lock_t = typename Policy::Lock;
public:

	void Print(int min, int max)
	{
		Lock_t _lock(*this);

		char buf[256] = { 0 };
		sprintf_s(buf, sizeof(buf), "%d ~ %d\n", min, max);
		OutputDebugStringA(buf);

		for (int i = min; i < max; ++i)
		{
			printf("%3d ", i);
		}
		printf("\n\n");
	}
};

template<typename Policy>
void ThreadProc(Test<Policy>* pT, int min, int max)
{
	pT->Print(min, max);
}



int main(int argc, char** argv)
{
	// using Policy_t = SingleThreadPolicy;
	using Policy_t = StdMultiThreadPolicy;

	static constexpr int NumThread = 30;
	int step = 20;

	Test<Policy_t> t;
	std::thread thread[NumThread];

	for (int i = 0; i < NumThread; ++i)
	{
		int min = i * step;
		int max = (i * step) + step;

		thread[i] = std::thread(ThreadProc<Policy_t>, &t, min, max);
	}

	for (int i = 0; i < NumThread; ++i)
	{
		thread[i].join();
	}
	return 0;
}