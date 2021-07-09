#include "PictureFactory.h"

PictureFactory::PictureFactory(QWidget* parent)
	: QMainWindow(parent)
	, pEditor(new PictureEditor)
{
	ui.setupUi(this);
	connect(ui.pushButton_editor, &QPushButton::clicked, this, &PictureFactory::SlotPushButtonEditorClick);
}

PictureFactory::~PictureFactory()
{
	delete pEditor;
}

void PictureFactory::SlotPushButtonEditorClick()
{
	pEditor->show();
}
