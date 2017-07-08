/**
*  @file    Graph_AdjacentNode.h
*  @author  Jordan Nesley
**/

#ifndef ADJNODE_H
#define ADJNODE_H

#include <memory>

#pragma unmanaged

	template<typename T>
	class AdjacentNode
	{
	private:
		int m_EndingNodeIndex;
		T m_Data;
	public:
		AdjacentNode();
		AdjacentNode(const unsigned aEndingNodeIndex);
		AdjacentNode(const unsigned aEndingNodeIndex, const T& aData);

		int getEndingNodeIndex() const;
		T getData() const;

		void setData(const T& aData);
	};

/** Default Constructor for AdjacentNodeBase
*/
template<typename T>
AdjacentNode<T>::AdjacentNode()
{
	this->m_EndingNodeIndex = -1;
	this->m_Data = T();
}

/** Constructor for AdjacentNodeBase
* @param EndingNodeIndex The index of the ending node
*/
template<typename T>
AdjacentNode<T>::AdjacentNode(const unsigned aEndingNodeIndex)
{
	this->m_EndingNodeIndex = aEndingNodeIndex;
	this->m_Data = T();
}

/** Constructor for AdjacentNodeBase
* @param EndingNodeIndex The index of the ending node
* @param aData The data of the edge
*/
template<typename T>
AdjacentNode<T>::AdjacentNode(const unsigned aEndingNodeIndex, const T& aData)
{
	this->m_EndingNodeIndex = aEndingNodeIndex;
	this->m_Data = aData;
}

/** Returns the ending node index of the Edge
* @return The index
*/
template<typename T>
int AdjacentNode<T>::getEndingNodeIndex() const
{
	return this->m_EndingNodeIndex;
}

/** Returns the type of the Edge
* @return The type
*/
template<typename T>
T AdjacentNode<T>::getData() const
{
	return this->m_Data;
}

/** Sets the data of the edge
* @param aData The new data of the edge
*/
template<typename T>
void AdjacentNode<T>::setData(const T& aData)
{
	this - m_Data = aData;
}
#endif