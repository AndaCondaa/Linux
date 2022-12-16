#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void homogenOp(Mat img, Mat& dst, int mask_size)
{
	dst = Mat(img.size(), CV_8U, Scalar(0));
	Point h_m(mask_size / 2, mask_size / 2);

	for (int i = h_m.y; i < img.rows - h_m.y; i++) {
		for (int j = h_m.x; j < img.cols - h_m.x; j++)
		{
			float max = 0;
			for (int u = 0; u < mask_size; u++) {
				for (int v = 0; v < mask_size; v++)
				{
					int y = i + u - h_m.y;
					int x = j + v - h_m.x;
					float difference = abs(img.at<uchar>(i, j) - img.at<uchar>(y, x));
					if (difference > max) max = difference;
				}
			}
			dst.at<uchar>(i, j) = max;
		}
	}
}


int main()
{
		Mat image = imread("./test.jpg", 0);
		CV_Assert(image.data);

		Mat edge;
		homogenOp(image, edge, 3);

		imshow("image", image), imshow("edge-homogenOp", edge);
		waitKey();

		return 0;
}