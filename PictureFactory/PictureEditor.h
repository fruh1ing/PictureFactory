#pragma once

#include <QWidget>
#include "ui_PictureEditor.h"
#include<opencv2/opencv.hpp>

class PictureEditor : public QWidget
{
	Q_OBJECT

public:
	PictureEditor(QWidget* parent = Q_NULLPTR);
	~PictureEditor();

	QImage cvMat2QImage(const cv::Mat& mat);

private slots:
	void SlotPushButtonLoadClick();
	void SlotPushButtonEronClick();
	void SlotPushButtonBlurClick();
	void SlotPushButtonEdgeClick();

private:
	Ui::PictureEditor ui;
	cv::Mat srcImage;
	cv::Mat dstImage;
};
