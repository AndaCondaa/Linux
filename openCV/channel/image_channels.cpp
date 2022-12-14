#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("./Lenna.jpg", IMREAD_COLOR);
	CV_Assert(image.data);

	Mat bgr[3];
	split(image, bgr);

	imshow("image", image);
	imshow("blue channel", bgr[0]);
	imshow("green channel", bgr[1]);
	imshow("red channel", bgr[2]);
	waitKey(0);

	return 0;
}

