#pragma once

#include <QGraphicsPolygonItem>
#include "HexCell.h"
class HexCellGraphicsItem : public QGraphicsPolygonItem
{
public:
    HexCellGraphicsItem(HexCell *cell, int size, QGraphicsItem *parent = nullptr);
    ~HexCellGraphicsItem();
public:
    void setHighlight(bool val);
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    HexCell *m_cell = nullptr;
    int m_size = 10;
    QGraphicsTextItem *m_text = nullptr;
};
