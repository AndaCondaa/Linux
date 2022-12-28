#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void find_histoPos(Mat img, int& start, int& end, int direct)
{
	reduce(img, img, direct, REDUCE_AVG);

	int  minFound = 0;
	for (int i = 0; i < (int)img.total(); i++) {
		if (img.at<uchar>(i) < 250) 		// 빈라인이 아니면
		{
			end = i;					// 히스토그램 마지막 위치
			if (!minFound) {
				start = i;			// 히스토그램 첫 위치
				minFound = 1;
			}
		}
	}
}

Mat find_number(Mat part)
{
	Point start, end;
	find_histoPos(part, start.x, end.x, 0);		// 수직 투영 
	find_histoPos(part, start.y, end.y, 1);   	// 수평 투영 

	return part(Rect(start, end));			// 숫자객체 영상
}

Mat  place_middle(Mat number, Size new_size)
{
	int  big = max(number.cols, number.rows);
	Mat  square(big, big, number.type(), Scalar(255));	// 정방영상

	Point start = (square.size() - number.size()) / 2;

	Rect middle_rect(start, number.size());			// 중심 사각형
	Mat middle = square(middle_rect);
	number.copyTo(middle);

	resize(square, square, new_size);				// 크기 변경
	square.convertTo(square, CV_32F);

	return square.reshape(0, 1);
}



int main()
{
	Size  size(40, 40);			// 학습 셀 크기 
	int  K = 15;				// 선출하는 이웃 샘플수
	int  Nclass = 10;			// 인식 숫자(카테고리) 개수
	int  Nsample = 20;		// 숫자당 학습 샘플수

	string image_file = "./train_numbers.bmp";
	Mat  train_image = imread(image_file, 0);		// 영상 로드
	CV_Assert(train_image.data);

	threshold(train_image, train_image, 32, 255, THRESH_BINARY);

	Mat  trainData, classLable;
	for (int i = 0, k = 0; i < Nclass; i++) {
		for (int j = 0; j < Nsample; j++, k++)
		{
			Point start(j * size.width, i * size.height);
			Rect  roi(start, size);
			Mat   part = train_image(roi);	// 숫자 영상 분리


			Mat  num = find_number(part);	// 숫자 영역 영상
			Mat  data = place_middle(num, size);	// 셀 중심에 숫자 배치 
			trainData.push_back(data);		// 학습 데이터 수집
			classLable.push_back(i);		// 레이블링
		}
	}

	Ptr<ml::KNearest>  knn = ml::KNearest::create();
	knn->train(trainData, ml::ROW_SAMPLE, classLable);	// k-NN 학습

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++)
		{
			string demo_file = format("./image/%02d.png", i, j);
			Mat  demo_img = imread(demo_file, 0);	// 영상 로드
			if (demo_img.empty()) {
				cout << demo_file + " 파일이 없습니다." << endl;
				continue;
			}

			threshold(demo_img, demo_img, 128, 255, THRESH_BINARY);
			Mat  num = find_number(demo_img);
			Mat  data = place_middle(num, size);

			Mat result;
			knn->findNearest(data, K, result);	// 숫자 분류 수행

			cout << "분류결과 : " << result.at<float>(1) << endl;
			imshow("demo_img", demo_img);
			waitKey();
		}
	}

	return 0;
}