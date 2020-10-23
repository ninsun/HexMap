#include "HexCell.h"
#include "HexGrid.h"

HexCell::HexCell(HexGrid *grid, int x, int y, DirctionType type)
    : m_grid(grid), m_x(x), m_y(y), m_type(type)
{
}

HexCell::~HexCell()
{
}

std::tuple<int, int, int> HexCell::cube()
{
    int x = 0;
    int y = 0;
    int z = 0;
    switch (m_type)
    {
    case HexCell::DirectionH:
        x = m_x - (m_y - (m_y & 1)) / 2;
        z = m_y;
        y = -x - z;
        break;
    case HexCell::DirectionV:
        x = m_x;
        z = m_y - (m_x - (m_x & 1)) / 2;
        y = -x - z;
        break;
    default:
        break;
    }
    return std::tuple<int, int, int>(x, y, z);
}

HexCell *HexCell::neighborCell(Direction dir)
{
    // 横向时使用纵向的方位或者纵向时使用横向的方位, 就可以返回一个空
    if ((m_type == DirectionH && dir >= Up) || (m_type == DirectionV && dir < Up))
        return nullptr;
    int x = m_x;
    int y = m_y;
    switch (dir)
    {
    case HexCell::UpLeft:
        --y;
        break;
    case HexCell::UpRight:
        ++x;
        --y;
        break;
    case HexCell::Right:
        ++x;
        break;
    case HexCell::DownRight:
        ++x;
        ++y;
        break;
    case HexCell::DownLeft:
        ++y;
        break;
    case HexCell::Left:
        --x;
        break;
    case HexCell::Up:
        --y;
        break;
    case HexCell::RightUp:
        ++x;
        --y;
        break;
    case HexCell::RightDown:
        ++x;
        break;
    case HexCell::Down:
        ++y;
        break;
    case HexCell::LeftDown:
        --x;
        break;
    case HexCell::LeftUp:
        --x;
        --y;
        break;
    default:
        break;
    }

    return m_grid->cell(x, y);
}

QList<HexCell *> HexCell::neighborCells()
{
    QList<HexCell *> _list;

    if (m_type == DirectionH)
    {
        for (int i = UpLeft; i <= Left; ++i)
        {
            auto _cell = neighborCell((Direction)i);
            if (_cell)
                _list << _cell;
        }
    }
    else
    {
        for (int i = Up; i <= LeftUp; ++i)
        {
            auto _cell = neighborCell((Direction)i);
            if (_cell)
                _list << _cell;
        }
    }

    return _list;
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
        {
            bool _nextCol = (cell->x() - m_x == 1);
            return (_sameRow && _neighborCol) || (_neighborRow && (_sameCol || _nextCol));
        }
        case HexCell::DirectionV:
        {
            bool _nextRow = (cell->y() - m_y == 1);
            return (_sameCol && _neighborRow) || (_neighborCol && (_sameRow || _nextRow));
        }
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
        // 将平面坐标系转化为cube坐标系
        auto a = cube();
        auto b = cell->cube();
        // 两个Cell的相对坐标x,y,z中最大的那个数字就是两个Cell之间的最短距离
        return qMax(qAbs(std::get<0>(a) - std::get<0>(b)),
            qMax(qAbs(std::get<1>(a) - std::get<1>(b)),
                qAbs(std::get<2>(a) - std::get<2>(b))
            )
        );
    }
    else
    {
        return -1;
    }
}

QList<HexCell *> HexCell::pathTo(HexCell *target)
{
    QList<HexCell *> _path;
    if (target && m_grid && target->type() == m_type && target->grid() == m_grid)
    {
        // 使用A*算法
        // A*算法可以得出每一个遍历过的位置如何到达起点, 这样我们就可以知道目标点如何到达起点了, 反过来也一样
        // 我们可以保存每一个点是从哪个点来的, 例如起点是没有来源的, 起点的邻居是从起点来的
        QMap<HexCell *, HexCell *> _cameFrom;
        _cameFrom[this] = nullptr;
        // A*算法也可以计算从起点到每一个遍历的点的消耗(假设从一点移动到相邻的点的基础消耗是1, 可以有额外消耗)
        // 从一个点移动到相邻点的额外消耗需要使用额外的方法记录/计算
        // 这样我们可以保证到达目标点的消耗总是最小的, 消耗是累加的, 起点的消耗显然是0, 我们可以用一个Map记录
        QMap<HexCell *, int> _cost;
        _cost[this] = 0;

        // A*算法需要一个"优先队列", 即"优先度"最大/最小的最先从队列中取出,
        // Qt中的QMultiMap将优先度(转化为int)作为key, 可以实现按照优先度排序,
        // 通过firstKey()和lastKey()可以获取优先度最高与最低的元素对应的优先度,
        // 通过take(firstKey()),take(lastKey())可以获取优先度最低的或最高的一个元素
        // A*算法中优先队列中的优先度, 指的就是从起点到其它点的消耗 + 那个点距目标的距离, 越小越好
        // 如果我们总是选择消耗最小的, 那么我们就能选择最好的路径, 显然, 起点处的优先度为0
        QMultiMap<int, HexCell *> _priorityMap;
        _priorityMap.insert(0, this);

        // 优先队列中如果有值, 那么我们就可以开始计算了
        while (!_priorityMap.isEmpty())
        {
            // 我们首先取出消耗最小的那个(firstKey对应的元素)作为当前位置
            auto _current = _priorityMap.take(_priorityMap.firstKey());

            // 如果当前的就是目标, 就表示我们已经到达了, 可以退出循环了
            if (_current == target)
            {
                break;
            }

            // 否则, 我们找到当前位置的所有邻居
            for (auto next : neighborCells())
            {
                // 如果邻居的类型为Unreachable, 则跳过该邻居
                if (!next || next->data(TypeRole) == Unreachable)
                {
                    continue;
                }
                // 计算到达当前位置时已经产生的消耗+到达邻居的消耗(目前默认为1)作为新的消耗
                int _newCost = _cost[_current] + 1;
                // 如果从来没有计算过当前位置的消耗, 或者当前的消耗小于之前计算过的消耗, 则使用这个新消耗
                if (!_cost.contains(next) || _newCost < _cost.value(next))
                {
                    _cost[next] = _newCost;
                    // 一个位置的优先度为到达当前位置的消耗+当前位置到目标点的距离
                    int _priority = _newCost + next->distanceTo(target);
                    _priorityMap.insert(_priority, next);
                    // 记录一下这个点是从哪个点来的
                    _cameFrom[next] = _current;
                }
            }
        }
        // 优先度队列中没值了或者已经找到目标了
        // 通过判断目标点是否可以从某个点到达, 就可以判断是否存在一个路径
        if (_cameFrom.contains(target))
        {
            // 目标可以从某个点到达, 那么路径一定存在, 反向推出这个路径
            auto _cell = target;
            // 总是把新发现的位置放在队列最前面
            _path.push_front(_cell);
            while (_cell && _cell != this)
            {
                _cell = _cameFrom.value(_cell);
                if (_cell)
                {
                    // 总是把新发现的位置放在队列最前面
                    _path.push_front(_cell);
                }
            }
        }
    }
    return _path;
}