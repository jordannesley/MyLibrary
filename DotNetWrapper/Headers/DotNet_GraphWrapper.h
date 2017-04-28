#pragma once

#include "Graph_Graph.h"
#include "DotNet_NodeWrapper.h"

using namespace System;

#pragma managed

namespace MyLibrary
{
	template<typename T1, typename T2>
	public ref class GraphWrapper
	{
	private:
		Graph<int, int>* m_Graph;

	public:
		GraphWrapper();
		GraphWrapper(const Const array<NodeWrapper<T1>^>^ aNodes);

		NodeWrapper<T1> getNode(const unsigned aNodeIndex);
		array<NodeWrapper<T1>^>^ getAllNodes();
		EdgeData<T2> getEdgeData(const unsigned aStartNode, const unsigned aEndNode);
		array<EdgeData<T2>>^ getAllEdges(const unsigned aNodeIndex);

		unsigned addNode(const NodeWrapper<T1> aNode);
		void addEdge(const unsigned aStartNode, const unsigned aEndNode);
		bool isConnected(const unsigned aStartNode, const unsigned aEndNode);
	};
}