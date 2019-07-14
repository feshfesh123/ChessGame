#ifndef AI_H
#define AI_H
#include "board.h"
#include "attacks.h"
#include "bitutils.h"
#include "defs.h"
#include "move.h"
#include "movegen.h"
#include "rays.h"
#include "psquaretable.h"
class ai
{

public:
    ai(int , GamePhase);
    GamePhase _phase = OPENING;
    int _depth = 3;
    int positionCount = 0;
    Move minimaxRoot(Board board, int isMaximisingPlayer = BLACK);
    int minimax(int depth, Board board, int alpha, int beta, int isMaximisingPlayer );
    int evalutateBoard(Board board);
};

#endif // AI_H
