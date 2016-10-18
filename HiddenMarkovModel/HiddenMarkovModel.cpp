/**
*  @file    HiddenMarkovModel.h
*  @author  Jordan Nesley
**/


#include "HiddenMarkovModel.h"


/** Constructor of the hidden markov model
*/
HiddenMarkovModel::HiddenMarkovModel(const HMMModelParameters& aModelParameters, const std::vector<double> aData)
{
	m_ModelParameters = aModelParameters;
	m_GMM = GuassianMixtureModel(m_ModelParameters.getNumberOfEmissionStates(), aData);
}

/** Constructor of the hidden markov model
* @param aInitialDistribution The initial distribution
* @param aTransitionMatrix The transition matrix
* @param aEmissionMatrix The emission matrix
* @param aData The to model after
*/
HiddenMarkovModel::HiddenMarkovModel(const std::vector<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix, const std::vector<double> aData)
{
	m_ModelParameters = HMMModelParameters(aInitialDistribution, aTransitionMatrix, aEmissionMatrix);
	m_GMM = GuassianMixtureModel(m_ModelParameters.getNumberOfEmissionStates(), aData);
}

/** Starts training the hidden markov model
* @param aBaumWelchIterations The number of times to run the Baum Welch Algorithm for training
* @param aConvergenceCheck Specifies whether or not a convergence check should occur between Baum Welch Iterations.
* If True and the fitness value of the Baum Welch algorithm has not changed by a percentage greater than aConvergenceValue.
* @param aConvergenceValue The percentage used in the Convergence Check. Only used if aConvergenceCheck = True. Default value of 1.0%
*/
void HiddenMarkovModel::startTraining(const unsigned& aBaumWelchIterations, const bool aConvergenceCheck, const double aConvergenceValue)
{
	unsigned lTimeIterations = this->m_GMM.getStates().getNumberOfElementsOfDimension(0) - 1;
	double lFitness = 0.0;
	double lOldFitness = 0.0;

	std::vector<double> lForwardAlgorithmResult;

	for (unsigned lBWCount = 0; lBWCount < aBaumWelchIterations; lBWCount++)
	{
		this->m_ModelParameters = BaumWelchAlorithm(m_GMM.getStates(), this->m_ModelParameters);

		// Get the fitness after the Baum Welch Algorithm
		lForwardAlgorithmResult = ForwardAlgorithm(this->m_GMM.getStates(), this->m_ModelParameters, lTimeIterations);
		lFitness = 0.0;
		for (unsigned lCountN = 0; lCountN < this->m_ModelParameters.getNumberOfHiddenStates(); lCountN++)
		{
			lFitness += lForwardAlgorithmResult[lCountN];
		}

		// Do the convergence check on the fitness score
		if (aConvergenceCheck)
		{
			if (lBWCount > 0)
			{
				//the Baum Welch Algorithm has converged to a point
				if (Utilities::Abs((lOldFitness - lFitness) / lOldFitness) < aConvergenceValue) break;
			}

			lOldFitness = lFitness;
		}
	}
}

/** Creates a sequence of observations based on the model parameters of the HMM
* @param aModelParameters The model parameters of the hidden markov model
* @param aTimeIterations The number of output states
*/
MultidimensionalArray<double> HiddenMarkovModel::OutputObservations(const HMMModelParameters aModelParameters, const unsigned aTimeIterations)
{
	MultidimensionalArray<double> lResult(2, { aTimeIterations, aModelParameters.getNumberOfEmissionStates() });

	std::vector<double> lHiddenStates = aModelParameters.getInitialDistribution();;
	unsigned lTCount = 0;

	// calculate the observations created at t=0
	lResult.setRow({ 0 }, CalculateObservations(lHiddenStates, aModelParameters.getEmissionMatrix()));

	// increment the time count
	lTCount++;


	while (lTCount < aTimeIterations)
	{
		std::vector<double> lTemp(aModelParameters.getNumberOfHiddenStates());
		for (unsigned lCountN1 = 0; lCountN1 < aModelParameters.getNumberOfHiddenStates(); lCountN1++)
		{
			for (unsigned lCountN2 = 0; lCountN2 < aModelParameters.getNumberOfHiddenStates(); lCountN2++)
			{
				lTemp[lCountN1] = lTemp[lCountN1] + aModelParameters.getTransitionMatrix().getElement({ lCountN2, lCountN1 })*lHiddenStates[lCountN2];
			}
		}
		lHiddenStates = lTemp;
		lResult.setRow({ lTCount }, CalculateObservations(lHiddenStates, aModelParameters.getEmissionMatrix()));
		lTCount++;
	}

	return lResult;
}

/** Calculates the observation states at a single time instant
* @param aHiddenState The hidden state distribution at the time instant
* @param aEmissionMAtrix The emission matrix of the HMM
* @comment Uses a hard clustering for the guassian mixture model
*/
std::vector<double> HiddenMarkovModel::CalculateObservations(std::vector<double> aHiddenState, MultidimensionalArray<double> aEmissionMatrix)
{
	std::size_t lNumberOfEmissionStates = aEmissionMatrix.getNumberOfElementsOfDimension(1);
	std::size_t lNumberOfHiddenStates = aHiddenState.size();

	std::vector<double> lResult(lNumberOfEmissionStates);
	std::vector<double> lTemp(lNumberOfEmissionStates);
	double lMax = -DBL_MAX;
	unsigned lSelectedState = 0;

	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		for (unsigned lCountM = 0; lCountM < lNumberOfEmissionStates; lCountM++)
		{
			lTemp[lCountM] = lTemp[lCountM] + aHiddenState[lCountN]*aEmissionMatrix.getElement({ lCountN, lCountM });
		}
	}

	// perform the hard clustering
	for (unsigned lCount = 0; lCount < lNumberOfEmissionStates; lCount++)
	{
		if (lTemp[lCount] > lMax)
		{
			lMax = lTemp[lCount];
			lSelectedState = lCount;
		}
	}

	lResult[lSelectedState] = 1.0;

	return lResult;
}

/** The Forward Algorithm: Calculates the probability that a sequence of observations given the model parameters of the HMM
* @param aObservations The discrete states of observations
* @param aModelParameters The model parameters of the hidden markov model
* @param aTimerIterations The number of time iterations to run the forward algorithm
*/
std::vector<double> HiddenMarkovModel::ForwardAlgorithm(MultidimensionalArray<double> aObservations, HMMModelParameters aModelParameters, unsigned aTimerIterations)
{
	if (aTimerIterations > aObservations.getNumberOfElementsOfDimension(0) - 1) throw 69;

	std::size_t lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	std::size_t lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();
	unsigned lTimeIterations = aTimerIterations + 1;
	unsigned lTCount = 0;

	std::vector<double> lAlpha = std::vector<double>(lNumberOfHiddenStates);

	//Step 1: Initialization
	//Calculate teh first step of the forward algorithm
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lAlpha[lCountN] = aModelParameters.getInitialDistribution()[lCountN] * HiddenStateEmission(lCountN, lTCount, aObservations, aModelParameters.getEmissionMatrix());
	}

	// increment the tcount variable
	lTCount++;

	//Step 2: Induction
	//Calculate each time step
	while (lTCount < lTimeIterations)
	{
		std::vector<double> lTemp = std::vector<double>(lNumberOfHiddenStates);

		for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
		{
			lTemp[lCountN1] =  0.0;
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lTemp[lCountN1] = lTemp[lCountN1] + aModelParameters.getTransitionMatrix().getElement({ lCountN2, lCountN1 }) * lAlpha[lCountN2];
			}
			lTemp[lCountN1] = lTemp[lCountN1] * HiddenStateEmission(lCountN1, lTCount, aObservations, aModelParameters.getEmissionMatrix());
		}
		lAlpha = lTemp;
		lTCount++;
	}
	return lAlpha;
}

/** The Backward Algorithm: Calculates the probability that a sequence of observations given the model parameters of the HMM
* @param aObservations The discrete states of observations
* @param aModelParameters The model parameters of the hidden markov model
* @param aTimerIterations The number of time iterations to run the forward algorithm
*/
std::vector<double> HiddenMarkovModel::BackwardAlgorithm(MultidimensionalArray<double> aObservations, HMMModelParameters aModelParameters, unsigned aTimerIterations)
{
	if (aTimerIterations > aObservations.getNumberOfElementsOfDimension(0) - 1) throw 69;

	std::size_t lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	std::size_t lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();

	// Set lTcount to the last time
	unsigned lTcount = aObservations.getNumberOfElementsOfDimension(0) - 1;

	std::vector<double> lBeta(lNumberOfHiddenStates);

	// Step 1: Initialization
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lBeta[lCountN] = 1.0;
	}

	//decrement lTcount
	lTcount--;

	//Step 2: Induction
	//calculate each time step
	std::vector<double> lTemp(lNumberOfHiddenStates);
	while (lTcount >= aTimerIterations)
	{
		for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
		{
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lTemp[lCountN1] = lTemp[lCountN1] + aModelParameters.getTransitionMatrix().getElement({ lCountN1, lCountN2 }) * lBeta[lCountN2] * HiddenStateEmission(lCountN2, lTcount + 1, aObservations, aModelParameters.getEmissionMatrix());
			}
		}
		lBeta = lTemp;
		lTcount--;
	}
	return lBeta;
}

/** Calculates the probability of a hidden state producing the observation
* @param aCountN The hidden state index
* @param aCountT The time iteration index
* @param aObservations The discrete states of observations
* @param aEmissionMatrix The emission matrix of the hidden markov model
*/
double HiddenMarkovModel::HiddenStateEmission(const unsigned aCountN, const unsigned aCountT, const MultidimensionalArray<double> aObservations, const MultidimensionalArray<double> aEmissionMatrix)
{
	double lResult = 0.0;
	for (unsigned lCountM = 0; lCountM < aObservations.getNumberOfElementsOfDimension(1); lCountM++)
	{
		lResult = lResult + aObservations.getElement({ aCountT, lCountM }) * aEmissionMatrix.getElement({ aCountN, lCountM });
	}

	return lResult;
}

/** The Baum Welch Algorithm for the hidden markov model
* @param aObservations The discrete states of observations
* @param aModelParameters The hidden markov model parameters
*/
HMMModelParameters HiddenMarkovModel::BaumWelchAlorithm(const MultidimensionalArray<double> aObservations, const HMMModelParameters aModelParameters)
{
	std::size_t lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	std::size_t lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();
	unsigned lTimeIterations = aObservations.getNumberOfElementsOfDimension(0);

	//lGamma = the probability of being in hidden state i at time t
	MultidimensionalArray<double> lGamma(2, { lTimeIterations, lNumberOfHiddenStates });

	//lEta = The probability of being in hidden state i at time t and hidden state j at time t+1
	MultidimensionalArray<double> lEta(3, { lTimeIterations - 1, lNumberOfHiddenStates, lNumberOfHiddenStates });

	//lAlpha = The result of the forward algorithm at time t
	MultidimensionalArray<double> lAlpha(2, { lTimeIterations, lNumberOfHiddenStates });

	//lBeta = the result of the backward algorithm at time t+1
	MultidimensionalArray<double> lBeta(2, { lTimeIterations, lNumberOfHiddenStates });

	//TransitionFrequency1 = the expected number of transitions from hidden state i
	std::vector<double> TransitionFrequency1(lNumberOfHiddenStates);

	//TransitionFrequency2 = the expected number of transitions from hidden state i to hidden state j
	MultidimensionalArray<double> TransitionFrequency2(2, { lNumberOfHiddenStates, lNumberOfHiddenStates });

	//lTransitionFrequency3 = the expected number of times in state i and observing symbol m
	MultidimensionalArray<double> lTransitionFrequency3(2, { lNumberOfHiddenStates, lNumberOfEmissionStates });

	std::vector<double> lFrequency4(lNumberOfHiddenStates);

	// The total probability of the total observation sequence happening
	double lProbabilityOfObservationSequence = 0.0;

	double lHiddenStateEmission;

	// calculate lAlpha and lBeta for all time iterations
	for (unsigned lTcount = 0; lTcount < lTimeIterations; lTcount++)
	{
		lAlpha.setRow({ lTcount }, ForwardAlgorithm(aObservations, aModelParameters, lTcount));
		lBeta.setRow({ lTcount }, BackwardAlgorithm(aObservations, aModelParameters, lTcount));
	}

	for (unsigned lTCount = 0; lTCount < lTimeIterations - 1; lTCount++)
	{
		lProbabilityOfObservationSequence = 0.0;
		for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
		{
			lProbabilityOfObservationSequence += lAlpha.getElement({ lTCount, lCountN }) * lBeta.getElement({ lTCount, lCountN });
		}

		for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
		{
			lGamma.setElement({ lTCount, lCountN1 }, (lAlpha.getElement({ lTCount, lCountN1 }) * lBeta.getElement({ lTCount, lCountN1 })) / lProbabilityOfObservationSequence);
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lHiddenStateEmission = HiddenStateEmission(lCountN2, lTCount + 1, aObservations, aModelParameters.getEmissionMatrix());
				lEta.setElement({ lTCount, lCountN1, lCountN2 }, lAlpha.getElement({ lTCount, lCountN1 }) * aModelParameters.getTransitionMatrix().getElement({ lCountN1, lCountN2 })*lHiddenStateEmission*lBeta.getElement({ lTCount + 1, lCountN2 }));
				lEta.setElement({ lTCount, lCountN1, lCountN2 }, lEta.getElement({ lTCount, lCountN1, lCountN2 }) / lProbabilityOfObservationSequence);
			}
		}
	}

	//Calculate the last time iteration for lGamma
	lProbabilityOfObservationSequence = 0.0;
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lProbabilityOfObservationSequence += lAlpha.getElement({ lTimeIterations - 1, lCountN }) *lBeta.getElement({ lTimeIterations - 1, lCountN });
	}
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lGamma.setElement({ lTimeIterations - 1, lCountN }, (lAlpha.getElement({ lTimeIterations - 1, lCountN }) * lBeta.getElement({ lTimeIterations - 1, lCountN })) / lProbabilityOfObservationSequence);
	}

	//Calculate TransitionFrequency2, TransitionFrequency1
	for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
	{
		for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
		{
			TransitionFrequency2.setElement({ lCountN1, lCountN2 }, 0.0);
			for (unsigned lCountT = 0; lCountT < lTimeIterations - 1; lCountT++)
			{
				TransitionFrequency2.setElement({ lCountN1, lCountN2 }, TransitionFrequency2.getElement({ lCountN1, lCountN2 }) + lEta.getElement({ lCountT, lCountN1, lCountN2 }));
				lFrequency4[lCountN1] = lFrequency4[lCountN1] + lEta.getElement({ lCountT, lCountN1, lCountN2 });
			}
		}
	}

	// Calculate TransitionFrequency1 and lTransitionFrequency3
	for (unsigned lCountT = 0; lCountT < lTimeIterations; lCountT++)
	{
		for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
		{
			TransitionFrequency1[lCountN] = TransitionFrequency1[lCountN] + lGamma.getElement({ lCountT, lCountN });
			for (unsigned lCountM = 0; lCountM < lNumberOfEmissionStates; lCountM++)
			{
				lTransitionFrequency3.setElement({ lCountN, lCountM }, lTransitionFrequency3.getElement({ lCountN, lCountM }) + lGamma.getElement({ lCountT, lCountN }) * aObservations.getElement({ lCountT, lCountM }));
			}
		}
	}

	//Calculate the new model parameters of the hidden markov model
	std::vector<double> lNewInitialDistribution(lNumberOfHiddenStates);
	MultidimensionalArray<double> lNewTransitionMatrix(2, { lNumberOfHiddenStates, lNumberOfHiddenStates });
	MultidimensionalArray<double> lNewEmissionMatrix(2, { lNumberOfHiddenStates, lNumberOfEmissionStates });
	for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
	{
		lNewInitialDistribution[lCountN1] = lGamma.getElement({ 0, lCountN1 });
		for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
		{
			// if there are never any transisitons from the state then it can not transition to another state
			if (lFrequency4[lCountN1] == 0.0)
			{
				lNewTransitionMatrix.setElement({ lCountN1, lCountN2 }, 0.0);
			}
			else
			{
				lNewTransitionMatrix.setElement({ lCountN1, lCountN2 }, TransitionFrequency2.getElement({ lCountN1, lCountN2 }) / lFrequency4[lCountN1]);
			}
		}
		for (unsigned lCountM = 0; lCountM < lNumberOfEmissionStates; lCountM++)
		{
			// if there are never any transitions from the state it can not emit an ovservation
			if (TransitionFrequency1[lCountN1] == 0.0)
			{
				lNewEmissionMatrix.setElement({ lCountN1, lCountM }, 0.0);
			}
			else
			{
				lNewEmissionMatrix.setElement({ lCountN1, lCountM }, lTransitionFrequency3.getElement({ lCountN1, lCountM }) / TransitionFrequency1[lCountN1]);
			}
		}
	}


	// if there a hidden state never transitions to another state (always transitions into itself) then use the previous state transitions for this state
	// Note: We are avoiding a state always transitioning into itself because this leads to a convergence of all states transitioning into this state
	for (unsigned lCount = 0; lCount < lNumberOfHiddenStates; lCount++)
	{
		double lSum = 0;
		for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
		{
			lSum += lNewTransitionMatrix.getElement({ lCount, lCountN2 });
		}
		if (lSum == 0.0)
		{
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lNewTransitionMatrix.setElement({ lCount, lCountN2 }, aModelParameters.getTransitionMatrix().getElement({ lCount, lCountN2 }));
			}
		}
	}

	// create the new model parameter object
	HMMModelParameters lResult(lNewInitialDistribution, lNewTransitionMatrix, lNewEmissionMatrix);

	// Check the new model parameters
	if (!lResult.checkParameters()) DebugLogger::getInstance().logMessage(DebugLogger::MessageLoggerType::Error, __FUNCTION__, "The new model parameters do not make sense");

	return HMMModelParameters();
}