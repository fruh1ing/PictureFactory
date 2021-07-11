#pragma once

#include <QWidget>
#include "QImageWidget.h"
#include <QHBoxLayout>
namespace Ui { class PicturesMix; };

class PicturesMix : public QWidget
{
	Q_OBJECT

public:
	PicturesMix(QWidget* parent = Q_NULLPTR);
	~PicturesMix();

private:
	Ui::PicturesMix* ui;
	QHBoxLayout* layout;
	QImageWidget* img1;
	QImageWidget* img2;
};
