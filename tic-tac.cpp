#include <bits/stdc++.h>
typedef long long ll;
typedef long double ld;
# define FAST_CODE ios_base::sync_with_stdio(false); cin.tie(NULL);
# define pb push_back
using namespace std;


//the states of nodes
#define WIN 1000
#define	DRAW 0
#define LOSS -1000



// the elements of the board
constexpr char AI_M = 'O';
constexpr char PLAY_M = 'X';
constexpr char EMPTY_SPACE = '-';

// Print game state
void print_game_state(int state)
{
	if (WIN == state) 
	{ 
		cout << "WIN" << "\n"; 
	}
	else if (DRAW == state) 
	{ 
		cout << "DRAW" << "\n"; 
	}
	else if (LOSS == state) 
	{ 
		cout << "LOSS" << "\n"; 
	}
}

// All possible winning states
vector<vector<pair<int, int>>> winning_states
{
	// Every row
	{ make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },
	{ make_pair(1, 0), make_pair(1, 1), make_pair(1, 2) },
	{ make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },

	// Every column
	{ make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },
	{ make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
	{ make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },

	// Every diagonal
	{ make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },
	{ make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }

};

void print_board(char board[3][3])
{
	cout << "\n";
	cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
	cout << "----------" << "\n";
	cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
	cout << "----------" << "\n";
	cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n" << "\n";
}


vector<pair<int, int>> get_legal_moves(char board[3][3])
{
	vector<pair<int, int>> legal_moves;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != AI_M && board[i][j] != PLAY_M)
			{
				legal_moves.pb(make_pair(i, j));
			}
		}
	}

	return legal_moves;
}


bool position_occupied(char board[3][3], pair<int, int> pos)
{
	vector<pair<int, int>> legal_moves = get_legal_moves(board);

	for (int i = 0; i < legal_moves.size(); i++)
	{
		if (pos.first == legal_moves[i].first && pos.second == legal_moves[i].second)
		{
			return false;
		}
	}

	return true;
}

vector<pair<int, int>> get_occupied_positions(char board[3][3], char marker)
{
	vector<pair<int, int>> occupied_positions;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (marker == board[i][j])
			{
				occupied_positions.pb(make_pair(i, j));
			}
		}
	}

	return occupied_positions;
}

bool board_is_full(char board[3][3])
{
	vector<pair<int, int>> legal_moves = get_legal_moves(board);

	if (0 == legal_moves.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool game_is_won(vector<pair<int, int>> occupied_positions)
{
	bool game_won;

	for (int i = 0; i < winning_states.size(); i++)
	{
		game_won = true;
		vector<pair<int, int>> curr_win_state = winning_states[i];
		for (int j = 0; j < 3; j++)
		{
			if (!(find(begin(occupied_positions), end(occupied_positions), curr_win_state[j]) != end(occupied_positions)))
			{
				game_won = false;
				break;
			}
		}

		if (game_won)
		{
			break;
		}
	}
	return game_won;
}

char get_opponent_marker(char marker)
{
	char opponent_marker;
	if (marker == PLAY_M)
	{
		opponent_marker = AI_M;
	}
	else
	{
		opponent_marker = PLAY_M;
	}

	return opponent_marker;
}

int get_board_state(char board[3][3], char marker)
{

	char opponent_marker = get_opponent_marker(marker);

	vector<pair<int, int>> occupied_positions = get_occupied_positions(board, marker);

	bool is_won = game_is_won(occupied_positions);

	if (is_won)
	{
		return WIN;
	}

	occupied_positions = get_occupied_positions(board, opponent_marker);
	bool is_lost = game_is_won(occupied_positions);

	if (is_lost)
	{
		return LOSS;
	}

	bool is_full = board_is_full(board);
	if (is_full)
	{
		return DRAW;
	}

	return DRAW;

}

// Apply the minimax game optimization algorithm , also added pruning, the he(art) of the project :P
pair<int, pair<int, int>> minimax_optimization(char board[3][3], char marker, int depth, int alpha, int beta)
{
	pair<int, int> best_move = make_pair(-1, -1);
	int best_score = (marker == AI_M) ? LOSS : WIN;

	if (board_is_full(board) || DRAW != get_board_state(board, AI_M))
	{
		best_score = get_board_state(board, AI_M);
		return make_pair(best_score, best_move);
	}

	vector<pair<int, int>> legal_moves = get_legal_moves(board);

	for (int i = 0; i < legal_moves.size(); i++)
	{
		pair<int, int> curr_move = legal_moves[i];
		board[curr_move.first][curr_move.second] = marker;

		if (marker == AI_M)
		{
			int score = minimax_optimization(board, PLAY_M, depth + 1, alpha, beta).first;

			if (best_score < score)
			{
				best_score = score - depth * 10;
				best_move = curr_move;
				alpha = max(alpha, best_score);
				board[curr_move.first][curr_move.second] = EMPTY_SPACE;
				// remember that beta should always be less than alpha :D
				if (beta <= alpha) 
				{ 
					break; 
				}
			}

		}

		else
		{
			int score = minimax_optimization(board, AI_M, depth + 1, alpha, beta).first;

			if (best_score > score)
			{
				best_score = score + depth * 10;
				best_move = curr_move;

				// Check if this branch's best move is worse than the best
				// option of a previously search branch. If it is, skip it, pruning
				beta = min(beta, best_score);
				board[curr_move.first][curr_move.second] = EMPTY_SPACE;
				if (beta <= alpha) 
				{ 
					break; 
				}
			}

		}

		board[curr_move.first][curr_move.second] = EMPTY_SPACE;

	}

	return make_pair(best_score, best_move);
}

// Check if the game is finished
bool game_is_done(char board[3][3])
{
	if (board_is_full(board))
	{
		return true;
	}

	if (DRAW != get_board_state(board, AI_M))
	{
		return true;
	}

	return false;
}


int main()
{
	char board[3][3] = { EMPTY_SPACE };

	cout << "Player = X\t AI Computer = O \t sry :P" << "\n" << "\n";

	print_board(board);

	while (!game_is_done(board))
	{
		int row, col;
		cin >> row;
		cin >> col;
		cout << "\n \n";

		if (position_occupied(board, make_pair(row, col)))
		{
			cout << "invalid move" << "\n";
			continue;
		}
		else
		{
			board[row][col] = PLAY_M;
		}

		pair<int, pair<int, int>> ai_move = minimax_optimization(board, AI_M, 0, LOSS, WIN); // strating with the 0 depth

		board[ai_move.second.first][ai_move.second.second] = AI_M;

		print_board(board);
	}

	cout << "GAME OVER XD" << "\n" << "\n";

	int player_state = get_board_state(board, PLAY_M);

	cout << "PLAYER "; print_game_state(player_state);

	return 0;

}