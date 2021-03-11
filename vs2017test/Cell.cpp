#include "Cell.h"


Cell::Cell()
{
	this->row = -1;
	this->column = -1;
	this->status = SPACE;
	this->locationX = 0;
	this->locationY = 0;

	this->f = 0;
	this->g = 0;
	this->h = 0;

	this->parent = nullptr;
	this->pathValue = 0;
}

void Cell::setRow(int r)
{
	this->row = r;
}

void Cell::setColumn(int c)
{
	this->column = c;
}

void Cell::setStatus(int s)
{
	this->status = s;
}

void Cell::setF(int f)
{
	this->f = f;
}

void Cell::setG(int g)
{
	this->g = g;
}

void Cell::setH(int h)
{
	this->h = h;
}

void Cell::setWall(int i, bool b)
{
	if (i >= 0 && i < 4)
		this->walls[i] = b;
}

void Cell::setVisited(bool b)
{
	this->visited = b;
	//if (this->visited && this->status == SPACE)
	//	setStatus(GENERATED);
}

void Cell::setScanned(bool b)
{
	this->scanned = b;
}

void Cell::setLocationX(double x)
{
	this->locationX = x;
}

void Cell::setLocationY(double y)
{
	this->locationY = y;
}

void Cell::setParent(Cell* p)
{
	this->parent = p;
}

void Cell::setPathValue(double val)
{
	this->pathValue = val;
}

int Cell::getRow()
{
	return this->row;
}

int Cell::getColumn()
{
	return this->column;
}

int Cell::getStatus()
{
	return this->status;
}

int Cell::getF()
{
	return this->f;
}

int Cell::getG()
{
	return this->g;
}

int Cell::getH()
{
	return this->h;
}

bool Cell::isWall(int i)
{
	if(i >= 0 && i < 4)
		return this->walls[i];
	return false;
}

bool Cell::isVisited()
{
	return this->visited;
}

bool Cell::isScanned()
{
	return this->scanned;
}

double Cell::getLocationX()
{
	return this->locationX;
}

double Cell::getLocationY()
{
	return this->locationY;
}

Cell* Cell::getParent()
{
	return this->parent;
}

double Cell::getPathValue()
{
	return this->pathValue;
}

void Cell::checkNeighbors()
{
	switch (this->status)
	{
	case SOURCE:
		this->neighbors.push_back(make_tuple(this->row, this->column + 1));
		break;
	case TARGET:
		this->neighbors.push_back(make_tuple(this->row, this->column - 1));
		break;
	case SPACE:
	case GENERATED:
		if (this->row > 1)
			this->neighbors.push_back(make_tuple(this->row - 1, this->column));
		if (this->row < MAZE_SIZE - 2)
			this->neighbors.push_back(make_tuple(this->row + 1, this->column));
		if (this->column > 1)
			this->neighbors.push_back(make_tuple(this->row, this->column - 1));
		if (this->column < MAZE_SIZE - 2)
			this->neighbors.push_back(make_tuple(this->row, this->column + 1));
		break;
	default:
		break;
	}
}

int Cell::getNeighborRow(int i)
{
	return get<0>(this->neighbors.at(i));
}

int Cell::getNeighborColumn(int i)
{
	return get<1>(this->neighbors.at(i));
}

vector<tuple<int, int>> Cell::getNeighbors()
{
	return this->neighbors;
}
