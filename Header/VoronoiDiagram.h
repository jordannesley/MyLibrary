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
#include <tuple>
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
			double Radius;
			Position Center;
		};

		struct EdgeTracker
		{
			std::list<unsigned>::iterator LeftParabola;
			std::list<unsigned>::iterator RightParabola;
			unsigned NodeIndex;
		};

		std::list<unsigned> m_ParabolaList;
		std::vector<Position> m_PolygonCenters;
		std::list<EdgeTracker> m_EdgeTracker;
		Graph<Position, int> m_Graph;
		Position m_MaxPosition;
		Position m_MinPosition;
		
		void ProcessCircleEvents(double aSweepLine);
		void ProcessSiteEvents(unsigned& aCurrent, double aSweepLine);
		void ProcessEdgesForParabolaInsertion(const std::list<unsigned>::iterator& aInsertLocation);
		void InsertNewNode(const std::list<unsigned>::iterator& aRemoveLocation, const Position& aNewNodePosition);
		void PostProcessing();

		static Circle CreateCircle(const Position& a, const Position& b, const Position& c);
		static int getIntersections(const MyLibrary::Position& aPosition1, const MyLibrary::Position& aPosition2, const double& aSweep, MyLibrary::Position& aIntersection1, MyLibrary::Position& aIntersection2);
		static double calculateDeterminant(const MyLibrary::Position& aA, const MyLibrary::Position& aB, const MyLibrary::Position& aC);
	public:
		
		VoronoiDiagram();
		VoronoiDiagram(const std::vector<Position>& aPolygonCenters);

		void CreateDiagram();
	};
}

#endif