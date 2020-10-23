#pragma once

#include <QMap>
#include <QVariant>

class HexGrid;
class HexCell
{
public:
    enum DirctionType
    {
        DirectionH, // 横向表示横向可排列为直线
        DirectionV  // 纵向表示纵向可排列为直线
    };
    enum Direction
    {
        UpLeft,     // 横向方位
        UpRight,    // 横向方位
        Right,      // 横向方位
        DownRight,  // 横向方位
        DownLeft,   // 横向方位
        Left,       // 横向方位
        Up,         // 纵向方位
        RightUp,    // 纵向方位
        RightDown,  // 纵向方位
        Down,       // 纵向方位
        LeftDown,   // 纵向方位
        LeftUp,     // 纵向方位
    };
    enum DataRole
    {
        TypeRole,           // 类型
        TextRole,           // 文字
        FrontgroundRole,    // 前景
        BackgroundRole,     // 背景
        BorderRole,         // 边框
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