#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PictureFactory.h"
#include "PictureEditor.h"
#include "PicturesMix.h"

class PictureFactory : public QMainWindow
{
	Q_OBJECT

public:
	PictureFactory(QWidget* parent = Q_NULLPTR);
	~PictureFactory();

private slots:
	void SlotPushButtonEditorClick();
	void SlotPushButtonMixClick();

private:
	Ui::PictureFactoryClass ui;
	PictureEditor* pEditor;
	PicturesMix* pMix;
};
