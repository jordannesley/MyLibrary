/**
*  @file    HiddenMarkovModel.h
*  @author  Jordan Nesley
**/

#ifndef HIDDENMARKOVMODEL_H
#define HIDDENMARKOVMODEL_H

#include "HMM_ModelParameters.h"
#include "GuassianMixtureModel.h"
#include "Utilities.h"
#include <vector>
#include "MultidimensionalArray.h"
#include "DebugLogger.h"
#include <float.h> // DBL_MAX;

#pragma unmanaged

class HiddenMarkovModel
{
private:
	HMMModelParameters m_ModelParameters;
	GuassianMixtureModel m_GMM;

	static HMMModelParameters BaumWelchAlorithm(const MultidimensionalArray<double> aObservations, const HMMModelParameters aModelParameters);
	static std::vector<double> ForwardAlgorithm(const MultidimensionalArray<double> aObservations, const HMMModelParameters aModelParameters, const unsigned aTimerIterations);
	static std::vector<double> BackwardAlgorithm(const MultidimensionalArray<double> aObservations, const HMMModelParameters aModelParameters, const unsigned aTimerIterations);
	static double HiddenStateEmission(const unsigned aCountN, const unsigned aCountT, const MultidimensionalArray<double> aObservations, const MultidimensionalArray<double> aEmissionMatrix);
	static std::vector<double> CalculateObservations(const std::vector<double> aHiddenState, const MultidimensionalArray<double> aEmissionMAtrix);

public:
	HiddenMarkovModel(const HMMModelParameters& aModelParameters, const std::vector<double> aData);
	HiddenMarkovModel(const std::vector<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix, const std::vector<double> aData);
	void startTraining(const unsigned& aBaumWelchIterations, const bool aConvergenceCheck, const double aConvergenceValue = 1.0);
	static MultidimensionalArray<double> OutputObservations(const HMMModelParameters aModelParameters, const unsigned aTimeIterations);
};

#endif 