/**
*  @file    Parent.cpp
*  @author  Jordan Nesley
**/

#include "ParentPropertyDouble.h"

/** Default constructor for ParentPropertyDouble.
*/
ParentPropertyDouble::ParentPropertyDouble()
{
	this->m_Value = 0.0;
	this->m_MaxValue = 0.0;
	this->m_MinValue = 0.0;
}

/** Constructor for ParentPropertyDouble.
* @param aMaxValue The maximum value that the property can have.
* @param aMinValue The minimum value that the property can have.
*/
ParentPropertyDouble::ParentPropertyDouble(const double aMaxValue, const double aMinValue)
{
	this->m_Value = aMinValue;
	this->m_MaxValue = aMaxValue;
	this->m_MinValue = aMinValue;
}

/** Constructor for ParentPropertyDouble.
* @param aValue a Value for the property to start with.
* @param aMaxValue The maximum value that the property can have.
* @param aMinValue The minimum value that the property can have.
*/
ParentPropertyDouble::ParentPropertyDouble(const double aValue, const double aMaxValue, const double aMinValue)
{
	this->m_Value = aValue;
	this->m_MaxValue = aMaxValue;
	this->m_MinValue = aMinValue;
}

/** Copy Constructor for ParentPropertyDouble.
* @param aNew The property to copy.
*/
ParentPropertyDouble::ParentPropertyDouble(const ParentPropertyDouble& aNew)
{
	this->m_Value = aNew.m_Value;
	this->m_MaxValue = aNew.m_MaxValue;
	this->m_MinValue = aNew.m_MinValue;
}

/** Destructor for ParentPropertyDouble.
*/
ParentPropertyDouble::~ParentPropertyDouble()
{
	DebugLogger::getInstance().logMessage(DebugLogger::Information, __FUNCTION__, "Parent Property Double Destructor");
}

/** Clone function.
*/
ParentPropertyBase* ParentPropertyDouble::Clone() 
{
	return new ParentPropertyDouble(*this);
}

/** Returns the value of the property.
*/
double ParentPropertyDouble::getValue() const
{
	return this->m_Value;
}

/** Returns the max of the property.
*/
double ParentPropertyDouble::getMax() const
{
	return this->m_MaxValue;
}

/** Returns the min of the property.
*/
double ParentPropertyDouble::getMin() const
{
	return this->m_MinValue;
}

/** Constructor for ParentPropertyDouble.
* @param aParent A Parent to cross with.
* @param aSeed The maximum value that the property can have.
* @return A new property that has been crossed between the two parents. 
*/
ParentPropertyBase* ParentPropertyDouble::Crossover(const ParentPropertyBase * const aParentProperty, const unsigned aSeed) const
{
	// Convert the base class object to the derived type
	const ParentPropertyDouble * const lDoublePropertyPtr = dynamic_cast<const ParentPropertyDouble*>(aParentProperty);
 	
	ParentPropertyDouble lResult(this->m_MaxValue, this->m_MinValue);

	double lRandomNumber = Utilities::RandomNumber(aSeed, 0.0, 1.0);

	lResult.m_Value = (this->m_Value * lRandomNumber) + lDoublePropertyPtr->m_Value*(1.0 - lRandomNumber);

	return new ParentPropertyDouble(lResult);
}

/** Randomizes the property.
* @param aSeed The maximum value that the property can have.
*/
void ParentPropertyDouble::Randomize(const unsigned aSeed)
{
	this->m_Value = Utilities::RandomNumber(aSeed, this->m_MaxValue, this->m_MinValue);
}
