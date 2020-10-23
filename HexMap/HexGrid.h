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
    int cellCount() { return m_grid.count(); }
    int width() { return m_width; }
    int height() { return m_height; }
    HexCell::DirctionType type() { return m_type; }
private:
    int m_width = 0;
    int m_height = 0;
    HexCell::DirctionType m_type = HexCell::DirectionH;
    QMap<QPair<int, int>, HexCell *> m_grid;
};
