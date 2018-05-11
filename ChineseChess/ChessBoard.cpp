#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "ChessBoard.h"

#include <QPainter>
#include <QMouseEvent>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
    for(int i = 0; i < 32; ++i)
    {
        chess_piece_[i].Initial(i);
    }

}


void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //画棋盘
    //画10条横线
    for(int i = 1; i <= 10; ++i)
    {
        painter.drawLine(grid_length_, i * grid_length_, 9 * grid_length_, i * grid_length_);
    }
    //画9条竖线
    for(int i = 1; i <= 9; ++i)
    {
        if(i == 1 || i == 9)
        {
            painter.drawLine(i * grid_length_, grid_length_, i * grid_length_, 10 * grid_length_);
        }
        else
        {
            painter.drawLine(i * grid_length_, grid_length_, i * grid_length_, 5 * grid_length_);
            painter.drawLine(i * grid_length_, 6 * grid_length_, i * grid_length_, 10 * grid_length_);
        }

    }
    //画九宫格
    painter.drawLine(4 * grid_length_, grid_length_, 6 * grid_length_, 3 * grid_length_);
    painter.drawLine(4 * grid_length_, 3 * grid_length_, 6 * grid_length_, 1 * grid_length_);

    painter.drawLine(4 * grid_length_, 10 * grid_length_, 6 * grid_length_, 8 * grid_length_);
    painter.drawLine(4 * grid_length_, 8 * grid_length_, 6 * grid_length_, 10 * grid_length_);
    //楚河、汉界 旋转字实现？？
//    QRect rect1(2 * grid_length_, 5 * grid_length_, grid_length_, grid_length_);
//    painter.setFont(QFont("system", grid_length_ / 2, 700));
//    //painter.rotate(-90);
//    painter.drawText(rect1, Qt::AlignCenter, tr("楚"));

    //画棋子
    for(int i = 0; i < 32; ++i)
    {
        DrawChessPiece(painter, i);
    }

}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    QPoint rest = event->pos();
    int row, col;
    bool clicked_board = false;
    clicked_board = PixelToCoordinate(rest, row, col);
    //点击棋盘之外 直接返回
    if(!clicked_board)
        return;

    //循环查询点击处是否有棋子, 有棋进行规则判断 吃棋
    for(int i = 0; i < 32; ++i)
    {
        if(chess_piece_[i].is_dead_)
            continue;

        if(chess_piece_[i].row_ == row && chess_piece_[i].col_ == col) //有活棋，只会有一个
        {          
            //连续两次选中棋子
            if(select_id_ != -1)
            {             
                //同一边 切换焦点
                if(chess_piece_[i].is_red_ == chess_piece_[select_id_].is_red_)
                {
                    select_id_ = i;
                    update();
                    return;
                }
                //不同边 符合规则就吃
                //.......规则
                //.......
                if(CanMove(chess_piece_[select_id_], row, col, true))
                {
                    chess_piece_[i].is_dead_ = true;
                    chess_piece_[select_id_].col_ = chess_piece_[i].col_;
                    chess_piece_[select_id_].row_ = chess_piece_[i].row_;

                    last_id_ = select_id_;
                    red_turn_ = !red_turn_;
                    select_id_ = -1;
                }

            }
            else //第一次选中棋子
            {
                //如果红方轮次，选中黑棋 或者 黑方轮次选中红棋 跳出
                if((red_turn_ && !chess_piece_[i].is_red_ ) || (!red_turn_ && chess_piece_[i].is_red_))
                    return;
                select_id_ = i;
            }
            update();
            return;
        }
    }
    //无棋规则判断走棋 炮不能移动
    if(select_id_ != -1)
    {
        if(CanMove(chess_piece_[select_id_], row, col, false))
        {
            chess_piece_[select_id_].col_ = col;
            chess_piece_[select_id_].row_ = row;
            red_turn_ = !red_turn_;

            last_id_ = select_id_;
            select_id_ = -1;
            update();
            return;
        }
    }
}

void ChessBoard::DrawChessPiece(QPainter &painter, int id)
{
    //棋子被吃后就不显示了
    if(chess_piece_[id].is_dead_)
        return;

    painter.setBrush(Qt::yellow);
    if(id == select_id_) //鼠标选中棋子变灰
        painter.setBrush(Qt::gray);
    if(id == last_id_)
        painter.setBrush(Qt::green);
    painter.setFont(QFont("system", piece_radius_, 700));
    painter.setPen(Qt::black);  
    if(id >= 16)
        painter.setPen(Qt::red);
    QPoint center = CoordinateToPixel(chess_piece_[id].row_, chess_piece_[id].col_);
    painter.drawEllipse(center, piece_radius_, piece_radius_);

    QRect rect(center.x() - piece_radius_, center.y() - piece_radius_, 2 * piece_radius_, 2 * piece_radius_);

    painter.drawText(rect, Qt::AlignCenter, chess_piece_[id].GetText());

}

QPoint ChessBoard::CoordinateToPixel(int row, int col)
{
    QPoint rest;
    rest.rx() = (col + 1) * grid_length_;
    rest.ry() = (row + 1) * grid_length_;
    return rest;
}

bool ChessBoard::PixelToCoordinate(QPoint point, int &row, int &col)
{
//    int dx = 0;
//    int dy = 0;
//    for(row = 0; row < 10; ++row)
//    {
//        for(col = 0; col < 9; ++col)
//        {
//            dx = abs(point.x() - (col + 1) * grid_length_);
//            dy = abs(point.y() - (row + 1) * grid_length_);
//            if(dx <= piece_radius_ && dy <= piece_radius_)
//                return true;
//        }
//    }
    col = point.x() / grid_length_;
    col = point.x() % grid_length_ <= piece_radius_ ? col - 1 : col;
    if(col < 0 || col > 8)
        return false;
    row = point.y() / grid_length_;
    row = point.y() % grid_length_ <= piece_radius_ ? row - 1 : row;
    if(row < 0 || row > 9)
        return false;
    return true;
}

int ChessBoard::Relation(int dx, int dy)
{
    return 10 * abs(dx) + abs(dy);
}

bool ChessBoard::CanMove(ChessPiece piece, int move2row, int move2col, bool flag)
{
    switch (piece.type_) {
    case ChessPiece::JIANG:
    case ChessPiece::SHUAI:
        return CanMoveJIANG(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::SHI_RED:
    case ChessPiece::SHI_BLACK:
        return CanMoveSHI(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::XIANG_BLACK:
    case ChessPiece::XIANG_RED:
        return CanMoveXIANG(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::ZU:
        return CanMoveZU(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::BING:
        return CanMoveBING(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::CHE:
        return CanMoveCHE(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::MA:
        return CanMoveMA(piece.row_, piece.col_, move2row, move2col);
    case ChessPiece::PAO:
        return CanMovePAO(piece.row_, piece.col_, move2row, move2col, flag);
    }
    return false;
}

bool ChessBoard::CanMoveJIANG(int row, int col, int move2row, int move2col)
{
    //九宫格内走一格，被将的情况不能走
    if(move2col < 3 || move2col > 5)
        return false;
    if(move2row > 2 && move2row < 7)
        return false;
    int d = Relation(move2col - col, move2row - row);
    if(d != 1 && d != 10)
        return false;
    //这里还要考虑对将
    return true;
}

bool ChessBoard::CanMoveSHI(int row, int col, int move2row, int move2col)
{
    if(move2col < 3 || move2col > 5)
        return false;
    if(move2row > 2 && move2row < 7)
        return false;
    int d = Relation(move2col - col, move2row - row);
    if(d != 11)
        return false;
    return true;
}

bool ChessBoard::CanMoveXIANG(int row, int col, int move2row, int move2col)
{
    int d = Relation(move2col - col, move2row - row);
    if(d != 22)
        return false;
    if((row < 5 && move2row >=5) || (row >= 5 && move2row < 5))
        return false;
    return true;
}

bool ChessBoard::CanMoveCHE(int row, int col, int move2row, int move2col)
{
    //都不用考虑移动前后位置相同的情况
    //直线 且中间不能有其它的棋子
    int flag = 1;
    if(row == move2row)
    {
        flag = col > move2col ? -1 : 1;
        for(int i = 1; i < abs(col - move2col); ++i)
        {
            if(HasPiece(row, col + flag * i))
                return false;
        }
    }
    else
    {
        if(col != move2col)
            return false;
        flag = row > move2row ? -1 : 1;
        for(int i = 1; i < abs(row - move2row); ++i)
        {
            if(HasPiece(row + flag * i, col))
                return false;
        }

    }

    return true;
}

bool ChessBoard::CanMoveMA(int row, int col, int move2row, int move2col)
{
    int d = Relation(move2col - col, move2row - row);
    if(d != 12 && d != 21)
        return false;
    //马腿被卡
    if(HasPiece(row, col + 1) && move2col - col == 2)
        return false;
    if(HasPiece(row, col - 1) && move2col - col == -2)
        return false;
    if(HasPiece(row + 1, col) && move2row - row == 2)
        return false;
    if(HasPiece(row - 1, col) && move2row - row == -2)
        return false;
    return true;
}

bool ChessBoard::CanMovePAO(int row, int col, int move2row, int move2col, bool eat)
{
    //直线 且中间有且仅有一个棋子
    int flag = 1;
    int count = 0;
    if(row == move2row)
    {
        flag = col > move2col ? -1 : 1;
        for(int i = 1; i < abs(col - move2col); ++i)
        {
            if(HasPiece(row, col + flag * i))
                count++;
        }
        if(eat && count != 1)
            return false;
        if(!eat && count != 0)
            return false;
    }
    else
    {
        if(col != move2col)
            return false;
        flag = row > move2row ? -1 : 1;
        for(int i = 1; i < abs(row - move2row); ++i)
        {
            if(HasPiece(row + flag * i, col))
                count++;
        }
        if(eat && count != 1)
            return false;
        if(!eat && count != 0)
            return false;
    }

    return true;
}

bool ChessBoard::CanMoveZU(int row, int col, int move2row, int move2col)
{
    if(row < 5)
    {
        if(col != move2col || move2row - row != 1)
            return false;
    }
    else
    {
        if(move2row < row)
            return false;
        int d = Relation(move2col - col, move2row - row);
        if(d != 1 && d != 10 )
            return false;
    }
    return true;
}

bool ChessBoard::CanMoveBING(int row, int col, int move2row, int move2col)
{
    if(row > 4)
    {
        if(col != move2col || row - move2row != 1)
            return false;
    }
    else
    {
        if(row < move2row)
            return false;
        int d = Relation(move2col - col, move2row - row);
        if(d != 10 && d != 1)
            return false;
    }
    return true;
}

bool ChessBoard::HasPiece(int row, int col)
{
    if(row < 0 || row > 9 || col < 0 || col > 8)
        return false;
    for(int i = 0; i < 32; ++i)
    {
        if(chess_piece_[i].is_dead_)
            continue;
        if(chess_piece_[i].row_ == row && chess_piece_[i].col_ == col)
            return true;
    }
    return false;
}






