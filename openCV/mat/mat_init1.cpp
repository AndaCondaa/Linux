#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat m1 = Mat::ones(3, 5, CV_8UC1);
	Mat m2 = Mat::zeros(3, 5, CV_8UC1);
	Mat m3 = Mat::eye(3, 5, CV_8UC1);

	cout << "[m1] = " << endl << m1 << endl;
	cout << "[m2] = " << endl << m2 << endl;
	cout << "[m3] = " << endl << m3 << endl;

	return 0;
}

