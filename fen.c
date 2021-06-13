#ifndef FEN
#define FEN

#include "game.c"

void readFen(Game *game, const char *fen){
    clearGame(game);

    int boardPos = 0;
    int spaces = 0;
    char currChar;
    char nextChar;

    for(int i = 0; fen[i] != '\0'; i++){
        currChar = fen[i];
        nextChar = fen[i + 1];

        if(currChar == ' '){
            spaces++;
        }else{
            switch (spaces){
                case 0:
                    switch(currChar){
                        case 'P':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = PAWN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'N':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KNIGHT;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'B':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = BISHOP;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'R':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = ROOK;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'Q':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = QUEEN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'K':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KING;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos++;
                            break;
                        
                        case 'p':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = PAWN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;

                        case 'n':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KNIGHT;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;

                        case 'b':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = BISHOP;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;

                        case 'r':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = ROOK;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;

                        case 'q':
                            game->board[boardPos % 8][(63 - boardPos)/ 8].type = QUEEN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;

                        case 'k':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KING;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos++;
                            break;
                        
                        case '/':
                            break;
                        
                        default:
                            int num = fen[i] - '0';
                            boardPos += num;
                            break;
                    }

                case 1:
                    if(currChar == 'w'){
                        game->turn = WHITE;
                    }
                    if(currChar == 'b'){
                        game->turn = BLACK;
                    }
                    break;

                case 2:
                    if(currChar == 'K'){
                        game->whiteCanCastleKingside = TRUE;
                    }
                    if(currChar == 'Q'){
                        game->whiteCanCastleQueenside = TRUE;
                    }
                    if(currChar == 'k'){
                        game->blackCanCastleKingside = TRUE;
                    }
                    if(currChar == 'q'){
                        game->blackCanCastleQueenside = TRUE;
                    }
                    break;
                
                case 3:
                    if(currChar != '-'){
                        game->canEnPassent = TRUE;
                        game->enPassentX = currChar - 'a';
                        game->enPassentY = nextChar - '0';
                    }
                    break;

                case 4:
                    game->halfMoveClock = currChar - '0';
                    break;
                
                case 5:
                    game->moves = currChar - '0';
                    break;
            }
        }
    }
}

void initGame(Game *game){
    readFen(game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

#endif