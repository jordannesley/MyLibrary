/**
*  @file    Graph_Graph.h
*  @author  Jordan Nesley
**/

#ifndef GRAPH_H
#define GRAPH_H

#include "Graph_GraphNode.h"
#include "Graph_AdjacentNode.h"
#include "MyList.h"
#include <vector>

#pragma unmanaged


	template<typename T>
	struct EdgeData
	{
		int StartNode;
		int EndNode;
		T Data;
	};

	template<typename T1, typename T2>
	class Graph
	{
	private:
		std::vector<GraphNode<T1>> m_NodeList;
		std::vector<MyList<AdjacentNode<T2>>> m_AdjacentNodeList;

	public:
		Graph();
		Graph(const std::vector<GraphNode<T1>> aGraphNodes);
		Graph(const Graph<T1, T2>& aCopy);

		GraphNode<T1> getNode(const unsigned aGraphNodeIndex) const;
		std::vector<GraphNode<T1>> getAllNodes() const;
		EdgeData<T2> getEdgeData(const unsigned aStartNode, const unsigned aEndNode) const;
		MyList<EdgeData<T2>> getAllEdges(const unsigned aNodeIndex) const;

		unsigned addNode(const GraphNode<T1> aNode);
		void addEdge(const unsigned aStartNode, const unsigned aEndNode, const T2 aData);
		bool isConnected(const unsigned aStartNode, const unsigned aEndNode);

		Graph<T1, T2>& operator=(const Graph<T1, T2>& aRight);
	};

/** Default constructor for Graph
*/
template<typename T1, typename T2>
Graph<T1,T2>::Graph()
{
	// initialize the nodes list and edges
	this->m_NodeList = std::vector<GraphNode<T1>>();
	this->m_AdjacentNodeList = std::vector<MyList<AdjacentNode<T2>>>();
}

/** Constructor for Graph
* @param aNodes The nodes to add for the Graph (node indexes will be different in the graph)
*/
template<typename T1, typename T2>
Graph<T1,T2>::Graph(const std::vector<GraphNode<T1>> aNodes)
{
	this->m_NodeList = std::vector<GraphNode<T1>>(aNodes.size());
	for (unsigned lCount = 0; lCount < aNodes.size(); lCount++)
	{
		this->m_NodeList[lCount] = GraphNode<T1>(lCount, aNodes[lCount].getData());
	}

	// initialize the edges
	this->m_AdjacentNodeList = std::vector<MyList<AdjacentNode<T2>>>(aNodes.size());
}

/** Copy constructor for Graph
* @param aCopy The object to copy
*/
template<typename T1, typename T2>
Graph<T1, T2>::Graph(const Graph<T1, T2>& aCopy)
{
	this->m_NodeList = aCopy.m_NodeList;
	this->m_AdjacentNodeList = aCopy.m_AdjacentNodeList;
}

/** Returns the node of the specified index
* @param aNodeIndex The index of the node
* @return The node
*/
template<typename T1, typename T2>
GraphNode<T1> Graph<T1,T2>::getNode(const unsigned aNodeIndex) const
{
	if (aNodeIndex >= this->m_NodeList.size()) throw 1;
	return this->m_NodeList[aNodeIndex];
}

/** Returns the node of the specified index
* @return All the nodes of the graph
*/
template<typename T1, typename T2>
std::vector<GraphNode<T1>> Graph<T1,T2>::getAllNodes() const
{
	return this->m_NodeList;
}

/** Returns the edge with the specified start node and end node
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
* @return The edge
*/
template<typename T1, typename T2>
EdgeData<T2> Graph<T1, T2>::getEdgeData(const unsigned aStartNode, const unsigned aEndNode) const
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	MyListIterator<AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode].begin();
	EdgeData<T2> lResult = EdgeData<T2>();
	while (lCurrent != this->m_AdjacentNodeList[aStartNode].end())
	{
		if ((*lCurrent).getEndingNodeIndex() == aEndNode)
		{
			lResult.StartNode = aStartNode;
			lResult.EndNode = (*lCurrent).getEndingNodeIndex();
			lResult.Data = (*lCurrent).getData();
			break;
		}
		lCurrent++;
	}

	return lResult;
}

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node
*/
template<typename T1, typename T2>
MyList<EdgeData<T2>> Graph<T1,T2>::getAllEdges(const unsigned aNodeIndex) const
{
	MyListIterator<AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aNodeIndex].begin();
	MyList<EdgeData<T2>> lResult;

	while (lCurrent != this->m_AdjacentNodeList[aNodeIndex].end())
	{
		EdgeData<T2> lTemp;
		lTemp.StartNode = aNodeIndex;
		lTemp.EndNode = (*lCurrent).getEndingNodeIndex();
		lTemp.Data = (*lCurrent).getData();

		lResult.pushBack(lTemp);

		lCurrent++;
	}

	return lResult;
}

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node index of the newly added node
*/
template<typename T1, typename T2>
unsigned Graph<T1,T2>::addNode(const GraphNode<T1> aNode)
{
	GraphNode<T1> lTemp(aNode);
	lTemp.setIndex(this->m_NodeList.size());
	this->m_NodeList.push_back(lTemp);

	this->m_AdjacentNodeList.push_back(MyList<AdjacentNode<T2>>());

	return this->m_NodeList[this->m_NodeList.size() - 1].getIndex();
}

/** Adds an Edge between two nodes to the graph
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
* @param aData The data for the edge to store
*/
template<typename T1, typename T2>
void Graph<T1,T2>::addEdge(const unsigned aStartNode, const unsigned aEndNode, const T2 aData)
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	// If the edge already exists then do nothing.
	if (this->isConnected(aStartNode, aEndNode)) return;

	AdjacentNode<T2> lNew = AdjacentNode<T2>(aEndNode, aData);

	this->m_AdjacentNodeList[aStartNode].pushBack(lNew);
}

/** Returns wither there is an edge exists between the two nodes
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
*/
template<typename T1, typename T2>
bool Graph<T1,T2>::isConnected(const unsigned aStartNode, const unsigned aEndNode)
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	MyListIterator<AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode].begin();
	while (lCurrent != this->m_AdjacentNodeList[aStartNode].end())
	{
		if ((*lCurrent).getEndingNodeIndex() == aEndNode) return true;
		lCurrent++;
	}
	return false;
}

/** The copy operator
* @param aRight The object to copy
* @return The copied object
*/
template<typename T1, typename T2>
Graph<T1, T2>& Graph<T1, T2>::operator=(const Graph<T1, T2>& aRight)
{
	this->m_NodeList = aRight.m_NodeList;
	this->m_AdjacentNodeList = aRight.m_AdjacentNodeList;
	return *(this);
}

#endif