#include "HexMapGraphicsScene.h"
#include "HexGrid.h"
#include "HexCellGraphicsItem.h"
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

HexMapGraphicsScene::HexMapGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

HexMapGraphicsScene::~HexMapGraphicsScene()
{
}

void HexMapGraphicsScene::generateGrid()
{
    clear();
    setSceneRect(0, 0, 0, 0);
    if (m_grid->cellCount())
    {
        HexCell *_cell = nullptr;
        if (m_grid->type() == HexCell::DirectionH)
        {
            double w = qSqrt(3) * m_size;
            double h = 2.0 * m_size;
            double posX = w * 0.5;
            double posY = h * 0.5;
            for (int y = 0; y < m_grid->height(); y++)
            {
                for (int x = 0; x < m_grid->width(); x++)
                {
                    _cell = m_grid->cell(x, y);
                    auto _item = new HexCellGraphicsItem(_cell, m_size);
                    posY = h * 0.5 + h * 0.75 * y;
                    if (y & 1)
                    {
                        posX = w + w * x;
                    }
                    else
                    {
                        posX = w * 0.5 + w * x;
                    }
                    _item->setPos(posX, posY);
                    addItem(_item);
                }
            }
        }
        else
        {
            double w = 2.0 * m_size;
            double h = qSqrt(3) * m_size;
            double posX = w * 0.5;
            double posY = h * 0.5;
            for (int x = 0; x < m_grid->width(); x++)
            {
                for (int y = 0; y < m_grid->height(); y++)
                {
                    _cell = m_grid->cell(x, y);
                    auto _item = new HexCellGraphicsItem(_cell, m_size);
                    posX = w * 0.5 + w * 0.75 * x;
                    if (x & 1)
                    {
                        posY = h + h * y;
                    }
                    else
                    {
                        posY = h * 0.5 + h * y;
                    }
                    _item->setPos(posX, posY);

                    addItem(_item);
                }
            }
        }
    }
}

void HexMapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto _items = items(mouseEvent->scenePos());
    for (auto item : _items)
    {
        auto _cellItem = qgraphicsitem_cast<HexCellGraphicsItem *>(item);
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}