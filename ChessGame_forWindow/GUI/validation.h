#ifndef VALIDATION_H
#define VALIDATION_H
#include "tile.h"
#include "Engine/attacks.h"
#include "Engine/rays.h"
#include "Engine/defs.h"
#include "Engine/board.h"
#include "Engine/move.h"
#include "Engine/movegen.h"
#include "Engine/psquaretable.h"
#include "Engine/ai.h"

extern Tile *tile[8][8];
extern int e[60], f[60] ,max,wR,wC,bR,bC;
extern Board board;
extern MoveList list_move;
extern Board boardHistory[400];
extern int current, _undo, _isAI, _isPlayer, _isChall, _numChall;
extern int _start, _player;
extern QTime time1, time2, _totalTime;
extern QTimer *timer;
extern Move moveHistory[400];
extern Move movePromotion;
extern bool isFinishPromotion;
extern int depthChoosed;
extern GamePhase phase;


class validation
{
    int flag,retVal;

public:
    validation();

    // Sinh nước đi có thể
    int chooser(Tile *temp);

    // Hiển thị các nước đi
    void orange();

};

#endif // VALIDATION_H
