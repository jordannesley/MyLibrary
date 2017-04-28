/**
*  @file    Graph_AdjacentNode.h
*  @author  Jordan Nesley
**/

#ifndef ADJNODE_H
#define ADJNODE_H

#include <memory>

#pragma unmanaged

namespace MyLibrary
{
	template<typename T>
	class AdjacentNode
	{
		template<typename T1, typename T2>
		friend class Graph;
	private:
		std::shared_ptr<AdjacentNode<T>> m_Next;
		int m_EndingNodeIndex;
		T m_Data;
	public:
		AdjacentNode();
		AdjacentNode(const unsigned aEndingNodeIndex);
		AdjacentNode(const unsigned aEndingNodeIndex, const T& aData);

		std::shared_ptr<AdjacentNode> getNext() const;
		int getEndingNodeIndex() const;
		T getData() const;

		void setData(const T& aData);
	};
}

/** Default Constructor for AdjacentNodeBase
*/
template<typename T>
MyLibrary::AdjacentNode<T>::AdjacentNode()
{
	this->m_Next = std::shared_ptr<AdjacentNode<T>>();
	this->m_EndingNodeIndex = -1;
	this->m_Data = T();
}

/** Constructor for AdjacentNodeBase
* @param EndingNodeIndex The index of the ending node
*/
template<typename T>
MyLibrary::AdjacentNode<T>::AdjacentNode(const unsigned aEndingNodeIndex)
{
	this->m_Next = std::shared_ptr<AdjacentNode<T>>();
	this->m_EndingNodeIndex = aEndingNodeIndex;
	this->m_Data = T();
}

/** Constructor for AdjacentNodeBase
* @param EndingNodeIndex The index of the ending node
* @param aData The data of the edge
*/
template<typename T>
MyLibrary::AdjacentNode<T>::AdjacentNode(const unsigned aEndingNodeIndex, const T& aData)
{
	this->m_Next = std::shared_ptr<AdjacentNode<T>>();
	this->m_EndingNodeIndex = aEndingNodeIndex;
	this->m_Data = aData;
}

/** Returns the ending node index of the Edge
* @return The index
*/
template<typename T>
std::shared_ptr<MyLibrary::AdjacentNode<T>> MyLibrary::AdjacentNode<T>::getNext() const
{
	return this->m_Next;
}

/** Returns the ending node index of the Edge
* @return The index
*/
template<typename T>
int MyLibrary::AdjacentNode<T>::getEndingNodeIndex() const
{
	return this->m_EndingNodeIndex;
}

/** Returns the type of the Edge
* @return The type
*/
template<typename T>
T MyLibrary::AdjacentNode<T>::getData() const
{
	return this->m_Data;
}

/** Sets the data of the edge
* @param aData The new data of the edge
*/
template<typename T>
void MyLibrary::AdjacentNode<T>::setData(const T& aData)
{
	this - m_Data = aData;
}
#endif