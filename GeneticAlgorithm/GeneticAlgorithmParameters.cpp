/**
*  @file    GeneticAlgorithmParameters.cpp
*  @author  Jordan Nesley
**/

#include "GeneticAlgorithmParameters.h"

/** Default Constructor for GeneticAlgorithmParameters.
*/
GeneticAlgorithmParameters::GeneticAlgorithmParameters()
{
	this->m_NumberOfGenerations = 0;
	this->m_NumberOfParents = 0;
	this->m_RandomParentRatio = 0;
	this->m_ParentTemplate = std::vector<std::shared_ptr<ParentPropertyBase>>();
}

/** Constructor for GeneticAlgorithmParameters.
* @param aNumberOfGenerations The number of generations.
* @param aNumberOfParents The number of parents for each generation.
* @param aNumberToSelect The number of parents to select for breeding for each generation.
* @param aRandomParentRatio The percentage of random parents each generation.
* @param aParentPropertyTemplate  The template of properties for each parent.
*/
GeneticAlgorithmParameters::GeneticAlgorithmParameters(const unsigned int aNumberOfGenerations, const unsigned int aNumberOfParents, const double aRandomParentRatio, std::vector<std::shared_ptr<ParentPropertyBase>> aParentPropertyTemplate)
{
	this->m_NumberOfGenerations = aNumberOfGenerations;
	this->m_NumberOfParents = aNumberOfParents;
	this->m_RandomParentRatio = aRandomParentRatio;
	this->m_ParentTemplate = aParentPropertyTemplate;
}

/** Copy Constructor for GeneticAlgorithmParameters.
* @param aCopy Object to copy.
*/
GeneticAlgorithmParameters::GeneticAlgorithmParameters(const GeneticAlgorithmParameters& aCopy)
{
	this->m_NumberOfGenerations = aCopy.m_NumberOfGenerations;
	this->m_NumberOfParents = aCopy.m_NumberOfParents;
	this->m_RandomParentRatio = aCopy.m_RandomParentRatio;
	this->m_ParentTemplate = aCopy.m_ParentTemplate;
}

/** Move Constructor for GeneticAlgorithmParameters.
* @param aCopy Object to move.
*/
GeneticAlgorithmParameters::GeneticAlgorithmParameters(GeneticAlgorithmParameters&& aMove)
{
	this->m_NumberOfGenerations = std::move(aMove.m_NumberOfGenerations);
	this->m_NumberOfParents = std::move(aMove.m_NumberOfParents);
	this->m_RandomParentRatio = std::move(aMove.m_RandomParentRatio);
	this->m_ParentTemplate = std::move(aMove.m_ParentTemplate);

	aMove.m_NumberOfGenerations = 0;
	aMove.m_NumberOfParents = 0;
	aMove.m_RandomParentRatio = 0;
	aMove.m_ParentTemplate = std::vector<std::shared_ptr<ParentPropertyBase>>();
}

/** Swap function for the GeneticAlgorithmParameters class.
* @param aFirst object one
* @param aSecond object two
*/
void GeneticAlgorithmParameters::swap(GeneticAlgorithmParameters& aFirst, GeneticAlgorithmParameters& aSecond)
{
	std::swap(aFirst.m_NumberOfGenerations, aSecond.m_NumberOfGenerations);
	std::swap(aFirst.m_NumberOfParents, aSecond.m_NumberOfParents);
	std::swap(aFirst.m_RandomParentRatio, aSecond.m_RandomParentRatio);
	std::swap(aFirst.m_ParentTemplate, aSecond.m_ParentTemplate);
}

/** Returns the number of generations.
* @return The number of generations.
*/
unsigned int GeneticAlgorithmParameters::getNumberOfGenerations() const
{
	return this->m_NumberOfGenerations;
}

/** Returns the number of parents.
* @return The number of parents.
*/
unsigned int GeneticAlgorithmParameters::getNumberOfParents() const
{
	return this->m_NumberOfParents;
}

/** Returns the random parent ratio.
* @return The random parent ratio.
*/
double GeneticAlgorithmParameters::getRandomParentRatio() const
{
	return this->m_RandomParentRatio;
}

/** Returns the parent template.
* @return The parent template.
*/
std::vector<std::shared_ptr<ParentPropertyBase>> GeneticAlgorithmParameters::getParentTemplate()
{
	return this->m_ParentTemplate;
}

/** Assignment operator
* @param The right side of the = operator
*/
GeneticAlgorithmParameters& GeneticAlgorithmParameters::operator=(const GeneticAlgorithmParameters& aRight)
{
	this->m_ParentTemplate = aRight.m_ParentTemplate;
	this->m_NumberOfParents = aRight.m_NumberOfParents;
	this->m_RandomParentRatio = aRight.m_RandomParentRatio;
	this->m_NumberOfGenerations = aRight.m_NumberOfGenerations;
	return *this;
}
