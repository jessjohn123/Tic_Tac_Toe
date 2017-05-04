// Win32Project1.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <string>
#include <list>


static const int nk = 100;

// declaring the possible states
static enum { START, PLAYING, QUIT, O_WIN, X_WIN, DRAW } state;
typedef struct
{

	char symbol;
	int move;
	bool selected;
	bool win;
}
Player;
//Defining prototypes
static Player player1, player2, current_player;
static std::string game_type;
static std::string prev_game_type;
static char board[9] = { 0 };
static char m_Symbol;
static int m_Move;
void DisplayBoard();
void GetMove();
void DisplayResult();
void SelectGameType();
void GetPlayerSymbol();
void FindWinner();
void UpdateBoard();
void UpdateScreen();
void VerifyMove();
void ResetState();
void ResetBoard();
void InitialisePlayerMove();
void DisplayGameProgress();
void UpdateGame();
bool WrongSymbol();
bool WrongSelection();
bool GameOver();
bool FreeSquare();
void GenerateMoves(char _board[9], std::list<int> &move_list);
void CheckGameState(char board[9]);
int EvaluatePosition(char _board[9], Player _player);
int MiniMax(char _board[9], Player _player);
int Min_Move(char _board[9], Player _player);
int MaxMove(char _board[9], Player _player);


int main()
{
	SelectGameType();

	if (state != QUIT)
	{

		GetPlayerSymbol();

		while (state != QUIT)
		{
			while (state == PLAYING)
			{
				InitialisePlayerMove();
				GetMove();
				UpdateGame();
			}

			if (GameOver())
			{
				FindWinner();
				DisplayResult();

				ResetState();
				ResetBoard();

			}

			SelectGameType();
		}
	}
	return 0;
}

// Menu
void SelectGameType()
{
	std::cout << "************** TIC TAC TOE **************\n";
	std::cout << "DEVELOPED by JESSICA J JOHN\n";
	std::cout << "Date: 4th May 2017\n";
	std::cout << "Website: https://jessicajohn95.wordpress.com/ \n";
	std::cout << "Email: jessicajosejohn94@gmail.com\n";
	std::cout << "AI Procedure: MinMax Programmed in C++\n";
	std::cout << "****************************************\n\n";
	std::cout << "Let's start the game....\n\n";
	std::cout << "   1 - Play Against the Computer" << std::endl;

	std::cout << "   2 - Quit the Program" << std::endl;
	std::cout << "Enter Selection " << std::endl;
	int choice;
	std::cin >> choice;
	if (!std::cin.good())
	{
		std::cout << "Please notice that you can only enter integers for the selection" << std::endl;
		UpdateScreen();
	}
	switch (choice)
	{
	case 1:
		game_type = "h";
		break;

	case 2:
		state = QUIT;
		break;
	default:
		std::cout << "Invalid Selection." << std::endl;
		UpdateScreen();
	}
	if (choice > 0 && choice < 3)
	{
		if (prev_game_type != "" && game_type != prev_game_type)
		{
			GetPlayerSymbol();
		}
		if (game_type.length() > 0)
		{
			prev_game_type = game_type;
		}
	}
}



// Symbol chosen by the current player
void GetPlayerSymbol()
{
	if (game_type == "h")
	{
		int selection = rand() % 2;

		if (selection == 0)
		{
			rand() % 2 == 0 ? player2.symbol = 'X' : player2.symbol = 'O';
			m_Symbol = player2.symbol;
			player2.selected = 1;
			std::cout << "Computer Will Play " << player2.symbol << std::endl;
		}
		else if (selection == 1)
		{
			std::cout << " Please Enter Your Symbol (X,O): ";
			std::cin >> player1.symbol;
			player1.symbol = toupper(player1.symbol);
			m_Symbol = player1.symbol;
			player1.selected = 1;
		}
	}


	if (!std::cin.good() || WrongSymbol())
	{
		std::cout << "Please Enter 'X' or 'O' " << std::endl;
		system("pause");
		GetPlayerSymbol();
	}
	if (!player2.selected)
	{
		player1.symbol == 'X' ? player2.symbol = 'O' : player2.symbol = 'X';
		player1.symbol == 'O' ? player2.symbol = 'X' : player2.symbol = 'O';
	}
	else if (!player1.selected)
	{
		player2.symbol == 'X' ? player1.symbol = 'O' : player1.symbol = 'X';
		player2.symbol == 'O' ? player1.symbol = 'X' : player1.symbol = 'O';
	}
	state = PLAYING;
}

// Gets move for the current turn
void GetMove()
{
	std::cin.sync();
	if (game_type == "h")
	{
		if (player1.selected)
		{
			std::cout << "\n" << " Please Enter Your Move (1-9): ";
			std::cin >> player1.move;
			if (!std::cin.good())
			{
				std::cin.clear();
				std::cin.sync();
			}
			m_Move = player1.move;
			m_Symbol = player1.symbol;
			current_player = player1;
			player1.selected = 0;
			player2.selected = 1;

		}
		else if (player2.selected)
		{
			player2.move = MiniMax(board, player2);
			m_Move = player2.move;
			m_Symbol = player2.symbol;
			current_player = player2;
			player1.selected = 1;
			player2.selected = 0;
			ResetState();
		}
	}
	VerifyMove();
	if (GameOver())
	{
		return;
	}
}


// Checks for the winner 
void FindWinner()
{
	if (state == X_WIN && player1.symbol == 'X')
	{
		player1.win = 1;
	}
	else if (state == O_WIN && player1.symbol == 'O')
	{
		player1.win = 1;
	}
	else if (state == X_WIN && player2.symbol == 'X')
	{
		player2.win = 1;
	}
	else if (state == O_WIN && player2.symbol == 'O')
	{
		player2.win = 1;
	}
}


// Verifies whether symbol entered is valid or not
bool WrongSymbol()
{
	return (m_Symbol != 'X' && m_Symbol != 'O');
}

// Checks for wrong move 
bool WrongSelection()
{
	return !(m_Move > 0 && m_Move < 10);
}

// Reinitializes player moves
void InitialisePlayerMove()
{
	player1.move = -1;
	player2.move = -1;
}

// Checks for the end of the game
bool GameOver()
{
	return (state == X_WIN || state == O_WIN || state == DRAW);
}

// Resets the state of the game
void ResetState()
{
	state = PLAYING;
}

// Clears the board
void ResetBoard()
{
	for (int i = 0; i < 9; i++)
	{
		board[i] = 0;
	}
}

// Updates currrent board position
void UpdateBoard()
{
	if (state == PLAYING)
	{
		if (player1.move != -1 && player2.move == -1)
		{
			board[player1.move - 1] = player1.symbol;
		}
		else if (player2.move != -1)
		{
			board[player2.move - 1] = player2.symbol;
		}
	}
}


// Displays outcome on to the screen 
void DisplayResult()
{
	if (player1.win)
	{
		std::cout << "You Have Won The Game, Toddles!" << std::endl;
	}
	else if (player2.win)
	{
		std::cout << "Computer Has Won The Game!" << std::endl;
	}
	else if (player1.win == 0 && player2.win == 0)
	{
		std::cout << "The Game Was a Draw." << std::endl;
	}
	system("pause");
	system("cls");
}

// Makes updates
void UpdateGame()
{
	UpdateBoard();
	DisplayGameProgress();
	CheckGameState(board);
}

// Checks if the square selected is empty
bool FreeSquare()
{
	if (player1.move != -1 && player2.move == -1)
	{
		return board[player1.move - 1] == 0;
	}
	else if (player2.move != -1)
	{
		return board[player2.move - 1] == 0;
	}
	return 0;
}

// Displays the tic tac toe board
void DisplayBoard()
{
	std::cout << std::endl;
	std::cout << " " << board[0] << " | " << board[1] << " | " << board[2] << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " << board[3] << " | " << board[4] << " | " << board[5] << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " << board[6] << " | " << board[7] << " | " << board[8] << std::endl;
	std::cout << std::endl;
	std::cin.sync();
}

// Displays the progress of the game
void DisplayGameProgress()
{
	system("cls");
	DisplayBoard();
}

// Verifies validity of the current move n if its not valid it will ask to provide a valid move again
void VerifyMove()
{
	if (WrongSelection() || !FreeSquare())
	{
		std::cout << "Invalid Move." << std::endl;
		if (player2.move == -1)
		{
			player1.selected = 1;
			player2.selected = 0;
		}
		else
		{
			player1.selected = 0;
			player2.selected = 1;
		}
		system("pause");
		if (game_type == "h")
		{
			player1.selected = 1;
			player2.selected = 0;
		}
		GetMove();
	}
}



// Refreshes the game screen
void UpdateScreen()
{
	system("pause");
	system("cls");
	std::cin.clear();
	std::cin.sync();
	SelectGameType();
}

// Determines if the current board position is a win or draw
void CheckGameState(char board[9])
{
	if ((board[0] == m_Symbol && board[1] == m_Symbol && board[2] == m_Symbol) ||
		(board[3] == m_Symbol && board[4] == m_Symbol && board[5] == m_Symbol) ||
		(board[6] == m_Symbol && board[7] == m_Symbol && board[8] == m_Symbol) ||
		(board[0] == m_Symbol && board[3] == m_Symbol && board[6] == m_Symbol) ||
		(board[1] == m_Symbol && board[4] == m_Symbol && board[7] == m_Symbol) ||
		(board[2] == m_Symbol && board[5] == m_Symbol && board[8] == m_Symbol) ||
		(board[0] == m_Symbol && board[4] == m_Symbol && board[8] == m_Symbol) ||
		(board[2] == m_Symbol && board[4] == m_Symbol && board[6] == m_Symbol))
	{
		if (m_Symbol == 'X')
		{
			state = X_WIN;
		}
		else if (m_Symbol == 'O')
		{
			state = O_WIN;
		}
	}
	else
	{
		state = DRAW;
		for (int i = 0; i < 9; ++i)
		{
			if (board[i] == 0) {
				state = PLAYING;
				break;
			}
		}
	}
}

// Generates all the possible moves
void GenerateMoves(char _board[9], std::list<int> &move_list)
{
	for (int i = 0; i < 9; ++i)
	{
		if (_board[i] == 0)
		{
			move_list.push_back(i);
		}
	}
}

// Evaluates the current board position
int EvaluatePosition(char _board[9], Player _player)
{
	CheckGameState(_board);
	if (GameOver())
	{
		if ((state == X_WIN && _player.symbol == 'X') ||
			(state == O_WIN && _player.symbol == 'O'))
		{
			return +nk;
		}
		else if ((state == X_WIN && _player.symbol == 'O') ||
			(state == O_WIN && _player.symbol == 'X'))
		{
			return -nk;
		}
		else if (state == DRAW)
		{
			return 0;
		}
	}
	return -1;
}

// Returns best move for the current computer player
int MiniMax(char _board[9], Player _player)
{
	int best_val = -nk, index = 0;
	std::list<int> move_list;
	char best_moves[9] = { 0 };
	GenerateMoves(_board, move_list);
	while (!move_list.empty())
	{
		_board[move_list.front()] = _player.symbol;
		m_Symbol = _player.symbol;
		int val = Min_Move(_board, _player);
		if (val > best_val)
		{
			best_val = val;
			index = 0;
			best_moves[index] = move_list.front() + 1;
		}
		else if (val == best_val)
		{
			best_moves[++index] = move_list.front() + 1;
		}
		_board[move_list.front()] = 0;
		move_list.pop_front();
	}
	if (index > 0)
	{
		index = rand() % index;
	}
	return best_moves[index];
}

// Finds best move for min player
int Min_Move(char _board[9], Player _player)
{
	int pos_value = EvaluatePosition(_board, _player);
	if (pos_value != -1)
	{
		return pos_value;
	}
	int best_val = +nk;
	std::list<int> move_list;
	GenerateMoves(_board, move_list);
	while (!move_list.empty())
	{
		_player.symbol == 'X' ? m_Symbol = 'O' : m_Symbol = 'X';
		_board[move_list.front()] = m_Symbol;
		int val = MaxMove(_board, _player);
		if (val < best_val)
		{
			best_val = val;
		}
		_board[move_list.front()] = 0;
		move_list.pop_front();
	}
	return best_val;
}

// Finds best move for max player
int MaxMove(char _board[9], Player _player)
{
	int pos_value = EvaluatePosition(_board, _player);
	if (pos_value != -1)
	{
		return pos_value;
	}
	int best_val = -nk;
	std::list<int> move_list;
	GenerateMoves(_board, move_list);
	while (!move_list.empty())
	{
		_player.symbol == 'X' ? m_Symbol = 'X' : m_Symbol = 'O';
		_board[move_list.front()] = m_Symbol;
		int val = Min_Move(_board, _player);
		if (val > best_val)
		{
			best_val = val;
		}
		_board[move_list.front()] = 0;
		move_list.pop_front();
	}
	return best_val;
}

