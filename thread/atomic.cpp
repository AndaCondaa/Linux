#include <iostream>
#include <atomic>

using namespace std;

int main()
{
	atomic<int> intAtomic = 1;

	intAtomic.fetch_add(1);
	cout << intAtomic << endl;

	intAtomic.fetch_sub(2);
	cout << intAtomic << endl;

	return 0;
}