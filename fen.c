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
            spaces += 1;
        }else{
            switch (spaces){
                case 0:
                    switch(currChar){
                        case 'P':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = PAWN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'N':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KNIGHT;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'B':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = BISHOP;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'R':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = ROOK;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'Q':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = QUEEN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'K':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KING;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = WHITE;
                            boardPos += 1;
                            break;
                        
                        case 'p':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = PAWN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
                            break;

                        case 'n':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KNIGHT;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
                            break;

                        case 'b':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = BISHOP;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
                            break;

                        case 'r':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = ROOK;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
                            break;

                        case 'q':
                            game->board[boardPos % 8][(63 - boardPos)/ 8].type = QUEEN;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
                            break;

                        case 'k':
                            game->board[boardPos % 8][(63 - boardPos) / 8].type = KING;
                            game->board[boardPos % 8][(63 - boardPos) / 8].color = BLACK;
                            boardPos += 1;
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
    readFen(game, "3qk3/8/8/8/8/8/8/4K3 w - 0 1");
}

#endif