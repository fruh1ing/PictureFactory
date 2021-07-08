#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PictureFactory.h"

class PictureFactory : public QMainWindow
{
    Q_OBJECT

public:
    PictureFactory(QWidget *parent = Q_NULLPTR);

private:
    Ui::PictureFactoryClass ui;
};
