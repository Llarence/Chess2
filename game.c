#ifndef GAME
#define GAME

const int FALSE = 0;
const int TRUE = 1;

const int NONE = 0;
const int PAWN = 1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int ROOK = 4;
const int QUEEN = 5;
const int KING = 6;

const int WHITE = 0;
const int BLACK = 1;

const int STALEMATE = 1;
const int CHECKMATE = 2;

typedef struct Piece{
    int type;
    int color;
} Piece;

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

    int moves;
} Game;

typedef struct Move{
    int fromX;
    int fromY;
    int toX;
    int toY;
    int promotion;
} Move;

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

    newGame.moves = game->moves;

    return newGame;
}

int isPsuedoLegal(Game *game, Move move);

int isAttacked(Game *game, Piece piece){
    for(int toX = 0; toX < 8; toX++){
        for(int toY = 0; toY < 8; toY++){
            Piece currPiece = game->board[toX][toY];
            if(currPiece.type == piece.type && currPiece.color == piece.color){
                for(int fromX = 0; fromX < 8; fromX++){
                    for(int fromY = 0; fromY < 8; fromY++){
                        if(isPsuedoLegal(game, (Move){fromX, fromY, toX, toY, 0})){
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

void doMove(Game *game, Move move){
    Piece piece = game->board[move.fromX][move.fromY];
    if(game->board[move.toX][move.toY].type == NONE){
        game->halfMoveClock += 1;
    }else{
        game->halfMoveClock = 0;
    }

    game->board[move.toX][move.toY] = game->board[move.fromX][move.fromY];
    game->board[move.fromX][move.fromY].type = NONE;

    if(game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == move.toY){
        if(game->turn == WHITE){
            game->board[move.toX][move.toY - 1].type = NONE;
        }else{
            game->board[move.toX][move.toY + 1].type = NONE;
        }
    }

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
                game->board[5][7] = game->board[7][0];
                game->board[7][7].type = NONE;
            }

            if(deltaX == -2){
                game->board[3][7] = game->board[7][0];
                game->board[0][7].type = NONE;
            }
        }
    }

    if(piece.type == ROOK){
        if(piece.color == WHITE){
            if(move.fromX == 7){
                game->whiteCanCastleKingside = FALSE;
            }

            if(move.fromX == 0){
                game->whiteCanCastleQueenside = FALSE;
            }
        }else{
            if(move.fromX == 7){
                game->blackCanCastleKingside = FALSE;
            }

            if(move.fromX == 0){
                game->blackCanCastleQueenside = FALSE;
            }
        }
    }

    if(piece.type == PAWN){
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
        game->moves += 1;
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

int isPsuedoLegal(Game *game, Move move){
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
                            if(deltaX == 1 || deltaX ==-1){
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
                        if(game->board[move.fromX + 1][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX + 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                return TRUE;
                            }
                        }
                    }

                    if(game->whiteCanCastleQueenside && deltaX == -2 && deltaY == 0){
                        if(game->board[move.fromX - 1][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX - 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                return TRUE;
                            }
                        }
                    }
                }else{
                    if(game->blackCanCastleKingside && deltaX == 2 && deltaY == 0){
                        if(game->board[move.fromX + 1][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX - 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                return TRUE;
                            }
                        }
                    }

                    if(game->blackCanCastleQueenside && deltaX == -2 && deltaY == 0){
                        if(game->board[move.fromX - 1][move.fromY].type == NONE){
                            Game newGame = copyGame(game);
        
                            doMove(&newGame, (Move){move.fromX, move.fromY, move.fromX - 1, move.fromY, 0});
                            if(!isAttacked(&newGame, (Piece){KING, WHITE})){
                                return TRUE;
                            }
                        }
                    }
                }

                break;
        }
    }

    return FALSE;
}

int isLegal(Game *game, Move move){
    if(isPsuedoLegal(game, move)){
        Game newGame = copyGame(game);
        
        doMove(&newGame, move);
        if(!isAttacked(&newGame, (Piece){KING, game->turn})){
            return TRUE;
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

int isOver(Game *game){
    Move moves[512];
    generateLegalMoves(game, moves);
    if(moves->fromX == -1){

        if(isAttacked(game, (Piece){KING, game->turn})){
            return CHECKMATE;
        }

        return STALEMATE;
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