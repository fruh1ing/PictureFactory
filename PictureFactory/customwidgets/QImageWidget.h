#pragma once
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QImage>
using namespace cv;

class QImageWidget : public QWidget
{
	Q_OBJECT

public:
	QImageWidget(QWidget* parent);
	~QImageWidget();
	void setPixmap(QPixmap& p);
	void setPixmap(QImage& img);
	void setPixmap(cv::Mat& mat);
protected:
	void paintEvent(QPaintEvent* event);
	QImage cvMat2QImage(const cv::Mat& mat);

private:
	QPixmap pix;
};
