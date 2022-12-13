#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int isDrawing = 0;
Point prevPt;
Mat image;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Scalar color(0, 0, 0);
	Point newPt(x, y);
	if(event == EVENT_LBUTTONDOWN) {
		isDrawing = 1;
		circle(image, newPt, 1, color, -1);
	} else if(event == EVENT_MOUSEMOVE) {
		if(isDrawing) {
			line(image, prevPt, newPt, color, 1);
		}
	} else if(event == EVENT_LBUTTONUP) {
		isDrawing = 0;
		line(image, prevPt, newPt, color, 1);
	}
	prevPt = newPt;
}

int main()
{
	image = Mat(300, 400, CV_8UC3, Scalar(255, 255, 255));

	namedWindow("Draw", WINDOW_NORMAL);

	setMouseCallback("Draw", CallBackFunc, NULL);

	while(1) {
		imshow("Draw", image);
		int key = waitKey(1) & 0xFF;
		if(key == 27) break;
	}
	destroyAllWindows();

	return 0;
}
