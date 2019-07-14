#ifndef TILE_H
#define TILE_H
#include <QLabel>
#include <QDebug>
#include <Engine/move.h>

class Tile: public QLabel
{
private:
    // Lần chọn thứ 1
    void _firstClick(Tile*);

    // Lần chọn thứ 2
    void _secondClick(Tile*);

    // Hủy chọn quân cờ
    void _cancelClick();

    // Chấp nhận chọn nước đi
    void _acceptClick(Tile*);



    // Di chuyển trong Engine
    void _moveEngine();

    // GUI phong cấp
    void _promotion();

    // AI
    void _AI();

    // endgame check

    void endgameCheck();

    // Sound
    void _sound();

    // Highlight
    void _hlight();

public:

    //Fields
    int tileColor,piece,pieceColor,row,col,tileNum;
    int pieceType;bool promotionFlag = false;

    //Constructors
    Tile(QWidget* pParent=0, Qt::WindowFlags f=0) : QLabel(pParent, f) {};
    Tile(const QString& text, QWidget* pParent = 0, Qt::WindowFlags f = 0) : QLabel(text, pParent, f){};

    //Methods

    // Sự kiện click quân cờ
    void mousePressEvent(QMouseEvent *event);

    // Gán ảnh quân cờ
    void display(int elem);

    // Hủy hiển thị nước đi
    void disOrange();

    // Hiển thị quân cờ
    void tileDisplay();

    // Sinh nước đi
    void validate(Tile*, int);

    // GUI chiếu
    void check();

    // call AI
    void callAI();

    // call Engine
    void callEngine();

};

#endif // TILE_H
