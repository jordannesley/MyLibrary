/**
*  @file    HMM_ModelParameters.h
*  @author  Jordan Nesley
**/

#include "HMM_ModelParameters.h"

/** Default constructor for HMMModelParameters
*/
HMMModelParameters::HMMModelParameters()
{
	m_NumberOfEmissionStates = 0;
	m_NumberOfHiddenStates = 0;

	m_InitialDistribution = Array<double>();
	m_TransitionMatrix = MultidimensionalArray<double>();
	m_EmissionMatrix = MultidimensionalArray<double>();
}

/** Constructor for HMMModelParameters
* @param aNumberOfHiddenStates The number of hidden states
* @param aNumberOfEmissionStates The number of emission states
* @param aInitialDistribution The initial distribution
* @param aTransitionMatrix The transition matrix
* @param aEmissionMatrix The emission matrix
*/
HMMModelParameters::HMMModelParameters(const Array<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix)
{
	unsigned lNumberOfHiddenStates = aInitialDistribution.getNumberOfElements();
	unsigned lNumberOfEmissionStates = aEmissionMatrix.getNumberOfElementsOfDimension(1);

	// check the input values
	if (aInitialDistribution.getNumberOfElements() != lNumberOfHiddenStates && aInitialDistribution.getNumberOfElements() != 1) throw 69;
	if (aTransitionMatrix.getNumberOfElementsOfDimension(0) != lNumberOfHiddenStates && aTransitionMatrix.getNumberOfElementsOfDimension(1) != lNumberOfHiddenStates) throw 69;
	if (aEmissionMatrix.getNumberOfElementsOfDimension(0) != lNumberOfHiddenStates && aEmissionMatrix.getNumberOfElementsOfDimension(1) != lNumberOfEmissionStates) throw 69;

	m_NumberOfHiddenStates = lNumberOfHiddenStates;
	m_NumberOfEmissionStates = lNumberOfEmissionStates;
	m_InitialDistribution = aInitialDistribution;
	m_TransitionMatrix = aTransitionMatrix;
	m_EmissionMatrix = aEmissionMatrix;
}

/** Returns the number of hidden states
*/
unsigned HMMModelParameters::getNumberOfHiddenStates() const
{
	return m_NumberOfHiddenStates;
}

/** Returns the number of emission states
*/
unsigned HMMModelParameters::getNumberOfEmissionStates() const
{
	return m_NumberOfEmissionStates;
}


/** Returns the emission Matrix
*/
MultidimensionalArray<double> HMMModelParameters::getEmissionMatrix() const
{
	return m_EmissionMatrix;
}

/** Returns the Transition Matrix
*/
MultidimensionalArray<double> HMMModelParameters::getTransitionMatrix() const
{
	return m_TransitionMatrix;
}

/** Returns the Initial Distribution
*/
Array<double> HMMModelParameters::getInitialDistribution() const
{
	return m_InitialDistribution;
}

/** sets the transition matrix
* @param aTransitionMatrix The new transition matrix
*/
void HMMModelParameters::setTransitionMatrix(const MultidimensionalArray<double>& aTransitionMatrix)
{
	m_TransitionMatrix = aTransitionMatrix;
}

/** sets the emission matrix
* @param aEmissionMatrix The new emission matrix
*/
void HMMModelParameters::setEmissionMatrix(const MultidimensionalArray<double>& aEmissionMatrix)
{
	m_EmissionMatrix = aEmissionMatrix;
}

/** sets the initial distribution matrix
* @param aInitialDistribution The new initial distribution matrix
*/
void HMMModelParameters::setInitialDistributionMatrix(const Array<double>& aInitialDistribution)
{
	m_InitialDistribution = aInitialDistribution;
}




