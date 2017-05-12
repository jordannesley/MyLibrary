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
*/
MyLibrary::VoronoiDiagram::VoronoiDiagram(const std::vector<Position>& aPolygonCenters)
{
	m_ParabolaList = std::list<unsigned>();
	m_PolygonCenters = aPolygonCenters;
}

/** The Fortune Algorithm
*/
void MyLibrary::VoronoiDiagram::CreateDiagram()
{
	m_ParabolaList = std::list<unsigned>();
	m_EdgeTracker = std::list<EdgeTracker>();

	//Sort the Polygon centers based on their x coordinates
	//lPolygonCenters.sort([](const Position& a, const Position& b)->bool {return a.Y < b.Y; });
	std::sort(m_PolygonCenters.begin(), m_PolygonCenters.end(), [](const Position& a, const Position& b)->bool {return a.Y > b.Y; });

	m_ParabolaList.insert(m_ParabolaList.begin(), 0);
	m_ParabolaList.insert(m_ParabolaList.begin(), 1);
	m_ParabolaList.insert(m_ParabolaList.begin(), 0);
	unsigned lCurrent = 2;

	while (lCurrent < m_PolygonCenters.size())
	{
		double lSweepLine = m_PolygonCenters[lCurrent].Y;

		// process any circle events
		ProcessCircleEvents(lSweepLine);

		// process the new site event
		ProcessSiteEvents(lCurrent, lSweepLine);
	}

	PostProcessing();
}

/** Finish up creating the diagram by processing any circle events left
*/
void MyLibrary::VoronoiDiagram::PostProcessing()
{
	std::list<unsigned>::iterator lRemoveLocation;
	struct Circle lCircle;
	bool CheckCircle = true;

	while (CheckCircle)
	{
		lRemoveLocation = m_ParabolaList.begin();
		for (std::list<unsigned>::iterator il = std::next(m_ParabolaList.begin()); il != std::prev(m_ParabolaList.end()); il++)
		{
			if ((*std::prev(il)) != (*std::next(il)))
			{
				double lDeterminant = calculateDeterminant(m_PolygonCenters[*std::prev(il)], m_PolygonCenters[(*il)], m_PolygonCenters[*std::next(il)]);
				if (lDeterminant < 0.0)
				{
					// should calculate the intersections of the two parabolas, would need to increment the sweep line in this case
					struct Circle lTempCircle = CreateCircle(m_PolygonCenters[*std::prev(il)], m_PolygonCenters[*(il)], m_PolygonCenters[*std::next(il)]);
					if (lRemoveLocation == m_ParabolaList.begin() || (lCircle.Radius > 0.0 && lTempCircle.Center.Y > lCircle.Center.Y))
					{
						lCircle = lTempCircle;
						lRemoveLocation = il;
					}
				}
			}
		}

		if (lRemoveLocation != m_ParabolaList.begin())
		{
			// add the new node to the graph
			InsertNewNode(lRemoveLocation, lCircle.Center);

			// reset the remove location
			lRemoveLocation = m_ParabolaList.begin();
		}
		else
		{
			// no more points need to be removed from the parabola list
			CheckCircle = false;
		}
	}
}

/** Process any circle events in the parabola list
* @param aSweepLine The value of the sweep line
*/
void MyLibrary::VoronoiDiagram::ProcessCircleEvents(double aSweepLine)
{
	std::list<unsigned>::iterator lRemoveLocation;
	struct Circle lCircle;
	bool CheckCircle = true;

	// Process any circle events
	if (m_ParabolaList.size() > 2)
	{
		while (CheckCircle)
		{
			lCircle = { 0.0, aSweepLine, -1.0 };
			lRemoveLocation = m_ParabolaList.begin();
			for (std::list<unsigned>::iterator il = std::next(m_ParabolaList.begin()); il != std::prev(m_ParabolaList.end()); il++)
			{
				if ((*std::prev(il)) != (*std::next(il)))
				{
					double lDeterminant = calculateDeterminant(m_PolygonCenters[*std::prev(il)], m_PolygonCenters[(*il)], m_PolygonCenters[*std::next(il)]);
					if (lDeterminant < 0.0)
					{
						// should calculate the intersections of the two parabolas, would need to increment the sweep line in this case
						struct Circle lTempCircle = CreateCircle(m_PolygonCenters[*std::prev(il)], m_PolygonCenters[*(il)], m_PolygonCenters[*std::next(il)]);
						if (abs(lTempCircle.Center.Y - aSweepLine) >= lTempCircle.Radius)
						{
							if (lRemoveLocation == m_ParabolaList.begin() || (lCircle.Radius > 0.0 && lTempCircle.Center.Y > lCircle.Center.Y))
							{
								lCircle = lTempCircle;
								lRemoveLocation = il;
							}
						}
					}
				}
			}


			if (lRemoveLocation != m_ParabolaList.begin())
			{
				// add the new node to the graph
				InsertNewNode(lRemoveLocation, lCircle.Center);

				// reset the remove location
				lRemoveLocation = m_ParabolaList.begin();
			}
			else
			{
				// no more points need to be removed from the parabola list
				CheckCircle = false;
			}
		}
	}
}

/** Perform the processing needed to insert a new node into the graph
* @param aRemoveLocation The location in the parabola list of the parabla being removed
* @param aNewNodePosition The position of the new node
*/
void MyLibrary::VoronoiDiagram::InsertNewNode(const std::list<unsigned>::iterator& aRemoveLocation, const Position& aNewNodePosition)
{
	// add the new node to the graph
	Node<Position> lNewNode = Node<Position>(0, aNewNodePosition);
	unsigned lNodeIndex = m_Graph.addNode(lNewNode);

	EdgeTracker lNewEdge1, lNewEdge2;
	lNewEdge1.NodeIndex = lNodeIndex;
	lNewEdge2.NodeIndex = lNodeIndex;

	Position temp = m_PolygonCenters[*std::prev(aRemoveLocation)].Y < m_PolygonCenters[*std::next(aRemoveLocation)].Y ? m_PolygonCenters[*std::prev(aRemoveLocation)] : m_PolygonCenters[*std::next(aRemoveLocation)];
	if (temp.X < aNewNodePosition.X)
	{
		lNewEdge2.LeftParabola = std::prev(std::prev(aRemoveLocation));
		lNewEdge2.RightParabola = std::prev(aRemoveLocation);

		lNewEdge1.LeftParabola = std::prev(aRemoveLocation);
		lNewEdge1.RightParabola = std::next(aRemoveLocation);
	}
	else
	{
		lNewEdge1.LeftParabola = std::prev(aRemoveLocation);
		lNewEdge1.RightParabola = std::next(aRemoveLocation);

		lNewEdge2.LeftParabola = std::next(aRemoveLocation);
		lNewEdge2.RightParabola = std::next(std::next(aRemoveLocation));
	}

	// Add edges that are now complete to the graph
	{
		std::list<std::list<EdgeTracker>::iterator> lRemoveList;
		for (std::list<EdgeTracker>::iterator ik = m_EdgeTracker.begin(); ik != m_EdgeTracker.end(); ik++)
		{
			if ((*ik).LeftParabola == aRemoveLocation || (*ik).RightParabola == aRemoveLocation)
			{
				m_Graph.addEdge((*ik).NodeIndex, lNodeIndex, 0);
				m_Graph.addEdge(lNodeIndex, (*ik).NodeIndex, 0);
				lRemoveList.push_front(ik);
			}
		}
		while (!lRemoveList.empty())
		{
			m_EdgeTracker.erase(lRemoveList.front());
			lRemoveList.pop_front();
		}
	}

	// store the history of the new node so that the edge can be created at a later time
	m_EdgeTracker.push_front(lNewEdge1);
	m_EdgeTracker.push_front(lNewEdge2);

	// remove the parabola from the beach line
	m_ParabolaList.erase(aRemoveLocation);
}

/** Perform the processing needed to add a new parabola to the list
* @param aCurrent The location in the polygon center list of the new parabola focus
* @param aSweepLine The value of the sweep line
*/
void MyLibrary::VoronoiDiagram::ProcessSiteEvents(unsigned& aCurrent, double aSweepLine)
{
	std::list<unsigned>::iterator it;
	Position lOldIntersection;
	bool lInserted = false;

	// Process the point event
	for (std::list<unsigned>::iterator ik = std::next(m_ParabolaList.begin()); ik != m_ParabolaList.end(); ik++)
	{
		Position lIntersection1, lIntersection2;
		int lNumberOfIntersections = getIntersections(m_PolygonCenters[*std::prev(ik)], m_PolygonCenters[*(ik)], aSweepLine, lIntersection1, lIntersection2);

		if (lNumberOfIntersections == 0) throw 1; // same x, different y
		if (lNumberOfIntersections == 1) // same y, different x
		{
			if (m_PolygonCenters[aCurrent].X < lIntersection1.X)
			{
				it = m_ParabolaList.insert(ik, aCurrent);
				m_ParabolaList.insert(ik, *std::prev(it));
				ProcessEdgesForParabolaInsertion(it);
				aCurrent++;
				lInserted = true;
				break;
			}
			lOldIntersection = lIntersection1;
		}
		if (lNumberOfIntersections == 2)
		{
			it = std::find(m_ParabolaList.begin(), ik, *ik);
			if (ik != std::next(m_ParabolaList.begin()) && (it != ik || lOldIntersection.X > lIntersection1.X))
			{
				if (m_PolygonCenters[aCurrent].X < lIntersection2.X)
				{
					it = m_ParabolaList.insert(ik, aCurrent);
					m_ParabolaList.insert(ik, *std::prev(it));
					ProcessEdgesForParabolaInsertion(it);
					aCurrent++;
					lInserted = true;
					break;
				}
				lOldIntersection = lIntersection2;
			}
			else
			{
				if (m_PolygonCenters[aCurrent].X < lIntersection1.X)
				{
					it = m_ParabolaList.insert(ik, aCurrent);
					m_ParabolaList.insert(ik, *std::prev(it));
					ProcessEdgesForParabolaInsertion(it);
					aCurrent++;
					lInserted = true;
					break;
				}
				lOldIntersection = lIntersection1;
			}
		}
	}
	if (!lInserted)
	{
		// insert it on top of the last parabola
		it = std::prev(m_ParabolaList.end());
		it = m_ParabolaList.insert(it, aCurrent);
		m_ParabolaList.insert(it, *std::prev(it));
		ProcessEdgesForParabolaInsertion(it);
		aCurrent++;
	}
}

/** Perform any processing on the current edges for a new parabola in the list
* @parama InsertLocation The location in the parabola list of the new parabola
*/
void MyLibrary::VoronoiDiagram::ProcessEdgesForParabolaInsertion(const std::list<unsigned>::iterator& aInsertLocation)
{
	// If the left parabola of any edge is equal to the left of the new insertion then
	// set the letf parabola of the edge to the parabola to the edge after the insertion
	for (std::list<EdgeTracker>::iterator ik = m_EdgeTracker.begin(); ik != m_EdgeTracker.end(); ik++)
	{
		if ((*ik).LeftParabola == std::prev(aInsertLocation))
		{
			(*ik).LeftParabola = std::next(aInsertLocation);
			break;
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