#ifndef SWEEPBOMB_BLOCK_H
#define SWEEPBOMB_BLOCK_H

#include <vector>
#include <random>
#include <time.h>

using namespace std;

enum State{
    UNDIGGED,
    DIGGED,
    REDFLAG
};

enum gameState{
    PLAYING,
    WIN,
    LOSE
};
struct unit{
    State CurrentState;
    int bombNum;
    bool isBomb;
    unit():CurrentState(UNDIGGED),bombNum(0),isBomb(false) {}
    ~unit() = default;
};

class block {
public:
    block(int theRow = 10,int theCol = 10,int theTotalBomb =15 );
    ~block() = default;
public:
    void dig(int theRow,int theCol);
    void mark(int theRow, int theCol);
    bool isBomb(int theRow,int theCol) const;
    int getRow() const;
    int getCol() const;
    int getBombNum() const;
    gameState checkGame();
    int restBomb = 0;
    vector<vector<unit>> map;
private:

    int row;
    int col;
    int totalBomb;
    int restNum;
    gameState curGameState;
private:
    void callBombNum();
    int callBombnum(int theRow,int theCol) const;
};


#endif //SWEEPBOMB_BLOCK_H
