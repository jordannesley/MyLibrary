/**
*  @file    Array.h
*  @author  Jordan Nesley
**/

#ifndef MULTIDIMENSIONALARRAY_H
#define MULTIDIMENSIONALARRAY_H

#include "DebugLogger.h"
#include <vector>
#include <utility> //std::move

template<typename T>
class MultidimensionalArray
{
private:
	std::vector<T> m_Elements;
	unsigned m_NumberOfDimensions;
	std::vector<unsigned> m_DimLengths;
	unsigned m_TotalNumberOfElements;

	unsigned calculateElementPosition(const std::vector<unsigned> aCoordinates) const;

public:
	MultidimensionalArray();
	MultidimensionalArray(const unsigned aNumberOfDimensions, std::vector<unsigned> aNumberOfElementsOfAllDimensions, const std::vector<T> aElements);
	MultidimensionalArray(const unsigned aNumberOfDimensions, const std::vector<unsigned>  aNumberOfElementsOfAllDimensions);

	MultidimensionalArray(const MultidimensionalArray<T>& aCopy);
	MultidimensionalArray(MultidimensionalArray<T>&& aMove);
	~MultidimensionalArray() = default;

	T getElement(const std::vector<unsigned> aCoordinates) const;
	unsigned getTotalNumberOfElements() const;
	unsigned getNumberOfElementsOfDimension(const unsigned aDimension) const;
	std::vector<unsigned> getNumberOfElementsOfAllDimensions() const;

	void setElement(const std::vector<unsigned> aCoordinates, const T aValue);
	void setRow(const std::vector<unsigned> aCoordinatesOfRow, const std::vector<T> aValues);

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
	this->m_DimLengths = std::vector<unsigned>();
	this->m_TotalNumberOfElements = 0;
}

/** Constructor for MultidimensionalArray
* @param aNumberOfDimensions The number of dimensions
* @param aNumberOfElementsForDimensions An array of the lengths for each dimension
* @param aElements All the elements of the multidimensional array
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const unsigned aNumberOfDimensions, std::vector<unsigned> aNumberOfElementsOfAllDimensions, const std::vector<T> aElements)
{
	this->m_TotalNumberOfElements = 1;
	for (unsigned lCountDim = 0; lCountDim < aNumberOfDimensions; lCountDim++)
	{
		this->m_TotalNumberOfElements = m_TotalNumberOfElements * aNumberOfElementsOfAllDimensions.getElement(lCountDim);
	}

	if (this->m_TotalNumberOfElements != aElements.getNumberOfElements()) throw MultidimensionalArray::CONSTRUCTOR_EXCEPTION;

	this->m_Elements = aElements;
	this->m_NumberOfDimensions = aNumberOfDimensions;
	this->m_DimLengths = aNumberOfElementsOfAllDimensions;
}

/** Constructor for MultidimensionalArray
* @param aNumberOfDimensions The number of dimensions
* @param aNumberOfElementsForDimensions An array of the lengths for each dimension
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const unsigned aNumberOfDimensions, std::vector<unsigned> aNumberOfElementsOfAllDimensions)
{
	this->m_TotalNumberOfElements = 1;
	for (unsigned lCountDim = 0; lCountDim < aNumberOfDimensions; lCountDim++)
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
T MultidimensionalArray<T>::getElement(const std::vector<unsigned> aCoordinates) const
{
	if (aCoordinates.size()!= this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned lElementPosition = this->calculateElementPosition(aCoordinates);

	return this->m_Elements[lElementPosition];
}

/** Returns the total number of elements of the array
* @return The total number of elements
*/
template<typename T>
unsigned MultidimensionalArray<T>::getTotalNumberOfElements() const
{
	return this->m_TotalNumberOfElements;
}

/** Returns the total number of elements of the array
* @return The total number of elements
*/
template<typename T>
unsigned MultidimensionalArray<T>::getNumberOfElementsOfDimension(const unsigned aDimension) const
{
	if (aDimension > this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	return this->m_DimLengths[aDimension];
}

/** Returns the lengths of all the dimensions
* @return All the dimension lengths
*/
template<typename T>
std::vector<unsigned> MultidimensionalArray<T>::getNumberOfElementsOfAllDimensions() const
{
	return this->m_DimLengths;
}

/** Sets the element located at the coordinates
* @param aCoordinates The coordinates for the element
*/
template<typename T>
void MultidimensionalArray<T>::setElement(const std::vector<unsigned> aCoordinates, const T aValue)
{
	if (aCoordinates.size() != this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned lElementPosition = calculateElementPosition(aCoordinates);

	this->m_Elements[lElementPosition] = aValue;

}

/** Sets all the elements in a row
* @param aCoordinates The coordinates of the row to set
* @param aValues The new values of the row
*/
template<typename T>
void MultidimensionalArray<T>::setRow(const std::vector<unsigned> aCoordinatesOfRow, const std::vector<T> aValues)
{
	// The length of aCoordinatesOfRow must equal the total number of dimensions - 1
	if (aCoordinatesOfRow.size() != this->m_NumberOfDimensions - 1) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	// the length of aValues must be the same length of the last dimension
	if (aValues.size() != this->getNumberOfElementsOfDimension(this->m_NumberOfDimensions - 1)) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	std::vector<unsigned>lStartCoordinates(aCoordinatesOfRow);
	lStartCoordinates.push_back(0);

	std::vector<unsigned>lEndCoordinates(aCoordinatesOfRow);
	lEndCoordinates.push_back(this->m_DimLengths[this->m_NumberOfDimensions - 1]);

	unsigned lStart = calculateElementPosition(lStartCoordinates);
	unsigned lEnd = calculateElementPosition(lEndCoordinates);

	for (unsigned lCount = lStart; lCount < lEnd; lCount++)
	{
		this->m_Elements[lCount] = aValues[lCount - lStart];
	}
}

/** Calculates the position in m_Elements based on coordinates
* @param aCoordinates the coordinates of the position
*/
template<typename T>
unsigned MultidimensionalArray<T>::calculateElementPosition(const std::vector<unsigned> aCoordinates) const
{
	unsigned lElementPosition = aCoordinates[this->m_NumberOfDimensions - 1];
	unsigned lSkip = 1;
	for (char lCount = this->m_NumberOfDimensions - 2; lCount >= 0; lCount--)
	{
		if (aCoordinates[lCount] >= this->m_DimLengths[lCount]) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;
		lSkip = lSkip*this->m_DimLengths[lCount + 1];
		lElementPosition = lElementPosition + aCoordinates[lCount] * lSkip;
	}
	return lElementPosition;
}


#endif