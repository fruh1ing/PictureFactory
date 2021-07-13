#include "PictureFactory.h"

PictureFactory::PictureFactory(QWidget* parent)
	: QMainWindow(parent)
	, pEditor(new PictureEditor)
	, pMix(new PicturesMix)
	, pInpaint(new PictureInpaint)
{
	ui.setupUi(this);
	connect(ui.pushButton_editor, &QPushButton::clicked, this, &PictureFactory::SlotPushButtonEditorClick);
	connect(ui.pushButton_muti, &QPushButton::clicked, this, &PictureFactory::SlotPushButtonMixClick);
	connect(ui.pushButton_muti_2, &QPushButton::clicked, pInpaint, &PictureInpaint::show);
}

PictureFactory::~PictureFactory()
{
	delete pEditor;
	delete pMix;
}

void PictureFactory::SlotPushButtonEditorClick()
{
	pEditor->show();
}

void PictureFactory::SlotPushButtonMixClick()
{
	pMix->show();
}
