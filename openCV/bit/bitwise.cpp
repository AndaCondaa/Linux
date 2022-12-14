#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image1(300, 300, CV_8U, Scalar(0));
	Mat image2(300, 300, CV_8U, Scalar(0));
	Mat image3, image4, image5, image6;

	Point center = image1.size() / 2;
	circle(image1, center, 100, Scalar(255), -1);
	rectangle(image2, Point(0,0), Point(150, 300), Scalar(255), -1);

	bitwise_or(image1, image2, image3);
	bitwise_and(image1, image2, image4);
	bitwise_xor(image1, image2, image5);
	bitwise_not(image1, image6);

	imshow("image1", image1), imshow("image2", image2);
	imshow("bitwise_or", image3), imshow("bitwise_and", image4);
	imshow("bitwise_xor", image5), imshow("bitwise_now", image6);
	waitKey();

	return 0;
}
