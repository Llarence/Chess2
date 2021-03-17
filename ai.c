#ifndef AI
#define AI

#include <stdlib.h>

#include "game.c"
#include "values.c"

typedef struct ValuedMove{
    int value;
    Move move;
} ValuedMove;

int eval(Game *game, int color, int depth){
    int over = isOver(game);
    if(over == CHECKMATE){
        if(game->turn == color){
            return 100000 + (depth * 10000);
        }else{
            return -100000 - (depth * 10000);
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
                switch(piece.type){
                    case PAWN:
                        value += PAWN_VALUE;
                        if(piece.color == WHITE){
                            value += WHITE_PAWN_TABLE[y][x];
                        }else{
                            value += BLACK_PAWN_TABLE[y][x];
                        }

                        break;
                    
                    case KNIGHT:
                        value += KNIGHT_VALUE;
                        if(piece.color == WHITE){
                            value += WHITE_KNIGHT_TABLE[y][x];
                        }else{
                            value += BLACK_KNIGHT_TABLE[y][x];
                        }

                        break;

                    case BISHOP:
                        value += BISHOP_VALUE;
                        if(piece.color == WHITE){
                            value += WHITE_BISHOP_TABLE[y][x];
                        }else{
                            value += BLACK_BISHOP_TABLE[y][x];
                        }

                        break;

                    case ROOK:
                        value += ROOK_VALUE;
                        if(piece.color == WHITE){
                            value += WHITE_ROOK_TABLE[y][x];
                        }else{
                            value += BLACK_ROOK_TABLE[y][x];
                        }

                        break;
                    
                    case QUEEN:
                        value += QUEEN_VALUE;
                        if(piece.color == WHITE){
                            value += WHITE_QUEEN_TABLE[y][x];
                        }else{
                            value += BLACK_QUEEN_TABLE[y][x];
                        }

                        break;
                    
                    case KING:
                        if(piece.color == WHITE){
                            value += WHITE_KING_TABLE[y][x];
                        }else{
                            value += BLACK_KING_TABLE[y][x];
                        }
                }
            }else{
                switch(piece.type){
                    case PAWN:
                        value -= PAWN_VALUE;
                        if(piece.color == WHITE){
                            value -= WHITE_PAWN_TABLE[y][x];
                        }else{
                            value -= BLACK_PAWN_TABLE[y][x];
                        }

                        break;
                    
                    case KNIGHT:
                        value -= KNIGHT_VALUE;
                        if(piece.color == WHITE){
                            value -= WHITE_KNIGHT_TABLE[y][x];
                        }else{
                            value -= BLACK_KNIGHT_TABLE[y][x];
                        }

                        break;

                    case BISHOP:
                        value -= BISHOP_VALUE;
                        if(piece.color == WHITE){
                            value -= WHITE_BISHOP_TABLE[y][x];
                        }else{
                            value -= BLACK_BISHOP_TABLE[y][x];
                        }

                        break;

                    case ROOK:
                        value -= ROOK_VALUE;
                        if(piece.color == WHITE){
                            value -= WHITE_ROOK_TABLE[y][x];
                        }else{
                            value -= BLACK_ROOK_TABLE[y][x];
                        }

                        break;
                    
                    case QUEEN:
                        value -= QUEEN_VALUE;
                        if(piece.color == WHITE){
                            value -= WHITE_QUEEN_TABLE[y][x];
                        }else{
                            value -= BLACK_QUEEN_TABLE[y][x];
                        }

                        break;
                    
                    case KING:
                        if(piece.color == WHITE){
                            value -= WHITE_KING_TABLE[y][x];
                        }else{
                            value -= BLACK_KING_TABLE[y][x];
                        }
                }
            }
        }
    }

    return value;
}

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth);

ValuedMove minMove(Game *game, int color, int alpha, int beta, int depth){
    if(depth == 0){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[512];
    generateLegalMoves(game, moves);
    if(moves->fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 512; i++){
        if(moves[i].fromX == -1){
            break;
        }
        
        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);
        ValuedMove curr = maxMove(&newGame, color, alpha, beta, depth - 1);

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

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth){
    if(depth == 0){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[512];
    generateLegalMoves(game, moves);
    if(moves->fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {-1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 512; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);
        ValuedMove curr = minMove(&newGame, color, alpha, beta, depth - 1);

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

#endif