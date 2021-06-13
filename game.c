#ifndef GAME
#define GAME

#define FALSE 0
#define TRUE 1

#define NONE 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define WHITE 0
#define BLACK 1

#define STALEMATE 1
#define CHECKMATE 2

typedef struct Piece{
    int type;
    int color;
} Piece;

typedef struct Move{
    int fromX;
    int fromY;
    int toX;
    int toY;
    int promotion;
} Move;

typedef struct Game{
    Piece board[8][8];

    int turn;

    int whiteCanCastleKingside;
    int whiteCanCastleQueenside;
    int blackCanCastleKingside;
    int blackCanCastleQueenside;

    int canEnPassent;
    int enPassentX;
    int enPassentY;

    int halfMoveClock;

    Move prevMoveWhite1;
    Move prevMoveWhite2;

    Move prevMoveBlack1;
    Move prevMoveBlack2;
    
    int repeatMoves;

    int movesSinceCapture;

    int moves;
} Game;

void clearGame(Game *game){
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            game->board[x][y].type = NONE;
            game->board[x][y].color = WHITE;
        }
    }

    game->turn = WHITE;

    game->whiteCanCastleKingside = FALSE;
    game->whiteCanCastleQueenside = FALSE;
    game->blackCanCastleKingside = FALSE;
    game->blackCanCastleQueenside = FALSE;

    game->canEnPassent = FALSE;
    game->enPassentX = 0;
    game->enPassentY = 0;

    game->halfMoveClock = 0;

    game->prevMoveWhite1 = (Move){-1, -1, -1, -1, -1};
    game->prevMoveWhite2 = (Move){-1, -1, -1, -1, -1};

    game->prevMoveBlack1 = (Move){-1, -1, -1, -1, -1};
    game->prevMoveBlack2 = (Move){-1, -1, -1, -1, -1};

    game->repeatMoves = 0;

    game->movesSinceCapture = 0;

    game->moves = 0;
}

Game copyGame(Game *game){
    Game newGame;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            newGame.board[x][y] = game->board[x][y];
        }
    }

    newGame.turn = game->turn;

    newGame.whiteCanCastleKingside = game->whiteCanCastleKingside;
    newGame.whiteCanCastleQueenside = game->whiteCanCastleQueenside;
    newGame.blackCanCastleKingside = game->blackCanCastleKingside;
    newGame.blackCanCastleQueenside = game->blackCanCastleQueenside;

    newGame.canEnPassent = game->canEnPassent;
    newGame.enPassentX = game->enPassentX;
    newGame.enPassentY = game->enPassentY;

    newGame.halfMoveClock = game->halfMoveClock;

    newGame.prevMoveWhite1 = game->prevMoveWhite1;
    newGame.prevMoveWhite2 = game->prevMoveWhite2;

    newGame.prevMoveBlack1 = game->prevMoveBlack1;
    newGame.prevMoveBlack2 = game->prevMoveBlack2;

    newGame.repeatMoves = game->repeatMoves;

    newGame.movesSinceCapture = game->movesSinceCapture;

    newGame.moves = game->moves;

    return newGame;
}

int isPseudoLegal(Game *game, Move move);

int isAttacked(Game *game, Piece piece){
    for(int toX = 0; toX < 8; toX++){
        for(int toY = 0; toY < 8; toY++){
            Piece currPiece = game->board[toX][toY];
            if(currPiece.type == piece.type && currPiece.color == piece.color){
                for(int fromX = 0; fromX < 8; fromX++){
                    for(int fromY = 0; fromY < 8; fromY++){
                        if(isPseudoLegal(game, (Move){fromX, fromY, toX, toY, QUEEN})){
                            return TRUE;
                        }
                    }
                }

                return FALSE;
            }
        }
    }

    return FALSE;
}

int isCapture(Game *game, Move move){
    Piece piece = game->board[move.fromX][move.fromY];

    if(piece.type == PAWN){
        if(game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == move.toY){
            return TRUE;
        }
    }

    if(game->board[move.toX][move.toY].type != NONE){
        return TRUE;
    }

    return FALSE;
}

//Claimable 50 move rule
void doMove(Game *game, Move move){
    Piece piece = game->board[move.fromX][move.fromY];

    game->movesSinceCapture++;
    int capture = isCapture(game, move);
    if(capture){
        game->movesSinceCapture = 0;
    }

    if(game->turn == WHITE){
        if(!capture && piece.type != PAWN && (game->prevMoveWhite2.fromX == move.fromX && game->prevMoveWhite2.fromY == move.fromY && game->prevMoveWhite2.toX == move.toX && game->prevMoveWhite2.toY == move.toY)){
            game->repeatMoves++;
        }else{
            game->repeatMoves = 0;
        }

        game->prevMoveWhite2 = game->prevMoveWhite1;
        game->prevMoveWhite1 = move;
    }else{
        if(!capture && piece.type != PAWN && (game->prevMoveBlack2.fromX == move.fromX && game->prevMoveBlack2.fromY == move.fromY && game->prevMoveBlack2.toX == move.toX && game->prevMoveBlack2.toY == move.toY)){
            game->repeatMoves++;
        }else{
            game->repeatMoves = 0;
        }

        game->prevMoveBlack2 = game->prevMoveBlack1;
        game->prevMoveBlack1 = move;
    }

    if(game->board[move.toX][move.toY].type == NONE){
        game->halfMoveClock++;
    }else{
        game->halfMoveClock = 0;
    }

    game->board[move.toX][move.toY] = game->board[move.fromX][move.fromY];
    game->board[move.fromX][move.fromY].type = NONE;

    int deltaX = move.toX - move.fromX;
    int deltaY = move.toY - move.fromY;

    if(piece.type == KING){
        if(piece.color == WHITE){
            game->whiteCanCastleKingside = FALSE;
            game->whiteCanCastleQueenside = FALSE;

            if(deltaX == 2){
                game->board[5][0] = game->board[7][0];
                game->board[7][0].type = NONE;
            }

            if(deltaX == -2){
                game->board[3][0] = game->board[7][0];
                game->board[0][0].type = NONE;
            }
        }else{
            game->blackCanCastleKingside = FALSE;
            game->blackCanCastleQueenside = FALSE;

            if(deltaX == 2){
                game->board[5][7] = game->board[7][7];
                game->board[7][7].type = NONE;
            }

            if(deltaX == -2){
                game->board[3][7] = game->board[7][7];
                game->board[0][7].type = NONE;
            }
        }
    }

    if((move.fromX == 7 && move.fromY == 0) || (move.toX == 7 && move.toY == 0)){
        game->whiteCanCastleKingside = FALSE;
    }

    if((move.fromX == 0 && move.fromY == 0) || (move.toX == 0 && move.toY == 0)){
        game->whiteCanCastleQueenside = FALSE;
    }

    if((move.fromX == 7 && move.fromY == 7) || (move.toX == 7 && move.toY == 7)){
        game->blackCanCastleKingside = FALSE;
    }

    if((move.fromX == 0 && move.fromY == 7) || (move.toX == 0 && move.toY == 7)){
        game->blackCanCastleQueenside = FALSE;
    }

    if(piece.type == PAWN){
        if(game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == move.toY){
            if(game->turn == WHITE){
                game->board[move.toX][move.toY - 1].type = NONE;
            }else{
                game->board[move.toX][move.toY + 1].type = NONE;
            }
        }

        if(deltaY == 2 || deltaY == -2){
            game->canEnPassent = TRUE;
            game->enPassentX = move.toX;
            if(game->turn == WHITE){
                game->enPassentY = 2;
            }else{
                game->enPassentY = 5;
            }
        }else{
            game->canEnPassent = FALSE;
        }

        if(piece.color == WHITE){
            if(move.toY == 7){
                game->board[move.toX][move.toY].type = move.promotion;
            }
        }else{
            if(move.toY == 0){
                game->board[move.toX][move.toY].type = move.promotion;
            }
        }
    }

    if(game->turn == WHITE){
        game->turn = BLACK;
    }else{
        game->turn = WHITE;
        game->moves++;
    }
}

int isValidMove(Move move){
    if(move.fromX != move.toX || move.fromY != move.toY){
        if((move.fromX < 8 && move.fromX >= 0) && (move.fromY < 8 && move.fromY >= 0) && (move.toX < 8 && move.toX >= 0) && (move.toY < 8 && move.toY >= 0)){
            if(move.promotion == QUEEN || move.promotion == ROOK || move.promotion == BISHOP || move.promotion == KNIGHT){
                return TRUE;
            }
        }
    }

    return FALSE;
}

int isPseudoLegal(Game *game, Move move){
    Piece piece = game->board[move.fromX][move.fromY];
    Piece attacked = game->board[move.toX][move.toY];
    int deltaX = move.toX - move.fromX;
    int deltaY = move.toY - move.fromY;
    int x;
    int y;

    if((attacked.type == NONE || attacked.color != piece.color) && game->turn == piece.color){
        switch (piece.type){
            case PAWN:
                if(piece.color == WHITE){
                    if(deltaY == 1){
                        if(attacked.type != NONE || (game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == 5 && move.toY == 5)){
                            if(deltaX == 1 || deltaX ==-1){
                                return TRUE;
                            }
                        }else{
                            if(deltaX == 0){
                                return TRUE;
                            }
                        }
                    }else if(move.fromY == 1 && deltaY == 2){
                        if(deltaX == 0){
                            if(game->board[move.fromX][move.fromY + 1].type == NONE && attacked.type == NONE){
                                return TRUE;
                            }
                        }
                    }
                }else{
                    if(deltaY == -1){
                        if(attacked.type != NONE || (game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == 2 && move.toY == 2)){
                            int deltaX = move.toX - move.fromX;
                            if(deltaX == 1 || deltaX == -1){
                                return TRUE;
                            }
                        }else{
                            if(deltaX == 0){
                                return TRUE;
                            }
                        }
                    }else if(move.fromY == 6 && deltaY == -2){
                        if(deltaX == 0){
                            if(game->board[move.fromX][move.fromY - 1].type == NONE && attacked.type == NONE){
                                return TRUE;
                            }
                        }
                    }
                }

                break;

            case KNIGHT:
                if(((deltaX == 1 || deltaX == -1) && (deltaY == 2 || deltaY == -2)) || (deltaX == 2 || deltaX == -2) && (deltaY == 1 || deltaY == -1)){
                    return TRUE;
                }

                break;
            
            case BISHOP:
                if(deltaX == deltaY){
                    if(deltaX > 0 && deltaY > 0){
                        x = move.fromX + 1;
                        y = move.fromY + 1;
                        while(x < move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x++;
                            y++;
                        }

                        return TRUE;
                    }

                    if(deltaX < 0 && deltaY < 0){
                        x = move.fromX - 1;
                        y = move.fromY - 1;
                        while(x > move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x--;
                            y--;
                        }

                        return TRUE;
                    }
                }

                if(-deltaX == deltaY){
                    if(deltaX > 0 && deltaY < 0){
                        x = move.fromX + 1;
                        y = move.fromY - 1;
                        while(x < move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x++;
                            y--;
                        }

                        return TRUE;
                    }

                    if(deltaX < 0 && deltaY > 0){
                        x = move.fromX - 1;
                        y = move.fromY + 1;
                        while(x > move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x--;
                            y++;
                        }

                        return TRUE;
                    }
                }

                break;

            case ROOK:
                if(deltaX > 0 && deltaY == 0){
                    for(int x = move.fromX + 1; x < move.toX; x++){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX < 0 && deltaY == 0){
                    for(int x = move.fromX - 1; x > move.toX; x--){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX == 0 && deltaY > 0){
                    for(int y = move.fromY + 1; y < move.toY; y++){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX == 0 && deltaY < 0){
                    for(int y = move.fromY - 1; y > move.toY; y--){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                break;
            
            case QUEEN:
                if(deltaX == deltaY){
                    if(deltaX > 0 && deltaY > 0){
                        x = move.fromX + 1;
                        y = move.fromY + 1;
                        while(x < move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x++;
                            y++;
                        }

                        return TRUE;
                    }

                    if(deltaX < 0 && deltaY < 0){
                        x = move.fromX - 1;
                        y = move.fromY - 1;
                        while(x > move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x--;
                            y--;
                        }

                        return TRUE;
                    }
                }

                if(-deltaX == deltaY){
                    if(deltaX > 0 && deltaY < 0){
                        x = move.fromX + 1;
                        y = move.fromY - 1;
                        while(x < move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x++;
                            y--;
                        }

                        return TRUE;
                    }

                    if(deltaX < 0 && deltaY > 0){
                        x = move.fromX - 1;
                        y = move.fromY + 1;
                        while(x > move.toX){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }

                            x--;
                            y++;
                        }

                        return TRUE;
                    }
                }

                if(deltaX > 0 && deltaY == 0){
                    for(int x = move.fromX + 1; x < move.toX; x++){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX < 0 && deltaY == 0){
                    for(int x = move.fromX - 1; x > move.toX; x--){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX == 0 && deltaY > 0){
                    for(int y = move.fromY + 1; y < move.toY; y++){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                if(deltaX == 0 && deltaY < 0){
                    for(int y = move.fromY - 1; y > move.toY; y--){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }
                    }

                    return TRUE;
                }

                break;
            
            case KING:
                if((deltaX <= 1 && deltaX >= -1) && (deltaY <= 1 && deltaY >= -1) && (deltaX != 0 || deltaY != 0)){
                    return TRUE;
                }

                if(piece.color == WHITE){
                    if(game->whiteCanCastleKingside && deltaX == 2 && deltaY == 0){
                        if(game->board[move.fromX + 1][move.fromY].type == NONE && game->board[move.toX][move.toY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX + 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                newGame = copyGame(game);
                                newGame.turn = BLACK;
                                if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                    return TRUE;
                                }
                            }
                        }
                    }

                    if(game->whiteCanCastleQueenside && deltaX == -2 && deltaY == 0){
                        if(game->board[move.fromX - 1][move.fromY].type == NONE && game->board[move.toX][move.toY].type == NONE && game->board[move.fromX - 3][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX - 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                newGame = copyGame(game);
                                newGame.turn = BLACK;
                                if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                    return TRUE;
                                }
                            }
                        }
                    }
                }else{
                    if(game->blackCanCastleKingside && deltaX == 2 && deltaY == 0){
                        if(game->board[move.fromX + 1][move.fromY].type == NONE && game->board[move.toX][move.toY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX + 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, BLACK})){
                                newGame = copyGame(game);
                                newGame.turn = WHITE;
                                if(!isAttacked(&newGame, (Piece){KING, BLACK})){
                                    return TRUE;
                                }
                            }
                        }
                    }

                    if(game->blackCanCastleQueenside && deltaX == -2 && deltaY == 0){
                        if(game->board[move.fromX - 1][move.fromY].type == NONE && game->board[move.toX][move.toY].type == NONE && game->board[move.fromX - 3][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX - 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, BLACK})){
                                newGame = copyGame(game);
                                newGame.turn = WHITE;
                                if(!isAttacked(&newGame, (Piece){KING, BLACK})){
                                    return TRUE;
                                }
                            }
                        }
                    }
                }

                break;
        }
    }

    return FALSE;
}

int isOver(Game *game, int hasMove);

int isLegal(Game *game, Move move){
    if(isPseudoLegal(game, move)){
        if(!isOver(game, TRUE)){
            Game newGame = copyGame(game);
            doMove(&newGame, move);

            if(!isAttacked(&newGame, (Piece){KING, game->turn})){
                return TRUE;
            }
        }
    }

    return FALSE;
}

void generateLegalMoves(Game *game, Move *moves){
    int i = 0;

    for(int fromX = 0; fromX < 8; fromX++){
        for(int fromY = 0; fromY < 8; fromY++){
            for(int toX = 0; toX < 8; toX++){
                for(int toY = 0; toY < 8; toY++){
                    Move move = {fromX, fromY, toX, toY, QUEEN};
                    if(isLegal(game, move)){
                        moves[i] = move;
                        i++;
                    }
                }
            }
        }
    }

    moves[i] = (Move){-1, -1, -1, -1, -1};
}

int isMove(Game *game){
    for(int fromX = 0; fromX < 8; fromX++){
        for(int fromY = 0; fromY < 8; fromY++){
            for(int toX = 0; toX < 8; toX++){
                for(int toY = 0; toY < 8; toY++){
                    if(isLegal(game, (Move){fromX, fromY, toX, toY, QUEEN})){
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

int isOver(Game *game, int hasMove){
    if(game->repeatMoves == 6 || game->movesSinceCapture == 75){
        return STALEMATE;
    }

    if(!hasMove && !isMove(game)){
        int orginalTurn = game->turn;
        
        if(game->turn == WHITE){
            game->turn = BLACK;
        }else{
            game->turn = WHITE;
        }

        if(isAttacked(game, (Piece){KING, orginalTurn})){
            game->turn = orginalTurn;
            return CHECKMATE;
        }else{
            game->turn = orginalTurn;
            return STALEMATE;
        }
    }
    
    return FALSE;
}

int tryMove(Game *game, Move move){
    if(isValidMove(move)){
        if(isLegal(game, move)){
            doMove(game, move);
            return TRUE;
        }
    }

    return FALSE;
}

#endif