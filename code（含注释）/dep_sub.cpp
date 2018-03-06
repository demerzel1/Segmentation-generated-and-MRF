#include <iostream>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  


using namespace cv;


int main()
{
	Mat img1, img2, result;
	//图像读入
	img1 = imread("D:\\pic\\dep1.png");
	img2 = imread("D:\\pic\\dep2.png");

	//转为灰度图
	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);

	imshow("img1", img1);
	imshow("img2", img2);

	Mat tt;
	//图像相减
	subtract(img2, img1, tt);
	imshow("tt", tt);

	//二值化处理
	threshold(tt, result, 0.1, 200.0, CV_THRESH_BINARY);
	imshow("erzhi", result);
	imwrite("D:\\pic\\dep_sub.png", result);
	waitKey(26000);
	return 0;
}