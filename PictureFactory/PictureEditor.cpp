#include "PictureEditor.h"
#include <QFileDialog>
#include <QDebug>
#include<opencv2/imgproc/imgproc.hpp>

PictureEditor::PictureEditor(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	connect(ui.pushButton_load, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonLoadClick);
	connect(ui.pushButton_erosion, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonEronClick);
	connect(ui.pushButton_blur, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonBlurClick);
	connect(ui.pushButton_edge, &QPushButton::clicked, this, &PictureEditor::SlotPushButtonEdgeClick);
}

PictureEditor::~PictureEditor()
{
}

void PictureEditor::SlotPushButtonLoadClick()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp)"));
	if (filename.isEmpty())
		return;
	QPixmap p(filename);
	ui.label_pic->setPixmap(p.scaled(ui.label_pic->width(), ui.label_pic->height()));
	srcImage = cv::imread(filename.toStdString());
	qDebug() << filename;
}

void PictureEditor::SlotPushButtonEronClick()
{
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	erode(srcImage, dstImage, element);
	auto qimage = cvMat2QImage(dstImage);
	auto pix = QPixmap::fromImage(qimage);
	ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
}

void PictureEditor::SlotPushButtonBlurClick()
{
	cv::blur(srcImage, dstImage, cv::Size(7, 7));
	auto qimage = cvMat2QImage(dstImage);
	auto pix = QPixmap::fromImage(qimage);
	ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
}

void PictureEditor::SlotPushButtonEdgeClick()
{
	cv::Mat grayImage;
	//dstImage.create(srcImage.size(), srcImage.type());
	cv::cvtColor(srcImage, grayImage, cv::COLOR_BGR2GRAY);
	cv::blur(grayImage, dstImage, cv::Size(3, 3));
	cv::Canny(dstImage, dstImage, 3, 9, 3);
	auto qimage = cvMat2QImage(dstImage);
	auto pix = QPixmap::fromImage(qimage);
	ui.label_pic->setPixmap(pix.scaled(ui.label_pic->width(), ui.label_pic->height()));
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
