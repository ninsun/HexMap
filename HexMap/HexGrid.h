#pragma once

#include <QObject>
#include <QMap>
#include "HexCell.h"

class HexGrid : public QObject
{
    Q_OBJECT
public:
public:
    HexGrid(QObject *parent = nullptr);
    ~HexGrid();
public:
    void initGrid(int width, int height, HexCell::DirctionType type);
    HexCell *cell(int x, int y) { return m_grid.value({ x, y }); }
private:
    QMap<QPair<int, int>, HexCell *> m_grid;
};
