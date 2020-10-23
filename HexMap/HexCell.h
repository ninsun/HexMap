#pragma once

#include <QMap>
#include <QVariant>

class HexGrid;
class HexCell
{
public:
    enum DirctionType
    {
        DirectionH, // �����ʾ���������Ϊֱ��
        DirectionV  // �����ʾ���������Ϊֱ��
    };
    enum Direction
    {
        UpLeft,     // ����λ
        UpRight,    // ����λ
        Right,      // ����λ
        DownRight,  // ����λ
        DownLeft,   // ����λ
        Left,       // ����λ
        Up,         // ����λ
        RightUp,    // ����λ
        RightDown,  // ����λ
        Down,       // ����λ
        LeftDown,   // ����λ
        LeftUp,     // ����λ
    };
    enum DataRole
    {
        TypeRole,           // ����
        TextRole,           // ����
        FrontgroundRole,    // ǰ��
        BackgroundRole,     // ����
        BorderRole,         // �߿�
        UserRole = 1000,
    };
    enum TypeRole
    {
        Reachable,
        Unreachable,
    };
public:
    HexCell(HexGrid *grid, int x = 0, int y = 0, DirctionType type = DirectionH);
    ~HexCell();
public:
    HexGrid *grid() { return m_grid; }
    int x() { return m_x; }
    int y() { return m_y; }
    std::tuple<int, int, int> cube();
    DirctionType type() { return m_type; }

    const QVariant &data(int role) const { return m_data[role]; }
    void setData(int role, const QVariant &data) { m_data[role] = data; }

    const QMap<int, QVariant> &allData() const { return m_data; }
    void setAllData(const QMap<int, QVariant> &data) { m_data = data; }
public:
    HexCell *neighborCell(Direction dir);
    QList<HexCell *> neighborCells();
    bool isNeighborCell(HexCell *cell);
    int distanceTo(HexCell *cell);
    QList<HexCell *> pathTo(HexCell *cell);
private:
    HexGrid *m_grid = nullptr;
    int m_x = 0;
    int m_y = 0;
    DirctionType m_type = DirectionH;
    QMap<int, QVariant> m_data;
};