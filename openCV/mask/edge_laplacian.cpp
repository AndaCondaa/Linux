#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("./test.jpg", 0);
	CV_Assert(image.data);

	short data1[] =
	{
		0, 1, 0,
		1, -4, 1,
		0, 1, 0
	};

	short data2[] =
	{
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};

	Mat dst1, dst2, dst3;
	Mat mask4(3, 3, CV_16S, data1);
	Mat mask8(3, 3, CV_16S, data2);

	filter2D(image, dst1, CV_16S, mask4);
	filter2D(image, dst2, CV_16S, mask8);
	Laplacian(image, dst3, CV_16S, 1);

	convertScaleAbs(dst1, dst1);
	convertScaleAbs(dst2, dst2);
	convertScaleAbs(dst3, dst3);

	imshow("image", image), imshow("filter2D_4방향", dst1);
	imshow("filter2D_8방향", dst2), imshow("Laplacian_OpenCV", dst3);
	waitKey();
	
	return 0;
}