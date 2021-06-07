#include "gp142.h"
#include<iostream>
#include <set>
#include <stack>
#include <iterator>
#include <list>
using namespace std;

#define MAX_ROW 40
#define MAX_COL 40
#define MAX_DISTANCE 10000

struct Point {
	double x, y;
};

struct node {
	int x, y, parent_x, parent_y;		//coordinates
	float f, g, h;						//values for heuristic
};

bool operator < (const node& obj1, const node& obj) {
	return obj1.f < obj.f;
}

bool operator > (const node& obj1, const node& obj) {
	return obj1.f > obj.f;
}

const ostream& operator << (ostream& out, node cell1) {
	out << cell1.f;
	return out;
}

void initCoordinates(Point coordinates[MAX_ROW + 1][MAX_COL + 1])
{
	int x = 600 / MAX_ROW;
	int y = 520 / MAX_COL;

	int xStart = -300;
	int yStart = 260;
	for (int i = 0; i < MAX_ROW + 1; i++) {
		xStart = -300;
		for (int j = 0; j < MAX_COL + 1; j++) {
			Point a;
			a.x = xStart;
			a.y = yStart;
			xStart += x;

			coordinates[i][j] = a;
		}
		yStart -= y;
	}
}


void fill(Point obj1, Point obj2, int color){
	int x1, x2, y1, y2;
	x1 = obj1.x;
	x2 = obj2.x;
	y1 = obj1.y;
	y2 = obj2.y;

	int y = y1 - 2;
	while (y > y2) {
		GP142_lineXY(color, x1, y, x2 - 1, y, 2);
		y -= 2;
	}
}

bool contains(Point leftCorner, Point rightCorner, Point p) 
{
	if (p.x >= leftCorner.x && p.x <= rightCorner.x && p.y <= leftCorner.y && p.y >= rightCorner.y)
		return true;
	else
		return false;
}

void initDisplay() {
	GP142_rectangleXY(     
		0,                   
		-300, 260,               
		300, -260,       
		3);

	int x = 600 / MAX_ROW;
	int y = 520 / MAX_COL;
	for (int i = 0; i < MAX_ROW; i++) {
		GP142_lineXY(0, -300 + i*x, 260, -300 + i*x, -260, 3);
	}
	for (int i = 0; i < MAX_COL; i++) {
		GP142_lineXY(0, -300, -260 + i*y, 300 , -260 + i*y, 3);
	}
}



//Using the euclidean distance as heuristic
double heuristicValue(int row, int col, const node& dest) {
	return (sqrt((row - dest.x)*(row - dest.x) + (col - dest.y)*(col - dest.y)));
}


void determineShortestPath(node bitMap[][MAX_COL], const node& destination, Point c[][MAX_COL + 1]) {
	int row = destination.x;
	int col = destination.y;

	stack<Point> ShortestPath;

	Point point;
	point.x = row;
	point.y = col;
	while (!(bitMap[row][col].parent_x == row
		&& bitMap[row][col].parent_y == col)) {
		ShortestPath.push(point);
		int temp_row = bitMap[row][col].parent_x;
		int temp_col = bitMap[row][col].parent_y;
		row = temp_row;
		col = temp_col;
		point.x = row;
		point.y = col;
	}

	
	ShortestPath.push(point);
	ShortestPath.pop();
	while (ShortestPath.size() > 1)
	{
		Point p = ShortestPath.top();
		ShortestPath.pop();
		
		fill(c[(int)p.x][(int)p.y], c[(int)p.x+1][(int)p.y+1], 8);
	}

}

void initPath(node pathDetails[][MAX_COL], const node& dest) {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			pathDetails[i][j].g = pathDetails[i][j].f = MAX_DISTANCE;
			pathDetails[i][j].h = heuristicValue(i, j, dest);
			pathDetails[i][j].parent_x = pathDetails[i][j].parent_y = -1;

		}
	}
}

void AStarAlgorithm(int bitMap[][MAX_COL], const node& source, const node& destination, Point c[][MAX_COL + 1]) {
	set<node> openList;
	list<node> closedList;

	node pathDetails[MAX_ROW][MAX_COL];
	initPath(pathDetails, destination);

	int i = source.x;
	int j = source.y;

	pathDetails[i][j].parent_x = i;
	pathDetails[i][j].parent_y = j;
	pathDetails[i][j].x = i;
	pathDetails[i][j].y = j;

	pathDetails[i][j].f = pathDetails[i][j].g = 0;

	openList.insert(pathDetails[i][j]);
	int count = 0;
	while (!openList.empty())
	{
		count++;
		node currentNode = *openList.begin();
		openList.erase(openList.begin());
		i = currentNode.x;
		j = currentNode.y;

		int successor_x, successor_y;

		//-----------------------generating the successors------------------------------
		
		for (successor_x = i - 1; successor_x <= i + 1; successor_x++) {
			for (successor_y = j - 1; successor_y <= j + 1; successor_y++) {
				if (successor_x == i && successor_y == j) {
					successor_y++;
				}

				if (successor_x >= 0 && successor_x < MAX_ROW && successor_y >= 0 && successor_y < MAX_COL)
				{
					pathDetails[successor_x][successor_y].x = successor_x;
					pathDetails[successor_x][successor_y].y = successor_y;

					if (successor_x == destination.x && successor_y == destination.y)
					{
						pathDetails[successor_x][successor_y].parent_x = i;
						pathDetails[successor_x][successor_y].parent_y = j;
						determineShortestPath(pathDetails, destination, c);
						return;
					}
					else if (bitMap[successor_x][successor_y] != -1)
					{
						double f, g, h;
						g = currentNode.g + 1;
						h = heuristicValue(successor_x, successor_y, destination);
						f = g + h;

						bool found = false;

						list<node>::iterator itr;
						for (itr = closedList.begin(); itr != closedList.end(); itr++) {
							node obj = *itr;
							if (obj.x == successor_x && obj.y == successor_y && obj.f < f)
								found = true;
						}
						if (found == true);
						else if (pathDetails[successor_x][successor_y].f < f);
						else {
							//fill(c[successor_x][successor_y], c[successor_x + 1][successor_y + 1], 23);
							pathDetails[successor_x][successor_y].g = g;
							pathDetails[successor_x][successor_y].f = f;
							pathDetails[successor_x][successor_y].h = h;
							pathDetails[successor_x][successor_y].parent_x = i;
							pathDetails[successor_x][successor_y].parent_y = j;
							openList.insert(pathDetails[successor_x][successor_y]);
						}

					}
				}
			}
		}



		list<node>::iterator itr = closedList.end();
		node obj;
		closedList.push_back(currentNode);
	}
}


int main(void)
{

	int quit;                   /* Track whether the user has asked to quit */
	int event;                  /* Holds GP142 events                       */
	int mouse_x, mouse_y;       
	char key_pressed;           

								/* Open a blank GP142 graphics window.*/
	GP142_open();
	
	GP142_clear();

	initDisplay();
	Point c[MAX_ROW + 1][MAX_COL + 1];
	initCoordinates(c);

	int bitMap[MAX_ROW][MAX_COL] = { 0 };

	quit = FALSE;
	
	bool found = false;
	event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	int x, y;
	for (int i = 0; i < MAX_ROW && !found; i++) {
			for (int j = 0; j < MAX_COL && !found; j++) {
			if (contains(c[i][j], c[i + 1][j + 1], Point{ (double)mouse_x , (double)mouse_y })) {
				x = i;
				y = j;
				fill(c[i][j], c[i + 1][j + 1], 0);
				found = true;
				}
		}
	}

	node src;
	src.x = x;
	src.y = y;

	found = false;
	event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	for (int i = 0; i < MAX_ROW && !found; i++) {
		for (int j = 0; j < MAX_COL && !found; j++) {
			if (contains(c[i][j], c[i + 1][j + 1], Point{ (double)mouse_x , (double)mouse_y })) {
				x = i;
				y = j;
				fill(c[i][j], c[i + 1][j + 1], 3);
				found = true;
			}
		}
	}

	node dest;
	dest.x = x;
	dest.y = y;
	event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	while (event == GP142_MOUSE) {
		for (int i = 0; i < MAX_ROW; i++) {
			for (int j = 0; j < MAX_COL; j++) {
				if (contains(c[i][j], c[i + 1][j + 1], Point{ (double)mouse_x , (double)mouse_y })) {
					bitMap[i][j] = -1;
					fill(c[i][j], c[i + 1][j + 1], 2);
				}
			}
		}

		event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	}


	AStarAlgorithm(bitMap, src, dest, c);

	event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	GP142_close();
	return 0;

}
