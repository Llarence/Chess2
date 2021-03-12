#include "game.c"

void clear_game(Game *game){
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            game->board[x][y].type = NONE;
        }
    }

    game->turn = WHITE;

    game->white_can_castle_kingside = FALSE;
    game->white_can_castle_queenside = FALSE;

    game->black_can_castle_kingside = FALSE;
    game->black_can_castle_queenside = FALSE;

    game->can_en_passent = FALSE;

    game->half_move_clock = 0;

    game->moves = 0;
}

void read_fen(Game *game, const char *fen){
    clear_game(game);

    int board_pos = 0;
    int spaces = 0;
    char curr_char;
    char next_char;

    for(int i = 0; fen[i] == '\0'; i++){
        curr_char = fen[i];
        next_char = fen[i + 1];

        if(curr_char == ' '){
            spaces += 1;
        }else{
            switch (spaces){
                case 0:
                    switch(curr_char){
                        case 'P':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = PAWN;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;
                        
                        case 'p':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = PAWN;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;
                        
                        case 'N':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = KNIGHT;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;

                        case 'n':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = KNIGHT;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;

                        case 'B':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = BISHOP;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;

                        case 'b':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = BISHOP;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;
                        
                        case 'R':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = ROOK;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;

                        case 'r':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = ROOK;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;
                        
                        case 'Q':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = ROOK;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;

                        case 'q':
                            game->board[board_pos % 8][(63 - board_pos)/ 8].type = ROOK;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;
                        
                        case 'K':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = KING;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = WHITE;
                            board_pos += 1;
                            break;

                        case 'k':
                            game->board[board_pos % 8][(63 - board_pos) / 8].type = KING;
                            game->board[board_pos % 8][(63 - board_pos) / 8].color = BLACK;
                            board_pos += 1;
                            break;
                        
                        case '/':
                            break;
                        
                        default:
                            int num = fen[i] - '0';
                            board_pos += num;
                            break;
                    }

                case 1:
                    if(curr_char == 'w'){
                        game->turn = WHITE;
                    }
                    if(curr_char == 'b'){
                        game->turn = BLACK;
                    }
                    break;

                case 2:
                    if(curr_char == 'K'){
                        game->white_can_castle_kingside = TRUE;
                    }
                    if(curr_char == 'Q'){
                        game->white_can_castle_queenside = TRUE;
                    }
                    if(curr_char == 'k'){
                        game->black_can_castle_kingside = TRUE;
                    }
                    if(curr_char == 'q'){
                        game->black_can_castle_queenside = TRUE;
                    }
                    break;
                
                case 3:
                    if(curr_char != '-'){
                        game->can_en_passent = TRUE;
                        game->en_passent_x = curr_char - 'a';
                        game->en_passent_y = next_char - '0';
                    }
                    break;

                case 4:
                    game->half_move_clock = curr_char - '0';
                    break;
                
                case 5:
                    game->moves = curr_char - '0';
                    break;
            }
        }
    }
}

void init_game(Game *game){
    read_fen(game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}