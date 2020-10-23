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
    // ����ʱʹ������ķ�λ��������ʱʹ�ú���ķ�λ, �Ϳ��Է���һ����
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
        // ��ƽ������ϵת��Ϊcube����ϵ
        auto a = cube();
        auto b = cell->cube();
        // ����Cell���������x,y,z�������Ǹ����־�������Cell֮�����̾���
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
        // ʹ��A*�㷨
        // A*�㷨���Եó�ÿһ����������λ����ε������, �������ǾͿ���֪��Ŀ�����ε��������, ������Ҳһ��
        // ���ǿ��Ա���ÿһ�����Ǵ��ĸ�������, ���������û����Դ��, �����ھ��Ǵ��������
        QMap<HexCell *, HexCell *> _cameFrom;
        _cameFrom[this] = nullptr;
        // A*�㷨Ҳ���Լ������㵽ÿһ�������ĵ������(�����һ���ƶ������ڵĵ�Ļ���������1, �����ж�������)
        // ��һ�����ƶ������ڵ�Ķ���������Ҫʹ�ö���ķ�����¼/����
        // �������ǿ��Ա�֤����Ŀ��������������С��, �������ۼӵ�, ����������Ȼ��0, ���ǿ�����һ��Map��¼
        QMap<HexCell *, int> _cost;
        _cost[this] = 0;

        // A*�㷨��Ҫһ��"���ȶ���", ��"���ȶ�"���/��С�����ȴӶ�����ȡ��,
        // Qt�е�QMultiMap�����ȶ�(ת��Ϊint)��Ϊkey, ����ʵ�ְ������ȶ�����,
        // ͨ��firstKey()��lastKey()���Ի�ȡ���ȶ��������͵�Ԫ�ض�Ӧ�����ȶ�,
        // ͨ��take(firstKey()),take(lastKey())���Ի�ȡ���ȶ���͵Ļ���ߵ�һ��Ԫ��
        // A*�㷨�����ȶ����е����ȶ�, ָ�ľ��Ǵ���㵽����������� + �Ǹ����Ŀ��ľ���, ԽСԽ��
        // �����������ѡ��������С��, ��ô���Ǿ���ѡ����õ�·��, ��Ȼ, ��㴦�����ȶ�Ϊ0
        QMultiMap<int, HexCell *> _priorityMap;
        _priorityMap.insert(0, this);

        // ���ȶ����������ֵ, ��ô���ǾͿ��Կ�ʼ������
        while (!_priorityMap.isEmpty())
        {
            // ��������ȡ��������С���Ǹ�(firstKey��Ӧ��Ԫ��)��Ϊ��ǰλ��
            auto _current = _priorityMap.take(_priorityMap.firstKey());

            // �����ǰ�ľ���Ŀ��, �ͱ�ʾ�����Ѿ�������, �����˳�ѭ����
            if (_current == target)
            {
                break;
            }

            // ����, �����ҵ���ǰλ�õ������ھ�
            for (auto next : neighborCells())
            {
                // ����ھӵ�����ΪUnreachable, ���������ھ�
                if (!next || next->data(TypeRole) == Unreachable)
                {
                    continue;
                }
                // ���㵽�ﵱǰλ��ʱ�Ѿ�����������+�����ھӵ�����(ĿǰĬ��Ϊ1)��Ϊ�µ�����
                int _newCost = _cost[_current] + 1;
                // �������û�м������ǰλ�õ�����, ���ߵ�ǰ������С��֮ǰ�����������, ��ʹ�����������
                if (!_cost.contains(next) || _newCost < _cost.value(next))
                {
                    _cost[next] = _newCost;
                    // һ��λ�õ����ȶ�Ϊ���ﵱǰλ�õ�����+��ǰλ�õ�Ŀ���ľ���
                    int _priority = _newCost + next->distanceTo(target);
                    _priorityMap.insert(_priority, next);
                    // ��¼һ��������Ǵ��ĸ�������
                    _cameFrom[next] = _current;
                }
            }
        }
        // ���ȶȶ�����ûֵ�˻����Ѿ��ҵ�Ŀ����
        // ͨ���ж�Ŀ����Ƿ���Դ�ĳ���㵽��, �Ϳ����ж��Ƿ����һ��·��
        if (_cameFrom.contains(target))
        {
            // Ŀ����Դ�ĳ���㵽��, ��ô·��һ������, �����Ƴ����·��
            auto _cell = target;
            // ���ǰ��·��ֵ�λ�÷��ڶ�����ǰ��
            _path.push_front(_cell);
            while (_cell && _cell != this)
            {
                _cell = _cameFrom.value(_cell);
                if (_cell)
                {
                    // ���ǰ��·��ֵ�λ�÷��ڶ�����ǰ��
                    _path.push_front(_cell);
                }
            }
        }
    }
    return _path;
}