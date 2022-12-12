#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat image = Mat::zeros(300, 400, CV_8UC3);
	image.setTo(cv::Scalar(255, 255, 255));
	Scalar color(255, 0, 255);
	Point p1(50, 50), p5(150, 150);

	std::vector<Point> contour;
	contour.push_back(p1);
	contour.push_back(Point(200, 100));
	contour.push_back(Point(250, 50));
	contour.push_back(Point(180, 200));
	
	const Point *pts = (const cv::Point*) Mat(contour).data;
	int npts = contour.size();
	//polylines(image, &pts, &npts, 1, true, color);
	fillPoly(image, &pts, &npts, 1, color);

	imshow("Draw Polygon", image);
	waitKey(0);

	return 0;
}
