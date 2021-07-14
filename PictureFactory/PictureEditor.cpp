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

}

void PictureEditor::SlotPushButtonWarpClick()
{

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
