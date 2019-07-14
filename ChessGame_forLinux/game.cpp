#include "game.h"
#include "ui_game.h"
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QTime>
#include "Material/button/qtmaterialflatbutton.h"
#include "Material/button/qtmaterialraisedbutton.h"
#include <QListWidget>
#include "Material/dialog/qtmaterialdialog.h"
#include <QMediaPlayer>

int count=0, turn=0, max=0;
int wR, wC, bR, bC, e[60], f[60];
Tile *click1;
Tile *tile[8][8] = { { NULL } };
Board board;
MoveList list_move;
Board boardHistory[400];
Move moveHistory[400];
int current = 0, _undo = 0, _countHistory = 1;
int _isAI = 0, _isPlayer = 1, _isChall = 0, _numChall = 0;
int _isStart = 0, _player = 0;
QTime time1, time2, _totalTime = QTime(0,10,0,0);
Mode _mode = AI;
QLabel *ind[200]={NULL}, *h[400]={NULL};
QTimer *timer;
Move _move;
bool isFinishPromotion = true;
int depthChoosed = 3;
GamePhase phase = OPENING;
QMessageBox *endgame;
QMessageBox *sure;
QLabel *posi, *tim;

game::game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game)
{

    QFont font = QFont("Helvetica");
    font.setPointSize(10);
    font.setBold(true);

    QFont fontTime = QFont("Helvetica");
    fontTime.setPointSize(13);
    fontTime.setBold(true);

    // <----Tạo cửa sổ game------------------------------------------>
    this->setGeometry(200, 30, width, height);
    this->setFixedSize(width,height);

    // <----Tạo Border----------------------------------------------->
    char temp[8] = {'a','b','c','d','e','f','g','h'};
    int xR = xBoard - 15, yR = yBoard, xC = xBoard + 26, yC = yBoard + 50;

    for(int i= 0; i < 8;i ++) {
        rowBorder[i] = new QLabel(this);
        colBorder[i] = new QLabel(this);
        rowBorder[i]->setText(QString::number(i+1));
        rowBorder[i]->setFont(fontTime);
        colBorder[i]->setText(QString(temp[i]));
        colBorder[i]->setFont(fontTime);
        rowBorder[i]->setGeometry(xR,yR, 50,50);
        colBorder[i]->setGeometry(xC,yC,50,50);
        yR -= 64; xC += 64;
    }

    // <----Tạo logo------------------------------------------------->
    logo = new QLabel(this);
    logo->setGeometry(80, 10, 120, 100);
    logo->setPixmap(QPixmap(":/Images/chessflex_logo.jpg"));
    logo->setScaledContents(true);

    // <----End Game------------------------------------------------->
    endgame = new QMessageBox();
    endgame->setGeometry(xBoard + 200 + 64*3, yBoard - 30 - 64*4,0,0);
    endgame->setBackgroundRole(QPalette::Light);
    endgame->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // <----New Game------------------------------------------------->
    sure = new QMessageBox();
    sure->setGeometry(xBoard + 200 + 64*3, yBoard - 30 - 64*4,0,0);
    sure->setWindowTitle("New Game");
    sure->setText("Trận đấu vẫn đang diễn ra, bạn có chắc muốn chơi ván mới ?");
    sure->setStandardButtons(QMessageBox::Yes);
    sure->addButton(QMessageBox::No);
    sure->setDefaultButton(QMessageBox::No);


    // <----Tạo nút tính năng---------------------------------------->

        // PvP
    pvp_btn = new QtMaterialFlatButton("PvP", this);
    pvp_btn->setGeometry(0, 130, 80, 40);
    pvp_btn->setForegroundColor(QColor(00, 191, 255));
    pvp_btn->setFont(font);
    connect(pvp_btn,SIGNAL(clicked()),this,SLOT(pvp()));

        // PvC
    pvc_btn = new QtMaterialFlatButton("PvC", this);
    pvc_btn->setGeometry(80, 130, 80, 40);
    pvc_btn->setForegroundColor(QColor(00, 191, 255));
    pvc_btn->setFont(font);
    connect(pvc_btn,SIGNAL(clicked()),this,SLOT(pvc()));

        // Challenge
    chlg_btn = new QtMaterialFlatButton("Challenge", this);
    chlg_btn->setGeometry(160, 130, 80, 40);
    chlg_btn->setForegroundColor(QColor(00, 191, 255));
    chlg_btn->setFont(font);
    connect(chlg_btn,SIGNAL(clicked()),this,SLOT(chall()));

    chessPosition = new QComboBox(this);
    chessPosition->setGeometry(240, 130, 40, 40);
    connect(chessPosition,SIGNAL(currentTextChanged(QString)),this,SLOT(drawchall()));

        //play game
    play_btn = new QtMaterialRaisedButton("PLAY",this);
    play_btn->setGeometry(80, 230, 120, 30);
    play_btn->setForegroundColor(QColor(153, 204, 255));
    play_btn->setBackgroundColor(QColor(0, 102, 204));
    play_btn->setRippleStyle(Material::RippleStyle::CenteredRipple);
    connect(play_btn,SIGNAL(clicked()),this,SLOT(play()));
    font.setPointSize(13);
    play_btn->setFont(font);

    font.setPointSize(10);
        // Depth
    depth_btn = new QtMaterialFlatButton("Depth", this);
    depth_btn->setGeometry(0, 180, 100, 40);
    depth_btn->setForegroundColor(QColor(00, 191, 255));
    depth_btn->setFont(font);
    connect(depth_btn,SIGNAL(clicked()),this,SLOT(depthChooser()));

        // Time button
    time_btn = new QtMaterialFlatButton("Time ", this);
    time_btn->setGeometry(156, 180, 60, 40);
    time_btn->setForegroundColor(QColor(00, 191, 255));
    time_btn->setFont(font);
    connect(time_btn,SIGNAL(clicked()),this,SLOT(timelist()));

        // Time
    clock1 = new QLabel("WHITE", this);
    clock1->setGeometry(750, 610, 80, 80);
    clock1->setFont(fontTime);
    clock2 = new QLabel("BLACK", this);
    clock2->setGeometry(xBoard, 20, 80, 80);
    clock2->setFont(fontTime);
    clock3 = new QLabel("TIME", this);
    clock3->setGeometry(xBoard+220, 10, 100, 100);
    fontTime.setPointSize(25);
    clock3->setFont(fontTime);
    clock3->hide();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time()));

        // Undo
    undo_btn = new QPushButton("undo", this);
    undo_btn->setGeometry(30, 500, 100, 20);
    undo_btn->setFont(font);
    connect(undo_btn, SIGNAL(clicked()), this, SLOT(undo()));

        // Redo
    redo_btn = new QPushButton("redo", this);
    redo_btn->setGeometry(100, 500 ,100, 20);
    redo_btn->setFont(font);
    connect(redo_btn, SIGNAL(clicked()), this, SLOT(redo()));

    history_lwg = new QListWidget(this);
    history_lwg->setGeometry(50, 300, 182, 192);
    font.setPointSize(12);
    history_lwg->setFont(font);

        // Promotion
    for (int i =0 ;i<4;i++)
    {
        _promotion[i] = new QPushButton(this);
        connect(_promotion[i], SIGNAL(clicked()),this, SLOT(setPieceTypeforPromotion()));
    }

        // Choose Depth for AI combo box
    depthAI = new QComboBox(this);
    depthAI->setGeometry(80, 180, 40, 40);
    connect(depthAI,SIGNAL(currentTextChanged(QString)),this,SLOT(setAiDepth()));

        // choose time combo box
    timeChooser = new QComboBox(this);
    timeChooser->setGeometry(215,180,40,40);
    connect(timeChooser,SIGNAL(currentTextChanged(QString)),this,SLOT(setTime()));

        // Position evaluated
    QLabel *p = new QLabel("Position :", this);
    p->setFont(font);
    p->setGeometry(10, 550, 80, 20);
    posi = new QLabel(this);
    posi->setFont(font);
    posi->setGeometry(90, 550, 200, 20);

        // Time excuted
    QLabel *ti = new QLabel("Time      :", this);
    ti->setFont(font);
    ti->setGeometry(10, 600, 80, 20);
    tim = new QLabel(this);
    tim->setFont(font);
    tim->setGeometry(90, 600, 200, 20);

    // <----Tạo engine----------------------------------------------->
    Rays::init();
    Attacks::init();
    PSquareTable::init();

    // <----Set trạng thái bắt đầu----------------------------------->
    std:: string  Fen = "8/8/8/8/8/8/8/8 w KQkq - 0 1";
    board.setToFen(Fen);

    //boardHistory[current] = board;

}

game::~game()
{
    delete ui;
}

void game::undo()
{
    if (current >= 2)
    {
        current-=2;
        count = 0;
        _undo++;
        board = boardHistory[current];
        boardDisplay(board);
        tile[wR][wC]->check();

    }
   writeHistory();
}

void game::redo()
{
   if (_undo > 0)
   {
       _undo--;
       count = 0;
       current+=2;
       board = boardHistory[current];
       boardDisplay(board);
       tile[wR][wC]->check();
   }
   writeHistory();
}

void game::play()
{
    if (_isStart == 1){
        sure->show();
        timer->stop();

        if (sure->exec() == QMessageBox::Yes){
            newgame();
        }
        else {
            timer->start(1000);
        }
    }
    else {
        newgame();
    }

}

void game::chall()
{
    _isAI = 0; _isPlayer = 0; _isChall = 1;
    chessPosition->clear();
    for(int i=0;i<20;i++)
    {
        chessPosition->addItem(""+    QString::number(i+1));
    }
}

void game::pvp()
{
    _isAI = 0; _isPlayer = 1; _isChall = 0;
    turn = 0;
}

void game::pvc()
{
    _isAI = 1; _isPlayer = 0; _isChall = 0;
    turn = 0;
}

void game::createchall(int a)
{

    std::string fenchall[20];
    fenchall[0]="1kbr1bnr/1p6/1Pq2p2/2p3p1/2Np3p/2PP1PN1/5QPP/R1B1RK2 w ---- -";
    fenchall[1]="2r3k1/pp4rp/1q1p2pQ/1N2p1PR/2nNP3/5P2/PPP5/2K4R w ---- -";
    fenchall[2]="1r1r4/Rp2np2/3k4/3P3p/2Q2p1P/2P4q/1P1N1P1P/6RK w ---- -";
    fenchall[3]="8/p6p/k5p1/2R2p2/1PN1rn2/2KP3r/P1R5/8 w ---- -";
    fenchall[4]="5Q2/1p3p1N/2p3p1/5b1k/2P3n1/P4RP1/3q2rP/5R1K w ---- -";
    fenchall[5]="6k1/1p2q2p/p3P1pB/8/1P2p3/2Qr2P1/P4P1P/2R3K1 w ---- -";
    fenchall[6]="6r1/1p1qp1rk/p2pR1p1/3P2Qp/7P/5P2/PPP5/1K4R1 w ---- -";
    fenchall[7]="2r2k2/pb4bQ/1p1qr1pR/3p1pB1/3Pp3/2P5/PPB2PP1/1K5R w ---- -";
    fenchall[8]="3Q1b2/5pk1/p3rNpp/1p2P3/4NP2/nP6/P3q1PP/3R3K w ---- -";
    fenchall[9]="r2q2rk/ppp2p1p/3b1pn1/5R1Q/3P4/2P4N/PP4PP/R1B3K1 w ---- -";
    fenchall[10]="rqr3k1/3bppBp/3p2P1/p7/1n2P3/1p3P2/1PPQ2P1/2KR3R w ---- -";
    fenchall[11]="2Rr1qk1/5ppp/p2N4/P7/5Q2/8/1r4PP/5BK1 w ---- -";
    fenchall[12]="r1bqr3/4pkbp/2p1N2B/p3P1Q1/2pP4/2N2P2/PP4P1/R3K2R w ---- -";
    fenchall[13]="4r1k1/3r1p1p/bqp1n3/p2p1NP1/Pn1Q1b2/7P/1PP3B1/R2NR2K w - -";
    fenchall[14]="3r1r1k/q2n3p/b1p2ppQ/p1n1p3/Pp2P3/1B1PBR2/1PPN2PP/R5K1 w ---- -";
    fenchall[15]="r1b2n2/2q3rk/p3p2n/1p3p1P/4N3/PN1B1P2/1PPQ4/2K3R1 w ---- -";
    fenchall[16]="4r3/p1r2p1k/1p2pPpp/2qpP3/3R2P1/1PPQ3R/1P5P/7K w ---- -";
    fenchall[17]="r1q5/2p2k2/p4Bp1/2Nb1N2/p6Q/7P/nn3PP1/R5K1 w KQkq -";
    fenchall[18]="r2r1k2/pbq2pp1/2p1p1p1/1pP1N1P1/6Q1/P6R/1P3P1P/4R1K1 w ---- -";
    fenchall[19]="8/2B3R1/2p1pkp1/1r2N3/1p6/7P/2r3PK/8 w ---- -";
    std::string fen=fenchall[a];
    board.setToFen(fen);
    boardDisplay(board);
    for (int i=0;i<64;i++){
        if (board.getPieceAtSquare(WHITE,i) == KING) {
            wR = i/8; wC = i%8;
        }
        if (board.getPieceAtSquare(BLACK,i) == KING) {
            bR = i/8; bC = i%8;
        }
    }
    boardHistory[current] = board;
}

void game::drawchall()
{
    if (_isStart == 0){
        int a=chessPosition->currentIndex();
        createchall(a);
    }
}

void game::boardDisplay(Board board)
{
    _numChall = (current + 1)/2;
    int kingSquare = _bitscanForward(board.getPieces(board.getActivePlayer(), KING));
    if(board.getActivePlayer() == 0){
        wR = kingSquare / 8;
        wC = kingSquare % 8;
    }else {
        bR = kingSquare / 8;
        bC = kingSquare % 8;
    }
    kingSquare = _bitscanForward(board.getPieces(board.getInactivePlayer(), KING));
    if(board.getInactivePlayer() == 0){
        wR = kingSquare / 8;
        wC = kingSquare % 8;
    }else {
        bR = kingSquare / 8;
        bC = kingSquare % 8;
    }
    int i,j,k=0,hor,ver;
    //Create 64 tiles (allocating memories to the objects of Tile class)
        ver = yBoard;
        for(i=0; i<8; i++)
        {
            hor = xBoard;
            for(j=0; j<8; j++)
            {
                tile[i][j]->tileColor=(i+j)%2;
                tile[i][j]->piece=0;
                if(board.getPieceAtSquare(BLACK,k) != EMPTY){
                    tile[i][j]->piece=1;
                    tile[i][j]->pieceType = board.getPieceAtSquare(BLACK,k);
                    tile[i][j]->pieceColor = BLACK;

                } else
                if(board.getPieceAtSquare(WHITE,k) != EMPTY){
                    tile[i][j]->piece=1;
                    tile[i][j]->pieceType = board.getPieceAtSquare(WHITE,k);
                    tile[i][j]->pieceColor = WHITE;
                }
                tile[i][j]->row=i;
                tile[i][j]->col=j;
                tile[i][j]->tileNum=k++;
                tile[i][j]->tileDisplay();
                tile[i][j]->display(tile[i][j]->pieceType);
                tile[i][j]->setGeometry(hor,ver,64,64);
                hor+=64;
            }
            ver-=64;
        }        
}

void game::writeHistory()
{
    history_lwg->clear();

    for (int i=1; i<= (current+1)/2; i++){
        int index;
        QString line= "";
        index = i;
        // Ghi index
        line = QString::number(index);

        // Tao bien ban
        for (int j=i*2-1; j<=i*2; j++)
        {
            if (j>current) break;
            QString lw="";
            Move mw = moveHistory[j];
            // Ky hieu castle
            if (mw.getFlags() & Move::KSIDE_CASTLE)
            {
                lw = "O-O";
            }
            else {
                if (mw.getFlags() & Move::QSIDE_CASTLE){
                    lw = "O-O-O";
                }
                else{
                    // Ky hieu location
                    std::string s = mw.indexToNotation(mw.getTo());
                    std::string s1 = mw.indexToNotation(mw.getFrom());

                    lw+= QString::fromStdString(s);

                    // Ky hieu capture
                    if (mw.getFlags() & Move::CAPTURE){
                        lw = "x" + lw;
                        if (mw.getPieceType() == PAWN) {
                            QString col_pawn = QString::fromStdString(s1);
                            lw = col_pawn[0] + lw;
                        }
                    }

                    // Ky hieu piece
                    if (mw.getPieceType() != PAWN){
                        switch(mw.getPieceType())
                        {
                            case ROOK: lw ='R' + lw;
                                      break;
                            case KNIGHT: lw ='N' + lw;
                                      break;
                            case KING: lw = 'K' + lw;
                                      break;
                            case QUEEN: lw = 'Q' + lw;
                                      break;
                            case BISHOP: lw = 'B' + lw;
                                      break;
                        }
                    }
                    // Ky hieu check
                    if (boardHistory[j].colorIsInCheck(boardHistory[j].getActivePlayer())){
                        lw+="+";
                    }
                }

            line = line + "    " + lw;
            }
        }


        // Ghi bien ban
        history_lwg->addItem(line);
    }
}

void game::time()
{
    if(_isStart == 0) {
        timer->stop();
    }else {
        if (_player == 0){
            time1 = time1.addSecs(-1);
            if (_mode == CHALL){
                clock3->setText(time1.toString("mm:ss"));
            }
            else{
                clock1->setText(time1.toString("mm:ss"));
            }
        }
        else{
            time2 = time2.addSecs(-1);
            clock2->setText(time2.toString("mm:ss"));
        }
        bool flag =false;
        QMediaPlayer *sound = new QMediaPlayer();
        if(time1 == QTime(0,0,0,0)) {
            endgame->setIconPixmap(QPixmap(":/Images/loseGame.png"));
            sound->setMedia(QUrl("qrc:/Images/lose.wav"));
            flag = true;
        }
        if(time2 == QTime(0,0,0,0)){
            endgame->setIconPixmap(QPixmap(":/Images/winGame.png"));
            sound->setMedia(QUrl("qrc:/Images/win.wav"));
            flag = true;
        }
        if (flag){
            sound->play();
            endgame->show();
            _isStart = 0;
        }
    }
}

void game::startgame(Board board)
{
    turn = (board.getInactivePlayer()+1) % 2;

    int i,j,k=0,hor,ver;
    //Create 64 tiles (allocating memories to the objects of Tile class)
        ver = yBoard;
        for(i=0; i<8; i++)
        {
            hor = xBoard;
            for(j=0; j<8; j++)
            {
                tile[i][j] = new Tile(this);
                tile[i][j]->setScaledContents(true);
                tile[i][j]->tileColor=(i+j)%2;
                tile[i][j]->piece=0;
                if(board.getPieceAtSquare(BLACK,k) != EMPTY){
                    tile[i][j]->piece=1;
                    tile[i][j]->pieceType = board.getPieceAtSquare(BLACK,k);
                    tile[i][j]->pieceColor = BLACK;

                } else
                if(board.getPieceAtSquare(WHITE,k) != EMPTY){
                    tile[i][j]->piece=1;
                    tile[i][j]->pieceType = board.getPieceAtSquare(WHITE,k);
                    tile[i][j]->pieceColor = WHITE;
                }
                tile[i][j]->row=i;
                tile[i][j]->col=j;
                tile[i][j]->tileNum=k++;
                tile[i][j]->tileDisplay();
                tile[i][j]->display(tile[i][j]->pieceType);
                tile[i][j]->setGeometry(hor,ver,64,64);
                hor+=64;
            }
            ver-=64;
        }
        wR=0;
        wC=4;

        bR=7;
        bC=4;
}

void game::setPieceTypeforPromotion()
{
    int i=0;

    QObject *obj = sender();
    for(i = 0 ; i < 4; i ++){
        if(obj == _promotion[i]) break;
    }
    _move.setPromotionPieceType(this->_promotionPieceType[i]);

    for(i =0 ;i < 4; i ++){
        _promotion[i]->hide();
    }

    board = boardHistory[current-1];
    current-=1;
    tile[0][0]->callEngine();

    isFinishPromotion = true;
    if( (_mode == AI || _mode == CHALL) && board.getActivePlayer()==BLACK)
        tile[0][0]->callAI();
}

void game::setAiDepth()
{
    depthChoosed = depthAI->currentIndex() + 1;
}

void game::depthChooser()
{
    depthAI->clear();
    for(int i=0;i<6;i++)
    {
        depthAI->addItem(""+    QString::number(i+1));
    }
}

void game::timelist()
{
    timeChooser->clear();
    timeChooser->addItem(""+    QString::number(5));
    timeChooser->addItem(""+    QString::number(10));
    timeChooser->addItem(""+    QString::number(15));
    timeChooser->addItem(""+    QString::number(20));
}

void game::setTime()
{
    int index = timeChooser->currentIndex();
    switch(index){
    case 0:
        _totalTime = QTime(0,5,0,0);
        break;
    case 1:
        _totalTime = QTime(0,10,0,0);
        break;
    case 2:
        _totalTime = QTime(0,15,0,0);
        break;
    case 3:
        _totalTime = QTime(0,20,0,0);
        break;
    }
}

void game::close_endgame()
{
    endgame->hide();
}

void game::newgame()
{
    QMediaPlayer *sound = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:/Images/newgame.wav"));
    sound->play();
    history_lwg->clear();
    posi->setText("");
    tim->setText("");
    _isStart = 1;
    _player = 0;
    turn = 0;
    time1 = _totalTime;
    time2 = _totalTime;
    clock1->setText("WHITE");
    clock2->setText("BLACK");
    clock3->hide();
    timer->stop();
    current = 0;
    phase = OPENING;
    if ( _isAI){
        _mode = AI;
        std:: string  Fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        board.setToFen(Fen);
    }
    if ( _isPlayer){
        _mode = PLAYER;
        std:: string  Fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        board.setToFen(Fen);

        clock1->setText(time1.toString("mm:ss"));
        clock2->setText(time2.toString("mm:ss"));
        timer->start(1000);
    }
    if ( _isChall) {
        _mode = CHALL;
        depthChoosed=4;
        phase = ENDGAME;
        _numChall = 0;
        int a=chessPosition->currentIndex();
        createchall(a);
        time1 = QTime(0,1,30,0);
        clock3->show();
        QMessageBox *role = new QMessageBox(this);
        role->setText("Bạn bên phe trắng và có 1 phút 30 giây để chiếu bí quân đen trong vòng 4 nước");
        role->show();
        if (role->exec() == QMessageBox::Ok){
            timer->start(1000);
        }
    }
    boardHistory[current] = board;
    boardDisplay(board);
}

void game::promotionDisplay(int pieceColor)
{
    promotionPieceDisplay(pieceColor);
    int ver = xBoard + 64*2, hor = yBoard - 64*8;
    for(int i =0; i <4; i ++){
        _promotion[i]->setGeometry(ver, hor, 64, 64);
        _promotion[i]->setIconSize(QSize(64,64));
        _promotion[i]->setBackgroundRole(QPalette::Light);
        _promotion[i]->show();
        ver += 64;
    }
}

void game::promotionPieceDisplay(int pieceColor)
{
    switch(pieceColor){
    case 0:
        _promotion[0]->setIcon(QIcon(QPixmap(":/Images/wq.png")));
        _promotion[1]->setIcon(QIcon(QPixmap(":/Images/wr.png")));
        _promotion[2]->setIcon(QIcon(QPixmap(":/Images/wb.png")));
        _promotion[3]->setIcon(QIcon(QPixmap(":/Images/wn.png")));
        break;
    case 1:
        _promotion[0]->setIcon(QIcon(QPixmap(":/Images/bq.png")));
        _promotion[1]->setIcon(QIcon(QPixmap(":/Images/br.png")));
        _promotion[2]->setIcon(QIcon(QPixmap(":/Images/bb.png")));
        _promotion[3]->setIcon(QIcon(QPixmap(":/Images/bn.png")));

    }
}
