#include "validation.h"

validation::validation()
{
    //Nothing here
}

int validation::chooser(Tile *temp)
{
    int type = temp->pieceType;

    MoveGen movegen(board);

    list_move = movegen.getLegalMoves();
    int retVal = 0;
    int num = temp->tileNum;
    for (auto& i: list_move)
    {
        if (i.getPieceType() == type && i.getFrom() == num)
        {
             e[max] = i.getTo();
             f[max] = &i- &list_move[0];
             max++;
             retVal = 1;
        }
    }

    orange();

    return retVal;
}

void validation::orange()
{
    int i,n;

    for(i=0;i<max;i++)
        tile[e[i]/8][e[i]%8]->setStyleSheet("QLabel {background-color: #b3ff99;}");
}
