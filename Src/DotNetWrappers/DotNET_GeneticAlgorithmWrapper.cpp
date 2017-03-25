// This is the main DLL file.

#include "stdafx.h"
#include "DotNET_GeneticAlgorithmWrapper.h"

#pragma managed

/** Constructor for Genetic Algorithm.
* @param aSeed The seed number to use for randomization.
* @param aGAParameters The parameters that define the genetic algorithm.
* @param aFunction The function that defines the fitness for each parent.
*/
MyLibrary::GeneticAlgorithmWrapper::GeneticAlgorithmWrapper(const unsigned% aSeed, MyLibrary::GeneticAlgorithmParametersWrapper^ aParameters, FitnessFunction^ aFitnessFunction)
{
	this->ManagedFitnessFunction = aFitnessFunction;
	this->m_FitnessFunctionClass = new UnmanagedFitnessFunctionClass();

	std::vector<std::shared_ptr<ParentPropertyBase>> lUnmanagedParentPropertyTemplate = ConvertWrapperToProperty(aParameters->getParentTemplate());

	GeneticAlgorithmParameters GAParameters = GeneticAlgorithmParameters(aParameters->getNumberOfGenerations(),aParameters->getNumberOfParents(),aParameters->getRandomParentRatio(), lUnmanagedParentPropertyTemplate);

	// Set up the fitness function pointer to send to the genetic algorithm
	UnmanagedFitnessFunctionDelegate^ lFitnessDelegate = gcnew UnmanagedFitnessFunctionDelegate(this, &GeneticAlgorithmWrapper::UnmanagedFitnessFunction);
	this->m_gc = GCHandle::Alloc(lFitnessDelegate);
	IntPtr ip = Marshal::GetFunctionPointerForDelegate(lFitnessDelegate);
	this->m_FitnessFunctionClass->FitnessFunction = static_cast<UNMANAGED_FITNESS_FUNCTION>(ip.ToPointer());

	this->m_GenAlg = new GeneticAlgorithm(aSeed, GAParameters, this->m_FitnessFunctionClass->FitnessFunction);
}

/** Descructor of GeneticAlgorithmWrapper
*/
MyLibrary::GeneticAlgorithmWrapper::~GeneticAlgorithmWrapper()
{
	delete this->m_GenAlg;
	delete this->m_FitnessFunctionClass;
	this->m_gc.Free();
}

/** Start the genetic algorithm.
*/
void MyLibrary::GeneticAlgorithmWrapper::Start()
{
	this->m_GenAlg->Start();
}

/** Call the managed fitness function
* @param aParentProperties The unmanaged parent properties.
*/
double MyLibrary::GeneticAlgorithmWrapper::UnmanagedFitnessFunction(const std::vector<std::unique_ptr<ParentPropertyBase>>& aParentProperties)
{

	array<MyLibrary::ParentPropertyBaseWrapper^>^ lManagedParentProperties = ConvertPropertyToWrapper(aParentProperties);

	// Call the fitness function and return the fitness
	return this->ManagedFitnessFunction(lManagedParentProperties);
}
