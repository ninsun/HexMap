#include "HexGrid.h"
#include "HexCell.h"

HexGrid::HexGrid(QObject *parent)
    : QObject(parent)
{
}

HexGrid::~HexGrid()
{
}

void HexGrid::initGrid(int width, int height, HexCell::DirctionType type)
{
    qDeleteAll(m_grid);
    m_grid.clear();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            m_grid[{x, y}] = new HexCell(this, x, y, type);
        }
    }
}