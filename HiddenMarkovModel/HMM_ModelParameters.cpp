/**
*  @file    HMM_ModelParameters.h
*  @author  Jordan Nesley
**/

#include "HMM_ModelParameters.h"

#pragma unmanaged

/** Default constructor for HMMModelParameters
*/
HMMModelParameters::HMMModelParameters()
{
	m_NumberOfEmissionStates = 0;
	m_NumberOfHiddenStates = 0;

	m_InitialDistribution = std::vector<double>();
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
HMMModelParameters::HMMModelParameters(const std::vector<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix)
{
	std::size_t lNumberOfHiddenStates = aInitialDistribution.size();
	std::size_t lNumberOfEmissionStates = aEmissionMatrix.getNumberOfElementsOfDimension(1);

	// check the input values
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
std::vector<double> HMMModelParameters::getInitialDistribution() const
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
void HMMModelParameters::setInitialDistributionMatrix(const std::vector<double>& aInitialDistribution)
{
	m_InitialDistribution = aInitialDistribution;
}

/** Checks all the parameters of the HMM to make sure they make sense
* @return The success of the check (True = all model parameters are good)
*/
bool HMMModelParameters::checkParameters() const
{
	bool lResult = true;
	lResult = lResult && this->checkInitialDistribution() && this->checkTransitionMatrix() && this->checkEmissionMatrix();
	return lResult;
}

/** Checks initial distribution of the HMM to make sure it makes sense
* @return The success of the check (True = all model parameters are good)
*/
bool HMMModelParameters::checkInitialDistribution() const
{
	bool lResult;
	double lSum = 0.0;
	for (unsigned lCountN = 0; lCountN < this->m_InitialDistribution.size(); lCountN++)
	{
		lSum += this->m_InitialDistribution[lCountN];
	}

	lResult = (lSum > 0.9999 && lSum < 1.0001);
	if (!lResult) DebugLogger::getInstance().logMessage(DebugLogger::MessageLoggerType::Error, __FUNCTION__, "Initial Distribution does not make sense");

	return (lSum > 0.9999 && lSum < 1.0001);
}

/** Checks emission matrix of the HMM to make sure it makes sense
* @return The success of the check (True = all model parameters are good)
*/
bool HMMModelParameters::checkEmissionMatrix() const
{
	double lSum = 0.0;
	bool lResult = true;
	for (unsigned lCountN = 0; lCountN < this->m_EmissionMatrix.getNumberOfElementsOfDimension(0); lCountN++)
	{
		lSum = 0.0;
		for (unsigned lCountM = 0; lCountM < this->m_EmissionMatrix.getNumberOfElementsOfDimension(1); lCountM++)
		{
			lSum += this->m_EmissionMatrix.getElement({ lCountN, lCountM });
		}
		lResult = lResult && ((lSum > .9999 && lSum < 1.0001) || (lSum == 0.0));
	}

	if (!lResult) DebugLogger::getInstance().logMessage(DebugLogger::MessageLoggerType::Error, __FUNCTION__, "Emission Matrix does not make sense");

	return lResult;
}

/** Checks transition matrix of the HMM to make sure it makes sense
* @return The success of the check (True = all model parameters are good)
*/
bool HMMModelParameters::checkTransitionMatrix() const
{
	double lSum = 0.0;
	bool lResult = true;
	for (unsigned lCountN1 = 0; lCountN1 < this->m_TransitionMatrix.getNumberOfElementsOfDimension(0); lCountN1++)
	{
		lSum = 0.0;
		for (unsigned lCountN2 = 0; lCountN2 < this->m_TransitionMatrix.getNumberOfElementsOfDimension(1); lCountN2++)
		{
			lSum += this->m_TransitionMatrix.getElement({ lCountN1, lCountN2 });
		}

		lResult = lResult && (lSum > .9999 && lSum < 1.0001);
	}

	if (!lResult) DebugLogger::getInstance().logMessage(DebugLogger::MessageLoggerType::Error, __FUNCTION__, "Transition Matrix does not make sense");

	return lResult;
}



