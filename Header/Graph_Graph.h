/**
*  @file    Graph_Graph.h
*  @author  Jordan Nesley
**/

#ifndef GRAPH_H
#define GRAPH_H

#include "Graph_Node.h"
#include "Graph_AdjacentNode.h"
#include <memory>
#include <vector>

#pragma unmanaged


namespace MyLibrary {

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
		std::vector<Node<T1>> m_NodeList;
		std::vector<std::shared_ptr<MyLibrary::AdjacentNode<T2>>> m_AdjacentNodeList;
	public:
		Graph();
		Graph(const std::vector<Node<T1>> aNodes);
		Graph(const Graph<T1, T2>& aCopy);
		Graph(Graph<T1, T2>&& aMove);

		Node<T1> getNode(const unsigned aNodeIndex) const;
		std::vector<Node<T1>> getAllNodes() const;
		EdgeData<T2> getEdgeData(const unsigned aStartNode, const unsigned aEndNode) const;
		std::vector<EdgeData<T2>> getAllEdges(const unsigned aNodeIndex) const;

		unsigned addNode(const Node<T1> aNode);
		void addEdge(const unsigned aStartNode, const unsigned aEndNode, const T2 aData);
		bool isConnected(const unsigned aStartNode, const unsigned aEndNode);
	};
};

/** Default constructor for Graph
*/
template<typename T1, typename T2>
MyLibrary::Graph<T1,T2>::Graph()
{
	// initialize the nodes list and edges
	this->m_NodeList = std::vector<Node<T1>>();
	this->m_AdjacentNodeList = std::vector<std::shared_ptr<MyLibrary::AdjacentNode<T2>>>();
}

/** Constructor for Graph
* @param aNodes The nodes to add for the Graph (node indexes will be different in the graph)
*/
template<typename T1, typename T2>
MyLibrary::Graph<T1,T2>::Graph(const std::vector<Node<T1>> aNodes)
{
	this->m_NodeList = std::vector<Node<T1>>(aNodes.size());
	for (unsigned lCount = 0; lCount < aNodes.size(); lCount++)
	{
		this->m_NodeList[lCount] = Node<T1>(lCount, aNodes[lCount].getData());
	}

	// initialize the edges
	this->m_AdjacentNodeList = std::vector<std::shared_ptr<MyLibrary::AdjacentNode<T2>>>(aNodes.size(), std::shared_ptr<MyLibrary::AdjacentNode<T2>>());
}

/** Copy Constructor for Graph
* @param aCopy The object to copy
*/
template<typename T1, typename T2>
MyLibrary::Graph<T1, T2>::Graph(const Graph<T1, T2>& aCopy)
{
	this->m_NodeList = aCopy.m_NodeList;
	this->m_AdjacentNodeList = std::vector<AdjacentNode<T2>>(aCopy.m_AdjacentNodeList.size());
	for (unsigned lCount = 0; lCount < this->m_AdjacentNodeList.size(); lCount++)
	{
		std::shared_ptr<MyLibrary::AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode];
		while (lCurrent != nullptr)
		{
			
		}
	}
}

/** Move Constructor for Graph
* @param aMove The object to move
*/
template<typename T1, typename T2>
MyLibrary::Graph<T1, T2>::Graph(Graph<T1, T2>&& aMove)
{

}

/** Returns the node of the specified index
* @param aNodeIndex The index of the node
* @return The node
*/
template<typename T1, typename T2>
MyLibrary::Node<T1> MyLibrary::Graph<T1,T2>::getNode(const unsigned aNodeIndex) const
{
	if (aNodeIndex >= this->m_NodeList.size()) throw 1;
	return this->m_NodeList[aNodeIndex];
}

/** Returns the node of the specified index
* @return All the nodes of the graph
*/
template<typename T1, typename T2>
std::vector<MyLibrary::Node<T1>> MyLibrary::Graph<T1,T2>::getAllNodes() const
{
	return this->m_NodeList;
}

/** Returns the edge with the specified start node and end node
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
* @return The edge
*/
template<typename T1, typename T2>
MyLibrary::EdgeData<T2> MyLibrary::Graph<T1, T2>::getEdgeData(const unsigned aStartNode, const unsigned aEndNode) const
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	std::shared_ptr<MyLibrary::AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode];
	EdgeData<T2> lResult = EdgeData<T2>();
	while (lCurrent != nullptr)
	{
		if (lCurrent->getEndingNodeIndex() == aEndNode)
		{
			lResult.StartNode = aStartNode;
			lResult.EndNode = lCurrent->getEndingNodeIndex();
			lResult.Data = lCurrent->getData();
			break;
		}
		lCurrent = lCurrent->getNext();
	}

	return lResult;
}

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node
*/
template<typename T1, typename T2>
std::vector<MyLibrary::EdgeData<T2>> MyLibrary::Graph<T1,T2>::getAllEdges(const unsigned aNodeIndex) const
{
	std::shared_ptr<MyLibrary::AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aNodeIndex];
	std::vector<EdgeData<T2>> lResult = std::vector<EdgeData<T2>>();
	while (lCurrent != nullptr)
	{
		EdgeData<T2> lTemp;
		lTemp.StartNode = aNodeIndex;
		lTemp.EndNode = lCurrent->getEndingNodeIndex();
		lTemp.Data = lCurrent->getData();
		lResult.push_back(lTemp);

		lCurrent = lCurrent->getNext();
	}

	return lResult;
}

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node index of the newly added node
*/
template<typename T1, typename T2>
unsigned MyLibrary::Graph<T1,T2>::addNode(const Node<T1> aNode)
{
	Node<T1> lTemp(aNode);
	lTemp.setIndex(this->m_NodeList.size());
	this->m_NodeList.push_back(lTemp);

	this->m_AdjacentNodeList.push_back(std::shared_ptr<MyLibrary::AdjacentNode<T2>>());

	return this->m_NodeList[this->m_NodeList.size() - 1].getIndex();
}

/** Adds an Edge between two nodes to the graph
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
* @param aData The data for the edge to store
*/
template<typename T1, typename T2>
void MyLibrary::Graph<T1,T2>::addEdge(const unsigned aStartNode, const unsigned aEndNode, const T2 aData)
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	// If the edge already exists then do nothing.
	if (this->isConnected(aStartNode, aEndNode)) return;

	std::unique_ptr<MyLibrary::AdjacentNode<T2>> lNew = std::unique_ptr<MyLibrary::AdjacentNode<T2>>(new MyLibrary::AdjacentNode<T2>(aEndNode, aData));

	if (this->m_AdjacentNodeList[aStartNode] == nullptr)
	{
		m_AdjacentNodeList[aStartNode].reset(lNew.release());
	}
	else
	{
		std::shared_ptr<MyLibrary::AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode];
		while (lCurrent->getNext() != nullptr)
		{
			lCurrent = lCurrent->getNext();
		}
		lCurrent->m_Next.reset(lNew.release());
	}

}

/** Returns wither there is an edge exists between the two nodes
* @param aStartNode Index of the first Node
* @param aEndNode Index of the first Node
*/
template<typename T1, typename T2>
bool MyLibrary::Graph<T1,T2>::isConnected(const unsigned aStartNode, const unsigned aEndNode)
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	std::shared_ptr<MyLibrary::AdjacentNode<T2>> lCurrent = this->m_AdjacentNodeList[aStartNode];
	while (lCurrent != nullptr)
	{
		if (lCurrent->getEndingNodeIndex() == aEndNode) return true;
		lCurrent = lCurrent->getNext();
	}
	return false;
}

#endif