/**
*  @file    Array.h
*  @author  Jordan Nesley
**/

#ifndef MULTIDIMENSIONALARRAY_H
#define MULTIDIMENSIONALARRAY_H

#include "DebugLogger.h"
#include <vector>
#include <utility> //std::move

#pragma unmanaged

template<typename T>
class MultidimensionalArray
{
private:
	std::vector<T> m_Elements;
	unsigned m_NumberOfDimensions;
	std::vector<unsigned long> m_DimLengths;
	unsigned m_TotalNumberOfElements;

	unsigned calculateElementPosition(const std::vector<unsigned long> aCoordinates) const;

public:
	MultidimensionalArray();
	MultidimensionalArray(const unsigned long aNumberOfDimensions, std::vector<unsigned long> aNumberOfElementsOfAllDimensions, const std::vector<T> aElements);
	MultidimensionalArray(const unsigned long aNumberOfDimensions, const std::vector<unsigned long>  aNumberOfElementsOfAllDimensions);

	MultidimensionalArray(const MultidimensionalArray<T>& aCopy);
	MultidimensionalArray(MultidimensionalArray<T>&& aMove);
	~MultidimensionalArray() = default;

	T getElement(const std::vector<unsigned long> aCoordinates) const;
	unsigned long getTotalNumberOfElements() const;
	unsigned long getNumberOfElementsOfDimension(const unsigned long aDimension) const;
	std::vector<unsigned long> getNumberOfElementsOfAllDimensions() const;
	std::vector<T> getRow(const std::vector<unsigned long> aCoordinatesOfRow) const;

	void setElement(const std::vector<unsigned long> aCoordinates, const T aValue);
	void setRow(const std::vector<unsigned long> aCoordinatesOfRow, const std::vector<T> aValues);

	MultidimensionalArray<T>& operator=(const MultidimensionalArray<T>& aRight);

	enum Exeception
	{
		CONSTRUCTOR_EXCEPTION,
		DIMENSIONS_DONT_MATCH,
	};
};

/** Default constructor for MultidimensionalArray
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray()
{
	this->m_Elements = std::vector<T>();
	this->m_NumberOfDimensions = 0;
	this->m_DimLengths = std::vector<unsigned long>();
	this->m_TotalNumberOfElements = 0;
}

/** Constructor for MultidimensionalArray
* @param aNumberOfDimensions The number of dimensions
* @param aNumberOfElementsForDimensions An array of the lengths for each dimension
* @param aElements All the elements of the multidimensional array
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const unsigned long aNumberOfDimensions, std::vector<unsigned long> aNumberOfElementsOfAllDimensions, const std::vector<T> aElements)
{
	this->m_TotalNumberOfElements = 1;
	for (unsigned long lCountDim = 0; lCountDim < aNumberOfDimensions; lCountDim++)
	{
		this->m_TotalNumberOfElements = m_TotalNumberOfElements * aNumberOfElementsOfAllDimensions[lCountDim];
	}

	if (this->m_TotalNumberOfElements != aElements.size()) throw MultidimensionalArray::CONSTRUCTOR_EXCEPTION;

	this->m_Elements = aElements;
	this->m_NumberOfDimensions = aNumberOfDimensions;
	this->m_DimLengths = aNumberOfElementsOfAllDimensions;
}

/** Constructor for MultidimensionalArray
* @param aNumberOfDimensions The number of dimensions
* @param aNumberOfElementsForDimensions An array of the lengths for each dimension
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const unsigned long aNumberOfDimensions, std::vector<unsigned long> aNumberOfElementsOfAllDimensions)
{
	this->m_TotalNumberOfElements = 1;
	for (unsigned long lCountDim = 0; lCountDim < aNumberOfDimensions; lCountDim++)
	{
		this->m_TotalNumberOfElements = m_TotalNumberOfElements * aNumberOfElementsOfAllDimensions[lCountDim];
	}

	this->m_Elements = std::vector<T>(this->m_TotalNumberOfElements);
	this->m_NumberOfDimensions = aNumberOfDimensions;
	this->m_DimLengths = aNumberOfElementsOfAllDimensions;
}

/** Copy Constructor for MultidimensionalArray
* @param aCopy The object to copy
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const MultidimensionalArray<T>& aCopy)
{
	this->m_Elements = aCopy.m_Elements;
	this->m_NumberOfDimensions = aCopy.m_NumberOfDimensions;
	this->m_DimLengths = aCopy.m_DimLengths;
	this->m_TotalNumberOfElements = aCopy.m_TotalNumberOfElements;
}

/** Move Constructor for MultidimensionalArray
* @param aCopy The object to copy
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(MultidimensionalArray&& aMove)
{
	this->m_Elements = std::move(aMove.m_Elements);
	this->m_NumberOfDimensions = std::move(aMove.m_NumberOfDimensions);
	this->m_DimLengths = std::move(aMove.m_DimLengths);
	this->m_TotalNumberOfElements = std::move(aMove.m_TotalNumberOfElements);

	aMove.m_NumberOfDimensions = 0;
	aMove.m_TotalNumberOfElements = 0;
	aMove.m_Elements.clear();
	aMove.m_DimLengths.clear();
}

/** Returns the element located at the specified coordinates
* @param aCoordinates The coordinates for the element
* @return The element
*/
template<typename T>
T MultidimensionalArray<T>::getElement(const std::vector<unsigned long> aCoordinates) const
{
	if (aCoordinates.size()!= this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned long lElementPosition = this->calculateElementPosition(aCoordinates);

	return this->m_Elements[lElementPosition];
}

/** Returns the total number of elements of the array
* @return The total number of elements
*/
template<typename T>
unsigned long MultidimensionalArray<T>::getTotalNumberOfElements() const
{
	return this->m_TotalNumberOfElements;
}

/** Returns the total number of elements of the array
* @return The total number of elements
*/
template<typename T>
unsigned long MultidimensionalArray<T>::getNumberOfElementsOfDimension(const unsigned long aDimension) const
{
	if (aDimension > this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	return this->m_DimLengths[aDimension];
}

/** Returns the lengths of all the dimensions
* @return All the dimension lengths
*/
template<typename T>
std::vector<unsigned long> MultidimensionalArray<T>::getNumberOfElementsOfAllDimensions() const
{
	return this->m_DimLengths;
}

/** Returns the all the values for a given row
* @return The elements of a given row
*/
template<typename T>
std::vector<T> MultidimensionalArray<T>::getRow(const std::vector<unsigned long> aCoordinatesOfRow) const
{
	// The length of aCoordinatesOfRow must equal the total number of dimensions - 1
	if (aCoordinatesOfRow.size() != this->m_NumberOfDimensions - 1) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	// calculate the start and ending positions of a row
	std::vector<unsigned long>lStartCoordinates(aCoordinatesOfRow);
	lStartCoordinates.push_back(0);

	std::vector<unsigned long>lEndCoordinates(aCoordinatesOfRow);
	lEndCoordinates.push_back(this->m_DimLengths[this->m_NumberOfDimensions - 1]);

	// calculate the vector positions of the start and end coordinates
	unsigned long lStart = calculateElementPosition(lStartCoordinates);
	unsigned long lEnd = calculateElementPosition(lEndCoordinates);

	std::vector<T> lResult(this->m_DimLengths[this->m_NumberOfDimensions - 1]);
	for (unsigned long lCount = lStart; lCount < lEnd; lCount++)
	{
		 lResult[lCount - lStart] = this->m_Elements[lCount];
	}
	return lResult;
}

/** Sets the element located at the coordinates
* @param aCoordinates The coordinates for the element
*/
template<typename T>
void MultidimensionalArray<T>::setElement(const std::vector<unsigned long> aCoordinates, const T aValue)
{
	if (aCoordinates.size() != this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned long lElementPosition = calculateElementPosition(aCoordinates);

	this->m_Elements[lElementPosition] = aValue;
}

/** Sets all the elements in a row
* @param aCoordinates The coordinates of the row to set
* @param aValues The new values of the row
*/
template<typename T>
void MultidimensionalArray<T>::setRow(const std::vector<unsigned long> aCoordinatesOfRow, const std::vector<T> aValues)
{
	// The length of aCoordinatesOfRow must equal the total number of dimensions - 1
	if (aCoordinatesOfRow.size() != this->m_NumberOfDimensions - 1) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	// the length of aValues must be the same length of the last dimension
	if (aValues.size() != this->getNumberOfElementsOfDimension(this->m_NumberOfDimensions - 1)) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	// calculate the start and ending positions of a row
	std::vector<unsigned long>lStartCoordinates(aCoordinatesOfRow);
	lStartCoordinates.push_back(0);

	std::vector<unsigned long>lEndCoordinates(aCoordinatesOfRow);
	lEndCoordinates.push_back(this->m_DimLengths[this->m_NumberOfDimensions - 1]);

	// calculate the vector positions of the start and end coordinates
	unsigned long lStart = calculateElementPosition(lStartCoordinates);
	unsigned long lEnd = calculateElementPosition(lEndCoordinates);

	for (unsigned long lCount = lStart; lCount < lEnd; lCount++)
	{
		this->m_Elements[lCount] = aValues[lCount - lStart];
	}
}

/** Assingment operator
* @param aRight The right side of the = operator
*/
template<typename T>
MultidimensionalArray<T>& MultidimensionalArray<T>::operator=(const MultidimensionalArray& aRight)
{
	this->m_Elements = aRight.m_Elements;
	this->m_DimLengths = aRight.m_DimLengths;
	this->m_NumberOfDimensions = aRight.m_NumberOfDimensions;
	this->m_TotalNumberOfElements = aRight.m_TotalNumberOfElements;
	return *this;
}

/** Calculates the position in m_Elements based on coordinates
* @param aCoordinates the coordinates of the position
*/
template<typename T>
unsigned MultidimensionalArray<T>::calculateElementPosition(const std::vector<unsigned long> aCoordinates) const
{
	unsigned long lElementPosition = aCoordinates[this->m_NumberOfDimensions - 1];
	unsigned long lSkip = 1;
	for (long lCount = this->m_NumberOfDimensions - 2; lCount >= 0; lCount--)
	{
		if (aCoordinates[lCount] >= this->m_DimLengths[lCount]) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;
		lSkip = lSkip*this->m_DimLengths[lCount + 1];
		lElementPosition = lElementPosition + aCoordinates[lCount] * lSkip;
	}
	return lElementPosition;
}


#endif
