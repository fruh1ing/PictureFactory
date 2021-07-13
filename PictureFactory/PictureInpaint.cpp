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

void PictureInpaint::SlotPushButtonLoadClick()
{
	filename = QFileDialog::getOpenFileName(this, tr("open image"), "D:/pic", tr("image files(*.png *.jpg *.bmp *jpeg)"));
	if (filename.isEmpty())
		return;
	srcImage = cv::imread(filename.toStdString());
	ui.imageSrc->setPixmap(srcImage);
}
