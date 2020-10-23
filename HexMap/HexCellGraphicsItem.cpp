#include "HexCellGraphicsItem.h"
#include <QtMath>
#include <QPen>
#include <QTextDocument>
#include <QTextOption>

HexCellGraphicsItem::HexCellGraphicsItem(HexCell *cell, int size, QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent), m_cell(cell), m_size(size)
{
    setAcceptHoverEvents(true);

    setPen(QPen(QBrush(Qt::black), 2));
    setBrush(Qt::white);

    m_text = new QGraphicsTextItem(this);
    m_text->setPlainText(QString("%1,%2").arg(cell->x()).arg(cell->y()));
    m_text->setVisible(false);
    auto document = m_text->document();
    auto option = document->defaultTextOption();
    option.setAlignment(Qt::AlignCenter);
    document->setDefaultTextOption(option);

    if (cell->type() == HexCell::DirectionH)
    {
        double w = w = qSqrt(3) * m_size;
        double h = 2.0 * m_size;

        QPolygonF _p;
        _p.append(QPointF(0, -h * 0.5));
        _p.append(QPointF(w * 0.5, -h * 0.25));
        _p.append(QPointF(w * 0.5, h * 0.25));
        _p.append(QPointF(0, h * 0.5));
        _p.append(QPointF(-w * 0.5, h * 0.25));
        _p.append(QPointF(-w * 0.5, -h * 0.25));
        setPolygon(_p);

        m_text->setTextWidth(w);
        m_text->setPos(-w * 0.5, -h * 0.25);
    }
    else
    {
        double w = 2.0 * m_size;
        double h = qSqrt(3) * m_size;

        QPolygonF _p;
        _p.append(QPointF(-w * 0.5, 0));
        _p.append(QPointF(-w * 0.25, -h * 0.5));
        _p.append(QPointF(w * 0.25, -h * 0.5));
        _p.append(QPointF(w * 0.5, 0));
        _p.append(QPointF(w * 0.25, h * 0.5));
        _p.append(QPointF(-w * 0.25, h * 0.5));
        setPolygon(_p);

        m_text->setTextWidth(w);
        m_text->setPos(-w * 0.5, -h * 0.25);
    }
}

HexCellGraphicsItem::~HexCellGraphicsItem()
{
}

void HexCellGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setPen(QPen(QBrush(Qt::red), 2));
    setBrush(Qt::yellow);
    setZValue(1);
    m_text->setVisible(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void HexCellGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setPen(QPen(QBrush(Qt::black), 2));
    setBrush(Qt::white);
    setZValue(0);
    m_text->setVisible(false);
    QGraphicsItem::hoverEnterEvent(event);
}