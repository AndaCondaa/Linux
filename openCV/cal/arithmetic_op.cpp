#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat m1(3, 6, CV_8UC1, Scalar(10));
	Mat m2(3, 6, CV_8UC1, Scalar(50));
	Mat m_add1, m_add2, m_sub, m_div1, m_div2;
	Mat mask(m1.size(), CV_8UC1);

	Rect rect(Point(3, 0), Size(3, 3));
	mask(rect).setTo(1);

	add(m1, m2, m_add1);
	add(m1, m2, m_add2, mask);
	divide(m1, m2, m_div1);
	m1.convertTo(m1, CV_32F);
	m2.convertTo(m2, CV_32F);
	divide(m1, m2, m_div2);

	cout << "m1 " << endl << m1 << endl;
	cout << "m2 " << endl << m2 << endl;
	cout << "mask " << endl << mask << endl << endl;

	cout << "m_add1 " << endl << m_add1 << endl;
	cout << "m_add2 " << endl << m_add2 << endl;
	cout << "m_div1 " << endl << m_div1 << endl;
	cout << "m_div2 " << endl << m_div2 << endl;

	return 0;
}
