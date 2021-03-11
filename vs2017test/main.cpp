#include "Cell.h"
#include "CompareCells.h"
#include <time.h>
#include <iostream>
#include <stack>
#include <math.h>
#include <queue>

Cell** maze;
priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
vector<Cell*> grays;

bool finished = false;
int algorithm = -1;
int randSource, randTarget;
int numOfPathCells = 0;
double pathValue;

void SetCellsValues(Cell* cell, int option)
{
	cell->setH(abs(cell->getRow() - randTarget) + abs(MAZE_SIZE - 1 - cell->getColumn()));
	cell->setG(cell->getParent()->getG() + (option - 1));
	cell->setF(cell->getH() + cell->getG());
}

void CellScan(Cell* cell, Cell* parent, int option)
{
	if (!cell->isScanned())
	{
		cell->setParent(parent);
		SetCellsValues(cell, option);
		pq.push(cell);
		grays.push_back(cell);
		cell->setScanned(true);
		if(cell->getStatus() == SPACE)
			cell->setStatus(GRAY);
	}
}

void RestorePath(Cell* current)
{
	Cell* tmp = current;
	while (tmp->getStatus() != SOURCE && tmp->getStatus() != TARGET)
	{
		tmp->setStatus(PATH);
		tmp->setPathValue((double)numOfPathCells);
		tmp = tmp->getParent();
		numOfPathCells++;
	}
}

void Iteration(int option)
{
	Cell* current;
	Cell* tmp = nullptr;
	vector<Cell*>::iterator grays_iterator;
	
	if (pq.empty())
	{
		cout << "No solution" << endl;
		finished = true;
		algorithm = -1;
		return;
	}
	
	current = pq.top();
	pq.pop();
	if (current->getStatus() == GRAY)
		current->setStatus(BLACK);

	grays_iterator = find(grays.begin(), grays.end(), current);
	grays.erase(grays_iterator);

	if (current->getRow() == randTarget && current->getColumn() == MAZE_SIZE - 2)
	{
		cout << "Path was found" << endl;
		finished = true;
		RestorePath(current);
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		bool skip = false;
		switch (i)
		{
		case 0: // Left
			tmp = &maze[current->getRow() - 1][current->getColumn()];
			break;
		case 1: // Top
			tmp = &maze[current->getRow()][current->getColumn() + 1];
			break;
		case 2: // Right
			tmp = &maze[current->getRow() + 1][current->getColumn()];
			break;
		case 3: // Bottom
			if (current->getStatus() != SOURCE)
				tmp = &maze[current->getRow()][current->getColumn() - 1];
			else
				skip = true;
			break;
		default:
			break;
		}

		if (!skip && !current->isWall(i))
			CellScan(tmp, current, option);
	}
}

bool AllVisited(bool* b, int n)
{
	for (int i = 0; i < n; i++)
		if (!b[i])
			return false;
	return true;
}

void RemoveWall(Cell* current, Cell* neighbor)
{
	if (current->getColumn() == neighbor->getColumn())
	{
		if (current->getRow() + 1 == neighbor->getRow())
		{
			current->setWall(2, false);
			neighbor->setWall(0, false);
		}
		else if (current->getRow() - 1 == neighbor->getRow())
		{
			current->setWall(0, false);
			neighbor->setWall(2, false);
		}
	}
	else if (current->getRow() == neighbor->getRow())
	{
		if (current->getColumn() + 1 == neighbor->getColumn())
		{
			current->setWall(1, false);
			neighbor->setWall(3, false);
		}
		if (current->getColumn() - 1 == neighbor->getColumn())
		{
			current->setWall(3, false);
			neighbor->setWall(1, false);
		}
	}
}

void GenerateMaze(int randSource, int randTarget)
{
	// Step 1
	Cell* current = &maze[randSource][0];
	pq.push(current);
	grays.push_back(current);
	stack<Cell*> cells;
	bool* neighborsVisited;
	current->setVisited(true);
	int visited = 1;

	// Step 2
	while (visited < pow(MAZE_SIZE - 2, 2) + 1)
	{
		// Step 2.1
		current->checkNeighbors();
		neighborsVisited = new bool[current->getNeighbors().size()];
		for (int i = 0; i < current->getNeighbors().size(); i++)
			neighborsVisited[i] = maze[current->getNeighborRow(i)][current->getNeighborColumn(i)].isVisited();
		if (!AllVisited(neighborsVisited, current->getNeighbors().size()))
		{
			// Step 2.1.1
			int neighborIndex;
			do
			{
				neighborIndex = rand() % current->getNeighbors().size();
			} while (maze[current->getNeighborRow(neighborIndex)][current->getNeighborColumn(neighborIndex)].isVisited());
			// Step 2.1.2
			cells.push(current);
			// Step 2.1.3
			RemoveWall(current, &maze[current->getNeighborRow(neighborIndex)][current->getNeighborColumn(neighborIndex)]);
			// Step 2.1.4
			current = &maze[current->getNeighborRow(neighborIndex)][current->getNeighborColumn(neighborIndex)];
			current->setVisited(true);
			visited++;
		}
		// Step 2.2
		else if (!cells.empty())
		{
			// Step 2.2.1
			current = cells.top();
			// Step 2.2.2
			cells.pop();
		}
	}
	RemoveWall(&maze[randTarget][MAZE_SIZE - 1], &maze[randTarget][MAZE_SIZE - 2]);
}

void InitMaze()
{
	maze = new Cell * [MAZE_SIZE];
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		maze[i] = new Cell[MAZE_SIZE];
		for (int j = 0; j < MAZE_SIZE; j++)
		{
			maze[i][j] = Cell();
			maze[i][j].setRow(i);
			maze[i][j].setColumn(j);

			maze[i][j].setLocationX(2 * (i / (double)MAZE_SIZE) - 1);
			maze[i][j].setLocationY(2 * (j / (double)MAZE_SIZE) - 1);
		}
	}

	for (int i = 0; i < MAZE_SIZE; i++)
	{
		maze[0][i].setStatus(WALL);
		maze[MAZE_SIZE - 1][i].setStatus(WALL);
		maze[i][0].setStatus(WALL);
		maze[i][MAZE_SIZE - 1].setStatus(WALL);
	}
	
	randSource = rand() % (MAZE_SIZE - 2) + 1;
	randTarget = rand() % (MAZE_SIZE - 2) + 1;
	maze[randTarget][MAZE_SIZE - 1].setStatus(TARGET);
	maze[randSource][0].setStatus(SOURCE);

	GenerateMaze(randSource, randTarget);
}

void DrawMaze()
{
	int i, j;

	for(i = 0; i < MAZE_SIZE; i++)
		for (j = 0; j < MAZE_SIZE; j++)
		{
			switch (maze[i][j].getStatus())
			{
			case SPACE:
				glColor3d(1, 1, 1);
				break;
			case WALL:
				glColor3d(0.9, 0.4, 0.2);
				break;
			case SOURCE:
				glColor3d(0, 0, 1);
				break;
			case TARGET:
				glColor3d(0, 0.8, 0.8);
				break;
			case GRAY:
				glColor3d(0, 1, 0);
				break;
			case BLACK:
				glColor3d(0.1, 0.4, 0.1);
				break;
			case PATH:
				glColor3d(0.5 * maze[i][j].getPathValue() / numOfPathCells + 0.5, 0, 0.5 * maze[i][j].getPathValue() / numOfPathCells + 0.5);
				break;
			case GENERATED:
				glColor3d(0.5, 1, 0.5);
				break;
			default:
				break;
			}

			glBegin(GL_POLYGON);
			glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY());
			glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY() + CS);
			glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY() + CS);
			glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY());
			glEnd();

			glColor3d(0, 0, 0);
			glBegin(GL_LINES);

			if (maze[i][j].isWall(0))
			{
				glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY());
				glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY() + CS);
			}

			if (maze[i][j].isWall(1))
			{
				glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY() + CS);
				glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY() + CS);
			}

			if (maze[i][j].isWall(2))
			{
				glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY() + CS);
				glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY());
			}

			if (maze[i][j].isWall(3)) // Bottom wall
			{
				glVertex2d(maze[i][j].getLocationX() + CS, maze[i][j].getLocationY());
				glVertex2d(maze[i][j].getLocationX(), maze[i][j].getLocationY());
			}

			glEnd();
		}
}

void init()
{
	glClearColor(0.8, 0.7, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand(time(0));

	InitMaze();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle()
{
	if (!finished)
		if (algorithm == 1 || algorithm == 2)
			Iteration(algorithm);
	glutPostRedisplay();
}


void menu(int choice)
{
	algorithm = choice;
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 20);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutCreateMenu(menu);
	glutAddMenuEntry("Best First Search", 1);
	glutAddMenuEntry("A Star Search", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}