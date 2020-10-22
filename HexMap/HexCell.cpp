#include "HexCell.h"
#include "HexGrid.h"

HexCell::HexCell(HexGrid *grid, int x = 0, int y = 0, DirctionType type = DirectionH)
    : m_grid(grid), m_x(x), m_y(y), m_type(type)
{
}

HexCell::~HexCell()
{
}

HexCell *HexCell::neighborCell(Direction dir)
{
    // 横向时使用纵向的方位或者纵向时使用横向的方位, 就可以返回一个空
    if ((m_type == DirectionH && dir >= Up) || (m_type == DirectionV && dir < Up))
        return nullptr;
    int _x = m_x;
    int _y = m_y;
    switch (dir)
    {
    case HexCell::UpLeft:
        --_y;
        break;
    case HexCell::UpRight:
        ++_x;
        --_y;
        break;
    case HexCell::Right:
        ++_x;
        break;
    case HexCell::DownRight:
        ++_x;
        ++_y;
        break;
    case HexCell::DownLeft:
        ++_y;
        break;
    case HexCell::Left:
        --_x;
        break;
    case HexCell::Up:
        --_y;
        break;
    case HexCell::RightUp:
        ++_x;
        --_y;
        break;
    case HexCell::RightDown:
        ++_x;
        break;
    case HexCell::Down:
        ++_y;
        break;
    case HexCell::LeftDown:
        --_x;
        break;
    case HexCell::LeftUp:
        --_x;
        --_y;
        break;
    default:
        break;
    }

    return m_grid->cell(_x, _y);
}

bool HexCell::isNeighborCell(HexCell *cell)
{
    if (cell == this)
    {
        return false;
    }
    else if (cell && m_grid && cell->type() == m_type && cell->grid() == m_grid)
    {
        bool _sameRow = (cell->y() == m_y);
        bool _sameCol = (cell->x() == m_x);
        bool _neighborRow = (qAbs(cell->y() - m_y) == 1);
        bool _neighborCol = (qAbs(cell->x() - m_x) == 1);
        switch (m_type)
        {
        case HexCell::DirectionH:
            bool _nextCol = (cell->x() - m_x == 1);
            return (_sameRow && _neighborCol) || (_neighborRow && (_sameCol || _nextCol));
        case HexCell::DirectionV:
            bool _nextRow = (cell->y() - m_y == 1);
            return (_sameCol && _neighborRow) || (_neighborCol && (_sameRow || _nextRow));
        default:
            return false;
        }
    }
    else
    {
        return false;
    }
}

int HexCell::distanceTo(HexCell *cell)
{
    if (cell == this)
    {
        return 0;
    }
    else if (cell && m_grid && cell->type() == m_type && cell->grid() == m_grid)
    {
        // TODO:
    }
    else
    {
        return -1;
    }
}