#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HexMap.h"

class HexMap : public QMainWindow
{
    Q_OBJECT

public:
    HexMap(QWidget *parent = Q_NULLPTR);

private:
    Ui::HexMapClass ui;
};
