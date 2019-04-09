#include "stdafx.h"
#include "QTQuad.h"
#include "Point.h"

using namespace std;

QTQuad::QTQuad() //think about adding boolean to tell whether leaf node or not. Then add if statement to constructor.
{
	//cout << "QT Quad Constructor" << endl;
	m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
	for (int i = 0; i < QTQ_GRID_DIM; i++)
	{
		//cout << "QT-Constructor-Test" << endl;
		for (int j = 0; j < QTQ_GRID_DIM; j++)
		{
			//cout << "test" << endl;
			//cout << "QT-Constructor-Test" << endl;
			//cout << "test" << endl;
			m_cells[i][j] = 0;
			//cout << m_cells[i][j] << endl;
			//cout << "QT-Constructor-Test" << endl;
		}
	}
	//cout << m_cells[0][0] << endl;
	//location = 5; //location will be used to keep track of a QTQuad Node that needs to be deleted due to being empty.
	x = 0;
	y = 0;
	parent = NULL;
	m_quads[0] = NULL;
	m_quads[1] = NULL;
	m_quads[2] = NULL;
	m_quads[3] = NULL;
	//cout << "End of QTQuad Constructor" << endl;
	//cout << "QT-Constructor-Test" << endl;
}

int QTQuad::get(const Point &pt, const BBox &bounds)
{
	for (int i = 0; i < QTQ_GRID_DIM; i++)
	{
		for (int j = 0; j < QTQ_GRID_DIM; j++)
		{
			if (bounds.m_bL.m_x + i == pt.m_x && bounds.m_bL.m_y + j == pt.m_y)
			{
				return m_cells[i][j];
			}
		}
	}
	/*
	int data = 0;
	int bound_x_left = bounds.m_bL.m_x;
	int bound_x_right = bounds.m_bL.m_x + bounds.m_dim;
	int bound_y_bot = bounds.m_bL.m_y;
	int bound_y_top = bounds.m_bL.m_y + bounds.m_dim;
	while (bound_x_right - bound_x_left > 2)
	{
	if (pt.m_x < (bound_x_right + bound_x_left) / 2)
	{
	bound_x_right = bound_x_right / 2;
	if (pt.m_y < (bound_y_top - bound_y_bot / 2))
	{
	bound_y_top = bound_y_top / 2;
	data = *m_cells[0];
	}
	else if (pt.m_y >(bounds.m_dim / 2))
	{
	bound_y_bot = bound_y_top / 2;
	data = *m_cells[1];
	}
	}
	else if (pt.m_x > (bounds.m_dim - 0) / 2)
	{
	bound_x_left = bound_x_right / 2;
	if (pt.m_y < (bounds.m_dim / 2))
	{
	bound_y_top = bound_y_top / 2;
	data = *m_cells[2];
	}
	else if (pt.m_y >(bounds.m_dim / 2))
	{
	bound_y_bot = bound_y_top / 2;
	data = *m_cells[3];
	}
	}
	}
	return data;
	*/
}

void QTQuad::set(const Point &pt, int data, const BBox &bounds)
{
	for (int i = 0; i < QTQ_GRID_DIM; i++)
	{
		for (int j = 0; j < QTQ_GRID_DIM; j++)
		{
			if (bounds.m_bL.m_x + i == pt.m_x && bounds.m_bL.m_y + j == pt.m_y)
			{
				m_cells[i][j] = data;
				return;
			}
		}
	}
	/*if (pt.m_x < (bounds.m_dim - 0) / 2)
	{
	if (pt.m_y < (bounds.m_dim / 2))
	{
	*m_cells[0] = data;
	}
	else if (pt.m_y >(bounds.m_dim / 2))
	{
	*m_cells[1] = data;
	}
	}
	else if (pt.m_x>(bounds.m_dim - 0) / 2)
	{
	if (pt.m_y < (bounds.m_dim / 2))
	{
	*m_cells[2] = data;
	}
	else if (pt.m_y >(bounds.m_dim / 2))
	{
	*m_cells[3] = data;
	}
	}*/
}

int QTQuad::increment(const Point &pt, int delta, const BBox &bounds)
{
	int dim_x_left = bounds.m_bL.m_x;
	int dim_x_right = bounds.m_bL.m_x + bounds.m_dim;
	int dim_y_bot = bounds.m_bL.m_y;
	int dim_y_top = bounds.m_bL.m_y + bounds.m_dim;
	int save_x;
	int save_y;
	for (int j = QTQ_GRID_DIM; j > 0; j--) //y value
	{
		for (int i = 0; i < QTQ_GRID_DIM; i++) //x value
		{
			if (i + dim_x_left == pt.m_x && j + dim_y_bot == pt.m_y)
			{
				m_cells[i][j] = m_cells[i][j] + delta;
				save_x = i;
				save_y = j;
				return m_cells[i][j];
			}
		}
	}
}

void QTQuad::clearAll()
{
	for (int i = 0; i < 4; i++)
	{
		*m_cells[i] = 0;
	}

}

static const char *dirNames[] = { "Bottom-Left","Bottom-Right","Top-Left","Top-Right" };
void QTQuad::dump(const BBox &bounds)
{
	std::cout << "[QTQuad bounds=" << bounds;
	std::cout << "TEST" << endl;
	if (m_cells == NULL)
	{ // Internal node
		cout << "QT-test" << endl;
		cout << " is INTERNAL:\n";
		for (int q = 0; q < QTQ_NUM_QUADS; q++)
		{
			cout << dirNames[q] << " child:" << endl;
			if (m_quads[q] == NULL) {
				cout << "[NULL]" << endl;
			}
			else
			{
				BBox kidBBox(bounds);  // Use parent's bounds as base

				kidBBox.m_dim /= 2;    // cut it in half
				if (q & 1) {
					kidBBox.m_bL.m_x += kidBBox.m_dim;
				}
				if (q & 2) {
					kidBBox.m_bL.m_y += kidBBox.m_dim;
				}
				m_quads[q]->dump(kidBBox);
			}
		}
	}
	else {
		cout << "QT-test" << endl;
		cout << " is GRID LEAF:\n";
		for (int y = QTQ_GRID_DIM - 1; y >= 0; --y) {
			cout << "[";
			for (int x = 0; x < QTQ_GRID_DIM; x++) {
				char c;

				if (m_cells[x][y] < 0) {
					c = '-';
				}
				else if (m_cells[x][y] > 15) {
					c = '*';
				}
				else if (m_cells[x][y] > 9) {
					c = 'a' + m_cells[x][y] - 10;
				}
				else {
					c = '0' + m_cells[x][y];
				}
				cout << c;
			}
			cout << "]" << endl;
		}
	}
	cout << "]" << endl;
}

QTQuad::~QTQuad()
{
	delete m_quads;
	delete m_cells;
	delete m_quads;
}
