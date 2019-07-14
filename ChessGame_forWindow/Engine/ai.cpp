#include "ai.h"
#include <algorithm>
ai::ai(int depth, GamePhase phase)
{
    this->_depth = depth;
    this->positionCount = 0;
    this->_phase = phase;
}

Move ai::minimaxRoot(Board board, int isMaximisingPlayer)
{
    MoveGen newGameMoves(board);
    MoveList list_move = newGameMoves.getLegalMoves();
    int bestMove = -99999;
    Move bestMoveFound;

    for (auto i: list_move){
        Board tmpBoard = board;
        tmpBoard.doMove(i);
        int value = minimax(_depth -1, tmpBoard, -100000, 100000, !isMaximisingPlayer);
        if (value >= bestMove){
            bestMove = value;
            bestMoveFound = i;
        }
    }
    return bestMoveFound;
}

int ai::minimax(int depth, Board board, int alpha, int beta, int isMaximisingPlayer)
{
    positionCount++;
    if (depth == 0 ) {
        return -evalutateBoard(board);
    }

    MoveGen m(board);
    MoveList list_move = m.getLegalMoves();

    if ( isMaximisingPlayer ){
        int bestMove = -99999;
        for (auto i: list_move){
            Board tmpBoard = board;
            tmpBoard.doMove(i);
            bestMove = std::max(bestMove, minimax(depth-1, tmpBoard, alpha, beta, !isMaximisingPlayer));
            alpha = std::max(alpha, bestMove);
            if (beta<= alpha){
                return bestMove;
            }
        }
        return bestMove;
    }
    else {
        int bestMove = 99999;
        for (auto i: list_move){
            Board tmpBoard = board;
            tmpBoard.doMove(i);
            bestMove = std::min(bestMove, minimax(depth -1, tmpBoard, alpha, beta, !isMaximisingPlayer));
            beta = std::min(beta, bestMove);
            if (beta<= alpha){
                return bestMove;
            }
        }
        return bestMove;
    }
}

int ai::evalutateBoard(Board board)
{
    PSquareTable pst(board);
    return pst.getScore(_phase, WHITE) - pst.getScore(_phase, BLACK);
}
