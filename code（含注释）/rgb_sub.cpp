#include <iostream>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  


using namespace cv;


int main()
{
	Mat img1, img2, result;

	img1 = imread("D:\\pic\\rgb1.jpg");
	img2 = imread("D:\\pic\\rgb2.jpg");

	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);

	imshow("img1", img1);
	imshow("img2", img2);

	Mat tt;
	subtract(img2, img1, tt);
	imshow("tt", tt);
	//Mat ttt;
	//cvtColor(tt, ttt, CV_BGR2GRAY);
	//imshow("ttt", ttt);
	threshold(tt, result, 20, 200.0, CV_THRESH_BINARY);
	imshow("erzhi", result);
	imwrite("D:\\pic\\rgb_sub.png", result);
	waitKey(26000);
	return 0;
}