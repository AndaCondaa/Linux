#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Point2f pt1, pt2;
Mat image;

void morphing()
{
	Mat dst(image.size(), image.type(), Scalar(0));
	int  width = image.cols;

	for (float y = 0; y < image.rows; y++) {
		for (float x = 0; x < image.cols; x += 0.1f)
		{
			float ratio;
			if (x < pt1.x) 	ratio = x / pt1.x;
			else 			ratio = (width - x) / (width - pt1.x);

			float dx = ratio * (pt2.x - pt1.x);
			dst.at<uchar>(y, x + dx) = image.at<uchar>(y, x);
		}
	}
	dst.copyTo(image);
	imshow("image", image);
}

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		pt1 = Point2f(x, y);
	}
	else if (event == EVENT_LBUTTONUP) {
		pt2 = Point2f(x, y);
		morphing();
	}
}

int main()
{
	image = imread("./lena.bmp", 0);
	CV_Assert(image.data);

	imshow("image", image);
	setMouseCallback("image", onMouse);
	waitKey();

	return 0;
}