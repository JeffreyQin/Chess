#ifndef __LEVEL4_H__
#define __LEVEL4_H__
#include <vector>
#include <stack>
#include <utility>
#include <map>
#include "player.h"


class LevelFour: public Player {
    const map<char, int> baseWeights{
        {'p', 100},
        {'n', 320},
        {'b', 330},
        {'r', 500},
        {'q', 900},
        {'k', 20000}
    };
    const vector<vector<int>> pawnWeights{
        {  0,  5,  5,  0,  5, 10, 50,  0},
        {  0, 10, -5,  0,  5, 10, 50,  0},
        {  0, 10,-10,  0, 10, 20, 50,  0},
        {  0,-20,  0, 20, 25, 30, 50,  0},
        {  0,-20,  0, 20, 25, 30, 50,  0},
        {  0, 10,-10,  0, 10, 20, 50,  0},
        {  0, 10, -5,  0,  5, 10, 50,  0},
        {  0,  5,  5,  0,  5, 10, 50,  0}
    };
    const vector<vector<int>> knightWeights{
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  5,  0,  5,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  0,-30},
        {-30,  5, 15, 20, 20, 15,  0,-30},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-40,-20,  5,  0,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    };
    const vector<vector<int>> bishopWeights{
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  5, 10,  0,  5,  0,  0,-10},
        {-10,  0, 10, 10,  5,  5,  0,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10,  0, 10, 10,  5,  5,  0,-10},
        {-10,  5, 10,  0,  5,  0,  0,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20},
    };
    const vector<vector<int>> rookWeights{
        {  0, -5, -5, -5, -5, -5,  5,  0},
        {  0,  0,  0,  0,  0,  0, 10,  0},
        {  0,  0,  0,  0,  0,  0, 10,  0},
        {  5,  0,  0,  0,  0,  0, 10,  0},
        {  5,  0,  0,  0,  0,  0, 10,  0},
        {  0,  0,  0,  0,  0,  0, 10,  0},
        {  0,  0,  0,  0,  0,  0, 10,  0},
        {  0, -5, -5, -5, -5, -5,  5,  0},
    };
    const vector<vector<int>> queenWeights{
        {-20,-10,-10,  0, -5,-10,-10,-20},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10,  -5, -5,-10,-10,-20}
    };
    const vector<vector<int>> kingWeights{
        { 20, 20,-10,-20,-30,-30,-30,-30},
        { 30, 20,-20,-30,-40,-40,-40,-40},
        { 10,  0,-20,-30,-40,-40,-40,-40},
        {  0,  0,-20,-40,-50,-50,-50,-50},
        {  0,  0,-20,-40,-50,-50,-50,-50},
        { 10,  0,-20,-30,-40,-40,-40,-40},
        { 30, 20,-20,-30,-40,-40,-40,-40},
        { 20, 20,-10,-20,-30,-30,-30,-30}
    };
    const vector<vector<int>> kingWeightsEndGame{
        {-50,-30,-30,-30,-30,-30,-30,-50},
        {-30,-30,-10,-10,-10,-10,-20,-40},
        {-30,  0, 20, 30, 30, 20,-10,-30},
        {-30,  0, 30, 40, 40, 30,  0,-20},
        {-30,  0, 30, 40, 40, 30,  0,-20},
        {-30,  0, 20, 30, 30, 20,-10,-30},
        {-30,-30,-10,-10,-10,-10,-20,-40},
        {-50,-30,-30,-30,-30,-30,-30,-50}
    };
    const map<char, vector<vector<int>>> positionalWeights{
        {'p', pawnWeights},
        {'n', knightWeights},
        {'b', bishopWeights},
        {'r', rookWeights},
        {'q', queenWeights},
        {'k', kingWeights},
        {'e', kingWeightsEndGame}
    };
    const map<char, int> gamePhaseWeights = {
        {'p', 0},
        {'n', 1},
        {'b', 1},
        {'r', 2},
        {'q', 4},
        {'k', 0},
        {'t', 24}
    };
    struct MoveTree {
        int evalScore;
        Move *mv;
        vector<MoveTree *> nextMoves;
        MoveTree();
        MoveTree(int evalScore, Move *mv);
        ~MoveTree();
    };
    MoveTree *allMoves;
    MoveTree *head;
    static inline int moves = 0;
    static inline int undos = 0;
    int eval(int color);
    vector<MoveTree *> getAllMoves(int curColor);
    void populateMoves(MoveTree *tree, int depth, int curColor);
    pair<int, Move *> quiesce(MoveTree *curMv, int alpha, int beta, int depth, int curColor);
    pair<int, Move *> negamax(MoveTree *curMv, int alpha, int beta, int depth, int curColor);
public:
    LevelFour(Board* b, int color);
    Move makeMove() override;
    ~LevelFour();
};
#endif
