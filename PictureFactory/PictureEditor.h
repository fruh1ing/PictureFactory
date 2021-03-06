#pragma once

#include <QWidget>
#include "ui_PictureEditor.h"
#include<opencv2/opencv.hpp>

using cv::Mat;
class QImageWidget;
class PictureEditor : public QWidget
{
	Q_OBJECT

public:
	PictureEditor(QWidget* parent = Q_NULLPTR);
	~PictureEditor();

	QImage cvMat2QImage(const cv::Mat& mat);
	void showImage(const cv::Mat& mat);
	void showImage2NewWidget(const cv::Mat& mat, const QString& windowname = "disImage");

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
	void SlotPushButtonContourClick(bool);
	void SlotPushButtonHSClick();
	void SlotPushButtonHistogramClick();
	void SlotPushButtonHistogram2Click();
	void SlotPushButtonBPClick();
	void SlotPushButtonMatchTemplateClick();
	void SlotPushButtonHarrisClick();
	void SlotPushButtonShiTomasiClick();
	void SlotPushButtonSubpixClick();
	void SlotPushButtonSurfDetectClick();
	void SlotPushButtonSurfDescClick();
	void SlotPushButtonFlannMatchClick();
	void SlotPushButtonFlannSurfClick();
	void SlotPushButtonSiftClick();
	void SlotPushButtonLookClick();
	void SlotPushButtonOrbClick();
	void SlotPushButtonRepairClick();
	void SlotSpinBox(int);


private:
	Ui::PictureEditor ui;
	cv::Mat srcImage;
	cv::Mat dstImage;
	QString filename;

	QImageWidget* grayImgWgt;
	QImageWidget* contourWgt;
};
