#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	//VideoCapture capture(0);
        VideoCapture capture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1280, height=800, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", CAP_GSTREAMER );	
	CV_Assert(capture.isOpened());

	double fps = 29.97;
	int delay = cvRound(1000.0 / fps);
	Size size(640, 360);
	int fourcc = VideoWriter::fourcc('D', 'X', '5', '0');

	capture.set(CAP_PROP_FRAME_WIDTH, size.width);
	capture.set(CAP_PROP_FRAME_HEIGHT, size.height);

	cout << size << endl;
	cout << fourcc << endl;
	cout << delay << endl;
	cout << fps << endl;
	

	VideoWriter writer;
	writer.open("./video.avi", fourcc, fps, size);
	CV_Assert(writer.isOpened());
	
	for(;;) {
		Mat frame;
		capture >> frame;
		writer << frame;

		imshow("camera", frame);
		if (waitKey(delay) >= 0)
			break;
	}
	return 0;
}
