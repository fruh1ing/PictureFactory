#include "Processing.h"


cv::Mat dft_procssing(std::string& imgpath)
{
	// 将输入图像延扩到最佳的尺寸，边界用0补充
	Mat srcImg = imread(imgpath, 0);

	int m = getOptimalDFTSize(srcImg.rows);
	int n = getOptimalDFTSize(srcImg.cols);

	//将添加的像素初始化为0
	Mat padded;
	copyMakeBorder(srcImg, padded, 0, m - srcImg.rows, 0, n - srcImg.cols, BORDER_CONSTANT, Scalar::all(0));
	// 为结果分配空间
	// 将planes数组组合合并成一个多通道的数组complexI
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
	Mat complesI;
	merge(planes, 2, complesI);
	// 离散傅里叶变换
	dft(complesI, complesI);
	split(complesI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat magnitudeImage = planes[0];
	// 对数尺寸缩放
	magnitudeImage += Scalar::all(1);
	log(magnitudeImage, magnitudeImage);
	// 剪切和重分布幅度图象限
	// 若有奇数行或奇数列，进行频谱裁剪
	magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	Mat q0(magnitudeImage, Rect(0, 0, cx, cy));// 左上
	Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));
	Mat q2(magnitudeImage, Rect(0, cy, cx, cy));
	Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));

	Mat tmp;
	// 左上和右下交换
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	// 右上和左下交换
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// 归一化
	normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);
	return magnitudeImage;
}
