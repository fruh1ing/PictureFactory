#pragma once

#include <QWidget>
#include "ui_PictureEditor.h"
#include<opencv2/opencv.hpp>

using cv::Mat;

class PictureEditor : public QWidget
{
	Q_OBJECT

public:
	PictureEditor(QWidget* parent = Q_NULLPTR);
	~PictureEditor();

	QImage cvMat2QImage(const cv::Mat& mat);
	void showImage(const cv::Mat& mat);

private slots:
	void SlotPushButtonLoadClick();
	void SlotPushButtonOrigClick();
	void SlotPushButtonEronClick();
	void SlotPushButtonBlurClick();
	void SlotPushButtonEdgeClick();
	void SlotPushButtonChanRClick();
	void SlotPushButtonWeightMixClick();
	void SlotPushButtonDftClick();
	void SlotPushButtonBoxClick();
	void SlotPushButtonMeanClick();
	void SlotPushButtonGaussClick();
	void SlotPushButtonSobelClick();
	void SlotPushButtonLapLacianClick();
	void SlotPushButtonScharrClick();
	void SlotPushButtonHougnLinesClick();
	void SlotPushButtonRemapClick();
	void SlotPushButtonWarpClick();
	void SlotPushButtonHistClick();


private:
	Ui::PictureEditor ui;
	cv::Mat srcImage;
	cv::Mat dstImage;
	QString filename;
};
