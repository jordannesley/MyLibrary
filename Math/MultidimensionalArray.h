/**
*  @file    Array.h
*  @author  Jordan Nesley
**/

#ifndef MULTIDIMENSIONALARRAY_H
#define MULTIDIMENSIONALARRAY_H

#include "Array.h"

template<typename T>
class MultidimensionalArray
{
	private:
		Array<T> m_Elements;
		unsigned m_NumberOfDimensions;
		Array<unsigned> m_DimLengths;
		unsigned m_TotalNumberOfElements;

		unsigned calculateElementPosition(const Array<unsigned> aCoordinates) const;

	public:
		MultidimensionalArray();
		MultidimensionalArray(const unsigned aNumberOfDimensions, Array<unsigned> aNumberOfElementsOfAllDimensions, const Array<T> aElements);
		MultidimensionalArray(const unsigned aNumberOfDimensions, const Array<unsigned>  aNumberOfElementsOfAllDimensions);

		T getElement(const Array<unsigned> aCoordinates) const;
		unsigned getTotalNumberOfElements() const;
		unsigned getNumberOfElementsOfDimension(const unsigned aDimension) const;
		Array<unsigned> getNumberOfElementsOfAllDimensions() const;

		void setElement(const Array<unsigned> aCoordinates, const T aValue);
		void setRow(const Array<unsigned> aCoordinatesOfRow, const Array<T> aValues);


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
	this->m_Elements = Array<T>();
	this->m_NumberOfDimensions = 0;
	this->m_DimLengths = Array<unsigned>();
	this->m_TotalNumberOfElements = 0;
}

/** Constructor for MultidimensionalArray
* @param aNumberOfDimensions The number of dimensions
* @param aNumberOfElementsForDimensions An array of the lengths for each dimension
* @param aElements All the elements of the multidimensional array 
*/
template<typename T>
MultidimensionalArray<T>::MultidimensionalArray(const unsigned aNumberOfDimensions, Array<unsigned> aNumberOfElementsOfAllDimensions, const Array<T> aElements)
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
MultidimensionalArray<T>::MultidimensionalArray(const unsigned aNumberOfDimensions, Array<unsigned> aNumberOfElementsOfAllDimensions)
{
	this->m_TotalNumberOfElements = 1;
	for (unsigned lCountDim = 0; lCountDim < aNumberOfDimensions; lCountDim++)
	{
		this->m_TotalNumberOfElements = m_TotalNumberOfElements * aNumberOfElementsOfAllDimensions.getElement(lCountDim);
	}

	this->m_Elements = Array<T>(this->m_TotalNumberOfElements);
	this->m_NumberOfDimensions = aNumberOfDimensions;
	this->m_DimLengths = aNumberOfElementsOfAllDimensions;
}

/** Returns the element located at the specified coordinates
* @param aCoordinates The coordinates for the element
* @return The element
*/
template<typename T>
T MultidimensionalArray<T>::getElement(const Array<unsigned> aCoordinates) const
{
	if (aCoordinates.getNumberOfElements() != this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned lElementPosition = this->calculateElementPosition(aCoordinates);

	return this->m_Elements.getElement(lElementPosition);
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

	return this->m_DimLengths.getElement(aDimension);
}

/** Returns the lengths of all the dimensions
* @return All the dimension lengths
*/
template<typename T>
Array<unsigned> MultidimensionalArray<T>::getNumberOfElementsOfAllDimensions() const
{
	return this->m_DimLengths;
}

/** Sets the element located at the coordinates
* @param aCoordinates The coordinates for the element
*/
template<typename T>
void MultidimensionalArray<T>::setElement(const Array<unsigned> aCoordinates, const T aValue)
{
	if (aCoordinates.getNumberOfElements() != this->m_NumberOfDimensions) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned lElementPosition = calculateElementPosition(aCoordinates);

	this->m_Elements.setElement(lElementPosition, aValue);

}

/** Sets all the elements in a row
* @param aCoordinates The coordinates of the row to set
* @param aValues The new values of the row
*/
template<typename T>
void MultidimensionalArray<T>::setRow(const Array<unsigned> aCoordinatesOfRow, const Array<T> aValues)
{
	// The length of aCoordinatesOfRow must equal the total number of dimensions - 1
	if (aCoordinatesOfRow.getNumberOfElements() != this->m_NumberOfDimensions - 1) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	// the length of aValues must be the same length of the last dimension
	if (aValues.getNumberOfElements() != this->getNumberOfElementsOfDimension(this->m_NumberOfDimensions - 1)) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;

	unsigned lStart = calculateElementPosition(aCoordinatesOfRow << Array<unsigned>{ 0 });
	unsigned lEnd = calculateElementPosition(aCoordinatesOfRow << Array<unsigned>{this->m_DimLengths.getElement(this->m_NumberOfDimensions - 1)});


	for (unsigned lCount = lStart; lCount < lEnd; lCount++)
	{
		this->m_Elements.setElement(lCount, aValues.getElement(lCount - lStart));
	}
}

/** Calculates the position in m_Elements based on coordinates
* @param aCoordinates the coordinates of the position
*/
template<typename T>
unsigned MultidimensionalArray<T>::calculateElementPosition(const Array<unsigned> aCoordinates) const
{
	unsigned lElementPosition = aCoordinates.getElement(this->m_NumberOfDimensions - 1);
	unsigned lSkip = 1;
	for (char lCount = this->m_NumberOfDimensions - 2; lCount >= 0; lCount--)
	{
		if (aCoordinates.getElement(lCount) >= this->m_DimLengths.getElement(lCount)) throw MultidimensionalArray::DIMENSIONS_DONT_MATCH;
		lSkip = lSkip*this->m_DimLengths.getElement(lCount + 1);
		lElementPosition = lElementPosition + aCoordinates.getElement(lCount) * lSkip;
	}
	return lElementPosition;
}


#endif