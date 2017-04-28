/**
*  @file    VoronoiDiagram.h
*  @author  Jordan Nesley
**/

#ifndef VORONOIDIAGRAM_H
#define VORONOIDIAGRAM_H

#include "Graph_Graph.h"
#include "Utilities.h"
#include <list>
#include <algorithm>
#include <math.h>

namespace MyLibrary
{
	struct Position
	{
		double X;
		double Y;
	};

	class VoronoiDiagram
	{
	private:
		struct Circle
		{
			double radius;
			Position center;
		};

		static Graph<Position, int> FortuneAlgorithm(std::list<Position>& aPolygonCenters,  const Position aMaxPosition, const Position aMinPosition, const double aSweepIncrement);
		static Circle CreateCircle(const Position& a, const Position& b, const Position& c);
		static int ParabolicIntersections(const Position& a, const Position&b, Position& aIntersection1, Position& aIntersection2);
	public:
	
		static Graph<Position, int> CreateDiagram(const unsigned aNumberOfPolygons, const unsigned aSeed, const Position aMaxPosition, const Position aMinPosition, const double aSweepIncrement);
		static Graph<Position, int> CreateDiagram(const unsigned aNumberOfPolygons, const unsigned aSeed, const Position aMaxPosition, const Position aMinPosition);
	};
}

#endif