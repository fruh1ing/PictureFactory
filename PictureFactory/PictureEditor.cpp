#include "PictureEditor.h"
#include <QFileDialog>
#include <QDebug>
#include<opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "Processing.h"
#include "QImageWidget.h"

PictureEditor::PictureEditor(QWidget* parent)
	: QWidget(parent)
	, grayImgWgt(nullptr)
	, contourWgt(nullptr)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	connect(ui.pushButton_load, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonLoadClick);
	connect(ui.pushButton_original, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonOrigClick);
	connect(ui.pushButton_erosion, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonEronClick);
	connect(ui.pushButton_blur, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonBlurClick);
	connect(ui.pushButton_edge, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonEdgeClick);
	connect(ui.pushButton_chanR, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonChanRClick);
	connect(ui.pushButton_dft, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonDftClick);
	connect(ui.pushButton_box, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonBoxClick);
	connect(ui.pushButton_mean, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonMeanClick);
	connect(ui.pushButton_gauss, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonGaussClick);
	connect(ui.pushButton_sobel, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonSobelClick);
	connect(ui.pushButton_laplacian, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonLapLacianClick);
	connect(ui.pushButton_scharr, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonScharrClick);
	connect(ui.pushButton_houghlines, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHougnLinesClick);
	connect(ui.pushButton_remap, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonRemapClick);
	connect(ui.pushButton_warp, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonWarpClick);
	connect(ui.pushButton_hist, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHistClick);
	connect(ui.pushButton_contour, &QPushButton::toggled, this, &PictureEditor::SlotPushButtonContourClick);
	connect(ui.pushButton_hs, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHSClick);
	connect(ui.pushButton_histogram, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHistogramClick);
	connect(ui.pushButton_histogram2, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHistogram2Click);
	connect(ui.pushButton_bp, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonBPClick);
	connect(ui.pushButton_matchTemplate, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonMatchTemplateClick);
	connect(ui.pushButton_harris, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonHarrisClick);
	connect(ui.pushButton_tomasi, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonShiTomasiClick);
	connect(ui.pushButton_subpix, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonSubpixClick);

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(SlotSpinBox(int)));
}

PictureEditor::~PictureEditor()
{
}

void PictureEditor::SlotPushButtonLoadClick()
{
	filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp *.jpeg)"));
	if (filename.isEmpty())
		return;
	srcImage = cv::imread(filename.toStdString());
	showImage(srcImage);
	qDebug() << filename;
}

void PictureEditor::SlotPushButtonOrigClick()
{
	showImage(srcImage);
}

void PictureEditor::SlotPushButtonEronClick()
{
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	erode(srcImage, dstImage, element);
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonBlurClick()
{
	cv::blur(srcImage, dstImage, cv::Size(7, 7));
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonEdgeClick()
{
	cv::Mat grayImage;
	//dstImage.create(srcImage.size(), srcImage.type());
	cv::cvtColor(srcImage, grayImage, cv::COLOR_BGR2GRAY);
	cv::blur(grayImage, dstImage, cv::Size(3, 3));
	cv::Canny(dstImage, dstImage, 3, 9, 3);

	showImage(dstImage);
}

void PictureEditor::SlotPushButtonChanRClick()
{
	std::vector<Mat> channels;
	cv::split(srcImage, channels);
	dstImage = channels[0];
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonWeightMixClick()
{

}

void PictureEditor::SlotPushButtonDftClick()
{
	Mat dstImg = dft_procssing(filename.toStdString());
	showImage(dstImg);
}

void PictureEditor::SlotPushButtonBoxClick()
{
	boxFilter(srcImage, dstImage, -1, Size(5, 5));
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonMeanClick()
{
	blur(srcImage, dstImage, Size(7, 7));
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonGaussClick()
{
	GaussianBlur(srcImage, dstImage, Size(3, 3), 0, 0);
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonSobelClick()
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, dst;
	// 求X方向梯度
	Sobel(srcImage, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(srcImage, grad_y, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	showImage(dst);
}

void PictureEditor::SlotPushButtonLapLacianClick()
{
	Mat src, src_gray, dst, abs_dst;
	src = srcImage.clone();
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	Laplacian(src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);
	showImage(abs_dst);
}

void PictureEditor::SlotPushButtonScharrClick()
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, dst;
	// 求X方向梯度
	Scharr(srcImage, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(abs_grad_x);
	imageWidget->show();
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);

	// 求Y方向梯度
	Scharr(srcImage, grad_y, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	QImageWidget* imageWidget2 = new QImageWidget;
	imageWidget2->resize(500, 500);
	imageWidget2->setPixmap(abs_grad_y);
	imageWidget2->show();
	connect(imageWidget2, &QImageWidget::closed, imageWidget2, &QImageWidget::deleteLater);

	//合并梯度
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	showImage(dst);
}

void PictureEditor::SlotPushButtonHougnLinesClick()
{
	Mat midImage;
	// 边缘检测和转化为灰度图
	Canny(srcImage, midImage, 50, 200, 3);
	cvtColor(midImage, dstImage, COLOR_GRAY2BGR);
	// 进行霍夫线变换
	std::vector<Vec2f> lines;
	HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);
	for (size_t i = 0; i < lines.size(); ++i)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dstImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA);
	}

	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(midImage);
	imageWidget->show();
	imageWidget->setWindowTitle("midImage");
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);

	showImage(dstImage);
}

void PictureEditor::SlotPushButtonRemapClick()
{
	Mat map_x, map_y;
	dstImage.create(srcImage.size(), srcImage.type());
	map_x.create(srcImage.size(), CV_32FC1);
	map_y.create(srcImage.size(), CV_32FC1);

	// 双层循环，遍历每一个像素点，改变map_x & map_y的值
	for (int j = 0; j < srcImage.rows; j++)
	{
		for (int i = 0; i < srcImage.cols; i++)
		{
			//改变map_x & map_y的值. 
			map_x.at<float>(j, i) = static_cast<float>(i);
			map_y.at<float>(j, i) = static_cast<float>(srcImage.rows - j);
		}
	}

	//进行重映射操作
	remap(srcImage, dstImage, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonWarpClick()
{
	Point2f srcTriangle[3];
	Point2f dstTriangle[3];
	//定义一些Mat变量
	Mat rotMat(2, 3, CV_32FC1);
	Mat warpMat(2, 3, CV_32FC1);
	Mat dstImage_warp, dstImage_warp_rotate;

	// 设置目标图像的大小和类型与源图像一致
	dstImage_warp = Mat::zeros(srcImage.rows, srcImage.cols, srcImage.type());

	//设置源图像和目标图像上的三组点以计算仿射变换
	srcTriangle[0] = Point2f(0, 0);
	srcTriangle[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
	srcTriangle[2] = Point2f(0, static_cast<float>(srcImage.rows - 1));

	dstTriangle[0] = Point2f(static_cast<float>(srcImage.cols * 0.0), static_cast<float>(srcImage.rows * 0.33));
	dstTriangle[1] = Point2f(static_cast<float>(srcImage.cols * 0.65), static_cast<float>(srcImage.rows * 0.35));
	dstTriangle[2] = Point2f(static_cast<float>(srcImage.cols * 0.15), static_cast<float>(srcImage.rows * 0.6));

	//【4】求得仿射变换
	warpMat = getAffineTransform(srcTriangle, dstTriangle);

	//【5】对源图像应用刚刚求得的仿射变换
	warpAffine(srcImage, dstImage_warp, warpMat, dstImage_warp.size());

	//【6】对图像进行缩放后再旋转
	// 计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
	Point center = Point(dstImage_warp.cols / 2, dstImage_warp.rows / 2);
	double angle = -50.0;
	double scale = 0.6;
	// 通过上面的旋转细节信息求得旋转矩阵
	rotMat = getRotationMatrix2D(center, angle, scale);
	// 旋转已缩放后的图像
	warpAffine(dstImage_warp, dstImage_warp_rotate, rotMat, dstImage_warp.size());

	showImage(dstImage_warp);

	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(dstImage_warp_rotate);
	imageWidget->show();
	imageWidget->setWindowTitle("dstImage");
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);
}

void PictureEditor::SlotPushButtonHistClick()
{
	cvtColor(srcImage, dstImage, COLOR_BGR2GRAY);
	equalizeHist(dstImage, dstImage);
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonContourClick(bool checked)
{
	if (checked)
		ui.spinBox->blockSignals(false);
	else
		ui.spinBox->blockSignals(true);
	ui.spinBox->setValue(20);
}

/**
 * 直方图
 */
void PictureEditor::SlotPushButtonHSClick()
{
	Mat hsvImage;
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);

	//将色调量化为30个等级，将饱和度量化为32个等级
	int hueBinNum = 30;//色调的直方图直条数量
	int saturationBinNum = 32;//饱和度的直方图直条数量
	int histSize[] = { hueBinNum, saturationBinNum };
	// 定义色调的变化范围为0到179
	float hueRanges[] = { 0, 180 };
	//定义饱和度的变化范围为0（黑、白、灰）到255（纯光谱颜色）
	float saturationRanges[] = { 0, 256 };
	const float* ranges[] = { hueRanges, saturationRanges };
	MatND dstHist;
	//参数准备，calcHist函数中将计算第0通道和第1通道的直方图
	int channels[] = { 0, 1 };

	//【3】正式调用calcHist，进行直方图计算
	calcHist(&hsvImage,//输入的数组
		1, //数组个数为1
		channels,//通道索引
		Mat(), //不使用掩膜
		dstHist, //输出的目标直方图
		2, //需要计算的直方图的维度为2
		histSize, //存放每个维度的直方图尺寸的数组
		ranges,//每一维数值的取值范围数组
		true, // 指示直方图是否均匀的标识符，true表示均匀的直方图
		false);//累计标识符，false表示直方图在配置阶段会被清零

	//【4】为绘制直方图准备参数
	double maxValue = 0;//最大值
	minMaxLoc(dstHist, 0, &maxValue, 0, 0);//查找数组和子数组的全局最小值和最大值存入maxValue中
	int scale = 10;
	Mat histImg = Mat::zeros(saturationBinNum * scale, hueBinNum * 10, CV_8UC3);

	//【5】双层循环，进行直方图绘制
	for (int hue = 0; hue < hueBinNum; hue++)
	{
		for (int saturation = 0; saturation < saturationBinNum; saturation++)
		{
			float binValue = dstHist.at<float>(hue, saturation);//直方图组距的值
			int intensity = cvRound(binValue * 255 / maxValue);//强度

			//正式进行绘制
			rectangle(histImg, Point(hue * scale, saturation * scale),
				Point((hue + 1) * scale - 1, (saturation + 1) * scale - 1),
				Scalar::all(intensity), FILLED);
		}
	}
	showImage(histImg);
}

/**
 *一维直方图
 */
void PictureEditor::SlotPushButtonHistogramClick()
{
	//载入原图并显示
	if (!srcImage.data)
		return;
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2RGBA);

	//system("color 1F");

	//【2】定义变量
	MatND dstHist;       // 在cv中用CvHistogram *hist = cvCreateHist
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float* ranges[] = { hranges };   // 这里需要为const类型
	int size = 256;
	int channels = 0;

	//【3】计算图像的直方图
	calcHist(&grayImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);    // cv 中是cvCalcHist
	int scale = 1;

	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	//【4】获取最大值和最小值
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);  //  在cv中用的是cvGetMinMaxHistValue

	//【5】绘制出直方图
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);           //   注意hist中是float类型    而在OpenCV1.0版中用cvQueryHistValue_1D
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		rectangle(dstImage, Point(i * scale, size - 1), Point((i + 1) * scale - 1, size - realValue), Scalar(255));
	}
	showImage(dstImage);
}

void PictureEditor::SlotPushButtonHistogram2Click()
{
	//【2】参数准备
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	MatND redHist, grayHist, blueHist;
	int channels_r[] = { 0 };

	//【3】进行直方图的计算（红色分量部分）
	calcHist(&srcImage, 1, channels_r, Mat(), //不使用掩膜
		redHist, 1, hist_size, ranges,
		true, false);

	//【4】进行直方图的计算（绿色分量部分）
	int channels_g[] = { 1 };
	calcHist(&srcImage, 1, channels_g, Mat(), // do not use mask
		grayHist, 1, hist_size, ranges,
		true, // the histogram is uniform
		false);

	//【5】进行直方图的计算（蓝色分量部分）
	int channels_b[] = { 2 };
	calcHist(&srcImage, 1, channels_b, Mat(), // do not use mask
		blueHist, 1, hist_size, ranges,
		true, // the histogram is uniform
		false);

	//-----------------------绘制出三色直方图------------------------
	//参数准备
	double maxValue_red, maxValue_green, maxValue_blue;
	minMaxLoc(redHist, 0, &maxValue_red, 0, 0);
	minMaxLoc(grayHist, 0, &maxValue_green, 0, 0);
	minMaxLoc(blueHist, 0, &maxValue_blue, 0, 0);
	int scale = 1;
	int histHeight = 256;
	Mat histImage = Mat::zeros(histHeight, bins * 3, CV_8UC3);

	//正式开始绘制
	for (int i = 0; i < bins; i++)
	{
		//参数准备
		float binValue_red = redHist.at<float>(i);
		float binValue_green = grayHist.at<float>(i);
		float binValue_blue = blueHist.at<float>(i);
		int intensity_red = cvRound(binValue_red * histHeight / maxValue_red);  //要绘制的高度
		int intensity_green = cvRound(binValue_green * histHeight / maxValue_green);  //要绘制的高度
		int intensity_blue = cvRound(binValue_blue * histHeight / maxValue_blue);  //要绘制的高度

		//绘制红色分量的直方图
		rectangle(histImage, Point(i * scale, histHeight - 1),
			Point((i + 1) * scale - 1, histHeight - intensity_red),
			Scalar(255, 0, 0));

		//绘制绿色分量的直方图
		rectangle(histImage, Point((i + bins) * scale, histHeight - 1),
			Point((i + bins + 1) * scale - 1, histHeight - intensity_green),
			Scalar(0, 255, 0));

		//绘制蓝色分量的直方图
		rectangle(histImage, Point((i + bins * 2) * scale, histHeight - 1),
			Point((i + bins * 2 + 1) * scale - 1, histHeight - intensity_blue),
			Scalar(0, 0, 255));

	}
	showImage(histImage);
}

void PictureEditor::SlotPushButtonBPClick()
{
	if (!srcImage.data)
		return;
	Mat g_hsvImage;
	Mat g_hueImage;
	int g_bins = 30;//直方图组距
	cvtColor(srcImage, g_hsvImage, COLOR_BGR2HSV);
	//【2】分离 Hue 色调通道
	g_hueImage.create(g_hsvImage.size(), g_hsvImage.depth());
	int ch[] = { 0, 0 };
	mixChannels(&g_hsvImage, 1, &g_hueImage, 1, ch, 1);

	//【1】参数准备
	MatND hist;
	int histSize = MAX(g_bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	//【2】计算直方图并归一化
	calcHist(&g_hueImage, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	//【3】计算反向投影
	MatND backproj;
	calcBackProject(&g_hueImage, 1, 0, hist, backproj, &ranges, 1, true);

	//【4】显示反向投影
	//imshow("反向投影图", backproj);
	showImage(backproj);

	//【5】绘制直方图的参数准备
	int w = 400; int h = 400;
	int bin_w = cvRound((double)w / histSize);
	Mat histImg = Mat::zeros(w, h, CV_8UC3);

	//【6】绘制直方图
	for (int i = 0; i < g_bins; i++)
	{
		rectangle(histImg, Point(i * bin_w, h), Point((i + 1) * bin_w, h - cvRound(hist.at<float>(i) * h / 255.0)), Scalar(100, 123, 255), -1);
	}

	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(histImg);
	imageWidget->show();
	imageWidget->setWindowTitle("dstImage");
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);
}

void PictureEditor::SlotPushButtonMatchTemplateClick()
{
	Mat templateImage;
	Mat resultImage;
	//  方法【0】- 平方差匹配法(SQDIFF)\n"
	//	方法【1】- 归一化平方差匹配法(SQDIFF NORMED)\n"
	//	方法【2】- 相关匹配法(TM CCORR)\n"
	//	方法【3】- 归一化相关匹配法(TM CCORR NORMED)\n"
	//	方法【4】- 相关系数匹配法(TM COEFF)\n"
	//	方法【5】- 归一化相关系数匹配法(TM COEFF NORMED)\n" );
	int nMatchMethod = 5;
	int nMaxTrackbarNum = 5;

	// 加载模板图片
	filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp *.jpeg)"));
	if (filename.isEmpty())
		return;
	templateImage = cv::imread(filename.toStdString());


	//【1】给局部变量初始化
	Mat srcImage_t;
	srcImage.copyTo(srcImage_t);

	//【2】初始化用于结果输出的矩阵
	int resultImage_cols = srcImage.cols - templateImage.cols + 1;
	int resultImage_rows = srcImage.rows - templateImage.rows + 1;
	resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//【3】进行匹配和标准化
	matchTemplate(srcImage, templateImage, resultImage, nMatchMethod);
	normalize(resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat());

	//【4】通过函数 minMaxLoc 定位最匹配的位置
	double minValue; double maxValue; Point minLocation; Point maxLocation;
	Point matchLocation;
	minMaxLoc(resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

	//【5】对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值有着更高的匹配结果. 而其余的方法, 数值越大匹配效果越好
	//此句代码的OpenCV2版为：
	//if( g_nMatchMethod  == CV_TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED )
	//此句代码的OpenCV3版为：
	if (nMatchMethod == TM_SQDIFF || nMatchMethod == TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//【6】绘制出矩形，并显示最终结果
	rectangle(srcImage_t, matchLocation, Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(resultImage, matchLocation, Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	showImage(srcImage_t);

	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(resultImage);
	imageWidget->show();
	imageWidget->setWindowTitle("dstImage");
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);
}

void PictureEditor::SlotPushButtonHarrisClick()
{
	Mat srcImage1, grayImage;
	int thresh = 30; //当前阈值
	int max_thresh = 175; //最大阈值

	srcImage1 = srcImage.clone();

	//存留一张灰度图
	cvtColor(srcImage1, grayImage, COLOR_BGR2GRAY);

	//---------------------------【1】定义一些局部变量-----------------------------
	Mat normImage;//归一化后的图
	Mat scaledImage;//线性变换后的八位无符号整型的图

	//---------------------------【2】初始化---------------------------------------
	//置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值
	dstImage = Mat::zeros(srcImage.size(), CV_32FC1);

	//---------------------------【3】正式检测-------------------------------------
	//进行角点检测
	cornerHarris(grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT);

	// 归一化与转换
	normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImage, scaledImage);//将归一化后的图线性变换成8位无符号整型 

	//---------------------------【4】进行绘制-------------------------------------
	// 将检测到的，且符合阈值条件的角点绘制出来
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				circle(srcImage1, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}
	//---------------------------【5】显示最终效果---------------------------------
	showImage(srcImage1);
	showImage2NewWidget(scaledImage, "scaledImage");
}

void PictureEditor::SlotPushButtonShiTomasiClick()
{
	Mat grayImage;
	int maxCornerNumber = 33;
	int maxTrackbarNumber = 500;
	RNG rng(12345);//初始化随机数生成器

	//【1】对变量小于等于1时的处理
	if (maxCornerNumber <= 1) { maxCornerNumber = 1; }

	//【2】Shi-Tomasi算法（goodFeaturesToTrack函数）的参数准备
	std::vector<Point2f> corners;
	double qualityLevel = 0.01;//角点检测可接受的最小特征值
	double minDistance = 10;//角点之间的最小距离
	int blockSize = 3;//计算导数自相关矩阵时指定的邻域范围
	double k = 0.04;//权重系数
	Mat copyImage = srcImage.clone();	//复制源图像到一个临时变量中，作为感兴趣区域

	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//【3】进行Shi-Tomasi角点检测
	goodFeaturesToTrack(grayImage,//输入图像
		corners,//检测到的角点的输出向量
		maxCornerNumber,//角点的最大数量
		qualityLevel,//角点检测可接受的最小特征值
		minDistance,//角点之间的最小距离
		Mat(),//感兴趣区域
		blockSize,//计算导数自相关矩阵时指定的邻域范围
		false,//不使用Harris角点检测
		k);//权重系数


	//【4】输出文字信息
	qDebug() << QString("\t>此次检测到的角点数量为:") << corners.size();

	//【5】绘制检测到的角点
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		//以随机的颜色绘制出角点
		circle(copyImage, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}
	showImage(copyImage);
}

void PictureEditor::SlotPushButtonSubpixClick()
{
	Mat grayImage;
	int maxCornerNumber = 33;
	int maxTrackbarNumber = 500;
	RNG rng(12345);//初始化随机数生成器

	//【1】对变量小于等于1时的处理
	if (maxCornerNumber <= 1) { maxCornerNumber = 1; }

	//【2】Shi-Tomasi算法（goodFeaturesToTrack函数）的参数准备
	std::vector<Point2f> corners;
	double qualityLevel = 0.01;//角点检测可接受的最小特征值
	double minDistance = 10;//角点之间的最小距离
	int blockSize = 3;//计算导数自相关矩阵时指定的邻域范围
	double k = 0.04;//权重系数
	Mat copyImage = srcImage.clone();	//复制源图像到一个临时变量中，作为感兴趣区域

	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//【3】进行Shi-Tomasi角点检测
	goodFeaturesToTrack(grayImage,//输入图像
		corners,//检测到的角点的输出向量
		maxCornerNumber,//角点的最大数量
		qualityLevel,//角点检测可接受的最小特征值
		minDistance,//角点之间的最小距离
		Mat(),//感兴趣区域
		blockSize,//计算导数自相关矩阵时指定的邻域范围
		false,//不使用Harris角点检测
		k);//权重系数


	//【4】输出文字信息
	qDebug() << QString("\t>此次检测到的角点数量为:") << corners.size();

	//【5】绘制检测到的角点
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		//以随机的颜色绘制出角点
		circle(copyImage, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}
	showImage(copyImage);

	//【7】亚像素角点检测的参数设置
	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);
	//此句代码的OpenCV2版为：
	//TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 );
	//此句代码的OpenCV3版为：
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.001);

	//【8】计算出亚像素角点位置
	cornerSubPix(grayImage, corners, winSize, zeroZone, criteria);

	//【9】输出角点信息
	for (int i = 0; i < corners.size(); i++)
	{
		qDebug() << " \t>>精确角点坐标[" << i << "]  (" << corners[i].x << "," << corners[i].y << ")" << endl;
	}
}

void PictureEditor::SlotSpinBox(int nThresh)
{
	if (srcImage.empty())
		return;
	Mat grayImage;
	int nThresh_max = 255;
	RNG rng(12345);
	Mat cannyMat_output;
	std::vector<std::vector<Point>> vContours;
	std::vector<Vec4i> vHierarchy;
	// 转成灰度并模糊化降噪
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	blur(grayImage, grayImage, Size(3, 3));

	//if (grayImgWgt == nullptr)
	//	grayImgWgt = new QImageWidget;
	//grayImgWgt->resize(ui.label_pic->size());
	//grayImgWgt->setPixmap(grayImage);
	//grayImgWgt->show();
	//grayImgWgt->setWindowTitle("midImage");

	// 用Canny算子检测边缘
	Canny(grayImage, cannyMat_output, nThresh, static_cast<long long>(nThresh) * 2, 3);
	// 寻找轮廓
	findContours(cannyMat_output, vContours, vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 绘出轮廓
	Mat drawing = Mat::zeros(cannyMat_output.size(), CV_8UC3);
	for (int i = 0; i < vContours.size(); ++i)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, vContours, i, color, 2, 8, vHierarchy, 0, Point());
	}

	//if (contourWgt == nullptr)
	//	contourWgt = new QImageWidget;
	//contourWgt->resize(ui.label_pic->size());
	//contourWgt->setPixmap(drawing);
	//contourWgt->show();
	//contourWgt->setWindowTitle("midImage");
	showImage(drawing);
}

QImage PictureEditor::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

void PictureEditor::showImage(const cv::Mat& mat)
{
	auto qimage = cvMat2QImage(mat);
	auto pix = QPixmap::fromImage(qimage);
	if (pix.width() <= ui.widget->width() && pix.height() <= ui.widget->height())
	{
		ui.label_pic->resize(pix.width(), pix.height());
		ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
	}
	else
	{
		if (pix.width() <= ui.widget->width())
		{
			ui.label_pic->resize(static_cast<int>(static_cast<double>(pix.width()) * ui.widget->height() / pix.height()), pix.height());
			ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
		}
		else if (pix.height() <= ui.widget->height())
		{
			ui.label_pic->resize(pix.width(), static_cast<int>(static_cast<double>(pix.height()) * ui.widget->width() / pix.width()));
			ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
		}
		else
		{
			double ratio_w = static_cast<double>(ui.widget->width()) / pix.width();
			double ratio_h = static_cast<double>(ui.widget->height()) / pix.height();
			double ratio_min = ratio_w < ratio_h ? ratio_w : ratio_h;
			ui.label_pic->resize(static_cast<int>(ratio_min * pix.width()), static_cast<int>(ratio_min * pix.height()));
			ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
		}
	}
}

void PictureEditor::showImage2NewWidget(const cv::Mat& mat, const QString& windowname /*= "disImage"*/)
{
	QImageWidget* imageWidget = new QImageWidget;
	imageWidget->resize(500, 500);
	imageWidget->setPixmap(mat);
	imageWidget->show();
	imageWidget->setWindowTitle(windowname);
	connect(imageWidget, &QImageWidget::closed, imageWidget, &QImageWidget::deleteLater);
}
