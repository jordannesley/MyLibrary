
#include "Stdafx.h"
#include "DotNET_GeneticAlgorithmParametersWrapper.h"

#pragma managed

/** Default Constructor for GeneticAlgorithmParameters.
*/
MyLibrary::GeneticAlgorithmParametersWrapper::GeneticAlgorithmParametersWrapper()
{
	this->m_GAParameters = new GeneticAlgorithmParameters(); // need to call delete m_GAParameters in the descructor
}

/** Constructor for GeneticAlgorithmParametersWrapper
* @param aNumberOfGenerations The number of generations.
* @param aNumberOfParents The number of parents for each generation.
* @param aNumberToSelect The number of parents to select for breeding for each generation.
* @param aRandomParentRatio The percentage of random parents each generation.
* @param aParentPropertyTemplate  The template of properties for each parent.
*/
MyLibrary::GeneticAlgorithmParametersWrapper::GeneticAlgorithmParametersWrapper(const unsigned aNumberOfGenerations, const unsigned aNumberOfParents, const double aRandomParentRatio, array<ParentPropertyBaseWrapper^>^ aParentPropertyTemplate)
{
	// Set up unmanaged parent property template
	std::vector<std::shared_ptr<ParentPropertyBase>> lUnmanagedParentPropertyTemplate = ConvertWrapperToProperty(aParentPropertyTemplate);

	this->m_GAParameters = new GeneticAlgorithmParameters(aNumberOfGenerations, aNumberOfParents, aRandomParentRatio, lUnmanagedParentPropertyTemplate);
}

/** Descructor of GeneticAlgorithmParametersWrapper
*/
MyLibrary::GeneticAlgorithmParametersWrapper::~GeneticAlgorithmParametersWrapper()
{
	delete m_GAParameters;
}

/** Returns the number of generations.
* @return The number of generations.
*/
unsigned MyLibrary::GeneticAlgorithmParametersWrapper::getNumberOfGenerations()
{
	return m_GAParameters->getNumberOfGenerations();
}

/** Returns the number of parents.
* @return The number of parents.
*/
unsigned MyLibrary::GeneticAlgorithmParametersWrapper::getNumberOfParents()
{
	return m_GAParameters->getNumberOfParents();
}

/** Returns the random parent ratio.
* @return The random parent ratio.
*/
double MyLibrary::GeneticAlgorithmParametersWrapper::getRandomParentRatio()
{
	return m_GAParameters->getRandomParentRatio();
}

/** Returns the parent template.
* @return The parent template.
*/
array<MyLibrary::ParentPropertyBaseWrapper^>^ MyLibrary::GeneticAlgorithmParametersWrapper::getParentTemplate()
{
	return ConvertPropertyToWrapper(this->m_GAParameters->getParentTemplate());
}
