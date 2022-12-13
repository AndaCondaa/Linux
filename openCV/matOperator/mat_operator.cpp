#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	float data[] = {
		1, 0, 2,
		-3, 4, 6,
		-1, -2, 3
	};

	Mat m1(3, 3, CV_32F, data);
	Mat m2(Matx13f(6, 30, 8));
	Mat m2_t = m2.t();

	Mat m1_inv = m1.inv(DECOMP_LU);
	Mat x = m1_inv * m2_t;

	cout << "[m1] = " << endl << m1 << endl;
	cout << "[m1_inv(reverse)] = " << endl << m1_inv << endl;
	cout << "[m2(T)] = " << endl << m2_t << endl;

	cout << "x1, x2, x3 = " << x.t() << endl << endl;

	return 0;
}
