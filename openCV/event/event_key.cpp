#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image(200, 300, CV_8U, Scalar(255));
	namedWindow("key event", WINDOW_AUTOSIZE);
	imshow("key event", image);

	while (1)
	{
		int key = waitKeyEx(100);
		if (key == 27) break;

		switch (key)
		{
			case 'a': cout << "Key a input" << endl; break;
			case 'b': cout << "Key b input" << endl; break;
			case 0x41: cout << "Key A input" << endl; break;
			case 66: cout << "Key B input" << endl; break;

			case 0xff51: cout << "Left Key input" << endl; break;
			case 0xff52: cout << "Top Key input" << endl; break;
			case 0xff53: cout << "Right Key input" << endl; break;
			case 0xff54: cout << "Bottom key input" << endl; break;
		}
	}

	return 0;

}

