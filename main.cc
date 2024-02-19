#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include "board.h"
#include "players/level1.h"
#include "players/level2.h"
#include "players/level3.h"
#include "players/level4.h"
using namespace std;

// returns arr coords
pair<int, int> parseCoords(string coords) {
    if (coords.size() != 2 || coords[0] < 'a' || coords[0] > 'h' || coords[1] < '1' || coords[1] > '8') {
        return pair<int, int>(-1, -1);
    }
    int xCoord = coords[0] - 'a';
    int yCoord = coords[1] - '1';
    return pair<int, int>(xCoord, yCoord);
}

int main() {
    bool gameStarted = false;
    bool setupMode = false;
    vector<vector<char>> initialState(8, vector<char>(8, ' '));
    int initialTurn = 0; // reversed on purpose, if checkmate check is done in main
    vector<bool> hasKing(2, false);
    Board *theBoard = new Board();
    theBoard->addPieces();
    vector<Player *> players;
    string line = "";
    string cmd = "";
    int wScore = 0;
    int bScore = 0;
    while (getline(cin, line)) {
        istringstream iss{line};
        iss >> cmd;
        if (cmd == "game") {
            if (setupMode) {
                cout << "[INVALID USE CASE] You are in setup mode, cannot start game. Please exit first." << endl;
                continue;
            }
            if (gameStarted) {
                cout << "[INVALID USE CASE] A game is already in progress. You must first finish the current game before starting a new one." << endl;
                continue;
            }
            string p1, p2;
            iss >> p1 >> p2;
            if (p1 == "computer1") {
                players.push_back(new LevelOne(theBoard, 0));
            } else if (p1 == "computer2") {
                players.push_back(new LevelTwo(theBoard, 0));
            } else if (p1 == "computer3") {
                players.push_back(new LevelThree(theBoard, 0));
            } else if (p1 == "computer4") {
                players.push_back(new LevelFour(theBoard, 0));
            } else if (p1 == "human") {
                players.push_back(nullptr); // represents a human
            } else {
                cout << "[INVALID] " << p1 << " is not a valid player type." << endl;
                cout << "Usage: game [human|computer[1-4]] [human|computer[1-4]]" << endl;
                while(players.size()) players.pop_back();
                continue;
            }
            if (p2 == "computer1") {
                players.push_back(new LevelOne(theBoard, 1));
            } else if (p2 == "computer2") {
                players.push_back(new LevelTwo(theBoard, 1));
            } else if (p2 == "computer3") {
                players.push_back(new LevelThree(theBoard, 1));
            } else if (p2 == "computer4") {
                players.push_back(new LevelFour(theBoard, 1));
            } else if (p2 == "human") {
                players.push_back(nullptr); // represents a human
            } else {
                cout << "[INVALID COMMAND] " << p2 << " is not a valid player type." << endl;
                cout << "Usage: game [human|computer[1-4]] [human|computer[1-4]]" << endl;
                while(players.size()) players.pop_back();
                continue;
            }
            gameStarted = true;
            theBoard->printBoard();
        } else if (cmd == "resign") {
            if (!gameStarted) {
                cout << "[INVALID USE CASE] There is no active game, cannot resign." << endl;
                continue;
            }
            if (theBoard->turn == 1) {
                cout << "White Wins!" << endl;
                wScore ++;
            } else if (theBoard->turn == 0) {
                cout << "Black Wins!" << endl;
                bScore ++;
            }
            gameStarted = false;
        } else if (cmd == "move") {
            if (!gameStarted) {
                cout << "[INVALID USE CASE] There is no active game, cannot perform move. Try using setup mode instead." << endl;
                continue;
            }
            string sc, ec;
            iss >> sc >> ec;
            pair<int, int> start, end;
            // cmd was just move
            if (sc == "" && ec == "") {
                if(players[theBoard->turn] == nullptr) {
                    cout << "[INVALID USE CASE] It is not the computer's turn.\n";
                    continue;
                }
                Move mv = players[theBoard->turn]->makeMove();
                cout << mv.getStartCoord().first << " " << mv.getStartCoord().second << " -> " << mv.getEndCoord().first << " " << mv.getEndCoord().second << "\n";
                start = mv.getStartCoord();
                end = mv.getEndCoord();
            } else {
                if(players[theBoard->turn] != nullptr) {
                    cout << "[INVALID USE CASE] It is the computer's turn.\n";
                    continue;
                }
                start = parseCoords(sc);
                if (start.first == -1 && start.second == -1) {
                    cout << "[INVALID COMMAND] " << sc << " is not a valid starting coordinate on the board." << endl;
                    cout << "Usage: move [a-h][1-8] [a-h][1-8]" << endl;
                    continue;
                }
                end = parseCoords(ec);
                if (end.first == -1 && end.second == -1) {
                    cout << "[INVALID COMMAND] " << ec << " is not a valid ending coordinate on the board." << endl;
                    cout << "Usage: move [a-h][1-8] [a-h][1-8]" << endl;
                    continue;
                }
                // no piece at starting pos
                if (theBoard->board[start.first][start.second].piece == nullptr) {
                    cout << "[INVALID USE CASE] There is no piece at coordinate " << sc << "." << endl;
                    continue;
                }
                if(theBoard->board[start.first][start.second].piece->getColor() != theBoard->turn) {
                    cout << "[INVALID USE CASE] You cannot move a piece of that color.\n";
                    continue;
                }
            }
            // otherwise valid start and end, and there is a piece to move, so try moving it.
            if(!theBoard->movePiece(start, end)) {
                cout << "Invalid Move\n";
            } else {
                theBoard->turn = !theBoard->turn;
                theBoard->printBoard();
            }
        } else if (cmd == "undo") {
            if (!gameStarted) {
                cout << "[ INVALID USE CASE ] There is no active game, cannot undo." << endl;
                continue;
            }
            if (setupMode) {
                cout << "[ INVALID USE CASE ] You are in setup mode, cannot undo. Please exit first." << endl;
                continue;
            }
            if (theBoard->undoMove()) {
                theBoard->turn = !theBoard->turn;
                theBoard->printBoard();
            } else {
                cout << "[ INVALID USE CASE ] No moves have been made, cannot undo." << endl;
            }
        } else if (cmd == "setup") {
            if (gameStarted) {
                cout << "[INVALID USE CASE] A game is already in progress, cannot enter setup mode." << endl;
                continue;
            }
            if (setupMode) {
                cout << "[INVALID USE CASE] You are already in setup mode." << endl;
                continue;
            }
            setupMode = true;
        } else if (cmd == "+") {
            if (gameStarted) {
                cout << "[INVALID USE CASE] This command cannot be used while a game is in progress." << endl;
                continue;
            }
            if (!setupMode) {
                cout << "[INVALID USE CASE] This command can only be used in setup mode." << endl;
                continue;
            }
            string piece, c;
            iss >> piece >> c;
            pair<int, int> pos = parseCoords(c);
            if (piece.size() > 1 || (tolower(piece[0]) != 'p' && tolower(piece[0]) != 'r' && tolower(piece[0]) != 'n' && tolower(piece[0]) != 'b' && tolower(piece[0]) != 'q' && tolower(piece[0]) != 'k')) {
                cout << "[INVALID COMMAND] " << piece << " is not a valid piece." << endl;
                cout << "Usage: + [P|R|N|B|Q|K|p|r|n|b|q|k] [a-h][1-8]" << endl;
                continue;
            }
            if (pos.first == -1 && pos.second == -1) {
                cout << "[INVALID COMMAND] " << c << " is not a valid coordinate on the board." << endl;
                cout << "Usage: + [P|R|N|B|Q|K|p|r|n|b|q|k] [a-h][1-8]" << endl;
                continue;
            }
            if (piece == "K" && hasKing[0]) {
                cout << "[INVALID USE CASE] White already has a king, you must remove it first." << endl;
                continue;
            } else if (piece == "K") {
                hasKing[0] = !hasKing[0];
            }
            if (piece == "k" && hasKing[1]) {
                cout << "[INVALID USE CASE] Black already has a king, you must remove it first." << endl;
                continue;
            } else if (piece == "k") {
                hasKing[1] = !hasKing[1];
            }
            if ((piece == "P" && pos.second == 7) || (piece == "p" && pos.second == 0)) {
                cout << "[INVALID USE CASE] You cannot place a pawn on a promoting rank." << endl;
                continue;
            }
            initialState[pos.first][pos.second] = piece[0];
        } else if (cmd == "-") {
            if (gameStarted) {
                cout << "[INVALID USE CASE] This command cannot be used while a game is in progress." << endl;
                continue;
            }
            if (!setupMode) {
                cout << "[INVALID USE CASE] This command can only be used in setup mode." << endl;
                continue;
            }
            string c;
            iss >> c;
            pair<int, int> pos = parseCoords(c);
            if (pos.first == -1 && pos.second == -1) {
                cout << "[INVALID COMMAND] " << c << " is not a valid coordinate on the board." << endl;
                cout << "Usage: + [P|R|N|B|Q|K|p|r|n|b|q|k] [a-h][1-8]" << endl;
                continue;
            }
            char piece = initialState[pos.first][pos.second];
            if (piece == 'K') {
                hasKing[0] = false;
            }
            if (piece == 'k') {
                hasKing[1] = false;
            }
            initialState[pos.first][pos.second] = ' ';
        } else if (cmd == "=") {
            if (gameStarted) {
                cout << "[INVALID USE CASE] This command cannot be used while a game is in progress." << endl;
                continue;
            }
            if (!setupMode) {
                cout << "[INVALID USE CASE] This command can only be used in setup mode." << endl;
                continue;
            }
            string colour;
            iss >> colour;
            if (colour == "white") {
                initialTurn = 1; // opposite on purpose, it will switch after code below runs
            } else if (colour == "black") {
                initialTurn = 0;
            } else {
                cout << "[INVALID COMMAND] " << colour << " is not a valid colour." << endl;
                cout << "Usage: = [white|black]" << endl;
                continue;
            }
        } else if (cmd == "done") {
            if (gameStarted) {
                cout << "[INVALID USE CASE] This command cannot be used while a game is in progress." << endl;
                continue;
            }
            if (!setupMode) {
                cout << "[INVALID USE CASE] This command can only be used in setup mode." << endl;
                continue;
            }
            if(!hasKing[0] || !hasKing[1]) {
                cout << "[INVALID USE CASE] Exactly one king is needed for each player.\n";
                continue;
            }
            delete theBoard;
            theBoard = new Board(initialTurn);
            theBoard->addPieces(initialState);
            bool inCheck = false;
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    if(theBoard->board[i][j].piece != nullptr && theBoard->board[i][j].piece->getPieceType() == 'k') {
                        if(theBoard->board[i][j].isAttacked(!theBoard->board[i][j].piece->getColor())) inCheck = true;
                    }
                }
            }
            if(inCheck) {
                delete theBoard;
                theBoard = new Board();
                cout << "[INVALID USE CASE] Neither king can be in check.\n";
                continue;
            }
            setupMode = false;
        } else if (cmd == "help") {
            cout << "All Commands:" << endl;
            cout << "1. Non-setup mode, pre-game:" << endl;
            cout << "Start a game: game [human|computer[1-4]] [human|computer[1-4]]" << endl;
            cout << "Enter setup mode: setup" << endl;
            cout << "2. Setup mode, pre-game:" << endl;
            cout << "Add a piece: + [P|R|N|B|Q|K|p|r|n|b|q|k] [a-h][1-8]" << endl;
            cout << "Remove a piece: - [a-h][1-8]" << endl;
            cout << "Change initial turn: = [white|black]" << endl;
            cout << "Exit setup mode: done" << endl;
            cout << "3. In-game:" << endl;
            cout << "Resign the game: resign" << endl;
            cout << "Move a piece: move [a-h][1-8] [a-h][1-8]" << endl;
            cout << "Undo the previous move: undo" << endl;
            continue;
        } else {
            cout << "[INVALID COMMAND] Please enter a valid command." << endl;
            continue;
        }
        if (gameStarted) {
            if(theBoard->isFinished()) {
                for(int i = 0; i < 8; i++) {
                    for(int j = 0; j < 8; j++) {
                        if(theBoard->board[i][j].piece != nullptr && theBoard->board[i][j].piece->getColor() == theBoard->turn) {
                            if(theBoard->board[i][j].piece->getPieceType() == 'k' && theBoard->board[i][j].isAttacked(!theBoard->turn)) {
                                if (theBoard->turn) {
                                    cout << "White Wins!\n";
                                    wScore ++;
                                } else {
                                    cout << "Black Wins!\n";
                                    bScore ++;
                                }
                                gameStarted = false;
                            }
                        }
                    }
                }
                if(gameStarted) {
                    cout << "Stalemate!\n";
                    gameStarted = false;
                }
            }
        }
        if(setupMode) {
            for (int y = 7; y >= 0; y--) {
                cout << y + 1 << " ";
                for (int x = 0; x < 8; x++) {
                    if(initialState[x][y] == ' ') {
                        if((x + y) % 2) {
                            cout << '_';
                        } else cout << ' ';
                    } else cout << initialState[x][y];
                }
                cout << "\n";
            }
            cout << "  ";
            for (int x = 0; x < 8; x++) {
                cout << string(1, 'a' + x);
            }
            cout << "\n";
        }
    }
    cout << "Final Score:\n" << "White: " << wScore << "\n" << "Black: " << bScore << "\n";
    return 0;
}
