/**
*  @file    HMM_ModelParameters.h
*  @author  Jordan Nesley
**/

#ifndef HMMMODELPARAMETERS_H
#define HMMMODELPARAMETERS_H
#include "MultidimensionalArray.h"

class HMMModelParameters
{
	private:
		unsigned m_NumberOfHiddenStates;
		unsigned m_NumberOfEmissionStates;
		MultidimensionalArray<double> m_TransitionMatrix;
		MultidimensionalArray<double>  m_EmissionMatrix;
		Array<double> m_InitialDistribution;

	public:
		HMMModelParameters();
		HMMModelParameters(const Array<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix);

		unsigned getNumberOfHiddenStates() const;
		unsigned getNumberOfEmissionStates() const;
		MultidimensionalArray<double>  getTransitionMatrix() const;
		MultidimensionalArray<double>  getEmissionMatrix() const;
		Array<double> getInitialDistribution() const;

		void setTransitionMatrix(const MultidimensionalArray<double>& aTransitionMatrix);
		void setEmissionMatrix(const MultidimensionalArray<double>& aEmissionMatrix);
		void setInitialDistributionMatrix(const Array<double>& aInitialDistribution);

		bool checkParameters() const;
		bool checkInitialDistribution() const;
		bool checkEmissionMatrix() const;
		bool checkTransitionMatrix() const;
};

#endif