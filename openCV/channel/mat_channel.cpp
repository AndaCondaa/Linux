#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat ch0(3, 4, CV_8U, Scalar(10));
	Mat ch1(3, 4, CV_8U, Scalar(20));
	Mat ch2(3, 4, CV_8U, Scalar(30));

	Mat bgr_arr[] = { ch0, ch1, ch2 };
	Mat bgr;
	merge(bgr_arr, 3, bgr);
	vector<Mat> bgr_vec;
	split(bgr, bgr_vec);

	cout << "ch0 = " << endl << ch0 << endl;
	cout << "ch1 = " << endl << ch1 << endl;
	cout << "ch2 = " << endl << ch2 << endl << endl;

	cout << "bgr = " << endl << bgr << endl << endl;
	cout << "bgr_vec[0] = " << endl << bgr_vec[0] << endl;
	cout << "bgr_ved[1] = " << endl << bgr_vec[1] << endl;
	cout << "bgr_ved[2] = " << endl << bgr_vec[2] << endl;

	return 0;
}
