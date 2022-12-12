#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	double data[] = {
		1.1, 2.2, 3.3, 4.4,
		5.5, 6.67.7, 8.9,
		9.9, 10, 11, 12
	};

	Mat m1(3, 4, CV_64F, data);
	Mat m2 = m1.clone();
	Mat m3, m4;
	m1.copyTo(m3);
	m1.convertTo(m4, CV_8U);

	cout << "m1 =\n" << m1 << endl;
	cout << "m2 =\n" << m2 << endl;
	cout << "m3 =\n" << m3 << endl;
	cout << "m4 =\n" << m4 << endl;

	return 0;
}

