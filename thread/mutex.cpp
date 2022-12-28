#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int value = 0;
mutex value_mutex;

void increase_value()
{
	value_mutex.lock();
	value++;
	cout << "start thread : " << value << endl;
	value_mutex.unlock();
}

int main()
{
	thread t[10];
	
	for (auto i = 0; i < 10; i++) {
		t[i] = thread(increase_value);
	}

	for (int i = 0; i < 10; i++) {
		t[i].join();
	}

	return 0;
}