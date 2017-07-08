/**
*  @file    Graph_GraphNode.h
*  @author  Jordan Nesley
**/

#ifndef GraphNode_H
#define GraphNode_H

#include <vector>


#pragma unmanaged

	template<typename T>
	class GraphNode
	{
	private:
		unsigned m_Index;
		T m_Data;
	public:
		GraphNode();
		GraphNode(const unsigned aIndex, const T aData);
		GraphNode(const GraphNode& aCopy);

		unsigned getIndex() const;
		T getData() const;

		void setIndex(const unsigned aIndex);

	};

/** Default constructor for GraphNode
*/
template<typename T>
GraphNode<T>::GraphNode()
{
	this->m_Index = 0;
	this->m_Data = T();
}

/** Constructor for GraphNode
* @param aIndex The index of the GraphNode;
*/
template<typename T>
GraphNode<T>::GraphNode(const unsigned aIndex, const T aData)
{
	this->m_Index = aIndex;
	this->m_Data = aData;
}

/** Copy constructor for GraphNode
*/
template<typename T>
GraphNode<T>::GraphNode(const GraphNode& aCopy)
{
	this->m_Index = aCopy.m_Index;
	this->m_Data = aCopy.m_Data;
}

/** Returns the index of the GraphNode
* @return The index
*/
template<typename T>
unsigned GraphNode<T>::getIndex() const
{
	return this->m_Index;
}

/** Returns the type of the GraphNode
* @return The type
*/
template<typename T>
T GraphNode<T>::getData() const
{
	return this->m_Data;
}

/** Sets the index of the GraphNode
* @param aIndex The index
*/
template<typename T>
void GraphNode<T>::setIndex(const unsigned aIndex)
{
	this->m_Index = aIndex;
}

#endif