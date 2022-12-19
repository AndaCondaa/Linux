#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{
	system_clock::time_point startTime = system_clock::now();
	for (auto i = 0; i < 1000000; i++);
	system_clock::time_point endTime = system_clock::now();

	nanoseconds nano = endTime - startTime;

	cout << "³ª³ë ÃÊ: " << nano.count() << endl;
	return 0;
}