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
	connect(ui.pushButton_bp, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonBPClick);
	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(SlotSpinBox(int)));
}

PictureEditor::~PictureEditor()
{
}

void PictureEditor::SlotPushButtonLoadClick()
{
	filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp)"));
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

void PictureEditor::SlotPushButtonBPClick()
{

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
