#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Size_<int> sz1(100, 200);
	Size_<float> sz2(192.3f, 25.3f);
	Size_<double> sz3(100.2, 30.9);

	Size sz4(120, 69);
	Size2f sz5(0.3f, 0.f);
	Size2d sz6(0.25, 0.6);

	Point2d pt1(0.25, 0.6);
	Size2i sz7 = sz1 + (Size2i)sz2;
	Size2d sz8 = sz3 - (Size2d)sz4;
	Size2d sz9 = sz5 + (Size2f)pt1;

	cout << "sz1.width = " << sz1.width;
	cout << ", sz1.height = " << sz1.height << endl;
	cout << "sz1 Area" << sz1.area() << endl;
	cout << "[sz7] = " << sz7 << endl;
	cout << "[sz8] = " << sz8 << endl;
	cout << "[sz9] = " << sz9 << endl;
	return 0;
}

