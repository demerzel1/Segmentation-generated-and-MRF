#include <iostream>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  

using namespace std;
using namespace cv;

//定义块的大小
const int blk = 3;

//将原图中的0-255的黑白表示转化为 黑色为1 白色为-1 方便运算
int cal(uchar t) {
	if (t > 150)
		return -1;
	else
		return 1;
}

//对块做运算，计算块中的颜色多少。块中黑色多，则返回黑色1，否则返回白色-1
//参数分别为 需要处理的图像img，块的左上角坐标（x,y)
int block(Mat& img, int x, int y) {
	int cnt1 = 0, cnt2 = 0;

	//块中遍历
	for (int i = x; i < x + blk; ++i) {
		for (int j = y; j < y + blk; ++j) {
			if (cal(img.at<uchar>(i, j)) == 1)
				cnt1++;
			else
				cnt2++;
		}
	}

	if (cnt1 > cnt2)
		return 1;
	else
		return -1;
}

//整块改变，最后因效果较差，没有使用此方法
void change(Mat& img, int x, int y, uchar v) {
	for (int i = x; i < x + blk; ++i) {
		for (int j = y; j < y + blk; ++j) {
			img.at<uchar>(i, j) = v;
		}
	}
}

int main()
{
	Mat img;
	img = imread("D:\\pic\\rgb_sub.png");
	cout << CV_VERSION << endl;

	//转化为灰度图
	cvtColor(img, img, CV_RGB2GRAY);
	imshow("img", img);

	//计算行和列数 为遍历做准备
	int row = img.rows;
	int col = img.cols;
	cout << row << " " << col << endl;
	cout << img.channels() << endl;

	Mat img1 = img;

	//设置MRF所需的参数，势函数定义见文档
	//h代表对整体图像取值的偏向，beta代表相邻的像素点的影响，eta代表希望观测值和最终值相同，因为噪声比例不大
	double h = 0, beta = 500, eta = 0.1;

	//定义循环次数
	int cnt = 0;
	while (true) {
		cnt++;

		//记录修改的次数
		int tot = 0;

		//遍历图像
		for (int i = blk; i < row - blk - 1; ++i) {
			for (int j = blk; j < col - blk - 1; ++j) {

				//记录原来的颜色
				int t = cal(img.at<uchar>(i, j));

				//中间变量计算，代表当前块周围块的取值偏向
				double x = block(img, i - 1, j) + block(img, i + 1, j) + block(img, i, j - 1) + block(img, i, j + 1);

				//定义当前点为白色，计算势函数
				int t1 = -1;
				double E1 = h*t1 - beta*t1*x - eta*t1*block(img1, i, j);

				//定义当前点为黑色，计算势函数
				t1 = 1;
				double E2 = h*t1 - beta*t1*x - eta*t1*block(img1, i, j);

				//取势函数较小的答案
				if (E1 < E2) {
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

		//如果全图保持稳定，或者循环次数到达某个设定值，退出循环
		if (tot < 1)
			break;
		if (cnt > 4)
			break;
	}
	imshow("rgb_block_result", img);

	//输出图像
	imwrite("D:\\pic\\rgb_block_result_3_5.png", img);
	waitKey(24000);
	return 0;
}