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

typedef struct PartialMaxMoveArgs{
    Game *game;
    Move *moves;
    int rangeMin;
    int rangeMax;
    int color;
    int alpha;
    int beta;
    int startValue;
    int depth;
    int extensions;
} PartialMaxMoveArgs;

//fix endgame idle
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
                value += CONTEMPT;
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
            value -= 20;
        }
        if(game->blackCanCastleQueenside){
            value -= 20;
        }
    }else{
        if(game->whiteCanCastleKingside){
            value -= 20;
        }
        if(game->whiteCanCastleQueenside){
            value -= 20;
        }
    }

    return value;
}

void sort(Game *game, Move *moves, int color, int depth, int min){
    int moveValues[218];
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);
        moveValues[i] = eval(&newGame, color, depth);
    }

    Move tempMove;
    int tempValue;
    if(min){
        for(int i = 0; i < 217; i++){
            if(moves[i + 1].fromX == -1){
                break;
            }

            for(int j = 0; j < 217; j++){
                if(moves[j + 1].fromX == -1){
                    break;
                }

                if (moveValues[j] < moveValues[j + 1]){
                    tempMove = moves[j + 1];
                    moves[j + 1] = moves[j];
                    moves[j] = tempMove;

                    tempValue = moveValues[j + 1];
                    moveValues[j + 1] = moveValues[j];
                    moveValues[j] = tempValue;
                }
            }
        }
    }else{
        for(int i = 0; i < 217; i++){
            if(moves[i + 1].fromX == -1){
                break;
            }

            for(int j = 0; j < 217; j++){
                if(moves[j + 1].fromX == -1){
                    break;
                }

                if (moveValues[j] > moveValues[j + 1]){
                    tempMove = moves[j + 1];
                    moves[j + 1] = moves[j];
                    moves[j] = tempMove;

                    tempValue = moveValues[j + 1];
                    moveValues[j + 1] = moveValues[j];
                    moveValues[j] = tempValue;
                }
            }
        }
    }
}

int nonPawns(Game *game){
    int value = 0;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int type = game->board[x][y].type;
            if(type != NONE && type != PAWN){
                value++;
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

int extend(Game *game, Game *newGame, int color, Move move, int depth, int startValue){
    int value = 0;

    int nonPawnsNew = nonPawns(newGame);
    if(depth == DEPTH && nonPawnsNew <= EXTEND_SEARCH_NONPAWNS){
        value += EXTEND_DEPTH;
    }

    int captureType = getCaptureType(game, move);
    if(captureType == NONE){
        value += -1;
    }else if(captureType != PAWN){
        value += 1;
    }

    int deltaEval = startValue - eval(newGame, color, depth);
    if(deltaEval < 400){
        value += -1;
    }

    return value;
}

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth, int startValue, int extensions);

ValuedMove minMove(Game *game, int color, int alpha, int beta, int depth, int startValue, int extensions){
    if((!(DEPTH - FULL_DEPTH < depth) && depth <= -extensions) || depth == DEPTH - MAX_DEPTH){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[218];
    generateLegalMoves(game, moves);
    if(moves[0].fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }
    sort(game, moves, color, depth, FALSE);

    ValuedMove best = {1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);

        ValuedMove curr = maxMove(&newGame, color, alpha, beta, depth - 1, startValue, extensions + extend(game, &newGame, color, moves[i], depth, startValue));

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

ValuedMove maxMove(Game *game, int color, int alpha, int beta, int depth, int startValue, int extensions){
    if((!(DEPTH - FULL_DEPTH < depth) && depth <= -extensions) || depth == DEPTH - MAX_DEPTH){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[218];
    generateLegalMoves(game, moves);
    if(moves[0].fromX == -1){
        return (ValuedMove){eval(game, color, depth), (Move){-1, -1, -1, -1, -1}};
    }
    sort(game, moves, color, depth, TRUE);

    ValuedMove best = {-1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);

        ValuedMove curr = minMove(&newGame, color, alpha, beta, depth - 1, startValue, extensions + extend(game, &newGame, color, moves[i], depth, startValue));

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

void *partialMaxMove(void *inpArgs){
    PartialMaxMoveArgs *args = (PartialMaxMoveArgs *)inpArgs;
    int alpha = args->alpha;

    ValuedMove *best = (ValuedMove *)malloc(sizeof(ValuedMove));
    best->value = -1000000;
    best->move = (Move){-1, -1, -1, -1, -1};
    for(int i = args->rangeMin; i < args->rangeMax; i++){
        Game newGame = copyGame(args->game);
        doMove(&newGame, args->moves[i]);

        ValuedMove curr = minMove(&newGame, args->color, alpha, args->beta, args->depth - 1, args->startValue, args->extensions + extend(args->game, &newGame, args->color, args->moves[i], args->depth, args->startValue));

        if(curr.value > best->value){
            best->move = args->moves[i];
            best->value = curr.value;
            if(alpha < curr.value){
                alpha = curr.value;
            }
        }
    }

    return (void *)best;
}

Move getAIMove(Game *game){
    Move moves[218];
    generateLegalMoves(game, moves);
    if(moves[0].fromX == -1){
        return (Move){-1, -1, -1, -1, -1};
    }

    int movesLen = 0;
    for(int i = 0; i < 218; i++){
        if(moves[i].fromX == -1){
            break;
        }

        movesLen++;
    }

    int movesPerThread = movesLen / THREADS;

    PartialMaxMoveArgs *inpArgs[THREADS];
    void *outs[THREADS];
    pthread_t threadIDs[THREADS];
    for(int i = 0; i < THREADS; i++){
        PartialMaxMoveArgs *inpArg = (PartialMaxMoveArgs *)malloc(sizeof(PartialMaxMoveArgs));
        inpArg->game = game;
        inpArg->moves = moves;
        inpArg->rangeMin = i * movesPerThread;
        if(i == THREADS - 1){
            inpArg->rangeMax = movesLen;
        }else{
            inpArg->rangeMax = (i + 1) * movesPerThread;
        }
        inpArg->color = game->turn;
        inpArg->alpha = -1000000;
        inpArg->beta = 1000000;
        inpArg->depth = DEPTH;
        inpArg->startValue = eval(game, game->turn, DEPTH);
        inpArg->extensions = 0;
        pthread_create(&threadIDs[i], NULL, partialMaxMove, (void *)inpArg);
        inpArgs[i] = inpArg;
    }

    ValuedMove topMoves[THREADS];
    for(int i = 0; i < THREADS; i++){
        pthread_join(threadIDs[i], &(outs[i]));
        topMoves[i] = *((ValuedMove *)outs[i]);
    }

    ValuedMove best;
    best.value = -1000000;
    for(int i = 0; i < THREADS; i++){
        if(topMoves[i].value > best.value){
            best.move = topMoves[i].move;
            best.value = topMoves[i].value;
        }
    }

    for(int i = 0; i < THREADS; i++){
        free(inpArgs[i]);
        free(outs[i]);
    }

    return best.move;
}

#endif