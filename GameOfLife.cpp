/*
 *
 * GameOfLife.cpp
 *
 *  Created on: Nov 18, 2022
 *      Author: noinitrd
 */

#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <random>
#include <chrono>

using namespace std;

class Arena {
public:
        vector<vector<bool> > _vector;
	int _generation;

        void flipCellStatus(uint x, uint y);
        bool liveOrDie(uint x, uint y);
        bool getCellStatus(uint x, uint y);
        bool cellExists(uint x, uint y);
        void ShowAndUpdateCells(void);
        void generateRandom(void);

        Arena(){};
        Arena(int const size): _vector(size, vector<bool>(size, false)) { _generation = 1; }
        Arena(Arena const &src) { _vector = src._vector; }
};

bool Arena::cellExists(uint x, uint y) { return x < _vector.size() && y < _vector.size(); }

void clearScreen(void) { cout << "\033[2J\033[1;1H"; } //ANSI escape characters

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

	//check left side of cell
	for(int i = 1; i > -2; i--) 
		if(getCellStatus(x - 1, y + i)) numAlive++;

	//check above and below cell
	for(int j = 1; j >= -1; j -= 2)
		if(getCellStatus(x, y + j)) numAlive++;

	//check right side of cell
	for(int k = 1; k > -2; k--) 
		if(getCellStatus(x + 1, y + k)) numAlive++;

	//determine if cell will live or die
        if(currentCellStatus && (numAlive == 2 || numAlive == 3)) return true; //survival and reproduction
        return !currentCellStatus && numAlive == 3; //death
}

void Arena::ShowAndUpdateCells(void)
{
        vector<vector<bool> > newVector(_vector.size(), vector<bool>(_vector.size(), false));

	//updates each cell in 2D vector
        for(uint y = 0; y < _vector.size(); y++)
        {
                for(uint x = 0; x < _vector.size(); x++)
                {
			if(_vector[x][y]) cout << "▄ ";
                        else cout << "  ";
                        bool decision = liveOrDie(x, y);
                        newVector[x][y] = decision;
                }
		cout << endl;
        }
        if(_vector == newVector) exit(1);
        _vector = newVector;
	_generation++;

}

bool randomBool(void)
{
	uint time = chrono::system_clock::now().time_since_epoch().count(); //get time in ms
	srand(time); //seed random with time
	return rand() % 2 == 1; //boolean conversion
}

void Arena::generateRandom(void)
{
        for(uint y = 0; y < _vector.size(); y++)
	{
                for(uint x = 0; x < _vector.size(); x++)
		{
                        _vector[x][y] = randomBool();
		}
	}
}	

void sleep(int ms) { this_thread::sleep_for(std::chrono::milliseconds(ms)); }

int main()
{
        Arena *a = new Arena(60);

	a->generateRandom(); //randomly fills arena with cells

	int generation = 0;

        while(true)
        {
                a->ShowAndUpdateCells();
		cout << "Generation: ";
		cout << a->_generation << endl;
                sleep(50);
		clearScreen();
        }

        delete a;
        return 0;
}
