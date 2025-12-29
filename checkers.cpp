// UMBC - CMSC 341 - Fall 2025 - Proj0
#include "checkers.h"
Checkers::Checkers(){
    m_game = nullptr;
    m_numRows = 0;
    m_numColumns = 0;
    m_whiteDisks = 0;
    m_blackDisks = 0;
    m_currPlayer = NONE;
}

Checkers::Checkers(int rows, int columns){
    //checks for invalid board size
    if (rows < 8 || columns < 8) {
        m_game = nullptr;
        m_numRows = 0;
        m_numColumns = 0;
        m_whiteDisks = 0;
        m_blackDisks = 0;
        m_currPlayer = NONE;
    }
    else {
        m_game = new CELL * [rows]; //allocate memory for 1D array of length rows
        for (int i = 0; i < rows; i++) {
            m_game[i] = new CELL[columns]; //allocate memory for 
        }
        m_numRows = rows;
        m_numColumns = columns;
        m_whiteDisks = (columns / 2) * 3;
        m_blackDisks = (columns / 2) * 3;
        m_currPlayer = BPLAYER;
    }

    //checks for rows == columns and vice versa for square board, also ensuring they're even
    if (rows % 2 == 0 && columns % 2 != 0) {
        m_numColumns = rows;
    }
    if (rows % 2 != 0 && columns % 2 == 0) {
        m_numRows = columns;
    }
    

}

Checkers::~Checkers(){
    if (m_game != nullptr) {
        for (int i = 0; i < m_numRows; i++) {
            delete[] m_game[i]; //deallocate all memory for each row
        }
    }
    delete[] m_game; //deallocate original pointer
    m_game = nullptr;
}

void Checkers::clear(){
    //deallocating all memory
    for (int i = 0; i < m_numRows; i++) {
        delete[] m_game[i];
    }
    delete[] m_game;
    m_game = nullptr;
    //reinitializing all member variables to default values
    m_numRows = 0;
    m_numColumns = 0;
    m_whiteDisks = 0;
    m_blackDisks = 0;
    m_currPlayer = NONE;
}

bool Checkers::initBoard(){
    bool success = true;

    if (m_numColumns == 0 || m_numRows == 0) {
        success = false;
    } //return false to represent empty board

    for (int i = 0; i < m_numRows; i++) {
        for (int j = 0; j < m_numColumns; j++) {
            //White cells lie in spots where indexes are both odd or even
            if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) { 
                m_game[i][j] = WCELL;
            }
            //Black cells lie in all other spots
            if ((i % 2 != 0 && j % 2 == 0) || (i % 2 == 0 && j % 2 != 0)) {
                m_game[i][j] = BCELL;
            }
        }
    }
    return success;
}

void Checkers::reportLostDisks(){
    int lostWDisks = ((m_numColumns / 2) * 3) - m_whiteDisks;
    int lostBDisks = ((m_numColumns / 2) * 3) - m_blackDisks;

    cout << "There are " << lostWDisks << " lost white disks." << endl;
    cout << "There are " << lostBDisks << " lost black disks." << endl;
}

int Checkers::getNumBlackDisks() {
    return m_blackDisks;
}

int Checkers::getNumWhiteDisks() {
    return m_whiteDisks;
}

bool Checkers::initGame(){
    bool success = true;

    if (m_numColumns == 0 || m_numRows == 0) {
        success = false;
    } //return false to represent empty board

    int wCounter = m_whiteDisks;
    int bCounter = m_blackDisks;
    int bCellCount = 0; // for the empty rows in the center.

    //tally the number of black cells
    for (int i = 0; i < m_numRows; i++) {
        for (int j = 0; j < m_numColumns; j++) {
            if (m_game[i][j] == BCELL) {
                bCellCount++;
            }
        }
    }

    for (int i = 0; i < m_numRows; i++) {
        for (int j = 0; j < m_numColumns; j++) {
            if (m_game[i][j] == BCELL && wCounter > 0){ //fill first 
                m_game[i][j] = WDISK;
                wCounter--;
                bCellCount--;
            }
            else if (m_game[i][j] == BCELL && bCellCount != bCounter) { //passing over the empty 2 rows
                m_game[i][j] = BCELL;
                bCellCount--;
            }
            else if (m_game[i][j] == BCELL && bCounter > 0 && bCellCount == bCounter) { //fill the rest BCELLS with BDISK
                m_game[i][j] = BDISK;
                bCounter--;
                bCellCount--;
            }
        }
    }
    return success;
}

bool Checkers::play(pair<int,int> origin, pair<int,int> destination){
    //accessing the individual ints for origin and destination for the indexes
    int oRow = origin.first;
    int oCol = origin.second;
    int dRow = destination.first;
    int dCol = destination.second;

    bool validMove = false;
    //If there is no disk in the origin cell, the move cannot happen
    if (m_game[oRow][oCol] == WCELL || m_game[oRow][oCol] == BCELL) {
        validMove = false;
    }
    //Check if a destination index is not valid (out of bounds) 
    if (dRow < 0 || dRow > m_numRows || dCol < 0 || dCol > m_numColumns) {
        validMove = false;
    }
    //If the origin cell contains the opponent's disk, the move is not valid.
    
    if ((m_currPlayer == BPLAYER) && (m_game[oRow][oCol] == WDISK || m_game[oRow][oCol] == WKING)) {
        validMove = false;
    }
    if ((m_currPlayer == WPLAYER) && (m_game[oRow][oCol] == BDISK || m_game[oRow][oCol] == BKING)) {
        validMove = false;
    }
    
    /*
    ALL MOVE CHECKS
    Each move also changes current player to the opposite player if
    move is successful
    */
    
    /*
    BDISK move check - can only move up 1 row, right or left 1 space
    Checks if the disk can move right or left, accounting for if it is
    residing on the edge of the board
    */
    if ((m_currPlayer == BPLAYER) && (m_game[oRow][oCol] == BDISK) &&
        (dRow == oRow-1 && oRow > 0) && 
        ((dCol == oCol + 1 && oCol <= m_numColumns - 1) || (dCol == oCol - 1 && oCol > 0)) 
        && m_game[dRow][dCol] == BCELL) {
        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = BDISK;
        validMove = true;
    }
    /*
    BDISK capture check - moves up 2 rows, right or left 2 spaces, with the space in between origin
    and destination containing a white disk or king
    */
    if ((m_currPlayer == BPLAYER) && (m_game[oRow][oCol] == BDISK) &&
        ((dCol == oCol + 2 && oCol < m_numColumns - 2) || (dCol == oCol - 2 && oCol > 1)) &&
        (dRow == oRow - 2 && oRow > 1)
        && (m_game[oRow - 1][oCol + 1] == WDISK || m_game[oRow - 1][oCol + 1] == WKING ||
            m_game[oRow - 1][oCol - 1] == WDISK || m_game[oRow - 1][oCol - 1] == WKING)
        && m_game[dRow][dCol] == BCELL) {

        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = BDISK;
        //did the disk make a capture moving right?
        if (dCol == oCol+2) {
            m_game[oRow - 1][oCol + 1] = BCELL;
        }
        //else, it captured moving left
        else {
            m_game[oRow - 1][oCol - 1] = BCELL;
        }
        m_whiteDisks--;
        validMove = true;
    }
    /*
    BKING move check - moves up or down 1 row, right or left 1 space
    */
    if ((m_currPlayer == BPLAYER) && (m_game[oRow][oCol] == BKING) &&
        ((dRow == oRow - 1 && oRow > 0) || (dRow == oRow+1 && oRow < m_numRows-1)) && 
        ((dCol == oCol + 1 && oCol < m_numColumns-1) || (dCol == oCol-1 && oCol > 0)) &&
        m_game[dRow][dCol] == BCELL) {
        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = BKING;
        validMove = true;
    }
    /*
    BKING capture check - moves up or down 2 rows, right or left 2 spaces 
    The space between origin and destination should contain a white disk or king
    */
    if ((m_currPlayer == BPLAYER) && (m_game[oRow][oCol] == BKING) &&
        ((dCol == oCol + 2 && oCol < m_numColumns - 2) || (dCol == oCol - 2 && oCol > 1)) &&
        ((dRow == oRow - 2 && oRow > 1) || (dRow == oRow + 2 && oRow < m_numRows - 2))
        && (m_game[oRow - 1][oCol + 1] == WDISK || m_game[oRow - 1][oCol + 1] == WKING ||
            m_game[oRow - 1][oCol - 1] == WDISK || m_game[oRow - 1][oCol - 1] == WKING ||
            m_game[oRow + 1][oCol + 1] == WDISK || m_game[oRow + 1][oCol + 1] == WKING ||
            m_game[oRow + 1][oCol - 1] == WDISK || m_game[oRow + 1][oCol - 1] == WKING)
        && m_game[dRow][dCol] == BCELL) {

        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = BKING;
        //did the disk make a capture moving up/right?
        if (dCol == oCol + 2 && dRow == oRow - 2) {
            m_game[oRow - 1][oCol + 1] = BCELL;
        }
        //did it capture moving up/left?
        else if (dCol == oCol - 2 && dRow == oRow - 2) {
            m_game[oRow - 1][oCol - 1] = BCELL;
        }
        //did it capture moving down/right?
        else if (dCol == oCol + 2 && dRow == oRow + 2) {
            m_game[oRow + 1][oCol + 1] = BCELL;
        }
        //else, it captured moving down/left
        else {
            m_game[oRow + 1][oCol - 1] = BCELL;
        }
        m_whiteDisks--;
        validMove = true;
    }

    //BPLAYER checks done, now for WPLAYER checks

    /*
    WDISK move check, can move down 1 row and left or right 1 space
    */
    if ((m_currPlayer == WPLAYER) && (m_game[oRow][oCol] == WDISK) &&
        (dRow == oRow + 1 && oRow < m_numRows-1) &&
        ((dCol == oCol + 1 && oCol <= m_numColumns - 1) || (dCol == oCol - 1 && oCol > 0))
        && m_game[dRow][dCol] == BCELL) {
        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = WDISK;
        validMove = true;
    }
    /*
    WDISK capture check, moves down 2 rows and left or right 2 rows.
    */
    if ((m_currPlayer == WPLAYER) && (m_game[oRow][oCol] == WDISK) &&
        ((dCol == oCol + 2 && oCol < m_numColumns - 2) || (dCol == oCol - 2 && oCol > 1)) &&
        (dRow == oRow + 2 && oRow < m_numRows-2)
        && (m_game[oRow + 1][oCol + 1] == BDISK || m_game[oRow + 1][oCol + 1] == BKING ||
            m_game[oRow + 1][oCol - 1] == BDISK || m_game[oRow + 1][oCol - 1] == BKING)
        && m_game[dRow][dCol] == BCELL) {

        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = WDISK;
        //did the disk make a capture moving right?
        if (dCol == oCol + 2) {
            m_game[oRow + 1][oCol + 1] = BCELL;
        }
        //else, it captured moving left
        else {
            m_game[oRow + 1][oCol - 1] = BCELL;
        }
        m_blackDisks--;
        validMove = true;
    }
    /*
    WKING move check, moves up or down 1 space and left or right 1 space
    Since kings are "omnidirectional" (diagonally), the BKING and WKING
    move checks are near identical
    */
    if ((m_currPlayer == WPLAYER) && (m_game[oRow][oCol] == WKING) &&
        ((dRow == oRow - 1 && oRow > 0) || (dRow == oRow + 1 && oRow < m_numRows - 1)) &&
        ((dCol == oCol + 1 && oCol < m_numColumns - 1) || (dCol == oCol - 1 && oCol > 0)) &&
        m_game[dRow][dCol] == BCELL) {
        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = WKING;
        validMove = true;
    }
    /*
    WKING capture checek, same thing as the move check,
    near identical to BKING capture check
    */
    if ((m_currPlayer == WPLAYER) && (m_game[oRow][oCol] == WKING) &&
        ((dCol == oCol + 2 && oCol < m_numColumns - 2) || (dCol == oCol - 2 && oCol > 1)) &&
        ((dRow == oRow - 2 && oRow > 1) || (dRow == oRow + 2 && oRow < m_numRows - 2))
        && (m_game[oRow - 1][oCol + 1] == BDISK || m_game[oRow - 1][oCol + 1] == BKING ||
            m_game[oRow - 1][oCol - 1] == BDISK || m_game[oRow - 1][oCol - 1] == BKING ||
            m_game[oRow + 1][oCol + 1] == BDISK || m_game[oRow + 1][oCol + 1] == BKING ||
            m_game[oRow + 1][oCol - 1] == BDISK || m_game[oRow + 1][oCol - 1] == BKING)
        && m_game[dRow][dCol] == BCELL) {

        m_game[oRow][oCol] = BCELL;
        m_game[dRow][dCol] = BKING;
        //did the disk make a capture moving up/right?
        if (dCol == oCol + 2 && dRow == oRow - 2) {
            m_game[oRow - 1][oCol + 1] = BCELL;
        }
        //did it capture moving up/left?
        else if (dCol == oCol - 2 && dRow == oRow - 2) {
            m_game[oRow - 1][oCol - 1] = BCELL;
        }
        //did it capture moving down/right?
        else if (dCol == oCol + 2 && dRow == oRow + 2) {
            m_game[oRow + 1][oCol + 1] = BCELL;
        }
        //else, it captured moving down/left
        else {
            m_game[oRow + 1][oCol - 1] = BCELL;
        }
        m_blackDisks--;
        validMove = true;
    }

    //checks to king any BDISK
    if (m_game[0][dCol] == BDISK) {
        m_game[0][dCol] = BKING;
    }
    //checks to king any WDISK
    if (m_game[m_numRows - 1][dCol] == WDISK) {
        m_game[m_numRows - 1][dCol] = WKING;
    }
    
    return validMove;
}

bool Checkers::setGameState(CELL game[], int n, PLAYER player){
    int newWDisk = 0;
    int newBDisk = 0;

    //traverse game[] and count the amount of disks
    for (int i = 0; i < n; i++) {
        if (game[i] == BDISK || game[i] == BKING) {
            newBDisk++;
        }
        if (game[i] == WDISK || game[i] == WKING) {
            newWDisk++;
        }
    }
    //Check if there is more than max disks for invalid set state
    if (newWDisk > (m_numColumns / 2) * 3 || newBDisk > (m_numColumns / 2) * 3) {
        return false;
    }

    newWDisk = 0;
    newBDisk = 0;

    //tracker of the index of game[]
    int gameIndex = 0;

    //m_game takes on the new form of the passed game[] parameter
    for (int i = 0; i < m_numRows; i++) {
        for (int j = 0; j < m_numColumns; j++) {

            //check current cell for modification
            if (game[gameIndex] != "") {
                if (game[gameIndex] == WDISK || game[gameIndex] == WKING ||
                    game[gameIndex] == BDISK || game[gameIndex] == BKING ||
                    game[gameIndex] == WCELL || game[gameIndex] == BCELL) {
                    m_game[i][j] = game[gameIndex];
                }
            }
            if (m_game[i][j] == BDISK || m_game[i][j] == BKING) {
                newBDisk++;
            }
            if (m_game[i][j] == WDISK || m_game[i][j] == WKING) {
                newWDisk++;
            }
            gameIndex++;
        }
    }
    //Should the game state be updated, the disk counts are also updated
    m_whiteDisks = newWDisk;
    m_blackDisks = newBDisk;
    
    m_currPlayer = player;

    return true;
}

void Checkers::dumpGame(){
    if (m_numRows > 0 && m_numColumns > 0 && m_game != nullptr){
        cout << "  ";
        for (int k=0;k<m_numColumns;k++){
            cout << k << " ";
        }
        cout << endl;
        for (int i=1;i <= m_numRows;i++){
            cout << i-1 << " ";
            for (int j=1;j<=m_numColumns;j++){
                cout << m_game[i-1][j-1];
            }
            cout << endl;
        }
        cout << endl;
    }
}
