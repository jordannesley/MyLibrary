/**
*  @file    VoronoiDiagram.cpp
*  @author  Jordan Nesley
**/

#include "VoronoiDiagram.h"

/** The default amount to increment the sweep line every iteration of the fortune algorithm
*/
const double DEFAULT_SWEEP_INCREMENT = 0.1;

/** Default constructor for the VoronoiDiagram
*/
MyLibrary::VoronoiDiagram::VoronoiDiagram()
{
}

/** Concstructor of VoronoiDiagram
* @param aPolygonCenters The number points that define the center of each polygon
* @param aMaxPosition The max x and y coordinates (the upper right corner of the rectangle)
* @param aMinPosition The min x and y coordinates (the bottom left corner of the rectangle)
*/
MyLibrary::VoronoiDiagram::VoronoiDiagram(const std::vector<Position>& aPolygonCenters, const Position& aMaxPosition, const Position& aMinPosition)
{
	m_ParabolaList = std::vector<unsigned>();
	m_PolygonCenters = aPolygonCenters;
}

/** The Fortune Algorithm
* @param aPolygonCenters The centers of the polygons
* @param aMaxPosition The max x and y coordinates (the upper right corner of the rectangle)
* @param aMinPosition The min x and y coordinates (the bottom left corner of the rectangle)
* @param aSweepIncrement The increment that the sweep line moves. This will affect the accuracy of the polygons centers and intersections but will slow down the algorithm
*/
void MyLibrary::VoronoiDiagram::CreateDiagram()
{
	m_ParabolaList = std::vector<unsigned>(3);
	std::list<std::tuple<unsigned, unsigned, unsigned>> lEdgeTracker = std::list<std::tuple<unsigned, unsigned, unsigned>>();

	//Sort the Polygon centers based on their x coordinates
	//lPolygonCenters.sort([](const Position& a, const Position& b)->bool {return a.Y < b.Y; });
	std::sort(m_PolygonCenters.begin(), m_PolygonCenters.end(), [](const Position& a, const Position& b)->bool {return a.Y > b.Y; });

	m_ParabolaList[0] = 0;
	m_ParabolaList[1] = 1;
	m_ParabolaList[0] = 0;
	unsigned lCurrent = 2;
	std::vector<unsigned>::iterator it;
	
	while (lCurrent < m_PolygonCenters.size())
	{
		double lSweepLine = m_PolygonCenters[lCurrent].Y;

		// Process any circle events
		if (m_ParabolaList.size() > 2)
		{
			struct Circle lCircle;
			unsigned lRemoveLocation;
			bool CheckVertix = true;

			while (CheckVertix)
			{
				lCircle = { 0.0, lSweepLine, -1.0 };
				lRemoveLocation = 0;
				for (unsigned lCount = 1; lCount < (m_ParabolaList.size() - 1); lCount++)
				{
					if (m_ParabolaList[lCount - 1] != m_ParabolaList[lCount + 1])
					{
						double lDeterminant = calculateDeterminant(m_PolygonCenters[m_ParabolaList[lCount - 1]], m_PolygonCenters[m_ParabolaList[lCount]], m_PolygonCenters[m_ParabolaList[lCount + 1]]);
						if (lDeterminant < 0.0)
						{
							// should calculate the intersections of the two parabolas, would need to increment the sweep line in this case
							struct Circle lTempCircle = CreateCircle(m_PolygonCenters[m_ParabolaList[lCount - 1]], m_PolygonCenters[m_ParabolaList[lCount]], m_PolygonCenters[m_ParabolaList[lCount + 1]]);
							if (abs(lTempCircle.Center.Y - lSweepLine) >= lTempCircle.Radius)
							{
								if (lCircle.Radius == 0.0 || (lCircle.Radius > 0.0 && abs(lTempCircle.Center.Y - lSweepLine) > abs(lCircle.Center.Y - lSweepLine)))
								{
									lCircle = lTempCircle;
									lRemoveLocation = lCount;
								}
							}
						}
					}
				}
				if (lRemoveLocation != 0)
				{
					// add the new node to the graph
					it = m_ParabolaList.begin() + lRemoveLocation;
					Node<Position> lNewNode = Node<Position>(0, lCircle.Center);
					unsigned lNodeIndex = m_Graph.addNode(lNewNode);
					
					for (std::list<std::tuple<unsigned, unsigned, unsigned>>::iterator ik = lEdgeTracker.begin(); ik != lEdgeTracker.end(); it++)
					{
						// if a previous node is connected to the new node then create the edges.
						if (std::get<0>(*ik)== lRemoveLocation || std::get<1>(*ik) == lRemoveLocation)
						{
							m_Graph.addEdge(std::get<2>(*ik), lNodeIndex, 0);
							m_Graph.addEdge(lNodeIndex, std::get<2>(*ik), 0);
							lEdgeTracker.erase(ik);
						}
					}

					// store the history of the new node so that the edge can be created at a later time
					lEdgeTracker.push_front(std::tuple<unsigned, unsigned, unsigned>(lRemoveLocation-1, lRemoveLocation, lNodeIndex));
					
					// remove the parabola from the beach line
					m_ParabolaList.erase(it);

					// reset the remove location
					lRemoveLocation = 0;
				}
				else
				{
					// no more points need to be removed from the parabola list
					CheckVertix = false;
				}
			} // CheckVertex
		} // Process Circle Events


		Position lOldIntersection;
		// Process the point event
		for (unsigned lCount = 1; lCount < m_ParabolaList.size(); lCount++)
		{
			Position lIntersection1, lIntersection2;
			int lNumberOfIntersections = getIntersections(m_PolygonCenters[m_ParabolaList[lCount - 1]], m_PolygonCenters[m_ParabolaList[lCount]], lSweepLine, lIntersection1, lIntersection2);

			if (lNumberOfIntersections == 0) throw 1;
			if (lNumberOfIntersections == 1)
			{
				if (m_PolygonCenters[lCurrent].X < lIntersection1.X)
				{
					it = m_ParabolaList.begin() + lCount;
					m_ParabolaList.insert(it - 1, lCurrent);

					it = m_ParabolaList.begin() + lCount;
					m_ParabolaList.insert(it - 1, m_ParabolaList[lCount]);
					lCurrent++;

					break;
				}
				lOldIntersection = lIntersection1;
			}
			if (lNumberOfIntersections == 2)
			{
				it = m_ParabolaList.begin() + lCount;
				it = std::find(m_ParabolaList.begin(), it, m_ParabolaList[lCount]);
				if (lCount > 1 && (it < m_ParabolaList.begin() + lCount || lOldIntersection.X > lIntersection1.X))
				{
					if (m_PolygonCenters[lCurrent].X < lIntersection2.X)
					{
						it = m_ParabolaList.begin() + lCount;
						m_ParabolaList.insert(it - 1, lCurrent);

						it = m_ParabolaList.begin() + lCount;
						m_ParabolaList.insert(it - 1, m_ParabolaList[lCount]);
						lCurrent++;

						
						break;
					}
					lOldIntersection = lIntersection2;
				}
				else
				{

					if (m_PolygonCenters[lCurrent].X < lIntersection1.X )
					{
						it = m_ParabolaList.begin() + lCount;
						m_ParabolaList.insert(it - 1, lCurrent);

						it = m_ParabolaList.begin() + lCount;
						m_ParabolaList.insert(it - 1, m_ParabolaList[lCount]);
						lCurrent++;
						break;
					}
					lOldIntersection = lIntersection1;
				}
			}
		}
	}
}

/** Calculate the determinant between two vectors. (Vector one = AB, Vector two = BC)
* @param aA Position one 
* @param aB Position two
* @param aC Position three
* @return The determinant
*/
double MyLibrary::VoronoiDiagram::calculateDeterminant(const Position& aA, const Position& aB, const Position& aC)
{
	return (aB.X - aA.X)*(aC.Y - aB.Y) - (aC.X - aB.X)*(aB.Y - aA.Y);
}

/** Finds the intersections of two parabolas based on their foci.
* @param Position1 Focus of parabola one
* @param Position1 Focus of parabola one
* @param Intersection1 The first intersection (will have an x value less than Intersection2)
* @param Intersection2 The second intersection (will have an x value greater than Intersection1)
* @return The number of intersections (range from 0 - 2)
*/
int MyLibrary::VoronoiDiagram::getIntersections(const Position& aPosition1, const Position& aPosition2, const double& aSweep, Position& aIntersection1, Position& aIntersection2)
{
	int lNumberOfIntersections = 0;
	double lFa = 0.5 * (aPosition1.Y - aSweep);
	double lFb = 0.5 * (aPosition2.Y - aSweep);
	double a = 0.25 * ((1 / lFa) - (1 / lFb));
	double b = 0.5 * ((aPosition2.X / lFb) - (aPosition1.X / lFa));
	double c = aPosition1.Y - aPosition2.Y + lFb - lFa + 0.25 * ((pow(aPosition1.X, 2.0) / lFa) - (pow(aPosition2.X, 2.0) / lFb));
	
	double ltemp1 = pow(b, 2.0) - 4.0*a*c;
	if (ltemp1 < 0.0)
	{
		aIntersection1 = { 0.0 , 0.0 };
		aIntersection2 = { 0.0 , 0.0 };
		return 0;
	}
	else if (ltemp1 == 0.0)
	{
		aIntersection1.X = -b / (2.0*a);
		aIntersection1.Y = (1 / (4.0*lFa))*pow((aIntersection1.X - aPosition1.X), 2.0) + aPosition1.Y - lFa;
		aIntersection2 = { 0.0 , 0.0 };
		return 1;
	}
	else
	{
		double lTemp2 = (-b - sqrt(ltemp1)) / (2.0*a);
		double lTemp3 = (-b + sqrt(ltemp1)) / (2.0*a);
		if (lTemp2 < lTemp3)
		{
			aIntersection1.X = lTemp2;
			aIntersection2.X = lTemp3;
		}
		else
		{
			aIntersection1.X = lTemp3;
			aIntersection2.X = lTemp2;
		}
		aIntersection1.Y = (1 / (4.0*lFa))*pow((aIntersection1.X - aPosition1.X), 2.0) + aPosition1.Y - lFa;
		aIntersection2.Y = (1 / (4.0*lFa))*pow((aIntersection2.X - aPosition1.X), 2.0) + aPosition1.Y - lFa;
		return 2;
	}
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
		lResult.Center.X = (a.X + b.X) / 2;
		lResult.Center.Y = a.Y;
	}
	else if (lDeltaY_AC == 0.0) // point A and point C have same Y coordinate
	{
		lResult.Center.X = (a.X + c.X) / 2;
		lResult.Center.Y = a.Y;
	}
	else
	{
		double lAlpha = -((c.X - a.X) / lDeltaY_AC);
		double lBeta = -((b.X - a.X) / lDeltaY_AB);
		lResult.Center.X = (lAlpha*(a.X + c.X) - lBeta*(a.X + b.X) + b.Y - c.Y)/(2.0*(lAlpha - lBeta));
		lResult.Center.Y = lAlpha*(lResult.Center.X - ((a.X + c.X)/2.0)) + ((a.Y + c.Y)/2.0);
	}

	// calculate the radius
	lResult.Radius = sqrt(pow(lResult.Center.X - a.X,2) + pow(lResult.Center.Y - a.Y, 2));
	return  lResult;
}