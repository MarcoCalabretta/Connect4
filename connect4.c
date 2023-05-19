#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const BOARD_WIDTH = 7;
const BOARD_HEIGHT = 6;
const PIECE_SIZE = 3;
const CONNECT = 4;

const FULL_BOARD = 1;
const NON_COLUMN = 2;
const SUCCESS = 0;

int main(){
	int **board = malloc(BOARD_HEIGHT * sizeof(int*));
	for(int i = 0; i < BOARD_HEIGHT; i++){
		board[i] = malloc(BOARD_WIDTH * sizeof(int));
		for(int j = 0; j < BOARD_WIDTH; j++){
			board[i][j] = 0;
		}
	}

    int num_players = 0;
	printf("Hi, welcome to connect 4, have fun!\n");
	printf("How many players? ");
    scanf("%d", &num_players);
	assert(num_players >= 1);
	assert(num_players <= 2);

	int player = 1;
	int col = 0;
	int game_state = 0;
	for(int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH && !game_state; i++){
        if(num_players == 1 && player == 2){
            col = 1 + (rand() % 7);
            printf("Computer picked column %d.\n", col);
        } else{
            printf("Choose a column, player %d: ", player);
            scanf("%d", &col);
        }
        if(move(player, col, board) == SUCCESS){
            player = (player == 1) ? 2 : 1;
            game_state = check_win(board);
        }
	}
	if(game_state){
        printf("Congratulations player %d, you win!\n", game_state);
	} else{
        printf("It's a draw. Good game everyone.\n");
    }
}

// all functions require valid pointers
// and requires that board points to a BOARD_HEIGHT X BOARD_WIDTH array (not asserted)


// checks if anyone won the game yet, and returns the winning player if someone won, 0 otherwise
int check_win(int **board){
    assert(board);

    int in_a_row = 1;
    // checks for vertical wins
    for(int j = 0; j < BOARD_WIDTH; j++){
        for(int i = 1; i < BOARD_HEIGHT; i++){
            if(board[i][j] && board[i][j] == board[i - 1][j]){
                in_a_row++;
                if(in_a_row >= CONNECT){
                    return board[i][j];
                }
            } else{
                in_a_row = 1;
            }
        }
        in_a_row = 1;
    }
    // checks for horizontal wins
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 1; j < BOARD_WIDTH; j++){
            if(board[i][j] && board[i][j] == board[i][j - 1]){
                in_a_row++;
            } else{
                in_a_row = 1;
            }
            if(in_a_row >= CONNECT){
                return board[i][j];
            }
        }
        in_a_row = 1;
    }
    // checks for diagonal wins going up and to the right
    for(int vert_offset = 0; vert_offset < BOARD_HEIGHT - 3; vert_offset++){
        for(int i = BOARD_HEIGHT - 2 - vert_offset, j = 1; i >= 0 && j < BOARD_WIDTH; i--, j++){
            if(board[i][j] && board[i][j] == board[i + 1][j - 1]){
                in_a_row++;
                if(in_a_row >= CONNECT){
                    return board[i][j];
                }
            } else{
                in_a_row = 1;
            }
        }
        in_a_row = 1;
    }
    for(int horiz_offset = 0; horiz_offset < BOARD_WIDTH - 3; horiz_offset++){
        for(int i = BOARD_HEIGHT - 2, j = 1 + horiz_offset; i >= 0 && j < BOARD_WIDTH; i--, j++){
            if(board[i][j] && board[i][j] == board[i + 1][j - 1]){
                in_a_row++;
                if(in_a_row >= CONNECT){
                    return board[i][j];
                }
            } else{
                in_a_row = 1;
            }
        }
        in_a_row = 1;
    }
    // checks for diagonal wins going down and to the left
    for(int vert_offset = 0; vert_offset < BOARD_HEIGHT - 3; vert_offset++){
        for(int i = 1 + vert_offset, j = 1; i < BOARD_HEIGHT && j < BOARD_WIDTH; i++, j++){
            if(board[i][j] && board[i][j] == board[i - 1][j - 1]){
                in_a_row++;
                if(in_a_row >= CONNECT){
                    return board[i][j];
                }
            } else{
                in_a_row = 1;
            }
        }
        in_a_row = 1;
    }
    for(int horiz_offset = 0; horiz_offset < BOARD_WIDTH - 3; horiz_offset++){
        for(int i = 1, j = 1 + horiz_offset; i < BOARD_HEIGHT && j < BOARD_WIDTH; i++, j++){
            if(board[i][j] && board[i][j] == board[i - 1][j - 1]){
                in_a_row++;
                if(in_a_row >= CONNECT){
                    return board[i][j];
                }
            } else{
                in_a_row = 1;
            }
        }
        in_a_row = 1;
    }
    return 0;
}


// makes player put a piece in col, then prints updated board
// effects: may mutate board through pointers
//          produces output
// requires: player = 1, 2
//           1 <= row <= BOARD_WIDTH + 1
int move(int player, int col, int **board){
    assert(player >= 1);
    assert(player <= 2);
    assert(board);
    if(col < 1 || col > BOARD_WIDTH + 1){
        printf("ERROR: %d is not a valid column. Try again.\n", col);
        return NON_COLUMN;
    } else if(board[0][col - 1]){
        printf("ERROR: Column %d is full. Try again.\n", col);
        return FULL_BOARD;
    } else{
        for(int i = BOARD_HEIGHT - 1; i >= 0; i--){
            if(board[i][col - 1] == 0){
                board[i][col - 1] = player;
                print_board(board);
                return SUCCESS;
            }
        }
    }

}

// Prints the whole connect4 board
// effects: produces output
void print_board(int **board){
	assert(board);
	for(int row = 0; row < BOARD_HEIGHT; row++){
		for(int i = 0; i < PIECE_SIZE; i++){
		       print_line(board[row]);
	      	}
		for(int j = 0; j < 1 + BOARD_WIDTH * (PIECE_SIZE + 1); j++){
			putchar('-');
		}
		putchar('\n');
	}
}

// prints one character line of the connect4 board
// effects: produces output
// requires: row points to an array with BOARD_WIDTH elements (not asserted)
void print_line(int *row){
	assert(row);
	char v = '|';
    putchar(v);
    for(int col = 0; col < BOARD_WIDTH; col++){
        print_piece_line(row[col]);
        putchar(v);
    }
    putchar('\n');
}

// Prints a connect4 piece of size size, either blank (0), player 1, or player 2
// effects: produces output
// requires: 0 <= player <= 2
void print_piece_line(int player){
	assert(player >= 0);
	assert(player <= 2);
	char c = ' ';
	if(player == 1){
		c = 'x';
	}else if(player == 2){
		c = 'o';
	}
	for(int i = 0; i < PIECE_SIZE; i++){
		putchar(c);
	}
}
