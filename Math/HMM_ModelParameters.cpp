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
	m_TransitionMatrix = TwoD_Matrix_D();
	m_EmissionMatrix = TwoD_Matrix_D();
}

/** Constructor for HMMModelParameters
* @param aNumberOfHiddenStates The number of hidden states
* @param aNumberOfEmissionStates The number of emission states
* @param aInitialDistribution The initial distribution
* @param aTransitionMatrix The transition matrix
* @param aEmissionMatrix The emission matrix
*/
HMMModelParameters::HMMModelParameters(const Array<double>& aInitialDistribution, const TwoD_Matrix_D& aTransitionMatrix, const TwoD_Matrix_D& aEmissionMatrix)
{
	unsigned lNumberOfHiddenStates = aInitialDistribution.getNumberOfElements();
	unsigned lNumberOfEmissionStates = aEmissionMatrix.getCols();

	// check the input values
	if (aInitialDistribution.getNumberOfElements() != lNumberOfHiddenStates && aInitialDistribution.getNumberOfElements() != 1) throw 69;
	if (aTransitionMatrix.getRows() != lNumberOfHiddenStates && aTransitionMatrix.getCols() != lNumberOfHiddenStates) throw 69;
	if (aEmissionMatrix.getRows() != lNumberOfHiddenStates && aEmissionMatrix.getCols() != lNumberOfEmissionStates) throw 69;

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
TwoD_Matrix_D HMMModelParameters::getEmissionMatrix() const
{
	return m_EmissionMatrix;
}

/** Returns the Transition Matrix
*/
TwoD_Matrix_D HMMModelParameters::getTransitionMatrix() const
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
void HMMModelParameters::setTransitionMatrix(const TwoD_Matrix_D& aTransitionMatrix)
{
	m_TransitionMatrix = aTransitionMatrix;
}

/** sets the emission matrix
* @param aEmissionMatrix The new emission matrix
*/
void HMMModelParameters::setEmissionMatrix(const TwoD_Matrix_D& aEmissionMatrix)
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




