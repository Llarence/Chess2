#ifndef AI
#define AI

#include <stdlib.h>
#include <math.h>

#include "game.c"
#include "values.c"

typedef struct ValuedMoveSet{
    int value;
    Move *moves;
} ValuedMoveSet;

typedef struct ValuedMove{
    int value;
    Move move;
} ValuedMove;

int eval(Game *game, int color, int depth){
    int over = isOver(game, FALSE);
    if(over == CHECKMATE){
        if(game->turn != color){
            return 100000 + depth;
        }else{
            return -100000 - depth;
        }
    }

    if(over == STALEMATE){
        return 0;
    }

    int value = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            Piece piece = game->board[x][y];
            if(piece.color == color){
                if(piece.color == WHITE){
                    switch(piece.type){
                        case PAWN:
                            value += PAWN_VALUE;
                            value += WHITE_PAWN_TABLE[y][x];
                            break;
                        
                        case KNIGHT:
                            value += KNIGHT_VALUE;
                            value += WHITE_KNIGHT_TABLE[y][x];
                            break;

                        case BISHOP:
                            value += BISHOP_VALUE;
                            value += WHITE_BISHOP_TABLE[y][x];
                            break;

                        case ROOK:
                            value += ROOK_VALUE;
                            value += WHITE_ROOK_TABLE[y][x];
                            break;
                        
                        case QUEEN:
                            value += QUEEN_VALUE;
                            value += WHITE_QUEEN_TABLE[y][x];
                            break;
                        
                        case KING:
                            value += WHITE_KING_TABLE[y][x];
                    }
                }else{
                    switch(piece.type){
                        case PAWN:
                            value += PAWN_VALUE;
                            value += BLACK_PAWN_TABLE[y][x];
                            break;
                        
                        case KNIGHT:
                            value += KNIGHT_VALUE;
                            value += BLACK_KNIGHT_TABLE[y][x];
                            break;

                        case BISHOP:
                            value += BISHOP_VALUE;
                            value += BLACK_BISHOP_TABLE[y][x];
                            break;

                        case ROOK:
                            value += ROOK_VALUE;
                            value += BLACK_ROOK_TABLE[y][x];
                            break;
                        
                        case QUEEN:
                            value += QUEEN_VALUE;
                            value += BLACK_QUEEN_TABLE[y][x];
                            break;
                        
                        case KING:
                            value += BLACK_KING_TABLE[y][x];
                    }
                }
            }else{
                if(piece.color == WHITE){
                    switch(piece.type){
                        case PAWN:
                            value -= PAWN_VALUE;
                            value -= WHITE_PAWN_TABLE[y][x];
                            break;
                        
                        case KNIGHT:
                            value -= KNIGHT_VALUE;
                            value -= WHITE_KNIGHT_TABLE[y][x];
                            break;

                        case BISHOP:
                            value -= BISHOP_VALUE;
                            value -= WHITE_BISHOP_TABLE[y][x];
                            break;

                        case ROOK:
                            value -= ROOK_VALUE;
                            value -= WHITE_ROOK_TABLE[y][x];
                            break;
                        
                        case QUEEN:
                            value -= QUEEN_VALUE;
                            value -= WHITE_QUEEN_TABLE[y][x];
                            break;
                        
                        case KING:
                            value -= WHITE_KING_TABLE[y][x];
                    }
                }else{
                    switch(piece.type){
                        case PAWN:
                            value -= PAWN_VALUE;
                            value -= BLACK_PAWN_TABLE[y][x];
                            break;
                        
                        case KNIGHT:
                            value -= KNIGHT_VALUE;
                            value -= BLACK_KNIGHT_TABLE[y][x];
                            break;

                        case BISHOP:
                            value -= BISHOP_VALUE;
                            value -= BLACK_BISHOP_TABLE[y][x];
                            break;

                        case ROOK:
                            value -= ROOK_VALUE;
                            value -= BLACK_ROOK_TABLE[y][x];
                            break;
                        
                        case QUEEN:
                            value -= QUEEN_VALUE;
                            value -= BLACK_QUEEN_TABLE[y][x];
                            break;
                        
                        case KING:
                            value -= BLACK_KING_TABLE[y][x];
                    }
                }
            }
        }
    }

    if(color == WHITE){
        if(game->blackCanCastleKingside){
            value -= 30;
        }
        if(game->blackCanCastleQueenside){
            value -= 30;
        }
    }else{
        if(game->whiteCanCastleKingside){
            value -= 30;
        }
        if(game->whiteCanCastleQueenside){
            value -= 30;
        }
    }

    return value;
}

int nonPawns(Game *game){
    int value = 0;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int type = game->board[x][y].type;
            if(type != NONE && type != PAWN){
                value += 1;
            }
        }
    }

    return value;
}

int getCaptureType(Game *game, Move move){
    if(game->board[move.fromX][move.fromY].type == PAWN){
        if(game->canEnPassent && game->enPassentX == move.toX && game->enPassentY == move.toY){
            if(game->turn == WHITE){
                return game->board[move.toX][move.toY - 1].type;
            }else{
                return game->board[move.toX][move.toY - 1].type;
            }
        }

        return NONE;
    }

    return game->board[move.toX][move.toY].type != NONE && game->board[move.toX][move.toY].type;
}

int extend(Game *game, Game *newGame, Move move, int depth){
    int nonPawnsNew = nonPawns(newGame);
    if(depth == DEPTH && nonPawnsNew <= EXTEND_SEARCH_NONPAWNS){
        return EXTEND_DEPTH;
    }

    int captureType = getCaptureType(game, move);
    if(captureType == NONE){
        return -1;
    }else if(captureType == PAWN){
        return 0;
    }else{
        return 1;
    }
}

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth, int extensions);

ValuedMove minMove(Game *game, int color, int alpha, int beta, int depth, int extensions){
    if((!(DEPTH - FULL_DEPTH < depth) && depth <= -extensions) || depth == DEPTH - MAX_DEPTH){
        int value = eval(game, color, depth);
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[218];
    generateLegalMoves(game, moves);
    if(moves[0].fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);

        ValuedMove curr = maxMove(&newGame, color, alpha, beta, depth - 1, extensions + extend(game, &newGame, moves[i], depth));

        if(curr.value < best.value){
            best.move = moves[i];
            best.value = curr.value;
            if(beta > curr.value){
                beta = curr.value;
            }
        }

        if(alpha >= beta){
            break;
        }
    }

    return best;
}

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth, int extensions){
    if((!(DEPTH - FULL_DEPTH < depth) && depth <= -extensions) || depth == DEPTH - MAX_DEPTH){
        int value = eval(game, color, depth);
        return (ValuedMove){value, (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[218];
    generateLegalMoves(game, moves);
    if(moves[0].fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {-1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);

        ValuedMove curr = minMove(&newGame, color, alpha, beta, depth - 1, extensions + extend(game, &newGame, moves[i], depth));

        if(curr.value > best.value){
            best.move = moves[i];
            best.value = curr.value;
            if(alpha < curr.value){
                alpha = curr.value;
            }
        }

        if(alpha >= beta){
            break;
        }
    }

    return best;
}

Move getAIMove(Game *game){
    return maxMove(game, game->turn, -1000000, 1000000, DEPTH, 0).move;
}

#endif