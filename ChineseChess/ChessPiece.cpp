#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "ChessPiece.h"

ChessPiece::ChessPiece()
{

}

ChessPiece::~ChessPiece()
{

}

void ChessPiece::Initial(int id)
{
    struct {
        int row;
        int col;
        ChessPiece::TYPE type;
    }pos[16]{
    {0, 0, CHE},
    {0, 1, MA},
    {0, 2, XIANG_BLACK},
    {0, 3, SHI_BLACK},
    {0, 4, JIANG},
    {0, 5, SHI_BLACK},
    {0, 6, XIANG_BLACK},
    {0, 7, MA},
    {0, 8, CHE},
    {2, 1, PAO},
    {2, 7, PAO},
    {3, 0, ZU},
    {3, 2, ZU},
    {3, 4, ZU},
    {3, 6, ZU},
    {3, 8, ZU}
    };

    id_ = id;
    is_dead_ = false;
    if(id < 16)
    {
        row_ = pos[id].row;
        col_ = pos[id].col;
        is_red_ = false;
        type_ = pos[id].type;
    }
    else
    {
        row_ = 9 - pos[id - 16].row;
        col_ = 8 - pos[id - 16].col;
        is_red_ = true;
        if( id >= 27)
            type_ = BING;       
        else if(id == 18 || id == 22)
            type_ = XIANG_RED;
        else if(id == 19 || id == 21)
            type_ = SHI_RED;
        else if(id == 20)
            type_ = SHUAI;
        else
            type_ = pos[id - 16].type;
    }

}

QString ChessPiece::GetText()
{
    switch(this->type_)
    {
    case CHE:
            return QObject::tr("車");
    case MA:
        return QObject::tr("馬");
    case PAO:
        return QObject::tr("炮");
    case XIANG_RED:
        return QObject::tr("相");
    case XIANG_BLACK:
        return QObject::tr("象");
    case SHI_RED:
        return QObject::tr("仕");
    case SHI_BLACK:
        return QObject::tr("士");
    case JIANG:
        return QObject::tr("将");
    case SHUAI:
        return QObject::tr("帥");
    case BING:
        return QObject::tr("兵");
    case ZU:
        return QObject::tr("卒");
    }
    return QObject::tr("error!");

}
