#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Matx<uchar, 2, 2> m1;
	Matx<uchar, 2, 2> m2;

	m1(0, 0) = -50;
	m1(0, 1) = 300;
	m1(1, 0) = saturate_cast<uchar>(-50);
	m1(1, 1) = saturate_cast<uchar>(300);

	cout << "[m1] = " << endl << m1 << endl;

	return 0;
}

