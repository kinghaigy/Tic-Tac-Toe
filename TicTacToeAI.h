

#ifndef TICTACTOE_AI
#define TICTACTOE_AI
	 int UtilityValue(int b[], int depth);
	 void PrintBoard(int b[], int depth);
	 int BestNextMove(int b[],int turn);
	 int SearchBranch(int b[], int turn, int depth);
	 bool Terminal(int b[]);
#endif