#include <iostream>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  

using namespace std;
using namespace cv;

const double eps = 1e-8;

//定义块的大小
const int blk = 10;

//定义循环次数
const int tim = 3;

//将原图中的0-255的黑白表示转化为 黑色为1 白色为-1 方便运算
int cal(uchar t) {
	if (t > 150)
		return -1;
	else
		return 1;
}

int dis(int x, int y, int xx, int yy) {
	return sqrt((x - xx)*(x - xx) + (y - yy)*(y - yy));
}

//块中点与当前点的距离和点的颜色
vector<pair<double,int> > v;

//对块做运算，计算点周围blk*blk大小的块，对每个块的beta参数随距离增大而减小
//参数分别为 需要处理的图像img，块的左上角坐标（x,y)，参数beta
double block(Mat& img, int x, int y,double beta) {
	//清空vector
	v.clear();

	//块中遍历
	for (int i = x-blk/2; i < x + blk/2; ++i) {
		for (int j = y-blk/2; j < y + blk/2; ++j) {
			//将当前点到计算点的距离和当前点的颜色push进vector
			if (cal(img.at<uchar>(i, j)) == 1) {
				v.push_back(make_pair(dis(i, j, x, y), 1));
			}
			else {
				v.push_back(make_pair(dis(i, j, x, y), -1));
			}
		}
	}

	double res = 0;

	//按距离排序（pair默认先first再second排序）
	sort(v.begin(), v.end());

	int sz = v.size();
	for (int i = 0; i < sz; ++i) {
		if (i == 0) {
			res += beta*v[i].second;
		}
		else {
			//如果距离变大了，将参数影响减半
			if (!fabs(v[i].first - v[i - 1].first) < eps) {
				beta /= 2;
			}
			res += beta*v[i].second;
		}
	}
	return res;
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
	double h = 0, beta = 10, eta =6;

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
				double x = block(img, i, j, beta);
				//定义当前点为白色，计算势函数
				int t1 = -1;
				double E1 = h*t1 - t1*x - eta*t1*t;

				//定义当前点为黑色，计算势函数
				t1 = 1;
				double E2 = h*t1 - t1*x - eta*t1*t;

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
		if (cnt >=tim)
			break;
	}
	imshow("result", img);

	//输出图像
	imwrite("D:\\pic\\rgb_block_result_v2_10_3.png", img);
	waitKey(24000);
	return 0;
}