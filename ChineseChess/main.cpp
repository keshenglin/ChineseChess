
#include <QApplication>
#include "ChessBoard.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ChessBoard b;
    b.setFixedSize(400, 440);
    b.show();

    return app.exec();

}
