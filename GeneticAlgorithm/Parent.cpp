/**
*  @file    Parent.cpp
*  @author  Jordan Nesley
**/

#include "Parent.h"

/** Default Constructor for Parent.
*/
Parent::Parent()
{
	this->m_Fitness = 0.0;
	this->m_Rank = 0;
	this->m_ParentProperties = std::vector<std::unique_ptr<ParentPropertyBase>>();
}

/** Constructor for Parent.
* @param aParentProperties The property template for the parent.
*/
Parent::Parent(std::vector<std::shared_ptr<ParentPropertyBase>> aParentProperties)
{
	this->m_Fitness = 0.0;
	this->m_Rank = 0;
	this->m_ParentProperties = std::vector<std::unique_ptr<ParentPropertyBase>>(aParentProperties.size());
	for (unsigned lCount = 0; lCount < this->m_ParentProperties.size(); lCount++)
	{
		this->m_ParentProperties[lCount].reset(aParentProperties[lCount]->Clone());
	}
}

/** Copy Constructor for Parent.
* @param aNew The parent to copy from
*/
Parent::Parent(const Parent& aNew)
{
	this->m_Fitness = aNew.m_Fitness;
	this->m_ParentProperties = std::vector<std::unique_ptr<ParentPropertyBase>>(aNew.m_ParentProperties.size());
	for (unsigned lCount = 0; lCount < this->m_ParentProperties.size(); lCount++)
	{
		this->m_ParentProperties[lCount].reset(aNew.m_ParentProperties[lCount]->Clone());
	}
}

/** Move Constructor for Parent.
* @param aMove The parent to swap from
*/
Parent::Parent(Parent&& aMove)
{
	this->m_Fitness = std::move(aMove.m_Fitness);
	this->m_ParentProperties = std::move(aMove.m_ParentProperties);

	aMove.m_Fitness = 0.0;
	aMove.m_ParentProperties.clear();
}

/** Destructor for Parent.
*/
Parent::~Parent()
{
	DebugLogger::getInstance().logMessage(DebugLogger::MessageLoggerType::Information, __FUNCTION__, "Parent Destructor");
}

/** Returns the fitness of the parent
* @return the fitness value of the parent.
*/
double Parent::getFitness() const
{
	return this->m_Fitness;
}

/** Sets the fitness of the parent.
* @param aFitness The new fitness value.
*/
void Parent::setFitness(const double aFitness)
{
	this->m_Fitness = aFitness;
}

/** Returns the rank of the parent
* @return the rank value of the parent.
*/
unsigned Parent::getRank() const
{
	return this->m_Rank;
}

/** Sets the rank of the parent.
* @param aRank The new rank value.
*/
void Parent::setRank(const unsigned aRank)
{
	this->m_Rank = aRank;
}

/** Crossover the the two parents property values.
* @param aMate The parent to mate with.
* @param aSeed The seed number for the random generator
* @return The new parent that was created as a result of the crossover. Mating parents are not touched.
*/
Parent Parent::Crossover(const Parent aMate, const unsigned aSeed) const
{	
	// If the parents done have the same number of elements then throw an exception
	if (this->m_ParentProperties.size() != aMate.m_ParentProperties.size()) throw 69;

	std::vector<std::shared_ptr<ParentPropertyBase>> lNewParentProperties(this->m_ParentProperties.size());

	// crossover the two parents
	for (unsigned lCount = 0; lCount < this->m_ParentProperties.size(); lCount++)
	{
		lNewParentProperties[lCount].reset(this->m_ParentProperties[lCount]->Crossover(aMate.m_ParentProperties[lCount].get(), aSeed + lCount));
	}

	return Parent(lNewParentProperties);
}

/** Randomizes the properties value
* @param aSeed The seed number for the random generator
*/
void Parent::Randomize(const unsigned aSeed)
{
	// randomize each of the parent's properties
	for (unsigned lCount = 0; lCount < this->m_ParentProperties.size(); lCount++)
	{
		this->m_ParentProperties[lCount]->Randomize(aSeed+lCount);
	}
}

/** Swap function for the Parent class
* @param aFirst object one
* @param aSecond object two
*/
void Parent::swap(Parent& aSwap)
{
	std::swap(this->m_Fitness, aSwap.m_Fitness);
	std::swap(this->m_ParentProperties, aSwap.m_ParentProperties);
}

/** Returns the parents properties.
* @return The parent's properties.
*/
std::vector<std::unique_ptr<ParentPropertyBase>> Parent::getProperties() const
{
	std::vector<std::unique_ptr<ParentPropertyBase>> lResult = std::vector<std::unique_ptr<ParentPropertyBase>>(this->m_ParentProperties.size());

	for (unsigned lCount = 0; lCount < lResult.size(); lCount++)
	{
		lResult[lCount].reset(this->m_ParentProperties[lCount]->Clone());
	}

	return lResult;
}

/** Assignment operator
* @param aRight The object to the right of the operator sign
*/
Parent& Parent::operator = (Parent& aRight)
{
	Parent lTemp(aRight);
	lTemp.swap(*this);

	return *this;
}

Parent& Parent::operator= (Parent&& aRight)
{	
	this->m_Fitness = std::move(aRight.m_Fitness);
	this->m_ParentProperties = std::move(aRight.m_ParentProperties);

	aRight.m_Fitness = 0.0;
	aRight.m_ParentProperties.clear();
	return *this;
}
