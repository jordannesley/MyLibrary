// Fill out your copyright notice in the Description page of Project Settings.

#include "VoronoiDiagram.h"

/** The Fortune Algorithm
*/
Graph<Position, int> VoronoiDiagram::CreateDiagram(const std::vector<Position>& aPolygonCenters, MyList<MapPolygon>& aPolygons)
{
	MyList<int> m_ParabolaList;
	std::vector<Position> m_PolygonCenters = aPolygonCenters;
	MyList<EdgeTracker> m_EdgeTracker;
	Graph<Position, int> m_Graph;
	MyList<PolygonTracker> m_PolygonTrackerList;
	MyList<MapPolygon> m_PolygonList;
	
	//Sort the Polygon centers based on their x coordinates
	//lPolygonCenters.sort([](const Position& a, const Position& b)->bool {return a.Y < b.Y; });
	//std::sort(m_PolygonCenters.begin(), m_PolygonCenters.end(), [](const Position& a, const Position& b)->bool {return a.Y > b.Y; });
	std::sort(m_PolygonCenters.begin(), m_PolygonCenters.end(),([](const Position& a, const Position& b)->bool {return a.Y > b.Y; }));
	m_ParabolaList.insert(m_ParabolaList.begin(), 0);
	m_ParabolaList.insert(m_ParabolaList.begin(), 1);
	m_ParabolaList.insert(m_ParabolaList.begin(), 0);
	int lCurrent = 2;

	while (lCurrent < m_PolygonCenters.size())
	{
		double lSweepLine = m_PolygonCenters[lCurrent].Y;

		// process any circle events
		ProcessCircleEvents(lSweepLine, m_PolygonCenters, m_ParabolaList, m_EdgeTracker, m_PolygonTrackerList, m_PolygonList, m_Graph);

		// process the new site event
		ProcessSiteEvents(lCurrent, lSweepLine, m_PolygonCenters, m_ParabolaList, m_EdgeTracker);
	}

	PostProcessing(m_PolygonCenters, m_ParabolaList, m_EdgeTracker, m_PolygonTrackerList, m_PolygonList, m_Graph);

	aPolygons = m_PolygonList;
	return m_Graph;
}

/** Finish up creating the diagram by processing any circle events left
*/
void VoronoiDiagram::PostProcessing(std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker, MyList<PolygonTracker>& aPolygonTrackerList, MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph)
{
	MyListIterator<int> lRemoveLocation;
	struct Circle lCircle {};
	bool CheckCircle = true;

	while (CheckCircle)
	{
		lRemoveLocation = aParabolaList.begin();
		for (MyListIterator<int> il = aParabolaList.begin().next(); il != aParabolaList.end().prev(); il++)
		{
			if ((*(il.prev())) != *(il.next()))
			{
				double lDeterminant = calculateDeterminant(aPolygonCenters[*(il.prev())], aPolygonCenters[(*il)], aPolygonCenters[*(il.next())]);
				if (lDeterminant < 0.0)
				{
					// should calculate the intersections of the two parabolas, would need to increment the sweep line in this case
					struct Circle lTempCircle = CreateCircle(aPolygonCenters[*(il.prev())], aPolygonCenters[*(il)], aPolygonCenters[*(il.next())]);
					if (lRemoveLocation == aParabolaList.begin() || (lCircle.Radius > 0.0 && lTempCircle.Center.Y > lCircle.Center.Y))
					{
						lCircle = lTempCircle;
						lRemoveLocation = il;
					}
				}
			}
		}

		if (lRemoveLocation != aParabolaList.begin())
		{
			// add the new node to the graph
			InsertNewNode(lRemoveLocation, lCircle.Center, aPolygonCenters, aParabolaList, aEdgeTracker, aPolygonTrackerList, aPolygonList, aGraph);

			// reset the remove location
			lRemoveLocation = aParabolaList.begin();
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
void VoronoiDiagram::ProcessCircleEvents(double aSweepLine, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker, MyList<PolygonTracker>& aPolygonTrackerList, MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph)
{
	MyListIterator<int> lRemoveLocation;
	struct Circle lCircle;
	bool CheckCircle = true;

	// Process any circle events
	if (aParabolaList.size() > 2)
	{
		while (CheckCircle)
		{
			lCircle = { 0.0, aSweepLine, -1.0 };
			lRemoveLocation = aParabolaList.begin();
			for (MyListIterator<int> il = aParabolaList.begin().next(); il != aParabolaList.end().prev(); il++)
			{
				if (*(il.prev()) != *(il.next()))
				{
					double lDeterminant = calculateDeterminant(aPolygonCenters[*(il.prev())], aPolygonCenters[(*il)], aPolygonCenters[*(il.next())]);
					if (lDeterminant < 0.0)
					{
						// should calculate the intersections of the two parabolas, would need to increment the sweep line in this case
						struct Circle lTempCircle = CreateCircle(aPolygonCenters[*(il.prev())], aPolygonCenters[*(il)], aPolygonCenters[*(il.next())]);
						if (Utilities::Abs(lTempCircle.Center.Y - aSweepLine) >= lTempCircle.Radius)
						{
							if (lRemoveLocation == aParabolaList.begin() || (lCircle.Radius > 0.0 && lTempCircle.Center.Y > lCircle.Center.Y))
							{
								lCircle = lTempCircle;
								lRemoveLocation = il;
							}
						}
					}
				}
			}

			if (lRemoveLocation != aParabolaList.begin())
			{
				// add the new node to the graph
				InsertNewNode(lRemoveLocation, lCircle.Center, aPolygonCenters, aParabolaList, aEdgeTracker, aPolygonTrackerList, aPolygonList, aGraph);

				// reset the remove location
				lRemoveLocation = aParabolaList.begin();
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
void VoronoiDiagram::InsertNewNode(MyListIterator<int>& aRemoveLocation, const Position& aNewNodePosition, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker, MyList<PolygonTracker>& aPolygonTrackerList, MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph)
{
	// add the new node to the graph
	GraphNode<Position> lNewNode = GraphNode<Position>(0, aNewNodePosition);
	int lNodeIndex = aGraph.addNode(lNewNode);

	// calculate the edge trackers
	MyList<EdgeTracker> NewEdges = CreateEdgeTrackers(aRemoveLocation, lNodeIndex, aNewNodePosition, aPolygonCenters, aEdgeTracker, aParabolaList);

	// complete any edges and polygons
	CompleteEdges(aRemoveLocation, lNodeIndex, aGraph, aEdgeTracker, aPolygonTrackerList, aPolygonList, aParabolaList);

	for (auto it = NewEdges.begin(); it != NewEdges.end(); it++)
	{
		aEdgeTracker.pushBack(*it);
	}
}

void VoronoiDiagram::CompleteEdges(MyListIterator<int>& aRemoveLocation, const int& aNewNodeIndex, Graph<Position, int>& aGraph,
	MyList<EdgeTracker>& aEdgeTrackerList, MyList<PolygonTracker>& aPolygonTracker, MyList<MapPolygon>& aPolygonList, MyList<int>& aParabolaList)
{
	{
		bool loop = true;
		bool VectorOrderCondition = false;
		MyList<MyListIterator<PolygonTracker>> EditedPolygons;
		bool PolygonFound = false;
		MyListIterator<EdgeTracker> ik = aEdgeTrackerList.begin();

		while (ik != aEdgeTrackerList.end())
		{
			// if the removed parabola is part of an edge then create the edge in the graph and link the nodes
			if (((*ik).LeftParabola == aRemoveLocation || (*ik).RightParabola == aRemoveLocation))
			{
				aGraph.addEdge((*ik).NodeIndex, aNewNodeIndex, 0);
				aGraph.addEdge(aNewNodeIndex, (*ik).NodeIndex, 0);

				// Search the all of hte Polygon Trackers to see if the new edge is connected to any of the previous polygons
				// It can be at most connected to two edges.
				for (auto ip = aPolygonTracker.begin(); ip != aPolygonTracker.end(); ip++)
				{
					for (auto in = (*ip).Nodes.begin(); in != (*ip).Nodes.end(); in++)
					{
						Edge temp = {};
						if ((*in) == (*ik).NodeIndex)
						{
							bool CorrectPolygon = true;
							for (auto ie = (*ip).Edges.begin(); ie != (*ip).Edges.end(); ie++)
							{
								Position A = aGraph.getNode((*ie).Node1).getData();
								Position B = aGraph.getNode((*ie).Node2).getData();
								Position C = aGraph.getNode(aNewNodeIndex).getData();
								if ((*ik).NodeIndex == (*ie).Node1 || (*ik).NodeIndex == (*ie).Node2) temp = (*ie);

								if (calculateDeterminant(A, B, C) < 0)
								{
									CorrectPolygon = false;
									break;
								}
							}

							if (CorrectPolygon)
							{
								bool Alreadycontaintnode = false;
								for (auto ie = (*ip).Nodes.begin(); ie != (*ip).Nodes.end(); ie++)
								{
									if (*(ie) == aNewNodeIndex)
									{
										Alreadycontaintnode = true;
										break;
									}
								}

								if (!Alreadycontaintnode)(*ip).Nodes.pushBack(aNewNodeIndex);

								if (!PolygonFound) EditedPolygons.pushBack(ip);
								PolygonFound = true;

								if (temp.Node1 == (*ik).NodeIndex)
								{
									(*ip).Edges.pushBack({ aNewNodeIndex, (*ik).NodeIndex });
									VectorOrderCondition = true;
								}
								else
								{
									(*ip).Edges.pushBack({ (*ik).NodeIndex, aNewNodeIndex });
									VectorOrderCondition = false;
								}
								break;
							}
						}
					}
				}

				// remove the edge after it has been created
				ik = aEdgeTrackerList.erase(ik);
			}
			else
			{
				ik++;
			}
		}
	}

	aRemoveLocation = aParabolaList.erase(aRemoveLocation).prev();

	MyList<EdgeData<int>> EdgesCreated = aGraph.getAllEdges(aNewNodeIndex);
	for (auto ik = EdgesCreated.begin(); ik != EdgesCreated.end(); ik++)
	{
		bool CreatePolygon1 = true;
		bool CreatePolygon2 = true;
		bool PolygonFound = false;
		for (auto ip = aPolygonTracker.begin(); ip != aPolygonTracker.end(); ip++)
		{
			for (auto ie = (*ip).Edges.begin(); ie != (*ip).Edges.end(); ie++)
			{
				if ((*ie).Node1 == (*ik).StartNode && (*ie).Node2 == (*ik).EndNode)
				{
					CreatePolygon1 = false;
				}
				if ((*ie).Node1 == (*ik).EndNode && (*ie).Node2 == (*ik).StartNode)
				{
					CreatePolygon2 = false;
				}
			}
		}
		if (CreatePolygon1)
		{
			PolygonTracker NewPolygon;
			NewPolygon.Nodes.pushBack((*ik).StartNode);
			NewPolygon.Nodes.pushBack((*ik).EndNode);
			NewPolygon.Edges.pushBack({ (*ik).StartNode  , (*ik).EndNode });
			aPolygonTracker.pushBack(NewPolygon);
		}
		if (CreatePolygon2)
		{
			PolygonTracker NewPolygon;
			NewPolygon.Nodes.pushBack((*ik).StartNode);
			NewPolygon.Nodes.pushBack((*ik).EndNode);
			NewPolygon.Edges.pushBack({ (*ik).EndNode, (*ik).StartNode });
			aPolygonTracker.pushBack(NewPolygon);
		}
	}

	// Check to see if any of the polygons are complete
	auto it = aPolygonTracker.begin();
	while (it != aPolygonTracker.end())
	{
		if ((*it).Nodes.size() == (*it).Edges.size())
		{
			// add the new polygon to the polygon list
			MapPolygon NewPolygon = {};
			NewPolygon.Nodes = (*it).Nodes;
			NewPolygon.Edges = (*it).Edges;

			aPolygonList.pushBack(NewPolygon);
			it = aPolygonTracker.erase(it);
		}
		else
		{
			it++;
		}
	}
}

MyList<VoronoiDiagram::EdgeTracker> VoronoiDiagram::CreateEdgeTrackers(const MyListIterator<int>& aRemoveLocation, const int& aNewNodeIndex, const Position& aNewNodePosition, const std::vector<Position>& aPolygonCenters, const MyList<EdgeTracker>& aEdgeTrackerList, MyList<int>& aParabolaList)
{
	MyList<EdgeTracker> Result;
	EdgeTracker Edge1, Edge2, Edge3;
	int Edge2Left, Edge2Right;
	int Edge3Left, Edge3Right;
	bool Edge2Valid = false, Edge3Valid = false;

	Edge1.LeftParabola = aRemoveLocation.prev();
	Edge1.RightParabola = aRemoveLocation.next();
	Edge1.NodeIndex = aNewNodeIndex;
	Result.pushBack(Edge1);

	if (aPolygonCenters[*aRemoveLocation.prev()].Y < aPolygonCenters[*aRemoveLocation.next()].Y)
	{
		if (aPolygonCenters[*aRemoveLocation].X < aNewNodePosition.X)
		{
			Edge2Left = *aRemoveLocation;
			Edge2Right = *aRemoveLocation.prev();

			Edge3Left = *aRemoveLocation.next();
			Edge3Right = *aRemoveLocation;
		}
		else
		{
			Edge2Left = *aRemoveLocation;
			Edge2Right = *aRemoveLocation.prev();

			Edge3Left = *aRemoveLocation.next();
			Edge3Right = *aRemoveLocation;
		}
	}
	else
	{
		if (aPolygonCenters[*aRemoveLocation].X < aNewNodePosition.X)
		{
			Edge2Left = *aRemoveLocation;
			Edge2Right = *aRemoveLocation.prev();

			Edge3Left = *aRemoveLocation.next();
			Edge3Right = *aRemoveLocation;
		}
		else
		{
			Edge2Left = *aRemoveLocation.next();
			Edge2Right = *aRemoveLocation;

			Edge3Left = *aRemoveLocation;
			Edge3Right = *aRemoveLocation.prev();
		}
	}

	for (auto it = aParabolaList.begin(); it != aParabolaList.end().prev(); it++)
	{
		if ((*it) == Edge2Left && (*it.next() == Edge2Right))
		{
			Edge2Valid = true;
			Edge2.LeftParabola = it;
			Edge2.RightParabola = it.next();
			Edge2.NodeIndex = aNewNodeIndex;
		}
		if ((*it) == Edge3Left && (*it.next() == Edge3Right))
		{
			Edge3Valid = true;
			Edge3.LeftParabola = it;
			Edge3.RightParabola = it.next();
			Edge3.NodeIndex = aNewNodeIndex;
		}
	}
	if (Edge2Valid) Result.pushBack(Edge2);
	if (Edge3Valid) Result.pushBack(Edge3);

	return Result;
}

/** Perform the processing needed to add a new parabola to the list
* @param aCurrent The location in the polygon center list of the new parabola focus
* @param aSweepLine The value of the sweep line
*/
void VoronoiDiagram::ProcessSiteEvents(int& aCurrent, double aSweepLine, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker)
{
	MyListIterator<int> it;
	Position lOldIntersection = {};
	bool lInserted = false;

	// Process the point event
	for (MyListIterator<int> ik = aParabolaList.begin().next(); ik != aParabolaList.end(); ik++)
	{
		Position lIntersection1, lIntersection2;
		int lNumberOfIntersections = getIntersections(aPolygonCenters[*(ik.prev())], aPolygonCenters[*(ik)], aSweepLine, lIntersection1, lIntersection2);

		if (lNumberOfIntersections == 0) throw 1; // same x, different y
		if (lNumberOfIntersections == 1) // same y, different x
		{
			if (aPolygonCenters[aCurrent].X < lIntersection1.X)
			{
				it = aParabolaList.insert(ik, aCurrent);
				aParabolaList.insert(ik, *(it.next()));
				ProcessEdgesForParabolaInsertion(it, aEdgeTracker);
				aCurrent++;
				lInserted = true;
				break;
			}
			lOldIntersection = lIntersection1;
		}
		if (lNumberOfIntersections == 2)
		{
			it = aParabolaList.find(aParabolaList.begin(), ik, *ik);
			if (ik != aParabolaList.begin().next() && (it != ik || lOldIntersection.X > lIntersection1.X))
			{
				if (aPolygonCenters[aCurrent].X < lIntersection2.X)
				{
					it = aParabolaList.insert(ik, aCurrent);
					aParabolaList.insert(ik, *(it.prev()));
					ProcessEdgesForParabolaInsertion(it, aEdgeTracker);
					aCurrent++;
					lInserted = true;
					break;
				}
				lOldIntersection = lIntersection2;
			}
			else
			{
				if (aPolygonCenters[aCurrent].X < lIntersection1.X)
				{
					it = aParabolaList.insert(ik, aCurrent);
					aParabolaList.insert(ik, *(it.prev()));
					ProcessEdgesForParabolaInsertion(it, aEdgeTracker);
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
		it = aParabolaList.end();
		it = aParabolaList.insert(aParabolaList.end(), aCurrent);
		aParabolaList.insert(aParabolaList.end(), *it.prev());
		ProcessEdgesForParabolaInsertion(it, aEdgeTracker);
		aCurrent++;
	}
}

/** Perform any processing on the current edges for a new parabola in the list
* @parama InsertLocation The location in the parabola list of the new parabola
*/
void VoronoiDiagram::ProcessEdgesForParabolaInsertion(const MyListIterator<int>& aInsertLocation, MyList<EdgeTracker>& aEdgeTracker)
{
	// If the left parabola of any edge is equal to the left of the new insertion then
	// set the letf parabola of the edge to the parabola to the edge after the insertion
	for (MyListIterator<EdgeTracker> ik = aEdgeTracker.begin(); ik != aEdgeTracker.end(); ik++)
	{
		if ((*ik).LeftParabola == aInsertLocation.prev())
		{
			(*ik).LeftParabola = aInsertLocation.next();
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
double VoronoiDiagram::calculateDeterminant(const Position& aA, const Position& aB, const Position& aC)
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
int VoronoiDiagram::getIntersections(const Position& aPosition1, const Position& aPosition2, const double& aSweep, Position& aIntersection1, Position& aIntersection2)
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
VoronoiDiagram::Circle VoronoiDiagram::CreateCircle(const Position& a, const Position& b, const Position& c)
{
	struct Circle lResult = { 0.0,{ 0.0, 0.0 } };

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
		lResult.Center.X = (lAlpha*(a.X + c.X) - lBeta*(a.X + b.X) + b.Y - c.Y) / (2.0*(lAlpha - lBeta));
		lResult.Center.Y = lAlpha*(lResult.Center.X - ((a.X + c.X) / 2.0)) + ((a.Y + c.Y) / 2.0);
	}

	// calculate the radius
	lResult.Radius = sqrt(pow(lResult.Center.X - a.X, 2) + pow(lResult.Center.Y - a.Y, 2));
	return  lResult;
}

/** Calculates the center of a polygon
* @param aVertexIndexList The indicies of the vertex of the polygon
* @param aGraph The graph
* @return The center of the polygon
*/
Position VoronoiDiagram::calculatePolygonCenter(const MyList<int>& aVertexIndexList, Graph<Position, int>& aGraph)
{
	Position lResult = { 0.0, 0.0 };
	for (auto it = aVertexIndexList.begin(); it != aVertexIndexList.end(); it++)
	{
		lResult.X += aGraph.getNode((*it)).getData().X;
		lResult.Y += aGraph.getNode((*it)).getData().Y;
	}
	lResult.X = lResult.X / aVertexIndexList.size();
	lResult.Y = lResult.Y / aVertexIndexList.size();
	return lResult;
}
