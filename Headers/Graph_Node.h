
#ifndef NODE_H
#define NODE_H

#include <vector>


#pragma unmanaged

namespace MyLibrary
{
	template<typename T>
	class Node
	{
	private:
		unsigned m_Index;
		T m_Data;
	public:
		Node();
		Node(const unsigned aIndex, const T aData);
		Node(const Node& aCopy);

		unsigned getIndex() const;
		T getData() const;

		void setIndex(const unsigned aIndex);

	};
}

/** Default constructor for Node
*/
template<typename T>
MyLibrary::Node<T>::Node()
{
	this->m_Index = 0;
	this->m_Data = T();
}

/** Constructor for Node
* @param aIndex The index of the Node;
*/
template<typename T>
MyLibrary::Node<T>::Node(const unsigned aIndex, const T aData)
{
	this->m_Index = aIndex;
	this->m_Data = aData;
}

/** Copy constructor for Node
*/
template<typename T>
MyLibrary::Node<T>::Node(const Node& aCopy)
{
	this->m_Index = aCopy.m_Index;
	this->m_Data = aCopy.m_Data;
}

/** Returns the index of the Node
* @return The index
*/
template<typename T>
unsigned MyLibrary::Node<T>::getIndex() const
{
	return this->m_Index;
}

/** Returns the type of the Node
* @return The type
*/
template<typename T>
T MyLibrary::Node<T>::getData() const
{
	return this->m_Data;
}

/** Sets the index of the Node
* @param aIndex The index
*/
template<typename T>
void MyLibrary::Node<T>::setIndex(const unsigned aIndex)
{
	this->m_Index = aIndex;
}

#endif