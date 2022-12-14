#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	vector<float> v1, v_exp, v_log;
	Matx<float, 1, 5> m1(1, 2, 3, 5, 10);
	Mat m_exp, m_sqrt, m_pow;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	exp(v1, v_exp);
	exp(m1, m_exp);
	log(m1, v_log);
	sqrt(m1, m_sqrt);
	pow(m1, 3, m_pow);

	cout << "m1 = " << m1 << endl << endl;
	cout << "v_exp = " << ((Mat)v_exp).reshape(1, 1) << endl;
	cout << "m_exp = " << m_exp << endl;
	cout << "v_log = " << ((Mat)v_log).reshape(1, 1) << endl << endl;

	cout << "m_sqrt = " << m_sqrt << endl;
	cout << "m_pow = " << m_pow << endl;

	return 0;
}
