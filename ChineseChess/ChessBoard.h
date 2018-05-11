#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include "ChessPiece.h"

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

    //画棋子
    void DrawChessPiece(QPainter &painter, int id);
    //棋盘坐标与像素值相互转换
    QPoint CoordinateToPixel(int row, int col);
    bool PixelToCoordinate(QPoint point, int &row, int &col);

    int Relation(int dx, int dy);

    //棋子移动规则
    bool CanMove(ChessPiece piece, int move2row, int move2col, bool flag);
    bool CanMoveJIANG(int row, int col, int move2row, int move2col);
    bool CanMoveSHI(int row, int col, int move2row, int move2col);
    bool CanMoveXIANG(int row, int col, int move2row, int move2col);
    bool CanMoveCHE(int row, int col, int move2row, int move2col);
    bool CanMoveMA(int row, int col, int move2row, int move2col);
    bool CanMovePAO(int row, int col, int move2row, int move2col, bool flag);
    bool CanMoveBING(int row, int col, int move2row, int move2col);
    bool CanMoveZU(int row, int col, int move2row, int move2col);

    //判断棋盘上某位置有没有棋子
    bool HasPiece(int row, int col);

    //棋盘格子长度
    const int grid_length_ = 40;
    //棋子半径
    const int piece_radius_ = grid_length_ / 2;
    //棋盘上的32棋子
    ChessPiece chess_piece_[32];

    //鼠标选中的棋子id
    int select_id_ = -1;
    //上一步操作
    int last_id_ = -1;
    //轮次标志
    bool red_turn_ = true;


signals:

public slots:
};

#endif // CHESSBOARD_H
