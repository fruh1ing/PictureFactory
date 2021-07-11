#include "PicturesMix.h"
#include "ui_PicturesMix.h"

PicturesMix::PicturesMix(QWidget* parent)
	: QWidget(parent)
{
	ui = new Ui::PicturesMix();
	ui->setupUi(this);
	layout = new QHBoxLayout(this);
	img1 = new QImageWidget(this);
	img2 = new QImageWidget(this);
	layout->addWidget(img1);
	layout->addWidget(img2);
	this->setLayout(layout);
	setAttribute(Qt::WA_QuitOnClose, false);

	QImage image1, image2;
	image1.load("D:\\pic\\astronaut.jpg");
	image2.load("D:\\pic\\lena.jpg");
	img1->setPixmap(image1);
	img2->setPixmap(image2);
}

PicturesMix::~PicturesMix()
{
	delete ui;
}
