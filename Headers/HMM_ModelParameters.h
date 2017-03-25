/**
*  @file    HMM_ModelParameters.h
*  @author  Jordan Nesley
**/

#ifndef HMMMODELPARAMETERS_H
#define HMMMODELPARAMETERS_H
#include "MultidimensionalArray.h"
#include "DebugLogger.h"
#include <vector>

#pragma unmanaged

class HMMModelParameters
{
private:
	unsigned m_NumberOfHiddenStates;
	unsigned m_NumberOfEmissionStates;
	MultidimensionalArray<double> m_TransitionMatrix;
	MultidimensionalArray<double>  m_EmissionMatrix;
	std::vector<double> m_InitialDistribution;

public:
	HMMModelParameters();
	HMMModelParameters(const std::vector<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix);

	unsigned getNumberOfHiddenStates() const;
	unsigned getNumberOfEmissionStates() const;
	MultidimensionalArray<double>  getTransitionMatrix() const;
	MultidimensionalArray<double>  getEmissionMatrix() const;
	std::vector<double> getInitialDistribution() const;

	void setTransitionMatrix(const MultidimensionalArray<double>& aTransitionMatrix);
	void setEmissionMatrix(const MultidimensionalArray<double>& aEmissionMatrix);
	void setInitialDistributionMatrix(const std::vector<double>& aInitialDistribution);

	bool checkParameters() const;
	bool checkInitialDistribution() const;
	bool checkEmissionMatrix() const;
	bool checkTransitionMatrix() const;
};

#endif