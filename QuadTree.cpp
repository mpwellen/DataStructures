#include "stdafx.h"
#include <cstdlib>
#include "QuadTree.h"
#include "QTQuad.h"
#include "Point.h"
#include "BBox.h"
#include <iterator>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;


QuadTree::QuadTree(const BBox &bounds) //create QuadTree
{
	std::cout << "Quad Tree Constructor" << endl;
	m_root = new QTQuad();
	m_qBounds = BBox(bounds.m_bL, bounds.m_dim);
}

QuadTree::QuadTree() //create the quadtree
{
	cout << "Quad Tree Constructor" << endl;
	m_root = new QTQuad();
	m_qBounds = BBox(Point(0, 0), 16);
}

QuadTree::iterator::iterator()
{
	cout << "GOT to iterator constructor" << endl;
	QTQuad * curr = NULL;
	QTQuad * currCell = NULL;
	cout << "finished consturctor!" << endl;
}

/*QuadTree::iterator::iterator(QuadTree *Q)
{
	Q->num = 42;
	QTQuad* curr = NULL; //pointer to QTQuad
	int* currCell = NULL; //pointer to m_cell
}*/

QuadTree::iterator::~iterator()
{
	delete curr;
	delete currCell;
}

void QuadTree::iterator::printNum(QuadTree *Q)
{
	cout << Q->num << endl;
}

QuadTree::iterator QuadTree::begin()
{
	cout << "Iterator Begin Running" << endl;
	iterator it=iterator();
	it.curr = m_root; //start with root
	
	int dim_x_left = (int)m_qBounds.m_bL.m_x; //initial dim_x_left is equal to the left wall
	int dim_x_right = (int)m_qBounds.m_bL.m_x + (int)m_qBounds.m_dim; // ditto right wall
	int dim_y_top = (int)m_qBounds.m_bL.m_y + (int)m_qBounds.m_dim; //ditto top wall
	int dim_y_bot = (int)m_qBounds.m_bL.m_y; //ditto bottom wall
	cout << "CHIMP" << endl;
	cout << "ENTER BEGIN LOOP" << endl;
	while (it.curr->m_quads[0] != NULL || it.curr->m_quads[1] != NULL || it.curr->m_quads[2] != NULL || it.curr->m_quads[3] != NULL
		|| it.curr->parent != NULL) //keep going unless all children are null or parent is null
	{
		//cout << "CHIMP" << endl;
		if (it.curr->m_quads[0] != NULL) //if first is not null, send pointer there
		{
			dim_x_right = (dim_x_right+dim_x_left) / 2; //update wall
			dim_y_top = (dim_y_bot+dim_y_top) / 2; //update top wall
			it.curr = it.curr->m_quads[0]; //send pointer there
			it.curr->location = 0; //QTQuad Locator
		}
		else if (it.curr->m_quads[1] != NULL) //and so on
		{
			dim_x_left = (dim_x_left+dim_x_right)/ 2; //update left wall
			dim_y_top = (dim_y_top+dim_y_bot) / 2; //update top wall
			it.curr = it.curr->m_quads[1]; //send pointer there
			it.curr->location = 1; //location
		}
		else if (it.curr->m_quads[2] != NULL)
		{
			dim_x_right = (dim_x_right+dim_x_left) / 2;
			dim_y_bot = (dim_y_top+dim_y_bot) / 2;
			it.curr = it.curr->m_quads[2];
			it.curr->location = 2;
		}
		else if (it.curr->m_quads[3] != NULL)
		{
			dim_x_left = (dim_x_right+dim_x_left) / 2;
			dim_y_bot = (dim_y_bot+dim_y_top) / 2;
			it.curr = it.curr->m_quads[3];
			it.curr->location = 3;
		}
	}
	if (it.curr->parent == NULL)
	{
		it.curr = m_root;
		it.currCell = 0;
	}
	cout << "dim_x_left"<<dim_x_left << endl;
	cout << "dim_x_right"<<dim_x_right << endl;
	cout << "dim_x_top"<<dim_y_top << endl;
	cout << "dim_y_bot"<<dim_y_bot << endl;
	cout << "ENTER M_CELLS" << endl;
	cout << it.curr->m_cells[0][0] << endl;

	//now that you've found the earliest node that contains something
	for (int j = QTQ_GRID_DIM - 1; j >= 0; j--) //y-axis
	{
		for (int i = 0; i < QTQ_GRID_DIM - 1; i++) //x-axis
		{
			//cout << "CHIMP" << endl;
			//cout << it.curr->m_cells[i][j] << endl;
			//cout << "LOOKING" << endl;
			//cout << "NUMBER CONTAINED:"<< it.curr->m_cells[i][j] << endl;
			if (it.curr->m_cells[i][j] != 0)
			{
				cout << "FOUND IT!!!" << endl;
				it.currCell = *(it.curr->m_cells + (j*QTQ_GRID_DIM) + i);
				it.x = dim_x_left + i;
				it.y = dim_y_bot + j;
			}
		}
	}
	//cout << it.curr->location << endl;
	
	return it;
}

QuadTree::iterator QuadTree::end() //same as begin(), except in opposite order
{
	cout << "Iterator END Running" << endl;
	iterator it = iterator();
	cout << "After iterator constructor!" << endl;
	it.curr = QuadTree::m_root; //start with root
	int dim_x_left = m_qBounds.m_bL.m_x; //initial dim_x_left is equal to the left wall
	int dim_x_right = m_qBounds.m_bL.m_x + m_qBounds.m_dim; // ditto right wall
	int dim_y_top = m_qBounds.m_bL.m_y + m_qBounds.m_dim; //ditto top wall
	int dim_y_bot = m_qBounds.m_bL.m_y; //ditto bottom wall
	cout << "END: Before While" << endl;
	while (it.curr->m_quads[0] != NULL || it.curr->m_quads[1] != NULL || it.curr->m_quads[2] != NULL || it.curr->m_quads[3] != NULL
		|| it.curr->parent != NULL) //keep going unless all children are null or parent is null
	{
		cout << "CHIMP" << endl;
		if (it.curr->m_quads[3] != NULL) //if first is not null, send pointer there
		{
			dim_x_right = dim_x_right / 2; //update wall
			dim_y_top = dim_y_top / 2; //update top wall
			it.curr = it.curr->m_quads[3]; //send pointer there
			it.curr->location = 3; //QTQuad Locator
		}
		else if (it.curr->m_quads[2] != NULL) //and so on
		{
			dim_x_left = dim_x_right / 2; //update left wall
			dim_y_top = dim_y_top / 2; //update top wall
			it.curr = it.curr->m_quads[2]; //send pointer there
			it.curr->location = 2; //location
		}
		else if (it.curr->m_quads[1] != NULL)
		{
			dim_x_left = dim_x_right / 2;
			dim_y_bot = dim_y_top / 2;
			it.curr = it.curr->m_quads[1];
			it.curr->location = 1;
		}
		else if (it.curr->m_quads[0] != NULL)
		{
			dim_x_right = dim_x_right / 2;
			dim_y_bot = dim_y_top / 2;
			it.curr = it.curr->m_quads[0];
			it.curr->location = 0;
		}
	}
	cout << "END:AFTER WHILE" << endl;
	if (it.curr->parent == NULL)
	{
		it.curr = m_root;
		it.currCell = 0;
		//cout << "Raptors!" << endl;
	}
	//now that you've found the earliest node that contains something
	for (int j = 0; j <= QTQ_GRID_DIM - 1; j++) //y-axis
	{
		//cout << "CHIMP" << endl;
		for (int i = QTQ_GRID_DIM - 1; i >= 0; i--) //x-axis
		{
			//cout << "Chuck Norris" << endl;
			if (it.curr->m_cells[i][j] != 0)
			{
				it.currCell = *(it.curr->m_cells + (j*QTQ_GRID_DIM) + i);
				it.x = dim_x_left + i;
				it.y = dim_y_bot + j;
			}
		}
	}
	//cout << "raptors!" << endl;
	cout << it.curr->location << "END: BEFORE RETURN"<< endl;
	return it;
}

const QuadTree::iterator & QuadTree::iterator::operator=(const iterator &rhs)
{
	//cout << "OPERATOR RUNNING" << endl;
	curr = rhs.curr;
	currCell = rhs.currCell;
	root = rhs.root;
	x = rhs.x;
	y = rhs.y;
	//cout << "OPERATOR RUNNING" << endl;
	return rhs;
}


bool QuadTree::iterator::operator==(const QuadTree::iterator &other)
{
	return(curr == other.curr && currCell == other.currCell);
}

bool QuadTree::iterator::operator!=(const QuadTree::iterator &other)
{
	return(curr != other.curr || currCell != other.currCell);
}

QuadTree::iterator QuadTree::iterator::operator++(int dummy) // Postfix: "it++"
{
	QuadTree::iterator result = iterator::iterator();
	//result.curr = curr;

	//get to the next possible location to start with
	int location = curr->location;
	if (location < 3)
	{
		curr = curr->m_quads[location + 1];
		location = location + 1;
	}
	else
	{
		curr = curr->parent;
	}

	//while curr is null, keep looking
	while (curr == NULL)
	{
		location = curr->location;
		if (location < 3)
		{
			curr = curr->m_quads[location + 1];
			location = location + 1;
		}
		else
		{
			curr = curr->parent;
		}
	}
	curr->location = location;
	//while currCell is 0, keep looking
	while (currCell == NULL)
	{
		for (int j = QTQ_GRID_DIM; j > 0; j--)
		{
			for (int i = 0; i < QTQ_GRID_DIM; i++)
			{
				currCell = currCell + j * QTQ_GRID_DIM + i;
			}
		}
	}
	result.curr = curr;
	result.currCell = currCell;
	result.x = x;
	result.y = y;
	return result;
}

QuadTree::iterator & QuadTree::iterator::operator++() // Prefix: e.g. "++it"
{
	iterator it = iterator();
	//QTQuad* temp = curr;
	while (curr == NULL)
	{
		int location = curr->location; //This updates the "location" so that it starts with the correct point
		if (location < 3)
		{
			curr = curr->m_quads[location + 1];
		}
		else
		{
			curr = curr->parent;
		}
	}
	while (currCell == NULL)
	{
		for (int j = QTQ_GRID_DIM; j > 0; j--)
		{
			for (int i = 0; i < QTQ_GRID_DIM; i++)
			{
				currCell = currCell + j * QTQ_GRID_DIM + i;
			}
		}
	}
	//result.curr = curr;
	return it;
}

Point & QuadTree::iterator::operator*()
{
	Point resultPoint = Point::Point(x, y);
	return resultPoint;
}

int QuadTree::get(const Point &pt)
{
	cout << "GET FUNCTION RUNNING" << endl;
	int data = 0;
	int save_x = 0;
	int save_y = 0;
	//cout << "Wedge Antilles" << endl;
	QTQuad * curr;
	curr = m_root;
	int dim_x_left = (int)m_qBounds.m_bL.m_x;
	int dim_x_right = (int)m_qBounds.m_bL.m_x + (int)m_qBounds.m_dim;
	int dim_y_bot = (int)m_qBounds.m_bL.m_y;
	int dim_y_top = (int)m_qBounds.m_bL.m_y + (int)m_qBounds.m_dim;
	cout << "dim_x_left" << dim_x_left << endl;
	cout << "dim_x_right" << dim_x_right << endl;
	//cout << "Wedge Antilles" << endl;
	if (!m_qBounds.inBounds(pt)) //if out of bounds, just return and don't do anything else. 
	{
		return 0;
	}
	//cout << "Past out of bounds" << endl;
	
	cout << "Past our of bounds" << endl;
	cout << dim_x_right - dim_x_left << endl;
	
	while (dim_x_right - dim_x_left > QTQ_GRID_DIM) //narrow down to QTQuad
	{
		cout << "inside while loop" << endl;
		cout << dim_x_right - dim_x_left << endl;
		if (pt.m_x < (dim_x_left + dim_x_right) / 2) //narrow down left and right side.
		{
			dim_x_right = (dim_x_right + dim_x_left) / 2;
			cout << "dim_x_right" << dim_x_right << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2) //narrow down top and bottom sides
			{
				//cout << "Wedge" << endl;
				if (curr->m_quads[0] == NULL)
				{
					curr->m_quads[0] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[0];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
			}
			else
			{
				if (curr->m_quads[2] == NULL)
				{
					curr->m_quads[2] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[2];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
			}
		}
		else if (pt.m_x > (dim_x_right + dim_x_left) / 2)
		{
			dim_x_left = (dim_x_right + dim_x_left) / 2;
			cout << "dim_x_left" << dim_x_left << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2)
			{
				if (curr->m_quads[1] == NULL)
				{
					curr->m_quads[1] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[1];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
				cout << "dim_y_top" << dim_y_top << endl;
			}
			else
			{
				if (curr->m_quads[3] == NULL)
				{
					curr->m_quads[3] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[3];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
				cout << "dim_y_bot" << dim_y_bot << endl;
			}
		}
		else
		{
			return 0;
		}
	}
	cout << "NODE FOUND" << endl;
	cout << "dim_y_bot" << dim_y_bot << endl;
	cout << "dim_x_left" << dim_x_left << endl;
	
	//Now that we have the bottom and left sides, we can search through the m_cells
	
	for (int j = QTQ_GRID_DIM; j >= 0; j--) //y value
	{
		for (int i = 0; i < QTQ_GRID_DIM; i++) //x value
		{
			if (dim_x_left + i == pt.m_x && dim_y_bot + j == pt.m_y) //leftWall+i, bottomWall+j equal to the point
			{
				cout << "Cell Found" << endl;
				cout << pt.m_x << " " << dim_x_left + i << endl;
				cout << pt.m_y << dim_y_bot + j << endl;
				//cout << "DATA SET" << endl;
				//cout << "Skywalker" << endl;
				data=curr->m_cells[i][j] ; //set the data
				cout << "DATA SET" << endl;
				cout << data << endl;
				//currCell = *(curr->m_cells + (j*QTQ_GRID_DIM) + i);//the currCell iterator value is set
																   //x = pt.m_x;
																   //y = pt.m_y;
				save_x = pt.m_x;
				save_y = pt.m_y;
			}
		}
	}
	cout << data << endl;
	return data;
}

void QuadTree::set(const Point &pt, int data)
{
	int save_x=0;
	int save_y=0;
	QTQuad * curr;
	curr = m_root;
	int dim_x_left = (int)m_qBounds.m_bL.m_x;
	int dim_x_right = (int)m_qBounds.m_bL.m_x + (int)m_qBounds.m_dim;
	int dim_y_bot = (int)m_qBounds.m_bL.m_y;
	int dim_y_top = (int)m_qBounds.m_bL.m_y + (int)m_qBounds.m_dim;
	
	if (!m_qBounds.inBounds(pt)) //if out of bounds, just return and don't do anything else. 
	{
		return;
	}
	
	while (dim_x_right - dim_x_left > QTQ_GRID_DIM) //narrow down to QTQuad
	{
		//cout << "inside while loop" << endl;
		if (pt.m_x < (dim_x_left + dim_x_right) / 2) //narrow down left and right side.
		{
			dim_x_right = (dim_x_right + dim_x_left) / 2;
			//cout << "dim_x_right"<<dim_x_right << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2) //narrow down top and bottom sides
			{
				if (curr->m_quads[0] == NULL)
				{
					curr->m_quads[0] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[0];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
			}
			else
			{
				if (curr->m_quads[2] == NULL)
				{
					curr->m_quads[2] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[2];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
			}
		}
		else //if (pt.m_x > (dim_x_right + dim_x_left) / 2)
		{
			dim_x_left = (dim_x_right + dim_x_left) / 2;
			//cout << "dim_x_left" << dim_x_left << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2)
			{
				if (curr->m_quads[1] == NULL)
				{
					curr->m_quads[1] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[1];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
			}
			else
			{
				if (curr->m_quads[3] == NULL)
				{
					curr->m_quads[3] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[3];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
			}
		}
	}
	//cout << "NODE FOUND" << endl;
	//cout << "dim_y_bot"<<dim_y_bot << endl;
	//cout << "dim_x_left" << dim_x_left << endl;
	/*
	Now that we have the bottom and left sides, we can search through the m_cells
	*/
	cout << "Point" << pt.m_x << "," << pt.m_y << endl;
	for (int j = QTQ_GRID_DIM; j >= 0; j--) //y value
	{
		for (int i = 0; i < QTQ_GRID_DIM; i++) //x value
		{
			if (dim_x_left + i == pt.m_x && dim_y_bot + j == pt.m_y) //leftWall+i, bottomWall+j equal to the point
			{
				cout << "CELL FOUND" << endl;
				//cout << "Cell Found" << endl;
				//cout << pt.m_x << " " << dim_x_left+i << endl;
				//cout << pt.m_y << dim_y_bot + j << endl;
				//cout << "Skywalker" << endl;
				curr->m_cells[i][j] = data; //set the data
				save_x = pt.m_x;
				save_y = pt.m_y;
			}
		}
	}
	cout << "SET FUNCTION" << endl;
	//cout << curr->m_cells[pt.m_x][pt.m_y] << endl;
	cout << "SET FUNCTION BEFORE IF" << endl;
	if (curr->m_cells[pt.m_x][pt.m_y] == 0) //IF the current value is set to zero, we have more stuff to do.
	{
		cout << "REFACTOR" << endl;
		for (int i = 0; i < QTQ_GRID_DIM; i++) //scan through x-values
		{
			for (int j = 0; j < QTQ_GRID_DIM; j++) //scan through y-values
			{
				if (curr->m_cells[i][j] == 0) //if you find a zero value
				{
					//it.curr = it.curr->parent;
					//it.curr->location = 0;
					//currCell = *(curr->m_cells + j * QTQ_GRID_DIM + i);
				}
				else //if you find a non-zero vlaue, then your work is done.
				{
					return;
				}
			}
		}
		curr = curr->parent; //you didn't find a valued cell, so you need to jump to the parent.
		curr->location = 0;
		cout << "BEFORE WHILE LOOP" << endl;
		/* Keep going if there are any non-null children AND there is a parent to the current cell*/
		while ((curr->m_quads[0] != NULL || curr->m_quads[1] != NULL || curr->m_quads[2] != NULL || curr->m_quads[3] != NULL) ||
			curr->parent != NULL)
		{
			cout << "INSIDE WHILE LOOP" << endl;
			if (curr->m_quads[0] != NULL) //if the first child is not null, update the current pointer
			{
				curr = curr->m_quads[0];
			}
			else if (curr->m_quads[1] != NULL)
			{
				curr = curr->m_quads[1];
			}
			else if (curr->m_quads[2] != NULL)
			{
				curr = curr->m_quads[2];
			}
			else if (curr->m_quads[3] != NULL)
			{
				curr = curr->m_quads[3];
			}
			else //if there are no non-null children, then update the current pointer to be the parent.
			{
				curr = curr->parent;
				curr->m_quads[curr->location] = NULL; //delete the node departed from
				curr->location = 0; //re-update location
			}
		}
	}
	//cout << "Skywalker" << endl;
	cout << "END OF SET FUNCTION" << endl;
	return;
}

int QuadTree::increment(const Point &pt, int delta)
{
	int returnValue = 0;
	int save_x = 0;
	int save_y = 0;
	//cout << "Wedge Antilles" << endl;
	QTQuad * curr;
	int * currCell;
	curr = m_root;
	int dim_x_left = (int)m_qBounds.m_bL.m_x;
	int dim_x_right = (int)m_qBounds.m_bL.m_x + (int)m_qBounds.m_dim;
	int dim_y_bot = (int)m_qBounds.m_bL.m_y;
	int dim_y_top = (int)m_qBounds.m_bL.m_y + (int)m_qBounds.m_dim;
	cout << "dim_x_left" << dim_x_left << endl;
	cout << "dim_x_right" << dim_x_right << endl;
	//cout << "Wedge Antilles" << endl;
	if (!m_qBounds.inBounds(pt)) //if out of bounds, just return and don't do anything else. 
	{
		return returnValue;
	}
	cout << pt.m_x << endl;
	cout << dim_x_right - dim_x_left << endl;
	while (abs(dim_x_right - dim_x_left) > QTQ_GRID_DIM) //narrow down to QTQuad
	{
		cout << "inside while loop" << endl;
		if (pt.m_x < (dim_x_left + dim_x_right) / 2) //narrow down left and right side.
		{
			dim_x_right = (dim_x_right + dim_x_left) / 2;
			cout << "dim_x_right" << dim_x_right << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2) //narrow down top and bottom sides
			{
				//cout << "Wedge" << endl;
				if (curr->m_quads[0] == NULL)
				{
					curr->m_quads[0] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[0];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
			}
			else
			{
				if (curr->m_quads[2] == NULL)
				{
					curr->m_quads[2] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[2];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
			}
		}
		else if (pt.m_x > (dim_x_right + dim_x_left) / 2)
		{
			dim_x_left = (dim_x_right + dim_x_left) / 2;
			cout << "dim_x_left" << dim_x_left << endl;
			if (pt.m_y < (dim_y_top + dim_y_bot) / 2)
			{
				if (curr->m_quads[1] == NULL)
				{
					curr->m_quads[1] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[1];
				dim_y_top = (dim_y_top + dim_y_bot) / 2;
				cout << "dim_y_top" << dim_y_top << endl;
			}
			else
			{
				if (curr->m_quads[3] == NULL)
				{
					curr->m_quads[3] = new QTQuad(); //CHECK with TA!!!
				}
				curr = curr->m_quads[3];
				dim_y_bot = (dim_y_top + dim_y_bot) / 2;
				cout << "dim_y_bot" << dim_y_bot << endl;
			}
		}
	}
	cout << "NODE FOUND" << endl;
	cout << "dim_y_bot" << dim_y_bot << endl;
	cout << "dim_x_left" << dim_x_left << endl;
	/*
	Now that we have the bottom and left sides, we can search through the m_cells
	*/
	for (int j = QTQ_GRID_DIM; j >= 0; j--) //y value
	{
		for (int i = 0; i < QTQ_GRID_DIM; i++) //x value
		{
			if (dim_x_left + i == pt.m_x && dim_y_bot + j == pt.m_y) //leftWall+i, bottomWall+j equal to the point
			{
				cout << "Cell Found" << endl;
				cout << pt.m_x << " " << dim_x_left + i << endl;
				cout << pt.m_y << dim_y_bot + j << endl;
				//cout << "Skywalker" << endl;
				curr->m_cells[i][j] = curr->m_cells[i][j]+delta; //set the data
				returnValue = curr->m_cells[i][j];
				cout << endl << endl << "RETURN VALUE" << returnValue << endl << endl;
				currCell = *(curr->m_cells + (j*QTQ_GRID_DIM) + i);//the currCell iterator value is set
																   //x = pt.m_x;
																   //y = pt.m_y;
				save_x = pt.m_x;
				save_y = pt.m_y;
			}
		}
	}
	//cout << "Wedge" << endl;
	//cout << save_x << endl;
	//cout << save_y << endl;
	if (curr->m_cells[save_x][save_y] == 0) //IF the current value is set to zero, we have more stuff to do.
	{
		//cout << "REFACTOR" << endl;
		for (int i = 0; i < QTQ_GRID_DIM; i++) //scan through x-values
		{
			for (int j = 0; j < QTQ_GRID_DIM; j++) //scan through y-values
			{
				if (curr->m_cells[i][j] == 0) //if you find a zero value, just update the currCell pointer
				{
					//it.curr = it.curr->parent;
					//it.curr->location = 0;
					currCell = *(curr->m_cells + j * QTQ_GRID_DIM + i);
				}
				else //if you find a non-zero vlaue, then your work is done.
				{
					currCell = *(curr->m_cells + j * QTQ_GRID_DIM + i);
					return returnValue;
				}
			}
		}
		curr = curr->parent; //you didn't find a valued cell, so you need to jump to the parent.
		curr->location = 0;
		/* Keep going if there are any non-null children AND there is a parent to the current cell*/
		while ((curr->m_quads[0] != NULL || curr->m_quads[1] != NULL || curr->m_quads[2] != NULL || curr->m_quads[3] != NULL) ||
			curr->parent != NULL)
		{
			if (curr->m_quads[0] != NULL) //if the first child is not null, update the current pointer
			{
				curr = curr->m_quads[0];
			}
			else if (curr->m_quads[1] != NULL)
			{
				curr = curr->m_quads[1];
			}
			else if (curr->m_quads[2] != NULL)
			{
				curr = curr->m_quads[2];
			}
			else if (curr->m_quads[3] != NULL)
			{
				curr = curr->m_quads[3];
			}
			else //if there are no non-null children, then update the current pointer to be the parent.
			{
				curr = curr->parent;
				curr->m_quads[curr->location] = NULL; //delete the node departed from
				curr->location = 0; //re-update location
			}
		}
	}
	//cout << "Skywalker" << endl;
	return returnValue;
}

void QuadTree::clearAll()
{
	iterator it = begin();
	while (m_root->m_quads[0] != NULL || m_root->m_quads[1] != NULL || m_root->m_quads[2] != NULL || m_root->m_quads[3] != NULL)
	{
		//it++;
		int x = it.curr->x;
		int y = it.curr->y;
		Point deleteptr = Point(x, y);
		set(deleteptr, 0);
	}
	it.curr = m_root;
}

QuadTree::~QuadTree()
{
	clearAll();
	delete m_root;
	m_root = NULL;
}

void QuadTree::dump() {
	cout << "============================================================\n"
		<< "START Dump of quadtree: true bounds = " << m_bounds << "\n"
		<< "------------------------------------------------------------"
		<< endl;
	m_root->dump(m_qBounds);
	cout << "------------------------------------------------------------\n"
		<< "END Dump of quadtree\n"
		<< "============================================================"
		<< endl;
}

