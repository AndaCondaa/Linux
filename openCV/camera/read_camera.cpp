#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void put_string(Mat &frame, string text, Point pt, int value)
{
	text += to_string(value);
	Point shade = pt + Point(2,2);
	int font = FONT_HERSHEY_SIMPLEX;
	putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);
	putText(frame, text, pt, font, 0.7, Scalar(120, 200, 90), 2);
}

int main()
{
	//VideoCapture capture(0);
	VideoCapture capture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1280, height=800, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", CAP_GSTREAMER );
	
	
	
	if (!capture.isOpened())
	{
		cout << "No camera connection" << endl;
		exit(1);
	}

	cout << "width " << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
	cout << "height " << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
	cout << "Ex " << capture.get(CAP_PROP_EXPOSURE) << endl;
	cout << "Br " << capture.get(CAP_PROP_BRIGHTNESS) << endl;


	for(;;) {
		Mat frame;
		capture.read(frame);

		put_string(frame, "EXPOS: ", Point(10, 40), capture.get(CAP_PROP_EXPOSURE));
		imshow("camera", frame);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
