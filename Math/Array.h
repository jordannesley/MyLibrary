/**
*  @file    Array.h
*  @author  Jordan Nesley
**/

#ifndef ARRAY_H
#define ARRAY_H

#include "DebugLogger.h"
#include <algorithm>   // std::copy
#include <initializer_list>

template< typename T > 
class Array
{
	private:
		T* m_Elements;
		unsigned m_NumberOfElements;

		bool rangeCheck(const unsigned& aElementLocation) const;
	public:

		Array();
		Array(const unsigned& aNumberOfElements, const T* aElements);
		Array(const unsigned& aNumberOfElements);
		Array(std::initializer_list<T> aValues);
		Array(const Array& aNew);

		void setElement(const unsigned& aElementLocation, const T& aElement);
		T getElement(const unsigned& aElementLocation) const;
		unsigned getNumberOfElements() const;

		Array<T> operator= (const Array<T>& aRight);
		Array<T> operator+ (const Array<T>& aRight) const;
		Array<T> operator* (const T& aRight) const;
		Array<T> operator<< (const Array<T>& aRight) const;

		~Array();
		
};

/** Default constructor for Array
*/
template<typename T>
Array<T>::Array()
{
	this->m_NumberOfElements = 0;
	this->m_Elements = nullptr;
}

/** Constructor for Array
* @ param aElements The elements of the array
* @ param aNumberOfElements The number of elements for the array
*/
template< typename T >
Array<T>::Array(const unsigned& aNumberOfElements, const T* aElements)
{
	this->m_NumberOfElements = aNumberOfElements;
	this->m_Elements = new T[aNumberOfElements];
	std::copy(aElements, aElements + aNumberOfElements, this->m_Elements);
}

/** Constructor for Array
* @param aNumberOfElements The number of elements for the array
*/
template< typename T >
Array<T>::Array(const unsigned& aNumberOfElements)
{
	this->m_NumberOfElements = aNumberOfElements;
	this->m_Elements = new T[aNumberOfElements];
	for (unsigned lCount = 0; lCount < aNumberOfElements; lCount++)
	{
		m_Elements[lCount] = T();
	}
}

/** Constructor for Array with initializer lists ( Array example = {1,2,3,4} )
* @param aValues the values of the array
*/
template< typename T >
Array<T>::Array(std::initializer_list<T> aValues)
{
	this->m_NumberOfElements = aValues.size();
	this->m_Elements = new T[aValues.size()];

	std::copy(aValues.begin(), aValues.end(), m_Elements);
}

/** Copy Constructor
* @param aNew The object to copy
*/
template<typename T>
Array<T>::Array(const Array& aNew)
{
	this->m_NumberOfElements = aNew.m_NumberOfElements;

	this->m_Elements = new T[aNew.m_NumberOfElements];
	std::copy(aNew.m_Elements, aNew.m_Elements + aNew.m_NumberOfElements, this->m_Elements);
}

/** The descructor for Arrat
*/
template<typename T>
Array<T>::~Array()
{
	DebugLogger::getInstance().logMessage(MessageLoggerType::Information, __FUNCTION__, "Desctructor Called");
	delete[] m_Elements;
}

/** sets an element in the array
* @param aElementLocation The index of the element
* @param aElement The Element
*/
template< typename T >
void Array<T>::setElement(const unsigned& aElementLocation, const T& aElement)
{
	if (this->rangeCheck(aElementLocation))
	{
		this->m_Elements[aElementLocation] = aElement;
	}
	else
	{
		DebugLogger::getInstance().logMessage(MessageLoggerType::Error, __FUNCTION__, "The the indicies were out of bounds");
		throw 69;
	}
}

/** sets an element in the array
* @param aElementLocation The index of the element
* @return The Element
*/
template< typename T >
T Array<T>::getElement(const unsigned& aElementLocation) const
{
	if (this->rangeCheck(aElementLocation))
	{
		return this->m_Elements[aElementLocation];
	}
	else
	{
		DebugLogger::getInstance().logMessage(MessageLoggerType::Error, __FUNCTION__, "The the indicies were out of bounds");
		throw 69;
	}
}

/** Checks if the element location is within the bounds of the array
* @param aElementLocation The index to check
* @return The result of the check (True = Index is within bounds of array, False = Index is outside bounds)
*/
template< typename T >
bool Array<T>::rangeCheck(const unsigned& aElementLocation) const
{
	return (aElementLocation < this->m_NumberOfElements);
}

/** Returns the number of elements in the array
* @return the number of elements in the array
*/
template< typename T >
unsigned Array<T>::getNumberOfElements() const
{
	return m_NumberOfElements;
}

/** copy operator
*  @param aRight The right side of the = operator
*/
template< typename T >
Array<T> Array<T>::operator= (const Array<T>& aRight)
{
	delete[] this->m_Elements;

	this->m_NumberOfElements = aRight.m_NumberOfElements;
	this->m_Elements = new T[this->m_NumberOfElements];
	std::copy(aRight.m_Elements, aRight.m_Elements + this->m_NumberOfElements, this->m_Elements);

	return *this;
}

/** addition operator between two matrices
*  @param aRight The right side of the + sign
*/
template< typename T >
Array<T> Array<T>::operator + (const Array<T>& aRight) const
{
	if (this->m_NumberOfElements == aRight.m_NumberOfElements)
	{
		T* lTemp = new T[this->m_NumberOfElements];
		for (unsigned lCount = 0; lCount < this->m_NumberOfElements; lCount++)
		{
			lTemp[lCount] = this->m_Elements[lCount] + aRight.m_Elements[lCount];
		}
		Array<T> lResult = Array<T>(lTemp, this->m_NumberOfElements);
		delete[] lTemp;
		return lResult;
	}
	else
	{
		throw 69;
	};
}

/** scalor multiplication
*  @param aRight The Scalor Value to the right of the * sign
*/
template< typename T >
Array<T> Array<T>::operator * (const T& aRight) const
{
	T* ltempElements = new T[this->m_NumberOfElements];
	for (unsigned lCount = 0; lCount < this->m_NumberOfElements; lCount++)
	{
		ltempElements[lCount] = this->m_Elements[lCount] * aRight;
	}
	Array<T> lResult(this->m_NumberOfElements, ltempElements);
	delete[] ltempElements;
	return lResult;
}

/** Scalor Multiplication
*  @param aLeft The Scalor Value to the left of the * sign
*  @param aRight The matrix to the right of the * sign
*/
template< typename T >
Array<T> operator* (const T& aLeft, const Array<T>& aRight)
{
	Array<T> lResult = aRight*aLeft;
	return lResult;
}

/** insertion operator - creates a new array with the right array appended to the left
*  @param aRight The array to the right of the << operator
*/
template< typename T >
Array<T> Array<T>::operator << (const Array<T>& aRight) const
{
	unsigned lNumberOfElements = aRight.m_NumberOfElements + this->m_NumberOfElements;
	T* lElements = new T[lNumberOfElements];

	std::copy(this->m_Elements, this->m_Elements + this->m_NumberOfElements, lElements);
	std::copy(aRight.m_Elements, aRight.m_Elements + aRight.m_NumberOfElements, lElements + this->m_NumberOfElements);

	Array<T> lResult(lNumberOfElements, lElements);

	delete[] lElements;

	return lResult;
}

template< typename T >
Array<T> operator* (const T& aLeft, const Array<T>& aRight);

#endif
