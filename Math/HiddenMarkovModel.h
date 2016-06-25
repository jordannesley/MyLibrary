/**
*  @file    HiddenMarkovModel.h
*  @author  Jordan Nesley
**/

#ifndef HIDDENMARKOVMODEL_H
#define HIDDENMARKOVMODEL_H

#include "TwoD_Matrix.h"
#include "HMM_ModelParameters.h"
#include "GuassianMixtureModel.h"
#include "Utilities.h"
#include "Array.h"

class HiddenMarkovModel
{
	private:
		HMMModelParameters m_ModelParameters;
		GuassianMixtureModel m_GMM;

		static HMMModelParameters BaumWelchAlorithm(const TwoD_Matrix_D aObservations, const HMMModelParameters aModelParameters);
		static Array<double> ForwardAlgorithm(const TwoD_Matrix_D aObservations, const HMMModelParameters aModelParameters, const unsigned aTimerIterations);
		static Array<double> BackwardAlgorithm(const TwoD_Matrix_D aObservations, const HMMModelParameters aModelParameters, const unsigned aTimerIterations);
		static double HiddenStateEmission(const unsigned aCountN, const unsigned aCountT, const TwoD_Matrix_D aObservations, const TwoD_Matrix_D aEmissionMatrix);

	public:
		HiddenMarkovModel(const HMMModelParameters& aModelParameters, const Array<double> aData);
		HiddenMarkovModel(const Array<double>& aInitialDistribution, const TwoD_Matrix_D& aTransitionMatrix, const TwoD_Matrix_D& aEmissionMatrix, const Array<double> aData);
		void startTraining(const unsigned& aBaumWelchIterations, const bool aConvergenceCheck, const double aConvergenceValue = 1.0);
};

#endif 