#pragma once
#include "Position.h"
#include "MyList.h"

struct Edge
{
	int Node1;
	int Node2;
};

/**
*
*/
struct MapPolygon
{
	MyList<int> Nodes;
	MyList<Edge> Edges;
	Position Center;
};