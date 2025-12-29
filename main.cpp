#include "checkers.h"
#include <iostream>
using namespace std;

class Tester {

public:

	//Decision matrix for black player's turn
	void blackPlay(Checkers& checkers) {
		int row = 0;
		int col = 0;
		bool disk = false;
		while (!disk) {
			cout << "\nEnter coordinates of a BLACK piece to play: " << endl;
			cout << "Enter the Row of the piece to play (0-7): ";
			cin >> row;
			cin.ignore(256, '\n');
			cout << "Enter the Column of the piece to play (0-7): ";
			cin >> col;
			cin.ignore(256, '\n');
			if (checkers.m_game[row][col] != BDISK && checkers.m_game[row][col] != BKING) {
				cout << "Black disk not found, please try again!" << endl;
				cin.clear();
			}
			else {
				//Destination row and column to move to
				int dRow = 0;
				int dCol = 0;

				//tracks if a capture was made
				int currentWhite = checkers.getNumWhiteDisks();

				if(checkers.m_game[row][col] == BDISK)
					cout << "\nBlack Disk Selected at: [" << row << "][" << col << "]." << endl;
				else
					cout << "\nBlack King Selected at: [" << row << "][" << col << "]." << endl;
				cout << "Enter the Row to move the piece to (0-7): ";
				cin >> dRow;
				cin.ignore(256, '\n');
				cout << " Enter the Column to move the piece to (0-7): ";
				cin >> dCol;
				cin.ignore(256, '\n');
				if (!checkers.play(pair<int, int>(row, col), pair<int, int>(dRow, dCol))) {
					cout << "Invalid move, please try again!" << endl;
				}
				else {
					cout << "Move made successfully." << endl;
					//Was a capture made? Can a chain capture be made?
					if (currentWhite != checkers.getNumWhiteDisks()) {
						while (availableBlackCapture(checkers, dRow, dCol)) {
							cout << "Current State of the game: " << endl;
							checkers.dumpGame();
							cin.clear();
							row = dRow;
							col = dCol;

							bool chainSuccess = false;
							while (!chainSuccess) {
								cout << "Chain capture available..." << endl;
								cout << "Enter the Row to move the piece to (0-7): ";
								cin >> dRow;
								cin.ignore(256, '\n');
								cout << " Enter the Column to move the piece to (0-7): ";
								cin >> dCol;
								cin.ignore(256, '\n');
								chainSuccess = checkers.play(pair<int, int>(row, col), pair<int, int>(dRow, dCol));
								if (!chainSuccess) {
									cout << "Invalid move, please try again!" << endl;
								}
							}
							cout << "Successful chain capture." << endl;
							currentWhite = checkers.getNumWhiteDisks();
							
						}
					}
					disk = true;
				}
			}
		}
		checkers.m_currPlayer = WPLAYER;
	}

	//Decision matrix for white player's turn
	void whitePlay(Checkers& checkers) {
		int row = 0;
		int col = 0;
		bool disk = false;
		while (!disk) {
			cout << "\nEnter coordinates of a WHITE piece to play: " << endl;
			cout << "Enter the Row of the piece to play (0-7): ";
			cin >> row;
			cin.ignore(256, '\n');
			cout << "Enter the Column of the piece to play (0-7): ";
			cin >> col;
			cin.ignore(256, '\n');
			if (checkers.m_game[row][col] != WDISK && checkers.m_game[row][col] != WKING) {
				cout << "White disk not found, please try again!" << endl;
				cin.clear();
			}
			else {
				//Destination row and column to move to
				int dRow = 0;
				int dCol = 0;

				//tracks if a capture was made
				int currentBlack = checkers.getNumBlackDisks();

				if (checkers.m_game[row][col] == WDISK)
					cout << "\nWhite Disk Selected at: [" << row << "][" << col << "]." << endl;
				else
					cout << "\nWhite King Selected at: [" << row << "][" << col << "]." << endl;
				cout << "Enter the Row to move the piece to (0-7): ";
				cin >> dRow;
				cin.ignore(256, '\n');
				cout << " Enter the Column to move the piece to (0-7): ";
				cin >> dCol;
				cin.ignore(256, '\n');
				if (!checkers.play(pair<int, int>(row, col), pair<int, int>(dRow, dCol))) {
					cout << "Invalid move, please try again!" << endl;
				}
				else {
					cout << "Move made successfully." << endl;
					//Was a capture made? Can a chain capture be made?
					if (currentBlack != checkers.getNumBlackDisks()) {
						while (availableWhiteCapture(checkers, dRow, dCol)) {
							cout << "Current State of the game: " << endl;
							checkers.dumpGame();
							cin.clear();
							row = dRow;
							col = dCol;

							bool chainSuccess = false;
							while (!chainSuccess) {
								cout << "Chain capture available..." << endl;
								cout << "Enter the Row to move the piece to (0-7): ";
								cin >> dRow;
								cin.ignore(256, '\n');
								cout << " Enter the Column to move the piece to (0-7): ";
								cin >> dCol;
								cin.ignore(256, '\n');
								chainSuccess = checkers.play(pair<int, int>(row, col), pair<int, int>(dRow, dCol));
								if (!chainSuccess) {
									cout << "Invalid move, please try again!" << endl;
								}
							}
							cout << "Successful chain capture." << endl;
							currentBlack = checkers.getNumBlackDisks();

						}
					}
					disk = true;
				}
			}
		}
		checkers.m_currPlayer = BPLAYER;
	}

	//Gives whether a chain capture is available or not for black player
	bool availableBlackCapture(Checkers& checkers, int row, int col) {
		//BDISK capture check
		if (checkers.m_game[row][col] == BDISK) {

			if (inBounds(checkers, row - 2, col + 2) &&
				(checkers.m_game[row - 1][col + 1] == WDISK ||
				checkers.m_game[row - 1][col + 1] == WKING) &&
				checkers.m_game[row - 2][col + 2] == BCELL)
				return true;

			if (inBounds(checkers, row - 2, col - 2) &&
				(checkers.m_game[row - 1][col - 1] == WDISK ||
				checkers.m_game[row - 1][col - 1] == WKING) &&
				checkers.m_game[row - 2][col - 2] == BCELL)
				return true;

			return false;

		}
		//BKING capture check
		if (checkers.m_game[row][col] == BKING) {

			int dRow[4] = { -2, -2, 2, 2 };
			int dCol[4] = { -2, 2, -2, 2 };

			for (int i = 0; i < 4; i++) {
				int r2 = row + dRow[i];
				int c2 = col + dCol[i];
				int r1 = row + dRow[i] / 2;
				int c1 = col + dCol[i] / 2;

				if (inBounds(checkers, r2, c2) &&
					(checkers.m_game[r1][c1] == WDISK ||
					checkers.m_game[r1][c1] == WKING) &&
					checkers.m_game[r2][c2] == BCELL)
					return true;
			}

			return false;
		}

		return false;
	}

	//Gives whether a chain capture is available for not for white player
	bool availableWhiteCapture(Checkers& checkers, int row, int col) {
		//WDISK capture check
		if (checkers.m_game[row][col] == WDISK) {

			if (inBounds(checkers, row - 2, col + 2) &&
				(checkers.m_game[row - 1][col + 1] == BDISK ||
				checkers.m_game[row - 1][col + 1] == BKING) &&
				checkers.m_game[row - 2][col + 2] == BCELL)
				return true;

			if (inBounds(checkers, row - 2, col - 2) &&
				(checkers.m_game[row - 1][col - 1] == BDISK ||
				checkers.m_game[row - 1][col - 1] == BKING) &&
				checkers.m_game[row - 2][col - 2] == BCELL)
				return true;

			return false;

		}
		//WKING capture check
		if (checkers.m_game[row][col] == WKING) {

			int dRow[4] = { -2, -2, 2, 2 };
			int dCol[4] = { -2, 2, -2, 2 };

			for (int i = 0; i < 4; i++) {
				int r2 = row + dRow[i];
				int c2 = col + dCol[i];
				int r1 = row + dRow[i] / 2;
				int c1 = col + dCol[i] / 2;

				if (inBounds(checkers, r2, c2) &&
					(checkers.m_game[r1][c1] == BDISK ||
					checkers.m_game[r1][c1] == BKING) &&
					checkers.m_game[r2][c2] == BCELL)
					return true;
			}

			return false;
		}

		return false;
	}

	bool inBounds(Checkers& checkers, int row, int col) {
		return row >= 0 && row < checkers.m_numRows && col >= 0 && col < checkers.m_numColumns;
	}
};

int main(){

	Tester tester;

	//Setting up the board
	Checkers checkers(DEFBOARDSIZE, DEFBOARDSIZE);
	checkers.initBoard();
	checkers.initGame();
	
	cout << "=====Let the games begin!=====\n" << endl;
	bool inProgress = true;
	string currPlayer = "BLACK"; //Black starts the game

	while (inProgress) {
		//Perform the player's turn
		cout << "It is currently " << currPlayer << "'s turn.\n" << endl;

		//Give current game state
		cout << "Current State of the Game:" << endl;
		checkers.dumpGame();

		if (currPlayer == "BLACK")
			tester.blackPlay(checkers);
		else
			tester.whitePlay(checkers);

		//Update who's turn it is
		if (currPlayer == "BLACK")
			currPlayer = "WHITE";
		else
			currPlayer = "BLACK";

		//Check if the game has ended
		if (checkers.getNumWhiteDisks() == 0) {
			cout << "=====The game is over! Congratulations on the victory, Black!=====" << endl;
			inProgress = false;
		}
		else if (checkers.getNumBlackDisks() == 0) {
			cout << "=====The game is over! Congratulations on the victory, White!=====" << endl;
			inProgress = false;
		}
	}
	return 0;
}