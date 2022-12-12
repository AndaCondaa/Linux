#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Size2d sz(100.5, 60.6);
	Point2f pt1(20.f, 30.f), pt2(100.f, 200.f);

	Rect_<int> rect1(10, 10, 30, 50);
	Rect_<float> rect2(pt1, pt2);
	Rect_<double> rect3(Point2d(20.5, 10), sz);

	Rect rect4 = rect1 + (Point)pt1;
	Rect2f rect5 = rect2 + (Size2f)sz;
	Rect2d rect6 = rect1 & (Rect)rect2;

	cout << "rect3 = " << rect3.x << ", " << rect3.y << ", ";
	cout << rect3.width  << "x" << rect3.height << endl;
	cout << "rect4 = " << rect4.tl() << " " << rect4.br() << endl;
	cout << "rect5 size = " << rect5.size() <<endl;
	cout << "[rect6] = " << rect6 << endl;

	return 0;
}

