#pragma once

#include <QWidget>
#include "ui_PictureInpaint.h"
#include<opencv2/opencv.hpp>
#include "Processing.h"

using cv::Mat;
class PictureInpaint : public QWidget
{
	Q_OBJECT

public:
	PictureInpaint(QWidget* parent = Q_NULLPTR);
	~PictureInpaint();

	friend QImage cvMat2QImage(const cv::Mat& mat);
	void showImage(const Mat& mat, int index);
private slots:
	void SlotPushButtonLoadClick();


private:
	Ui::PictureInpaint ui;
	cv::Mat srcImage;
	cv::Mat dstImage;
	QString filename;
};
