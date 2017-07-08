// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "MyList.h"
#include "Graph_Graph.h"
#include "Position.h"
#include "MapPolygon.h"
#include <algorithm>
#include "Utilities.h"

/**
*
*/
class VoronoiDiagram
{
private:
	struct Circle
	{
		double Radius;
		Position Center;
	};

	struct EdgeTracker
	{
		MyListIterator<int> LeftParabola;
		MyListIterator<int> RightParabola;
		int NodeIndex;
	};

	struct PolygonTracker
	{
		MyList<Edge> Edges;
		MyList<int> Nodes;
	};

	static void ProcessCircleEvents(double aSweepLine, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker,
		MyList<PolygonTracker>& aPolygonTrackerList, MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph);
	static void ProcessSiteEvents(int& aCurrent, double aSweepLine, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker);
	static void ProcessEdgesForParabolaInsertion(const MyListIterator<int>& aInsertLocation, MyList<EdgeTracker>& aEdgeTracker);


	static void InsertNewNode(MyListIterator<int>& aRemoveLocation, const Position& aNewNodePosition, std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList,
		MyList<EdgeTracker>& aEdgeTracker, MyList<PolygonTracker>& aPolygonTrackerList, MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph);

	static void PostProcessing(std::vector<Position>& aPolygonCenters, MyList<int>& aParabolaList, MyList<EdgeTracker>& aEdgeTracker, MyList<PolygonTracker>& aPolygonTrackerList,
		MyList<MapPolygon>& aPolygonList, Graph<Position, int>& aGraph);

	static MyList<EdgeTracker> CreateEdgeTrackers(const MyListIterator<int>& aRemoveLocation,
		const int& aNewNodeIndex, const Position& aNewNodePosition, const std::vector<Position>& aPolygonCenters,
		const MyList<EdgeTracker>& aEdgeTrackerList, MyList<int>& aParabolaList);

	static void CompleteEdges(MyListIterator<int>& aRemoveLocation, const int& aNewNodeIndex, Graph<Position, int>& aGraph,
		MyList<EdgeTracker>& aEdgeTrackerList, MyList<PolygonTracker>& aPolygonTracker, MyList<MapPolygon>& aPolygonList, MyList<int>& aParabolaList);

	static Circle CreateCircle(const Position& a, const Position& b, const Position& c);
	static int getIntersections(const Position& aPosition1, const Position& aPosition2, const double& aSweep, Position& aIntersection1, Position& aIntersection2);
	static double calculateDeterminant(const Position& aA, const Position& aB, const Position& aC);
	static Position calculatePolygonCenter(const MyList<int>& aVertexIndexList, Graph<Position, int>& aGraph);

public:
	static Graph<Position, int> CreateDiagram(const std::vector<Position>& aPolygonCenters, MyList<MapPolygon>& aPolygons);
};
