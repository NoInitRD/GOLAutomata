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
#include <cstring>
#include <fstream>

using namespace std;

class Arena {
public:
        vector<vector<bool> > _vector;
	int _sizeX; 
	int _sizeY;
	int _generation;
	int _numAlive;
	short _repetitions; //keeps track of duplicate generations
	bool _writeToFile;

        bool liveOrDie(uint x, uint y);
        bool getCellStatus(uint x, uint y);
        bool cellExists(uint x, uint y);
        void showAndUpdateCells(void);
        void generateRandom(void);
	void writeArenaToFile(void);

        Arena(){};

	Arena(int const sizeX, int const sizeY, bool const writeToFile): 
		_vector(sizeX, vector<bool>(sizeY, false)),
		_sizeX(sizeX),
		_sizeY(sizeY),
		_writeToFile(writeToFile)
		{
			_generation = 0; 
			_numAlive = 0;
			_repetitions = 0;
		}

	Arena(int const size, bool const writeToFile): 
		_vector(size, vector<bool>(size, false)),
		_sizeX(size),
		_sizeY(size),
		_writeToFile(writeToFile)
		{
			_generation = 0; 
			_numAlive = 0;
			_repetitions = 0;
		}

	Arena(Arena const &src) { _vector = src._vector; }
};

bool Arena::cellExists(uint x, uint y) { return x < _vector.size() && y < _vector.size(); }

void clearScreen(void) { cout << "\033[2J\033[1;1H"; } //ANSI escape characters

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

void Arena::showAndUpdateCells(void)
{
        vector<vector<bool> > newVector(_vector.size(), vector<bool>(_vector.size(), false));
	short lastGenPopulation = _numAlive;

	//updates each cell in 2D vector
        for(uint y = 0; y < _sizeY; y++)
        {
                for(uint x = 0; x < _sizeX; x++)
                {
			if(_vector[x][y]) cout << "▄ ";
                        else cout << "  ";

			bool oldCellStatus = getCellStatus(x, y);
                        bool decision = liveOrDie(x, y);
                        newVector[x][y] = decision;
			
			if(decision == true && oldCellStatus == false) _numAlive++;
			if(decision == false && oldCellStatus == true) _numAlive--;
                }
		cout << endl;
        }
        _vector = newVector;

	if(_numAlive == lastGenPopulation) _repetitions++;

	if(_repetitions >= 150) //repetition filter
	{
		generateRandom();
		_repetitions  = 0;
	}

	_generation++;
}

void Arena::writeArenaToFile(void)
{
	ofstream gridFile;
	gridFile.open("grid.txt");
	for(uint y = 0; y < _sizeY; y++)
	{
		for(uint x = 0; x < _sizeX; x++)
		{
			if(_vector[x][y]) gridFile << true;
			else gridFile << false;
		}
	}
	gridFile.close();
}


bool randomBool(void)
{
	uint time = chrono::system_clock::now().time_since_epoch().count(); //get time in ms
	srand(time); //seed random with time
	return rand() % 2 == 1; //boolean conversion
}

void Arena::generateRandom(void)
{
        for(uint y = 0; y < _sizeY; y++)
	{
                for(uint x = 0; x < _sizeX; x++)
		{
			bool random = randomBool();
                        _vector[x][y] = random;
			if(random == true) _numAlive++;
		}
	}

	if(_writeToFile) writeArenaToFile();

	_generation++;
}	

void sleep(int ms) { this_thread::sleep_for(std::chrono::milliseconds(ms)); }

bool argumentHandler(int args, char *argArry[])
{
	for(int i = 1; i < args; i++)
	{
		char *curr = argArry[i];

		if(strcmp(curr, "-o") == 0)
		{
			return true;
		}
		else
		{
			cout << "-------------------------------" << endl;
			cout << "Game of life" << endl;
			cout << "-h: prints this message" << endl;
			cout << "-o: records the seed in a file" << endl;
			cout << "--------------------------------" << endl;
			exit(1);
		}
	}
	return false;
}

int main(int args, char *argArry[])
{

	bool writeToFile = argumentHandler(args, argArry);

        Arena *a = new Arena(55,55, writeToFile); //115

	a->generateRandom(); //randomly fills arena with cells
			
	//add whether to record grid each restart to the constructor 

	int generation = 0;

        while(true)
        {
                a->showAndUpdateCells();
		cout << "Generation: ";
		cout << a->_generation;
		cout << " Number Alive: ";
		cout << a->_numAlive;
		cout << " Repetitions: ";
		cout << a->_repetitions << endl;
                sleep(15);
		clearScreen();
        }

        delete a;
        return 0;
	
}
