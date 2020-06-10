/*	Partner(s) Name & E-mail: Kyle Tran: ktran067@ucr.edu
 *	Lab Section: 022
 *	Assignment: Custom Project Tic-Tac-Toe vs A.I.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <stdio.h>
#include <timer.h>
#include <scheduler.h>
#include <keypad.h>

unsigned char board[3][3] =			{{0,0,0},
									 {0,0,0},
									 {0,0,0}};
unsigned char board_clear[3][3] = {{0,0,0},
								   {0,0,0},
								   {0,0,0}};
unsigned char board_RS1[3][3] = {{0,0,3},
								   {0,0,0},
								   {0,0,0}};
unsigned char board_RS2[3][3] = {{0,0,0},
								 {0,3,0},
								{0,0,0}};
unsigned char board_RS3[3][3] = {{3,0,0},
{0,0,0},
{0,0,0}};
unsigned char board_RS4[3][3] = {{0,0,0},
{3,0,0},
{0,0,0}};
unsigned char board_RS5[3][3] = {{0,0,0},
{0,0,0},
{0,3,0}};
unsigned char board_RS6[3][3] = {{0,0,0},
{0,0,0},
{3,0,0}};									   
									   
unsigned char Screen[8][8] ={{0,0,1,0,0,1,0,0},
							 {0,0,1,0,0,1,0,0},
							 {1,1,1,1,1,1,1,1},
							 {0,0,1,0,0,1,0,0},
							 {0,0,1,0,0,1,0,0},
							 {1,1,1,1,1,1,1,1},
							 {0,0,1,0,0,1,0,0},
							 {0,0,1,0,0,1,0,0}}; //0 = none | 1 = green | 2 = blue | 3 = red

unsigned char Clear[8][8] ={{0,0,1,0,0,1,0,0},
							{0,0,1,0,0,1,0,0},
							{1,1,1,1,1,1,1,1},
							{0,0,1,0,0,1,0,0},
							{0,0,1,0,0,1,0,0},
							{1,1,1,1,1,1,1,1},
							{0,0,1,0,0,1,0,0},
							{0,0,1,0,0,1,0,0}};
unsigned char Screen_RS1[8][8] ={{0,0,1,0,0,1,3,3},
								 {0,0,1,0,0,1,0,0},
								 {1,1,1,1,1,1,1,1},
								 {0,0,1,0,0,1,0,0},
								 {0,0,1,0,0,1,0,0},
								 {1,1,1,1,1,1,1,1},
								 {0,0,1,0,0,1,0,0},
								 {0,0,1,0,0,1,0,0}};
unsigned char Screen_RS2[8][8] ={{0,0,1,0,0,1,0,0},
								 {0,0,1,0,0,1,0,0},
								 {1,1,1,1,1,1,1,1},
								 {0,0,1,3,3,1,0,0},
								 {0,0,1,0,0,1,0,0},
								 {1,1,1,1,1,1,1,1},
								 {0,0,1,0,0,1,0,0},
								 {0,0,1,0,0,1,0,0}};
unsigned char Screen_RS3[8][8] ={{3,3,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0}};
unsigned char Screen_RS4[8][8] ={{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{3,3,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0}};
unsigned char Screen_RS5[8][8] ={{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,3,3,1,0,0},
{0,0,1,0,0,1,0,0}};
unsigned char Screen_RS6[8][8] ={{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0},
{1,1,1,1,1,1,1,1},
{3,3,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0}};

unsigned char player = 2; //o
unsigned char opponent = 3; //x
unsigned char playerMove = 0;
unsigned char gameOver = 0;
struct Move bestMove;

struct Move {
	int row;
	int col;
};

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
unsigned char isMovesLeft(unsigned char board[3][3])
{
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			if (board[i][j]==0) {
				return 1;
			}
		}
	}
	return 0;
}

int evaluate(unsigned char b[3][3])
{
	// Checking for Rows for X or O victory.
	for (int row = 0; row<3; row++)
	{
		if (b[row][0]==b[row][1] &&
		b[row][1]==b[row][2])
		{
			if (b[row][0]==player)
			return +10;
			else if (b[row][0]==opponent)
			return -10;
		}
	}
	
	// Checking for Columns for X or O victory.
	for (int col = 0; col<3; col++)
	{
		if (b[0][col]==b[1][col] &&
		b[1][col]==b[2][col])
		{
			if (b[0][col]==player)
			return +10;
			
			else if (b[0][col]==opponent)
			return -10;
		}
	}
	
	// Checking for Diagonals for X or O victory.
	if (b[0][0]==b[1][1] && b[1][1]==b[2][2])
	{
		if (b[0][0]==player)
		return +10;
		else if (b[0][0]==opponent)
		return -10;
	}
	
	if (b[0][2]==b[1][1] && b[1][1]==b[2][0])
	{
		if (b[0][2]==player)
		return +10;
		else if (b[0][2]==opponent)
		return -10;
	}
	
	// Else if none of them have won then return 0
	return 0;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int minimax(unsigned char board[3][3], int depth, unsigned char isMax)
{
	int score = evaluate(board);
	
	// If Maximizer has won the game return his/her
	// evaluated score
	if (score == 10)
	return score-depth;
	
	// If Minimizer has won the game return his/her
	// evaluated score
	if (score == -10)
	return score-depth;
	
	// If there are no more moves and no winner then
	// it is a tie
	if (isMovesLeft(board)==0)
	return 0;
	
	// If this maximizer's move
	if (isMax == 1)
	{
		int best = -1000;
		
		// Traverse all cells
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				// Check if cell is empty
				if (board[i][j]==0)
				{
					// Make the move
					board[i][j] = player;
					
					// Call minimax recursively and choose
					// the maximum value
					int x = minimax(board, depth+1, 0);
					if (best < x) {
						best = x;
					}
					//best = max( best,
					//minimax(board, depth+1, !isMax) );
					
					// Undo the move
					board[i][j] = 0;
				}
			}
		}
		return best;
	}
	
	// If this minimizer's move
	else
	{
		int best = 1000;
		
		// Traverse all cells
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				// Check if cell is empty
				if (board[i][j]==0)
				{
					// Make the move
					board[i][j] = opponent;
					
					// Call minimax recursively and choose
					// the minimum value
					int z = minimax(board, depth+1, 1);
					if (best > z) {
						best = z;
					}
					//best = min(best,
					//minimax(board, depth+1, !isMax));
					
					// Undo the move
					board[i][j] = 0;
				}
			}
		}
		return best;
	}
}

// This will return the best possible move for the player
struct Move findBestMove(unsigned char board[3][3])
{
	int bestVal = -1000;
	struct Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;
	
	// Traverse all cells, evaluate minimax function for
	// all empty cells. And return the cell with optimal
	// value.
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			// Check if cell is empty
			if (board[i][j]==0)
			{
				// Make the move
				//board[i][j] = opponent;
				board[i][j] = player;
				
				// compute evaluation function for this
				// move.
				int moveVal = minimax(board, 0, 1);
				
				// Undo the move
				board[i][j] = 0;
				
				// If the value of the current move is
				// more than the best value, then update
				// best/
				if (moveVal > bestVal)
				{
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}	
	return bestMove;
}

void checkGameOver() {
	// Checking for Rows for X or O victory.
	for (int row = 0; row<3; row++)
	{
		if (board[row][0]==board[row][1] &&
		board[row][1]==board[row][2])
		{
			if (board[row][0]==2) {
				gameOver = 1;				
			} else if (board[row][0]==3) {
				gameOver = 1;
			}
		}
	}
	
	// Checking for Columns for X or O victory.
	for (int col = 0; col<3; col++)
	{
		if (board[0][col]==board[1][col] &&
		board[1][col]==board[2][col])
		{
			if (board[0][col]== 2) {
				gameOver = 1;				
			} else if (board[0][col]== 3) {
				gameOver = 1;
			}
		}
	}
	
	// Checking for Diagonals for X or O victory.
	if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
	{
		if (board[0][0]==2)
			gameOver = 1;
		else if (board[0][0]==3)
			gameOver = 1;
	}
	
	if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
	{
		if (board[0][2]==3)
			gameOver = 1;
		else if (board[0][2]==2)
			gameOver = 1;
	}
	
	//No victory
}

void send0 () {
	PORTC = (PORTC & 0xF0) | 0x08; //SRCLR high, RCLK low, SRCLK low
	PORTC = (PORTC & 0xF8);// | 0x00; //SET SER to 0
	PORTC = (PORTC & 0xF9) | 0x04; //SET SRCLK high
	PORTC = (PORTC & 0xF8); //SET SRCLK low
	//PORTC = (PORTC & 0xF8) | 0x02; //SET RCLK high
}
void send1() {
	PORTC = (PORTC & 0xF0) | 0x08; //SRCLR high, RCLK low, SRCLK low
	PORTC = (PORTC & 0xF8) | 0x01; //SET SER to 1
	PORTC = (PORTC & 0xF9) | 0x04; //SET SRCLK high
	PORTC = (PORTC & 0xF8); //SET SRCLK low
	//PORTC = (PORTC & 0xF8) | 0x02; //SET RCLK high
}

enum Display_States {D_Start, D_S0, D_S1, D_S2, D_S3, D_S4, D_S5, D_S6, D_S7};

unsigned char k = 0;

int Display_Tick(int state) { //for the grid
	switch (state) {
		case D_Start:
		switch(k) {
			case 0:
			state = D_S0;
			break;
			case 1:
			state = D_S1;
			break;
			case 2:
			state = D_S2;
			break;
			case 3:
			state = D_S3;
			break;
			case 4:
			state = D_S4;
			break;
			case 5:
			state = D_S5;
			break;
			case 6:
			state = D_S6;
			break;
			case 7:
			state = D_S7;
			break;
		}
		break;
		case D_S0:
		state = D_Start;
		break;
		case D_S1:
		state = D_Start;
		break;
		case D_S2:
		state = D_Start;
		break;
		case D_S3:
		state = D_Start;
		break;
		case D_S4:
		state = D_Start;
		break;
		case D_S5:
		state = D_Start;
		break;
		case D_S6:
		state = D_Start;
		break;
		case D_S7:
		state = D_Start;
		break;
		default:
		break;
	}
	switch (state) { //0 = blank | 1 = green | 2 = red | 3 = blue
		case D_Start:
			PORTB = PORTB | 0x1F;
			PORTC = (PORTC | 0x70) & 0xF0;
			break;
		case D_S0:
		PORTA = 0x80;
		k = 1;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[0][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[0][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[0][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		//PORTB = (PORTB | 0x1F);
		//PORTC = PORTC | 0x70;
		break;
		case D_S1:
		PORTA = 0x40;
		k = 2;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[1][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[1][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[1][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		//PORTB = (PORTB | 0x1F);
		//PORTC = PORTC | 0x70;
		break;
		case D_S2:
		PORTA = 0x20;
		k = 3;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[2][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[2][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[2][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		case D_S3:
		PORTA = 0x10;
		k = 4;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[3][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[3][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[3][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		case D_S4:
		PORTA = 0x08;
		k = 5;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[4][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[4][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[4][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		case D_S5:
		PORTA = 0x04;
		k = 6;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[5][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[5][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[5][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		case D_S6:
		PORTA = 0x02;
		k = 7;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[6][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[6][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[6][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		case D_S7:
		PORTA = 0x01;
		k = 0;
		
		for (int i = 0; i < 8; i++) {
			if (Screen[7][i] == 1) {//green
				switch (i) {
					case 0:
					PORTB = PORTB & 0xFE;
					//PORTC = PORTC | 0x70;
					break;
					case 1:
					PORTB = PORTB & 0xFD;
					//PORTC = PORTC | 0x70;
					break;
					case 2:
					PORTB = PORTB & 0xFB;
					//PORTC = PORTC | 0x70;
					break;
					case 3:
					PORTB = PORTB & 0xF7;
					//PORTC = PORTC | 0x70;
					break;
					case 4:
					PORTB = PORTB & 0xEF;
					//PORTC = PORTC | 0x70;
					break;
					case 5:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xEF;
					break;
					case 6:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xDF;
					break;
					case 7:
					//PORTB = PORTB | 0x1F;
					PORTC = PORTC & 0xBF;
					break;
				}
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[7][j] == 2) {
				send0();
				} else {
				send1();
			}
		}
		for (int j = 7; j >= 0; j--) {
			if (Screen[7][j] == 3) {
				send0();
				} else {
				send1();
			}
		}
		PORTC = (PORTC & 0xF8) | 0x02;
		break;
		default:
		break;
	}
	return state;
}

unsigned char keypad_in;

enum key_states {key_start, key_press};
int Keypad_SM (int state) {
	switch(state) {
		case key_start:
		state = key_press;
		break;
		case key_press:
		state = key_press;
		break;
		default:
		state = key_start;
		break;
	}
	switch(state) {
		case key_start:
		break;
		case key_press:
		keypad_in = GetKeypadKey();
		switch(keypad_in) {
			case '\0':	
				break;
			case '1':
				if (board[0][0] == 0 && playerMove == 0) {
					Screen[0][0] = 2;
					Screen[0][1] = 2;
					
					board[0][0] = 2;
					playerMove = 1;
										
					checkGameOver();
				}						
				break;
			case '2':	
				if (board[0][1] == 0 && playerMove == 0) {
					Screen[0][3] = 2;
					Screen[0][4] = 2;
					
					board[0][1] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '3':
				if (board[0][2] == 0 && playerMove == 0) {
					Screen[0][6] = 2;
					Screen[0][7] = 2;
					
					board[0][2] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '4':
				if (board[1][0] == 0 && playerMove == 0) {
					Screen[3][0] = 2;
					Screen[3][1] = 2;
					
					board[1][0] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '5':
				if (board[1][1] == 0 && playerMove == 0) {
					Screen[3][3] = 2;
					Screen[3][4] = 2;
					
					board[1][1] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '6':
				if (board[1][2] == 0 && playerMove == 0) {
					Screen[3][6] = 2;
					Screen[3][7] = 2;
					
					board[1][2] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '7':
				if (board[2][0] == 0 && playerMove == 0) {
					Screen[6][0] = 2;
					Screen[6][1] = 2;
					
					board[2][0] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '8':
				if (board[2][1] == 0 && playerMove == 0) {
					Screen[6][3] = 2;
					Screen[6][4] = 2;
					
					board[2][1] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case '9':
				if (board[2][2] == 0 && playerMove == 0) {
					Screen[6][6] = 2;
					Screen[6][7] = 2;
					
					board[2][2] = 2;
					playerMove = 1;
					checkGameOver();
				}
				break;
			case 'A':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS6[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS6[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			case 'B':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS5[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS5[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			case 'C':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS4[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS4[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			case 'D':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS3[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS3[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			case '*':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_clear[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Clear[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				
				break;
			case '0':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS1[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS1[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			case '#':
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						board[i][j] = board_RS2[i][j];
					}
				}
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Screen[i][j] = Screen_RS2[i][j];
					}
				}
				playerMove = 0;
				gameOver = 0;
				break;
			default:
				break;
		}
		break;
		default:
			break;
	}
	return state;
}
void displayAIMove(int row, int col) {
	switch (row) {
		case 0:
			switch (col) {
				case 0:
					Screen[0][0] = 3;
					Screen[0][1] = 3;
					break;
				case 1:
					Screen[0][3] = 3;
					Screen[0][4] = 3;
					break;
				case 2:
					Screen[0][6] = 3;
					Screen[0][7] = 3;
					break;
			}
			break;
		case 1:
			switch (col) {
				case 0:
					Screen[3][0] = 3;
					Screen[3][1] = 3;
					break;
				case 1:
					Screen[3][3] = 3;
					Screen[3][4] = 3;
					break;
				case 2:
					Screen[3][6] = 3;
					Screen[3][7] = 3;
					break;
			}
			break;
		case 2:
			switch (col) {
				case 0:
					Screen[6][0] = 3;
					Screen[6][1] = 3;
					break;
				case 1:
					Screen[6][3] = 3;
					Screen[6][4] = 3;
					break;
				case 2:
					Screen[6][6] = 3;
					Screen[6][7] = 3;
					break;
			}
			break;
		default: 
			break;
	}
}

enum AI_States {AI_Start, AI_S0, AI_S1, GameOver};
	
int AI_Tick(int state) {
	switch (state) {
		case AI_Start:
			state = AI_S0;
			break;
		case AI_S0:
			if (gameOver == 1) {
				state = gameOver;
			}
			if (playerMove == 1 && gameOver == 0) {
				state = AI_S1;
			} else if (playerMove == 0 && gameOver == 0) {
				state = AI_S0;
			}			
			break;
		case AI_S1:
			state = AI_S0;
			break;
		case GameOver:
			if (gameOver == 1) {
				state = GameOver;
			} else if (gameOver == 0) {
				state = AI_S0;
			}
			break;
		default:
			break;
	}
	switch (state) {
		case AI_Start:
			break;
		case AI_S0:
			break;
		case AI_S1:			
			bestMove.col = -1;
			bestMove.row = -1;
			bestMove = findBestMove(board);
			
			if (board[bestMove.row][bestMove.col] == 0 && bestMove.col < 3 && bestMove.col >= 0 && bestMove.row < 3 && bestMove.col >= 0) {
				board[bestMove.row][bestMove.col] = 3;
			
				displayAIMove(bestMove.row, bestMove.col);
			}
			//bestMove = best();
			//board[bestMove.col][bestMove.row] = 3;
			//displayAIMove(bestMove.col, bestMove.row);
			
			checkGameOver();			
			playerMove = 0;
			
			break;
		case GameOver:
			break;
		default:
			break;
			
	}
	return state;
}

int main(void)
{
	/* Replace with your application code */
	
	DDRA = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x1F;
	DDRC = 0xFF; PORTC = 0x70;
	DDRD = 0xF0; PORTD = 0x0F;
	
	
	const unsigned short numTasks = 3;
	task tasks[3];
	
	tasks[0].state = D_Start;
	tasks[0].period = 1;
	tasks[0].elapsedTime = 0;
	tasks[0].TickFct = &Display_Tick;
	
	tasks[1].state = key_start;
	tasks[1].period = 100;
	tasks[1].elapsedTime = 0;
	tasks[1].TickFct = &Keypad_SM;
	
	tasks[2].state = AI_Start;
	tasks[2].period = 100;
	tasks[2].elapsedTime = 0;
	tasks[2].TickFct = &AI_Tick;
	
	TimerSet(1);
	TimerOn();
	k = 0;
	playerMove = 0;
	gameOver = 0;
	
	while (1)
	{
		unsigned short i;
		for (i = 0; i < numTasks; ++i) {
			if (tasks[i].elapsedTime == tasks[i].period) {
				// Setting next state for task
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				// Reset the elapsed time for next tick.
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}

