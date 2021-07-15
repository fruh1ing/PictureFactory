#pragma once
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QImage>
using namespace cv;

class QImageWidget : public QWidget
{
	Q_OBJECT

public:
	QImageWidget(QWidget* parent = nullptr);
	~QImageWidget();
	void setPixmap(const QPixmap& p);
	void setPixmap(const QImage& img);
	void setPixmap(const cv::Mat& mat);
signals:
	void closed();
protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void closeEvent(QCloseEvent* event);
	QImage cvMat2QImage(const cv::Mat& mat);

private:
	QPixmap pix;
};
