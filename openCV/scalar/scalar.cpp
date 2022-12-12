#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Scalar_<uchar> red(0, 0, 255);
	Scalar_<int> blue(255, 0, 0, 333);
	Scalar_<double> color1(500);
	Scalar_<float> color2(100.f, 200.f, 125.9f);

	Vec3d green(0, 0, 300.5);
	Scalar green1 = color1 + (Scalar)green;
	Scalar green2 = color2 + (Scalar_<float>)green;

	cout << "blue = " << blue[0] << ", " << blue[1];
	cout << ", " << blue[2] << ", " << blue[3] << endl;
	cout << "red = " << red << endl;
	cout << "green = " << green << endl << endl;
	cout << "green1 = " << green1 << endl;
	cout << "green2 = " << green2 << endl;

	return 0;

}
