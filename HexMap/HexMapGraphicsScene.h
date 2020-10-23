#pragma once

#include <QGraphicsScene>

class HexGrid;
class HexMapGraphicsScene : public QGraphicsScene
{
public:
    HexMapGraphicsScene(QObject *parent = nullptr);
    ~HexMapGraphicsScene();
public:
    void setHexGrid(HexGrid *grid) { m_grid = grid; }
    void setHexCellSize(int size) { m_size = size; }
    void generateGrid();
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    HexGrid *m_grid = nullptr;
    int m_size = 10;
};
