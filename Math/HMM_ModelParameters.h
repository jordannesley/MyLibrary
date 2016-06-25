/**
*  @file    HMM_ModelParameters.h
*  @author  Jordan Nesley
**/

#ifndef HMMMODELPARAMETERS_H
#define HMMMODELPARAMETERS_H
#include "TwoD_Matrix.h"

class HMMModelParameters
{
	private:
		unsigned m_NumberOfHiddenStates;
		unsigned m_NumberOfEmissionStates;
		TwoD_Matrix_D m_TransitionMatrix;
		TwoD_Matrix_D m_EmissionMatrix;
		Array<double> m_InitialDistribution;

	public:
		HMMModelParameters();
		HMMModelParameters(const Array<double>& aInitialDistribution, const TwoD_Matrix_D& aTransitionMatrix, const TwoD_Matrix_D& aEmissionMatrix);

		unsigned getNumberOfHiddenStates() const;
		unsigned getNumberOfEmissionStates() const;
		TwoD_Matrix_D getTransitionMatrix() const;
		TwoD_Matrix_D getEmissionMatrix() const;
		Array<double> getInitialDistribution() const;

		void setTransitionMatrix(const TwoD_Matrix_D& aTransitionMatrix);
		void setEmissionMatrix(const TwoD_Matrix_D& aEmissionMatrix);
		void setInitialDistributionMatrix(const Array<double>& aInitialDistribution);

};

#endif