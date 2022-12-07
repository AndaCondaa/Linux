#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat image = Mat::zeros(300, 400, CV_8UC3);
	image.setTo(cv::Scalar(255,255,255));
	Scalar color(255,0,0);
	Point p1(10, 10), p2(100,100);
	line(image, p1, p2, color, 5);

	imshow("Draw Line", image);
	waitKey(0);
	return 0;
}
