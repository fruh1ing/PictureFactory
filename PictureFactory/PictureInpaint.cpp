#include "PictureInpaint.h"
#include "QFileDialog"

PictureInpaint::PictureInpaint(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, &QPushButton::clicked, this, &PictureInpaint::SlotPushButtonLoadClick);

}

PictureInpaint::~PictureInpaint()
{
}

void PictureInpaint::showImage(const Mat& mat, int index)
{
	if (index < 0 || index>3)
		return;
	switch (index)
	{
	case 0:
		ui.imageSrc->setPixmap(mat);
		break;
	case 1:
		ui.imageMid1->setPixmap(mat);
		break;
	case 2:
		ui.imageMid2->setPixmap(mat);
		break;
	case 3:
		ui.imageDst->setPixmap(mat);
		break;
	default:
		break;
	}
}

void PictureInpaint::SlotPushButtonLoadClick()
{
	filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp *jpeg)"));
	if (filename.isEmpty())
		return;
	srcImage = cv::imread(filename.toStdString());
	ui.imageSrc->setPixmap(srcImage);
}
