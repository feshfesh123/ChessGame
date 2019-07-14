#include "GUI/tile.h"
#include "GUI/validation.h"
#include <time.h>
#include "game.h"
#include <QMediaPlayer>
#include <QTime>
validation *valid = new validation();


extern int count, turn;
extern Tile *click1;
extern Tile *tile[8][8];
extern game *Game;
extern int _countHistory;
extern Mode _mode;
extern QLabel *ind[200], *h[400];
extern Move _move;
extern QMessageBox *endgame;
extern QLabel *posi, *tim;
void Tile::mousePressEvent(QMouseEvent *event)
{
    if (_isStart && isFinishPromotion) validate(this,++count);
}

void Tile::validate(Tile *temp, int c)
{
    if ( c==1 ) // Nếu là lần click đầu tiên
    {
        _firstClick(temp);
    }

    else    // Nếu là lần click thứ 2
    {
        _secondClick(temp);
    }
}

void Tile::_firstClick(Tile *temp)
{
    int retValue = 0;
    if(temp->piece && (temp->pieceColor==turn)) // Nếu click vào quân cờ và đúng màu theo lượt
    {

        // Sinh nước đi hợp lệ
        // Nếu có nước đi retValue = 1 ngược lại = 0
        retValue = valid->chooser(temp);


        if (retValue)    // Nếu có nước đi - retValue = 1
        {

            click1= new Tile();

            // Đổi nền màu xanh lá
            temp->setStyleSheet("QLabel {background-color: #1a6600;}");

            // Lưu lại ô
            click1=temp;
        }


        else    // Nếu không có nước đi, đưa về trạng thái chưa thao tác
        {
            //temp->setStyleSheet("QLabel {background-color: red;}");
            count=0;
        }
     }

    else    // Nếu click vào ô bất kỳ ko phải quân cờ, đưa về trạng thái chưa thao tác
    {
        //qDebug()<<"Rascel, clicking anywhere";
        count=0;
    }

}

void Tile::_secondClick(Tile *temp)
{
    if (temp->tileNum == click1->tileNum) // Nếu click vào ô cũ
    {
        _cancelClick();
    }
    else
    // Nếu click vào ô nằm trong các bước đi
    _acceptClick(temp);
}

void Tile::_cancelClick()
{
    // Trả về màu nền ban đầu
    click1->tileDisplay();

    // Hủy hiển thị sinh bước đi
    disOrange();
    max=0;

    // Đưa về trạng thái chưa thao tác
    count=0;
}

void Tile::_acceptClick(Tile *temp)
{
    for (int i=0; i<max; i++)
    {
        if (temp->tileNum == e[i]) // Nếu chọn đúng ô hợp lệ
        {
            _move = list_move[f[i]];



            // Hủy hiển thị sinh bước đi
            disOrange();

            // Di chuyển trong Engine
            _moveEngine();

            // Promotion
            _promotion();

            max=0;

            Game->repaint();

            // Đổi lượt
            if ( (_mode==AI || _mode==CHALL ) && isFinishPromotion){
                _AI();
            }
            else if(_mode == PLAYER) turn = (turn+1) %2;


            count=0;
        }

        else    // Nếu click ko vào 2TH trên, xem như chưa click
            // Giữ nguyên trạng thái
            count=1;
        if(promotionFlag){
            promotionFlag = 0;
            exit;
        }
    }
}

void Tile::_moveEngine()
{
    if ( _move.getPieceType()==KING ){
        if (board.getActivePlayer()== WHITE){
            wR = _move.getTo()/8;
            wC = _move.getTo()%8;
        }
        else{
            bR = _move.getTo()/8;
            bC = _move.getTo()%8;
        }
    }
    board.doMove(_move);

    current++;
    _undo = 0;
    boardHistory[current] = board;
    moveHistory[current] = _move;
    Game->boardDisplay(board);
    Game->writeHistory();
    // Kiểm tra chiếu
    check();

    // Kiểm tra endgame
    endgameCheck();

    // Sound
    _sound();

    // Highlight
    _hlight();
    _player = (_player+1) % 2;
}

void Tile::_promotion()
{
    if(_move.getFlags() & Move::PROMOTION){
        Game->promotionDisplay(board.getInactivePlayer());
        this->promotionFlag = true;
        isFinishPromotion = false;
    }
}

void Tile::check()
{
    int retValue = board.colorIsInCheck(board.getActivePlayer());
    tile[wR][wC]->display(KING);
    tile[bR][bC]->display(KING);

    if (retValue)
    {
        if (board.getInactivePlayer() == WHITE)
        {
            tile[bR][bC]->setPixmap(QPixmap(":/Images/bk_check.png"));
        }
        else tile[wR][wC]->setPixmap(QPixmap(":/Images/wk_check.png"));
    }

}

void Tile::callAI()
{
    _AI();
}

void Tile::callEngine()
{
    _moveEngine();
}

void Tile::_AI()
{
    ai flex(depthChoosed, phase);
    clock_t tStart = clock();
    Move bestMove = flex.minimaxRoot(board, BLACK);
    double tExecuted = (double)(clock() -tStart)/CLOCKS_PER_SEC;
    posi->setText(QString::number(flex.positionCount));
    tim->setText(QString::number(tExecuted));
    //qDebug()<< QString::fromStdString(bestMove.getNotation())<<" Positions evaluated :"<<flex.positionCount;
    //qDebug()<< " Time :"<< fixed << qSetRealNumberPrecision(3) << tExecuted;
    _move = bestMove;

    if(_move.getFlags() & Move::PROMOTION){
       _move.setPromotionPieceType(QUEEN);
    }
    _moveEngine();

}

void Tile::endgameCheck()
{
    MoveGen movegen(board);
    QMediaPlayer *sound = new QMediaPlayer();
    auto listMove = movegen.getLegalMoves();
    if(listMove.size() == 0 || (_mode ==CHALL && listMove.size() == 0 && _numChall==4))
    {
        if (!board.colorIsInCheck(board.getActivePlayer()))
            endgame->setIconPixmap(QPixmap(":/Images/gameDraw.png"));

        else{
            if(board.getActivePlayer() == WHITE){
                sound->setMedia(QUrl("qrc:/Images/lose.wav"));
                endgame->setIconPixmap(QPixmap(":/Images/loseGame.png"));

            }
            else {
                sound->setMedia(QUrl("qrc:/Images/win.wav"));
                endgame->setIconPixmap(QPixmap(":/Images/winGame.png"));
            }
        }
        endgame->show();
        sound->play();
        _isStart = 0;
    }
    if (_mode == CHALL && _numChall > 4){
        sound->setMedia(QUrl("qrc:/Images/lose.wav"));
        endgame->setIconPixmap(QPixmap(":/Images/Lose.jpeg"));
        endgame->show();
        _isStart = 0;
        sound->play();
    }
}

void Tile::_sound()
{
    QMediaPlayer *sound = new QMediaPlayer();

    if (_move.getFlags() & Move::CAPTURE){
        sound->setMedia(QUrl("qrc:/Images/attack.wav"));
        sound->play();
    }
    else{
        sound->setMedia(QUrl("qrc:/Images/move.wav"));
        sound->play();
    }
}

void Tile::_hlight()
{
    int x1 = _move.getFrom()/8;
    int y1 = _move.getFrom()%8;
    int x2 = _move.getTo()/8;
    int y2 = _move.getTo()%8;

    tile[x1][y1]->setStyleSheet("QLabel {background-color: rgb(186, 202, 68);}");
    tile[x2][y2]->setStyleSheet("QLabel {background-color: rgb(246, 246, 130);}");
}

void Tile::display(int elem)
{
    this->pieceType=elem;

    if( this->pieceColor == WHITE && this->piece)
    {
        switch(elem)
        {
            case PAWN: this->setPixmap(QPixmap(":/Images/wp.png"));
                      break;
            case ROOK: this->setPixmap(QPixmap(":/Images/wr.png"));
                      break;
            case KNIGHT: this->setPixmap(QPixmap(":/Images/wn.png"));
                      break;
            case KING: this->setPixmap(QPixmap(":/Images/wk.png"));
                      break;
            case QUEEN: this->setPixmap(QPixmap(":/Images/wq.png"));
                      break;
            case BISHOP: this->setPixmap(QPixmap(":/Images/wb.png"));
                      break;
        }
    }

    else if(this->piece && this->pieceColor == BLACK)
    {
        switch(elem)
        {
            case PAWN: this->setPixmap(QPixmap(":/Images/bp.png"));
                      break;
            case ROOK: this->setPixmap(QPixmap(":/Images/br.png"));
                      break;
            case KNIGHT: this->setPixmap(QPixmap(":/Images/bn.png"));
                      break;
            case KING: this->setPixmap(QPixmap(":/Images/bk.png"));
                      break;
            case QUEEN: this->setPixmap(QPixmap(":/Images/bq.png"));
                      break;
            case BISHOP: this->setPixmap(QPixmap(":/Images/bb.png"));
                      break;
        }
    }
    else
        this->clear();
}

void Tile::tileDisplay()
{

    if(this->tileColor)
        this->setStyleSheet("QLabel {background-color: rgb(118, 121, 172);}:hover{background-color: rgb(170,85,127);}");
    else
        this->setStyleSheet("QLabel {background-color: rgb(191, 191, 193);}:hover{background-color: rgb(170,95,127);}");
}

void Tile::disOrange()
{
    int i,j;

   // for(i=0;i<max;i++)
       // tile[e[i]/8][e[i]%8]->tileDisplay();

    for(i=0;i<8;i++){
        for (j=0;j<8;j++) tile[i][j]->tileDisplay();
    }

}
