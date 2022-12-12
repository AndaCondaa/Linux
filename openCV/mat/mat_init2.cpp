#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	double a = 32.12345678, b = 2.7;
	short c = 400;
	float d = 10.f, e = 11.f, f = 13.f;

	Mat_<int> m1(2, 3);
	Mat_<uchar> m2(3, 4, 100);
	Mat_<short> m3(4, 5, c);

	m1 << 1, 2, 3, 4, 5, 6;
	Mat m4 = (Mat_<double>(2,3) << 1, 2, 3, 4, 5, 6);
	Mat m5 = (Mat_<float>(2,3) << a, b, c, d, e, f);

	cout << "[m1]=" << endl << m1 << endl;
	cout << "[m2]=" << endl << m2 << endl << endl;
	cout << "[m3]=" << endl << m3 << endl << endl;
	cout << "[m4]=" << endl << m4 << endl;
	cout << "[m5]=" << endl << m5 << endl;
	
	return 0;
}

