#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

int main()
{
	auto printMessage = [](const string& message) {
		cout << message << endl;
	};

	thread thread(printMessage, "Message from a child thread.");
	printMessage("waiting the child thread...");
	thread.join();

	return 0;
}