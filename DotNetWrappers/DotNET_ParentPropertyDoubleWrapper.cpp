
#include "Stdafx.h"
#include "DotNET_ParentPropertyDoubleWrapper.h"

#pragma managed

/** Default constructor for ParentPropertyDouble.
*/
MyLibrary::ParentPropertyDoubleWrapper::ParentPropertyDoubleWrapper()
{
	m_Property = new ParentPropertyDouble();
}

/** Constructor for ParentPropertyDouble.
* @param aMaxValue The maximum value that the property can have.
* @param aMinValue The minimum value that the property can have.
*/
MyLibrary::ParentPropertyDoubleWrapper::ParentPropertyDoubleWrapper(const double aMaxValue, const double aMinValue)
{
	m_Property = new ParentPropertyDouble(aMaxValue, aMinValue);
}

/** Constructor for ParentPropertyDouble.
* @param aValue a Value for the property to start with.
* @param aMaxValue The maximum value that the property can have.
* @param aMinValue The minimum value that the property can have.
*/
MyLibrary::ParentPropertyDoubleWrapper::ParentPropertyDoubleWrapper(const double aValue, const double aMaxValue, const double aMinValue)
{
	m_Property = new ParentPropertyDouble(aValue, aMaxValue, aMinValue);
}

/** Copy Constructor for ParentPropertyDouble.
* @param aNew The property to copy.
*/
MyLibrary::ParentPropertyDoubleWrapper::ParentPropertyDoubleWrapper(const ParentPropertyDoubleWrapper^ aNew)
{
	m_Property = new ParentPropertyDouble(*aNew->m_Property);
}

/** Destructor for ParentPropertyDouble.
*/
MyLibrary::ParentPropertyDoubleWrapper::~ParentPropertyDoubleWrapper()
{
	delete m_Property;
}

/** Returns the type of parent property
*/
PropertyType MyLibrary::ParentPropertyDoubleWrapper::Type()
{
	return PropertyType::Double;
}

/** Returns the value of the property.
*/
double MyLibrary::ParentPropertyDoubleWrapper::getValue()
{
	return m_Property->getValue();
}

/** Returns the max of the property.
*/
double MyLibrary::ParentPropertyDoubleWrapper::getMax()
{
	return m_Property->getMax();
}

/** Returns the min of the property.
*/
double MyLibrary::ParentPropertyDoubleWrapper::getMin()
{
	return m_Property->getMin();
}

/** Perform the crossing of the two properties.
* @param aParentProperty The property to cross with
* @param aSeed The seed for the random number generator
* @return A new property that has been crossed between the two parents.
*/
MyLibrary::ParentPropertyBaseWrapper^ MyLibrary::ParentPropertyDoubleWrapper::Crossover(const ParentPropertyBaseWrapper^ aParentProperty, const unsigned aSeed)
{
	// Downcast the ParentPropertyBaseWrapper object to ParentPropertyDoubleWrapper
	const ParentPropertyDoubleWrapper^ lParentPropertyDoubleWrapper = safe_cast<const ParentPropertyDoubleWrapper^>(aParentProperty);

    ParentPropertyBase* lNewNative = this->m_Property->Crossover(lParentPropertyDoubleWrapper->m_Property, aSeed);
	
	ParentPropertyDouble* lNewTemp = dynamic_cast<ParentPropertyDouble*>(lNewNative);
	
	ParentPropertyDoubleWrapper^ lNewManaged = gcnew ParentPropertyDoubleWrapper(lNewTemp->getValue(), lNewTemp->getMax(), lNewTemp->getMin());

	return lNewManaged;
}

/** Randomizes the property.
* @param aSeed The maximum value that the property can have.
*/
void MyLibrary::ParentPropertyDoubleWrapper::Randomize(const unsigned aSeed)
{
	this->m_Property->Randomize(aSeed);
}
