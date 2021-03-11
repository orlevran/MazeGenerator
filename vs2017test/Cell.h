#pragma once
#ifndef __CELL_H
#define __CELL_H
#include "glut.h"
#include <vector>
#include <tuple>
#include<iostream>
using namespace std;

const int MAZE_SIZE = 100;
const int SOURCE = 0, WALL = 1, TARGET = 2, SPACE = 3, GRAY = 4, BLACK = 5, PATH = 6, GENERATED = 7;
const double CS = 2.0 / MAZE_SIZE;

class Cell
{
private:
	int row, column, status, f, g, h;
	bool walls[4] = {
		true, // Left
		true, // Top
		true, // Right
		true  // Bottom
	};
	bool visited = false, scanned = false;
	double locationX, locationY;
	vector<tuple<int, int>> neighbors;
	Cell* parent;
	double pathValue;


public:
	Cell();

	void setRow(int r);
	void setColumn(int c);
	void setStatus(int s);
	void setF(int f);
	void setG(int g);
	void setH(int h);
	void setWall(int i, bool b);
	void setVisited(bool b);
	void setScanned(bool b);
	void setLocationX(double x);
	void setLocationY(double y);
	void setParent(Cell* p);
	void setPathValue(double val);

	int getRow();
	int getColumn();
	int getStatus();
	int getF();
	int getG();
	int getH();
	bool isWall(int i);
	bool isVisited();
	bool isScanned();
	double getLocationX();
	double getLocationY();
	Cell* getParent();
	double getPathValue();

	void checkNeighbors();
	int getNeighborRow(int i);
	int getNeighborColumn(int i);
	vector<tuple<int, int>> getNeighbors();
};

#endif // !__CELL_H
