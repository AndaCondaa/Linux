#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat m1(4, 3, CV_32FC3);
	cout << "Dimensions = " << m1.dims << endl;
	cout << "Rows = " << m1.rows << endl;
	cout << "Cols = " << m1.cols << endl;
	cout << "Mat Size = " << m1.size() << endl << endl;

	cout << "Total = " << m1.total() << endl;
	cout << "Size of a elem = " << m1.elemSize() << endl;
	cout << "ElemSize per channel = " << m1.elemSize() << endl << endl;

	cout << "Type = " << m1.type() << endl;
	cout << "Type(channel|depth) = " << ((m1.channels() -1) << 3) + m1.depth() << endl;
	cout << "Depth = " << m1.depth() << endl;
	cout << "Channel = " << m1.channels() << endl << endl;

	cout << "step = " << m1.step << endl;
	cout << "step1() = " << m1.step1() << endl;

	return 0;
}

