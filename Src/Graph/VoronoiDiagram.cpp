/**
*  @file    VoronoiDiagram.cpp
*  @author  Jordan Nesley
**/

#include "VoronoiDiagram.h"

/** The default amount to increment the sweep line every iteration of the fortune algorithm
*/
const double DEFAULT_SWEEP_INCREMENT = 0.1;



/** Creates a Voronoi Diagram
* @param aNumberOfPolygons The number of polygons to create
* @param aSeed The seed for the random number generator
* @param aMaxPosition The max x and y coordinates (the upper right corner of the rectangle)
* @param aMinPosition The min x and y coordinates (the bottom left corner of the rectangle)
*/
MyLibrary::Graph<MyLibrary::Position, int> MyLibrary::VoronoiDiagram::CreateDiagram(const unsigned aNumberOfPolygons, const unsigned aSeed, const Position aMaxPosition, const Position aMinPosition)
{
	Graph<Position, int> lGraph = Graph<Position, int>();
	std::list<int> lParabolaList = std::list<int>();
	std::vector<Position> lRandomPoints = std::vector<Position>(aNumberOfPolygons);

	std::vector<double> lX = Utilities::RandomNumber(aSeed, aNumberOfPolygons, aMaxPosition.X, aMinPosition.X);
	std::vector<double> lY = Utilities::RandomNumber(aSeed, aNumberOfPolygons, aMaxPosition.Y, aMinPosition.Y);

	for (unsigned lCount = 0; lCount < aNumberOfPolygons; lCount++)
	{
		lRandomPoints[lCount].X = lX[lCount];
		lRandomPoints[lCount].Y = lY[lCount];
	}

	return FortuneAlgorithm(lRandomPoints, aMaxPosition, aMinPosition, DEFAULT_SWEEP_INCREMENT);
}

/** Creates a Voronoi Diagram
* @param aNumberOfPolygons The number of polygons to create
* @param aSeed The seed for the random number generator
* @param aMaxPosition The max x and y coordinates (the upper right corner of the rectangle)
* @param aMinPosition The min x and y coordinates (the bottom left corner of the rectangle)
* @param aSweepIncrement The increment that the sweep line moves. This will affect the accuracy of the polygons centers and intersections but will slow down the algorithm
*/
MyLibrary::Graph<MyLibrary::Position, int> MyLibrary::VoronoiDiagram::CreateDiagram(const unsigned aNumberOfPolygons, const unsigned aSeed, const Position aMaxPosition, const Position aMinPosition, const double aSweepIncrement)
{
	Graph<Position, int> lGraph = Graph<Position, int>();
	std::list<int> lParabolaList = std::list<int>();
	std::list<Position> lRandomPoints = std::list<Position>(aNumberOfPolygons);

	// Create the random coordinates of the polygon centers
	std::vector<double> lX = Utilities::RandomNumber(aSeed, aNumberOfPolygons, aMaxPosition.X, aMinPosition.X);
	std::vector<double> lY = Utilities::RandomNumber(aSeed, aNumberOfPolygons, aMaxPosition.Y, aMinPosition.Y);

	// Create the list of random points for the fortune algorithm
	for (unsigned lCount = 0; lCount < aNumberOfPolygons; lCount++)
	{
		Position lTemp = { lX[lCount], lY[lCount] };
		lRandomPoints.push_back(lTemp);
	}

	return FortuneAlgorithm(lRandomPoints, aMaxPosition, aMinPosition, aSweepIncrement);
}

/** The Fortune Algorithm
* @param aPolygonCenters The centers of the polygons
* @param aMaxPosition The max x and y coordinates (the upper right corner of the rectangle)
* @param aMinPosition The min x and y coordinates (the bottom left corner of the rectangle)
* @param aSweepIncrement The increment that the sweep line moves. This will affect the accuracy of the polygons centers and intersections but will slow down the algorithm
*/
MyLibrary::Graph<MyLibrary::Position, int> MyLibrary::VoronoiDiagram::FortuneAlgorithm(std::list<Position>& aPolygonCenters, const Position aMaxPosition, const Position aMinPosition, const double aSweepIncrement)
{
	Graph<Position, int> lResult = Graph<Position, int>();
	std::vector<Position> lParabolaList = std::vector<Position>();

	//Sort the Polygon centers based on their x coordinates
	std::sort(aPolygonCenters.begin(), aPolygonCenters.end(), [](const Position& a, const Position& b)->bool {return a.X > b.Y; });



	double lSweepLine = aMinPosition.X;
	while (lSweepLine <= aMaxPosition.X)
	{
		if (lParabolaList.size() > 2)
		{
			for (int lCount = 0; lCount < (lParabolaList.size() - 2); lCount++)
			{
				struct Circle lTemp = CreateCircle(lParabolaList[lCount], lParabolaList[lCount+1], lParabolaList[lCount+2]);
				if (lTemp.center.X + lTemp.radius <= lSweepLine)
				{
					Node<Position> lNewNode = Node<Position>(0,lParabolaList[lCount+1]);
					lResult.addNode(lNewNode); // adds the voronoi vertex, edges will have to be added when all verticies have been added.
				}
			}
		}

		if (aPolygonCenters.front().X <= lSweepLine)
		{
			for (int lCount = 0; lCount < lParabolaList.size(); lCount++)
			{
					
			}
		}

		lSweepLine += aSweepIncrement;
	}

	return lResult;
}

/** Calculates the intersections between two parabolic functions
* @param a  Focus of parabola one
* @param b  Focus of parabola two
* @param aIntersection1 The Left intersection (x coordinate < aIntersection2 x coordinate)
* @param aIntersection2 The Right intersection (x coordinate > aIntersection1 x coordinate)
* @param The number of intersections (range from 0-2)
*/
int MyLibrary::VoronoiDiagram::ParabolicIntersections(const Position& a, const Position&b, Position& aIntersection1, Position& aIntersection2)
{

}

/** Defines a circle from the three positions
* @param a  Position one
* @param b  Position two
* @param c  Position three
* @return The definition of the circle
*/
MyLibrary::VoronoiDiagram::Circle MyLibrary::VoronoiDiagram::CreateCircle(const Position& a, const Position& b, const Position& c)
{
	struct Circle lResult = { 0.0, {0.0, 0.0} };

	double lDeltaY_AC = c.Y - a.Y;
	double lDeltaY_AB = b.Y - a.Y;

	if (lDeltaY_AB == 0.0) // point A and point B have same Y coordinate
	{
		lResult.center.X = (a.X + b.X) / 2;
		lResult.center.Y = a.Y;
	}
	else if (lDeltaY_AC == 0.0) // point A and point C have same Y coordinate
	{
		lResult.center.X = (a.X + c.X) / 2;
		lResult.center.Y = a.Y;
	}
	else
	{
		double lAlpha = -((c.X - a.X) / lDeltaY_AC);
		double lBeta = -((b.X - a.X) / lDeltaY_AB);
		lResult.center.X = (lAlpha*(a.X + c.X) - lBeta*(a.X + b.X) + b.Y - c.Y)/(2.0*(lAlpha - lBeta));
		lResult.center.Y = lAlpha*(lResult.center.X - ((a.X + c.X)/2.0)) + ((a.Y + c.Y)/2.0);
	}

	// calculate the radius
	lResult.radius = sqrt(pow(lResult.center.X - a.X,2) + pow(lResult.center.Y - a.Y, 2));
	return  lResult;
}