
#ifndef GRAPH_H
#define GRAPH_H

#include "Graph_Node.h"
#include "Graph_AdjacentNode.h"
#include <memory>
#include <vector>

#pragma unmanaged


namespace MyLibrary {
	template<typename T1, typename T2>
	class Graph
	{
	private:
		std::vector<Node<T1>> m_NodeList;
		std::vector<std::shared_ptr<MyLibrary::AdjacentNode<T2>>> m_AdjacentNodeList;
	public:
		Graph();
		Graph(const std::vector<Node<T1>> aNodes);

		Node<T1> getNode(const unsigned aNodeIndex) const;
		std::vector<Node<T1>> getAllNodes() const;
		std::shared_ptr<MyLibrary::AdjacentNode<T2>> getAllEdges(const unsigned aNodeIndex) const;

		unsigned addNode(const Node<T1> aNode);
		void addEdge(const unsigned aStartNode, const unsigned aEndNode);
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
	this->m_NodeList = aNodes;


	// initialize the edges
	this->m_AdjacentNodeList = std::vector<std::shared_ptr<MyLibrary::AdjacentNode<T2>>>(aNodes.size(), std::shared_ptr<MyLibrary::AdjacentNode<T2>>());
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

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node
*/
template<typename T1, typename T2>
std::shared_ptr<MyLibrary::AdjacentNode<T2>> MyLibrary::Graph<T1,T2>::getAllEdges(const unsigned aNodeIndex) const
{
	return this->m_AdjacentNodeList[aNodeIndex];
}

/** Returns the edges of a given node
* @param aNodeIndex The index of the node
* @return The node
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
*/
template<typename T1, typename T2>
void MyLibrary::Graph<T1,T2>::addEdge(const unsigned aStartNode, const unsigned aEndNode)
{
	if (aStartNode >= this->m_NodeList.size()) throw 1;
	if (aEndNode >= this->m_NodeList.size()) throw 1;

	// If the edge already exists then do nothing.
	if (this->isConnected(aStartNode, aEndNode)) return;

	std::unique_ptr<MyLibrary::AdjacentNode<T2>> lNew = std::unique_ptr<MyLibrary::AdjacentNode<T2>>(new MyLibrary::AdjacentNode<T2>(aEndNode));

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