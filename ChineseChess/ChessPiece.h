#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <QString>
#include <QObject>

class ChessPiece
{
public:
    ChessPiece();
    ~ChessPiece();

    enum TYPE {JIANG, SHUAI, SHI_RED, SHI_BLACK, XIANG_RED, XIANG_BLACK,CHE, MA, PAO, ZU, BING};
    int id_;
    int row_;
    int col_;
    TYPE type_;
    bool is_red_;
    bool is_dead_;

    //初始化棋子
    void Initial(int id);
    //根据棋子类型获取棋子对应中文显示
    QString GetText();

};

#endif // CHESSPIECE_H
