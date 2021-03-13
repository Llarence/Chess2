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

typedef struct Piece{
    int type;
    int color;
} Piece;

typedef struct Game{
    Piece board[8][8];

    int  turn;

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

int isValidMove(Move move){
    if(move.fromX != move.toX || move.fromY != move.toY){
        if((move.fromX < 8 && move.fromX >= 0) && (move.fromY < 8 && move.fromY >= 0) && (move.toX < 8 && move.toX >= 0) && (move.toY < 8 && move.toY >= 0)){
            return TRUE;
        }
    }

    return FALSE;
}

int isPuesdoLegal(Game *game, Move move){
    Piece piece = game->board[move.fromX][move.fromY];
    Piece attacked = game->board[move.toX][move.toY];
    int deltaX = move.toX - move.fromX;
    int deltaY = move.toY - move.fromY;

    if((attacked.type == NONE || attacked.color != piece.color) && game->turn == piece.color){
        switch (piece.type){
            case PAWN:
                if(piece.color == WHITE){
                    if(deltaY == 1){
                        if(attacked.type != NONE){
                            if(deltaX == 1 || deltaX ==-1){
                                return TRUE;
                            }
                        }else{
                            if(deltaX == 0){
                                return TRUE;
                            }
                        }
                    }
                }else{
                    if(deltaY == -1){
                        if(attacked.type != NONE){
                            int deltaX = move.toX - move.fromX;
                            if(deltaX == 1 || deltaX ==-1){
                                return TRUE;
                            }
                        }else{
                            if(deltaX == 0){
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
                if(deltaX > 0 || deltaY > 0){
                    for(int x = 1; x < deltaX; x++){
                        for(int y = 1; y < deltaY; y++){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY > 0){
                    for(int x = -1; x > deltaX; x--){
                        for(int y = 1; y < deltaY; y++){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY < 0){
                    for(int x = -1; x > deltaX; x--){
                        for(int y = -1; y > deltaY; y--){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX > 0 || deltaY < 0){
                    for(int x = 1; x < deltaX; x++){
                        for(int y = -1; y > deltaY; y--){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                break;

            case ROOK:
                if(deltaX > 0 || deltaY == 0){
                    for(int x = 1; x < deltaX; x++){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY == 0){
                    for(int x = -1; x > deltaX; x--){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX == 0 || deltaY > 0){
                    for(int y = 1; y < deltaY; y++){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX == 0 || deltaY < 0){
                    for(int y = -1; y > deltaY; y--){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                break;
            
            case QUEEN:
                if(deltaX > 0 || deltaY > 0){
                    for(int x = 1; x < deltaX; x++){
                        for(int y = 1; y < deltaY; y++){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY > 0){
                    for(int x = -1; x > deltaX; x--){
                        for(int y = 1; y < deltaY; y++){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY < 0){
                    for(int x = -1; x > deltaX; x--){
                        for(int y = -1; y > deltaY; y--){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX > 0 || deltaY < 0){
                    for(int x = 1; x < deltaX; x++){
                        for(int y = -1; y > deltaY; y--){
                            if(game->board[x][y].type != NONE){
                                return FALSE;
                            }
                        }

                        return TRUE;
                    }
                }

                if(deltaX > 0 || deltaY == 0){
                    for(int x = 1; x < deltaX; x++){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX < 0 || deltaY == 0){
                    for(int x = -1; x > deltaX; x--){
                        if(game->board[x][move.fromY].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX == 0 || deltaY > 0){
                    for(int y = 1; y < deltaY; y++){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }

                if(deltaX == 0 || deltaY < 0){
                    for(int y = -1; y > deltaY; y--){
                        if(game->board[move.fromX][y].type != NONE){
                            return FALSE;
                        }

                        return TRUE;
                    }
                }
            
            case KING:
                if((deltaX <= 1 && deltaX >= -1) && (deltaY <= 1 && deltaY >= -1) && (deltaX != 0 && deltaY != 0)){
                    return TRUE;
                }
        }
    }

    return FALSE;
}

void doMove(Game *game, Move move){
    game->board[move.toX][move.toY] = game->board[move.fromX][move.fromY];
    game->board[move.fromX][move.fromY].type = NONE;
}

void tryMove(Game *game, Move move){
    if(isValidMove(move)){
        if(isPuesdoLegal(game, move)){
            doMove(game, move);
        }
    }
}

#endif