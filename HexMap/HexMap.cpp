#include "HexMap.h"
#include <QOpenGLWidget>
HexMap::HexMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.graphicsView->setScene(&m_scene);
    m_scene.setHexGrid(&m_grid);

    auto _viewport = new QOpenGLWidget();
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(4);//多重采样
    _viewport->setFormat(surfaceFormat);
    ui.graphicsView->setViewport(_viewport);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui.graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui.graphicsView->setCursor(Qt::ArrowCursor);
    ui.graphicsView->setMouseTracking(true);
    ui.graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui.graphicsView->setBackgroundBrush(QBrush(QColor::fromRgb(255, 255, 255)));
}

void HexMap::on_btnInitGrid_clicked()
{
    m_grid.initGrid(ui.width->value(), ui.height->value(),
        ui.directionH->isChecked() ? HexCell::DirectionH : HexCell::DirectionV);
    m_scene.setHexCellSize(ui.size->value());
    ui.graphicsView->setSceneRect(0, 0, 0, 0);
    m_scene.generateGrid();
}