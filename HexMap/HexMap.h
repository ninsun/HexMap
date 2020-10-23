#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HexMap.h"
#include "HexGrid.h"
#include "HexMapGraphicsScene.h"
class HexMap : public QMainWindow
{
    Q_OBJECT

public:
    HexMap(QWidget *parent = Q_NULLPTR);
private slots:
    void on_btnInitGrid_clicked();
private:
    Ui::HexMapClass ui;
    HexGrid m_grid;
    HexMapGraphicsScene m_scene;
};
