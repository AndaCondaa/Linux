#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
void hough_coord(Mat image, Mat& acc_mat, double rho, double theta)
{
    int acc_h = (image.rows + image.cols) * 2 / rho;
    int acc_w = CV_PI / theta;
    acc_mat = Mat(acc_h, acc_w, CV_32S, Scalar(0));

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            Point pt(x, y);
            if (image.at<uchar>(pt) > 0)
            {
                for (int t = 0; t < acc_mat.cols; t++)
                {
                    double radian = t * theta;
                    float r = pt.x * cos(radian) + pt.y * sin(radian);
                    r = cvRound(r / rho + acc_mat.rows / 2);
                    acc_mat.at<int>(r, t)++;
                }
            }

        }
    }
}

void acc_mask(Mat acc_mat, Mat& acc_dst, Size size, int thresh)
{
    acc_dst = Mat(acc_mat.size(), CV_32S, Scalar(0));
    Point h_m = size / 2;

    for (int r = h_m.y; r < acc_mat.rows - h_m.y; r++)
    {
        for (int t = h_m.x; t < acc_mat.cols - h_m.x; t++)
        {
            Point center = Point(t, r) - h_m;
            int c_value = acc_mat.at<int>(center);
            if (c_value >= thresh)
            {
                double maxVal = 0;
                for (int u = 0; u < size.height; u++)
                {
                    for (int v = 0; v < size.width; v++)
                    {
                        Point start = center + Point(v, u);
                        if (start != center && acc_mat.at<int>(start) > maxVal)
                            maxVal = acc_mat.at<int>(start);
                    }
                }
                Rect rect(center, size);
                if (c_value >= maxVal)
                {
                    acc_dst.at<int>(center) = c_value;
                    acc_mat(rect).setTo(0);
                }
            }
        }
    }
}

void thres_lines(Mat acc_dst, Mat& lines, double _rho, double theta, int thresh)
{
    for (int r = 0; r < acc_dst.rows; r++)
    {
        for (int t = 0; t < acc_dst.cols; t++)
        {
            float value = (float)acc_dst.at<int>(r, t);
            if (value >= thresh)
            {
                float rho = (float)((r - acc_dst.rows / 2) * _rho);
                float radian = (float)(t * theta);

                Matx13f line(rho, radian, value);
                lines.push_back((Mat)line);
            }
        }
    }
}

void sort_lines(Mat lines, vector<Vec2f>& s_lines)
{
    Mat acc = lines.col(2), idx;
    sortIdx(acc, idx, SORT_EVERY_COLUMN + SORT_DESCENDING);

    for (int i = 0; i < idx.rows; i++)
    {
        int id = idx.at<int>(i);
        float rho = lines.at<float>(id, 0);
        float radian = lines.at<float>(id, 1);
        s_lines.push_back(Vec2f(rho, radian));
    }
}


void houghLines(Mat src, vector<Vec2f>& s_lines, double rho, double theta, int thresh)
{
    Mat acc_mat, acc_dst, lines;//lines º¯°æ
    hough_coord(src, acc_mat, rho, theta);
    acc_mask(acc_mat, acc_dst, Size(3, 7), thresh);

    thres_lines(acc_dst, lines, rho, theta, thresh);
    sort_lines(lines, s_lines);
}

void draw_houghLines(Mat src, Mat& dst, vector<Vec2f> lines, int nline)
{
    cvtColor(src, dst, COLOR_GRAY2BGR);
    for (size_t i = 0; i < min((int)lines.size(), nline); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        Point2d pt(a * rho, b * rho);
        Point2d delta(1000 * -b, 1000 * a);
        line(dst, pt + delta, pt - delta, Scalar(0, 255, 0), 1, LINE_AA);
    }
}

void detect_maxObject(Mat img, Rect& rect)
{
    vector<vector<Point>> contours;
    findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int max_area = 0;
    for (int i = 0; i < (int)contours.size(); i++)
    {
        Rect r = boundingRect(contours[i]);
        if (max_area < r.area()) {
            max_area = r.area();
            rect = r;
        }
    }

    rect = rect - Point(10, 10) + Size(20, 20);
}

void main()
{
    Rect rect;
    Mat gray, canny, morph, th_gray, canny_line, dst;
    double rho = 1, theta = CV_PI / 180;
    vector<Vec2f> lines;

    Mat image = imread("./harness.bmp", 1);
    CV_Assert(image.data);

    cvtColor(image, gray, COLOR_GRAY2BGR);
    threshold(gray, th_gray, 240, 255, THRESH_BINARY);
    erode(th_gray, morph, Mat(), Point(-1, -1), 2);

    detect_maxObject(morph, rect);

    Canny(th_gray(rect), canny, 40, 100);
    houghLines(canny, lines, rho, theta, 50);
    draw_houghLines(canny, canny_line, lines, 1);

    double angle = (CV_PI - lines[0][1]) * 180 / CV_PI;
    Point center = image.size() / 2;
    Mat rot_map = getRotationMatrix2D(center, -angle, 1);

    warpAffine(image, dst, rot_map, image.size(), INTER_LINEAR);

    imshow("image", image);
    imshow("canny_line", canny_line);
    imshow("dst", dst);
    waitKey();
}