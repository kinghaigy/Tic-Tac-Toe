#include "TicTacToeAI.h"
//#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int UtilityValue(int b[], int depth)
{
	/*this function takes in a game and returns a utility value for 
	how good the result of the game is*/

	int lines[8];
	//find the sum of each three horizontal lines
	lines[0] = b[0] + b[1] + b[2];
	lines[1] = b[3] + b[4] + b[5];
	lines[2] = b[6] + b[7] + b[8];

	//find the sum for each three vertical lines
	lines[3] = b[0] + b[3] + b[6];
	lines[4] = b[1] + b[4] + b[7];
	lines[5] = b[2] + b[5] + b[8];

	//find the sum for both diagonal lines
	lines[6] = b[0] + b[4] + b[8];
	lines[7] = b[2] + b[4] + b[6];

	//initialise the utility value that is to be returned
	int value = 0;

	//for each of the eight lines in the board calculate the score and then add it to the utility value
	for(int i = 0; i<8; i++)
	{
		if(lines[i] > 0)
		{
			value +=  (int)pow(10.0, (double)abs(lines[i])  );
		}
		else if(lines[i] < 0)
		{
			value -=  (int)pow(10.0, (double)abs(lines[i])  );
		}
		
	}
	value *= (10-depth);
	return value;
}

void PrintBoard(int b[], int depth)
{
	/*THis funciton prints out the board for tic tac toe.
	max is the character that is used to represent the computers piece.
	min is the character that is used to represent the opponents piece.
	depth is how much the board should be indented from the left.
	depth is usfull for show the depth of a search */
	
	//create an empty board for characters
	char cb[9];

	//convert -1 0 1 to there character representation
	for(int i=0; i<9; i++)
	{
		cb[i] = '-';
		if(b[i] == 1)
			cb[i] = 'X';
		if(b[i] == -1)
			cb[i] = 'O';
	}

	//for each depth indent the tic tac toe board by two
	int n = depth*3;
	char s[29] = "                            ";
	s[n] = '\0';


	printf("%s%c%c%c\n",s,cb[0],cb[1],cb[2]);
	printf("%s%c%c%c\n",s,cb[3],cb[4],cb[5]);
	printf("%s%c%c%c\n\n",s,cb[6],cb[7],cb[8]);
	
	
	
}

int BestNextMove(int ab[],int turn)
{
	/*THis function uses a min max recursive search algorithm to 
	find the best move the computer should make. It returns a number 
	from 0..8 indicating the best position to place a peice 
	
	1 = computers piece
	-1 = opponents piece
	0 empty 
	
	Eg 
	int board[] = {1, 0,1, -1 ,0 -1, 0, 0 ,0};
	int pos = BestNextMove{board,1}
	

	*/

	if(turn != 1 && turn != -1)
	{
		return -1;
	}

	//copy the actual game board and do some idiot checking
	int b[9];
	for(int i = 0; i < 9; i++)
	{
		if(ab[i] == 1)
		{
			b[i] = turn;
		}
		else if(ab[i] == -1)
		{
			b[i] = -turn;
		}
		else if(ab[i] == 0)
		{
			b[i] = 0;
		}
		else
		{
			return -1;
		}
	}

	//print the board as it currently is
	//printf("\n This is the board before any searching\n"); //DEBUG
	//PrintBoard(b,0); //DEBUG

	//place max's piece in every empty location and see how well it scores


	//array of the best positions that can be randomly chosen later
	int pos[9] = {0,2,4,6,8};
	//number of best positions that can be randomly chosen later
	int np = 5; 

	//if there are any pieces already on then board then do a full search
	if(b[0] || b[1] || b[2] || b[3] || b[4] || b[5] || b[6] || b[7] || b[8])
	{
		//curent score
		int cs; 
		//best score 
		int bs;
		bool firstBranch = true;

		for(int i = 0; i < 9; i++)
		{
			if(b[i] == 0)
			{
				//and a piece to the current position
				b[i] = 1;
				//seacrh this branch to see how well this piece scores
				cs = SearchBranch(b,-1,1);
				//printf("The best score is: %i\n",cs); //DEBUG
				//PrintBoard(b,0); //DEBUG

				//set the current position back to empty
				b[i] = 0;

				//if its the first branch of the tree then start the best score as the current score
				if(firstBranch)
				{
					firstBranch = false;
					bs = cs;
					pos[0] = i;
					np = 1;
				}else
					/*if the current score is equal to the best score then add this current 
					positoin to the array of best positions and increment the number of 
					best positoins */
					if(cs == bs)
					{
						pos[np] = i;
						np++;
					}else 
						/*if the current score is greater then the best score then set the best 
						score as the current score and empty the array of best positions*/
						if(cs > bs)
						{
							bs = cs;
							pos[0] = i;
							np = 1;
						}
			}
		}
	}

	//start the random sequence with time as a seed
	srand ( (unsigned int) time(NULL) );

	//random number between 0 and np-1
	int i = rand() % np;
	//return the best position between 0..8
	return pos[i];
}

int SearchBranch(int b[], int turn, int depth)
{
	//check if there is a win on this board. if so then find the utility value and return

	if(Terminal(b))
	{
		
		int u = UtilityValue(b,depth);
		//printf("Utility value: %i\n",u); //DEBUG
		//PrintBoard(b,depth); //DEBUG
		return u;
	}
	
	//maximum score return from branches
	int maxs;
	//minimum score return from branches
	int mins;
	//curent score
	int cs;

	bool firstBranch = true;


	for(int i = 0; i < 9; i++)
	{
		if(b[i] == 0)
		{
			//add a piece to the current position
			b[i] = turn;
			//seacrh this branch to see how well this piece scores
			cs = SearchBranch(b,-turn,depth+1);
			
			//set the current position back to empty
			b[i] = 0;

			if(firstBranch)
			{
				firstBranch = false;
				maxs = cs;
				mins = cs;	
			}else{
				if(cs > maxs)
					maxs = cs;
				if(cs < mins)
					mins = cs;
			}
		}
	}
	if(turn == 1)
		return maxs;
	else
		return mins;
}

bool Terminal(int b[])
{
	/*This function checks to see whether the board is in a winning or tie state */
	int lines[8];
	//find the sum of each three horizontal lines
	lines[0] = b[0] + b[1] + b[2];
	lines[1] = b[3] + b[4] + b[5];
	lines[2] = b[6] + b[7] + b[8];

	//find the sum for each three vertical lines
	lines[3] = b[0] + b[3] + b[6];
	lines[4] = b[1] + b[4] + b[7];
	lines[5] = b[2] + b[5] + b[8];

	//find the sum for both diagonal lines
	lines[6] = b[0] + b[4] + b[8];
	lines[7] = b[2] + b[4] + b[6];

	//Check if there is a row of 3
	for(int i = 0; i < 8; i++)
	{
		if(abs(lines[i]) == 3)
		{
			return true;
		}
	}
	

	//check for tie
	if(b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6] && b[7] && b[8])
	{
		return true;
	}

	
	return false;
}

