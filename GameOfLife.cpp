/*
 * GameOfLife.cpp
 *
 *  Created on: Nov 18, 2022
 *      Author: noinitrd
 */

#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>

using namespace std;

class Arena {
public:
        vector<vector<bool>> _vector;

        void showCells(void);
        void flipCellStatus(uint x, uint y);
        bool liveOrDie(uint x, uint y);
        bool getCellStatus(uint x, uint y);
        bool cellExists(uint x, uint y);
        void updateCells(void);
        void generateRandom(void);

        Arena(){};
        Arena(int const size) : _vector(size, vector<bool>(size, false)) {}
        Arena(Arena const &src) {_vector = src._vector;}
};

bool Arena::cellExists(uint x, uint y) { return x < _vector.size() && y < _vector.size(); }

void clearScreen(void) { cout << "\033[2J\033[1;1H"; }

void Arena::showCells(void)
{
        clearScreen();
        for(uint y = 0; y < _vector.size(); y++)
        {
                for(uint x = 0; x < _vector.size(); x++)
                {
                        if(_vector[x][y]) cout << "▄ ";
                        else cout << "  ";
                }
                cout << endl;
        }
}

void Arena::flipCellStatus(uint x, uint y)
{
        if(!cellExists(x, y)) return;
        _vector[x][y] = !_vector[x][y];
}

bool Arena::getCellStatus(uint x, uint y)
{
        if(!cellExists(x, y)) return false;
        return _vector[x][y];
}

bool Arena::liveOrDie(uint x, uint y)
{
        short numAlive = 0;
        bool currentCellStatus = getCellStatus(x, y);

        if(getCellStatus(x - 1, y + 1)) numAlive++;
        if(getCellStatus(x - 1, y)) numAlive++;
        if(getCellStatus(x - 1, y - 1)) numAlive++;
        if(getCellStatus(x, y + 1)) numAlive++;
        if(getCellStatus(x, y - 1)) numAlive++;
        if(getCellStatus(x + 1, y + 1)) numAlive++;
        if(getCellStatus(x + 1, y)) numAlive++;
        if(getCellStatus(x + 1, y - 1)) numAlive++;

        if(currentCellStatus && (numAlive == 2 || numAlive == 3)) return true; //survival and reproduction
        return !currentCellStatus && numAlive == 3; //death
}

void Arena::updateCells(void)
{
        vector<vector<bool>> newVector(_vector.size(), vector<bool>(_vector.size(), false));

        for(uint y = 0; y < _vector.size(); y++)
        {
                for(uint x = 0; x < _vector.size(); x++)
                {
                        bool decision = liveOrDie(x, y);
                        newVector[x][y] = decision;
                }
        }

        if(_vector == newVector) exit(1);

        _vector = newVector;
}

void Arena::generateRandom(void)
{
        for(uint y = 0; y < _vector.size(); y++)
        {
                for(uint x = 0; x < _vector.size(); x++)
                {
                        _vector[x][y] = rand() % 2;
                }
        }
}

void sleep(int ms) { this_thread::sleep_for(std::chrono::milliseconds(ms)); }

int main()
{
        Arena *a = new Arena(30);

        a->generateRandom();

        a->showCells();

        while(true)
        {
                a->updateCells();
                a->showCells();
                sleep(10);
        }

        delete a;

        return 0;
}
