#include <iostream>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  

using namespace std;
using namespace cv;

//将原图中的0-255的黑白表示转化为 黑色为1 白色为-1 方便运算
int cal(uchar t) {
	if (t > 150)
		return -1;
	else
		return 1;
}

int main()
{
	Mat img;
	img = imread("D:\\pic\\dep_sub.png");

	//转化为灰度图
	cvtColor(img, img, CV_RGB2GRAY);
	imshow("img", img);

	//计算行和列数 为遍历做准备
	int row = img.rows;
	int col = img.cols;
	cout << row << " " << col << endl;
//	cout << img.channels() << endl;

	Mat img1 = img;

	//设置MRF所需的参数，势函数定义见文档
	//h代表对整体图像取值的偏向，beta代表相邻的像素点的影响，eta代表希望观测值和最终值相同，因为噪声比例不大
	double h = 0, beta = 10, eta = 0.1;

	while (true) {
		
		//记录修改的次数
		int tot = 0;

		//遍历图像
		for (int i = 1; i < row - 1; ++i) {
			for (int j = 1; j < col - 1; ++j) {
				
				//记录原来的颜色
				int t = cal(img.at<uchar>(i, j));
				
				//中间变量计算，代表当前点周围点的取值偏向
				double x = cal(img.at<uchar>(i - 1, j)) + cal(img.at<uchar>(i + 1, j)) + cal(img.at<uchar>(i, j - 1)) + cal(img.at<uchar>(i, j + 1));
				
				//定义当前点为白色，计算势函数
				int t1 = -1;
				double E1 = h*t1 - beta*t1*x - eta*t1*cal(img1.at<uchar>(i, j));

				//定义当前点为黑色，计算势函数
				t1 = 1;
				double E2 = h*t1 - beta*t1*x - eta*t1*cal(img1.at<uchar>(i, j));

				//取势函数较小的答案
				if (E1 < E2) {
//					cout << "E1<E2\n";
					img.at<uchar>(i, j) = 255;
				}
				else {
					img.at<uchar>(i, j) = 0;
				}
				if (t != cal(img.at<uchar>(i, j)))
					tot++;
			}
		}
		cout << tot << endl;

		//如果全图保持稳定状态，则退出循环，处理完成
		if (tot < 1)
			break;
	}
	imshow("dep_result", img);

	//输出图像
	imwrite("D:\\pic\\dep_result.png", img);
	waitKey(24000);
	return 0;
}