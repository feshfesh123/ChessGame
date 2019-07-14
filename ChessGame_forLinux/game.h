#ifndef GAME_H
#define GAME_H
#include "GUI/tile.h"
#include "GUI/validation.h"
#include <QMainWindow>
#include <QComboBox>
#include <QMessageBox>
class QPushButton;
class QtMaterialFlatButton;
class QtMaterialRaisedButton;
class QListWidget;
class QtMaterialDialog;
class QMessageBox;

namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

private:
    int height = 700;
    int width = 850;

    int xBoard = 300;
    int yBoard = 550;

    int xHistory = 820;
    int yHistory = 80;
    int fontHistory = 13;

    QWidget *window;
    QPushButton *undo_btn;
    QPushButton *redo_btn;
    QtMaterialFlatButton *pvp_btn;
    QtMaterialFlatButton *pvc_btn;
    QtMaterialFlatButton *chlg_btn;
    QtMaterialFlatButton *depth_btn;
    QtMaterialFlatButton *time_btn;
    QtMaterialRaisedButton *t;

    QPushButton *_promotion[4];
    PieceType _promotionPieceType[4] = {QUEEN,ROOK,BISHOP,KNIGHT};
    QComboBox *chessPosition;
    QComboBox *depthAI;
    QComboBox *timeChooser;
    QtMaterialFlatButton *play_btn;
    QLabel *logo;
    QLabel *clock1;
    QLabel *clock2;
    QLabel *clock3;
    QLabel *rowBorder[8];
    QLabel *colBorder[8];
    QListWidget *history_lwg;
    QMessageBox *sure;

public:
    explicit game(QWidget *parent = nullptr);
    ~game();
    void boardDisplay(Board board);
    void writeHistory();

    void chooseMode();
    void startgame(Board);
    void promotionDisplay(int );
    void promotionPieceDisplay(int);

signals:
    void clicked();

public slots:
    void undo();
    void redo();
    void play();
    void chall();
    void pvp();
    void pvc();
    void createchall(int);
    void drawchall();
    void time();
    void setPieceTypeforPromotion();
    void setAiDepth();
    void depthChooser();
    void timelist();
    void setTime();
    void close_endgame();
    void newgame();
private:
    Ui::game *ui;
};

#endif // GAME_H
