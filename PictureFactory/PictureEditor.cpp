#include "PictureEditor.h"
#include <QFileDialog>
#include <QDebug>
#include<opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "Processing.h"

PictureEditor::PictureEditor(QWidget* parent)
	: QWidget(parent)
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

}

void PictureEditor::SlotPushButtonHougnLinesClick()
{

}

void PictureEditor::SlotPushButtonRemapClick()
{

}

void PictureEditor::SlotPushButtonWarpClick()
{

}

void PictureEditor::SlotPushButtonHistClick()
{

}

QImage PictureEditor::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		qDebug() << "CV_8UC1";
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
		qDebug() << "CV_8UC3";
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
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
