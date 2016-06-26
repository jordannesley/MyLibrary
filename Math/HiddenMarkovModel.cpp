#include "HiddenMarkovModel.h"


/** Constructor of the hidden markov model
*/
HiddenMarkovModel::HiddenMarkovModel(const HMMModelParameters& aModelParameters, const Array<double> aData)
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
HiddenMarkovModel::HiddenMarkovModel(const Array<double>& aInitialDistribution, const MultidimensionalArray<double>& aTransitionMatrix, const MultidimensionalArray<double>& aEmissionMatrix, const Array<double> aData)
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

	Array<double> lForwardAlgorithmResult;

	for (unsigned lBWCount = 0; lBWCount < aBaumWelchIterations; lBWCount++)
	{
		this->m_ModelParameters = BaumWelchAlorithm(m_GMM.getStates(), this->m_ModelParameters);

		// Get the fitness after the Baum Welch Algorithm
		lForwardAlgorithmResult = ForwardAlgorithm(this->m_GMM.getStates(), this->m_ModelParameters, lTimeIterations);
		lFitness = 0.0;
		for (unsigned lCountN = 0; lCountN < this->m_ModelParameters.getNumberOfHiddenStates(); lCountN++)
		{
			lFitness += lForwardAlgorithmResult.getElement(lCountN);
		}

		// Do the convergence check on the fitness score
		if (aConvergenceCheck)
		{
			if (lBWCount > 0)
			{
				//the Baum Welch Algorithm has converged to a point
				if (Abs((lOldFitness - lFitness) / lOldFitness) < aConvergenceValue) break;
			}

			lOldFitness = lFitness;
		}
	}
}

/** The Forward Algorithm: Calculates the probability that a sequence of observations given the model parameters of the HMM
* @param aObservations The discrete states of observations
* @param aModelParameters The model parameters of the hidden markov model
* @param aTimerIterations The number of time iterations to run the forward algorithm
*/
Array<double> HiddenMarkovModel::ForwardAlgorithm(MultidimensionalArray<double> aObservations, HMMModelParameters aModelParameters, unsigned aTimerIterations)
{
	if (aTimerIterations > aObservations.getNumberOfElementsOfDimension(0) - 1) throw 69;

	unsigned lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	unsigned lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();
	unsigned lTimeIterations = aTimerIterations + 1;
	unsigned lTCount = 0;

	Array<double> lAlpha = Array<double>(lNumberOfHiddenStates);

	//Step 1: Initialization
	//Calculate teh first step of the forward algorithm
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lAlpha.setElement(lCountN, aModelParameters.getInitialDistribution().getElement(lCountN) * HiddenStateEmission(lCountN, lTCount, aObservations, aModelParameters.getEmissionMatrix()));
	}

	// increment the tcount variable
	lTCount++;

	//Step 2: Induction
	//Calculate each time step
	while (lTCount < lTimeIterations)
	{
		Array<double> lTemp = Array<double>(lNumberOfHiddenStates);

		for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
		{
			lTemp.setElement(lCountN1, 0.0);
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lTemp.setElement(lCountN1, lTemp.getElement(lCountN1) + aModelParameters.getTransitionMatrix().getElement({ lCountN2, lCountN1 }) * lAlpha.getElement(lCountN2));
			}
			lTemp.setElement(lCountN1, lTemp.getElement(lCountN1) * HiddenStateEmission(lCountN1, lTCount, aObservations, aModelParameters.getEmissionMatrix()));
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
Array<double> HiddenMarkovModel::BackwardAlgorithm(MultidimensionalArray<double> aObservations, HMMModelParameters aModelParameters, unsigned aTimerIterations)
{
	if (aTimerIterations > aObservations.getNumberOfElementsOfDimension(0)-1) throw 69;

	unsigned lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	unsigned lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();

	// Set lTcount to the last time
	unsigned lTcount = aObservations.getNumberOfElementsOfDimension(0) - 1;

	Array<double> lBeta = Array<double>(lNumberOfHiddenStates);

	// Step 1: Initialization
	for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
	{
		lBeta.setElement(lCountN, 1.0);
	}

	//decrement lTcount
	lTcount--;

	//Step 2: Induction
	//calculate each time step
	Array<double> lTemp = Array<double>(lNumberOfHiddenStates);
	while (lTcount >= aTimerIterations)
	{
		for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
		{
			for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
			{
				lTemp.setElement(lCountN1, lTemp.getElement(lCountN1) + aModelParameters.getTransitionMatrix().getElement({ lCountN1, lCountN2 }) * lBeta.getElement(lCountN2) * HiddenStateEmission(lCountN2, lTcount + 1, aObservations, aModelParameters.getEmissionMatrix()));
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

HMMModelParameters HiddenMarkovModel::BaumWelchAlorithm(const MultidimensionalArray<double> aObservations, const HMMModelParameters aModelParameters)
{
	unsigned lNumberOfHiddenStates = aModelParameters.getNumberOfHiddenStates();
	unsigned lNumberOfEmissionStates = aModelParameters.getNumberOfEmissionStates();
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
	Array<double> TransitionFrequency1(lNumberOfHiddenStates);

	//TransitionFrequency2 = the expected number of transitions from hidden state i to hidden state j
	MultidimensionalArray<double> TransitionFrequency2(2, { lNumberOfHiddenStates, lNumberOfHiddenStates });

	//lTransitionFrequency3 = the expected number of times in state i and observing symbol m
	MultidimensionalArray<double> lTransitionFrequency3(2, { lNumberOfHiddenStates, lNumberOfEmissionStates });

	Array<double> lFrequency4(lNumberOfHiddenStates);

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
				lFrequency4.setElement(lCountN1, lFrequency4.getElement(lCountN1) + lEta.getElement({ lCountT, lCountN1, lCountN2 }));
			}
		}
	}

	// Calculate TransitionFrequency1 and lTransitionFrequency3
	for (unsigned lCountT = 0; lCountT < lTimeIterations; lCountT++)
	{
		for (unsigned lCountN = 0; lCountN < lNumberOfHiddenStates; lCountN++)
		{
			TransitionFrequency1.setElement(lCountN, TransitionFrequency1.getElement(lCountN) + lGamma.getElement({ lCountT, lCountN }));
			for (unsigned lCountM = 0; lCountM < lNumberOfEmissionStates; lCountM++)
			{
				lTransitionFrequency3.setElement({ lCountN, lCountM }, lTransitionFrequency3.getElement({ lCountN, lCountM }) + lGamma.getElement({ lCountT, lCountN }) * aObservations.getElement({ lCountT, lCountM }));
			}
		}
	}

	//Calculate the new model parameters of the hidden markov model
	Array<double> lNewInitialDistribution(lNumberOfHiddenStates);
	MultidimensionalArray<double> lNewTransitionMatrix(2, { lNumberOfHiddenStates, lNumberOfHiddenStates });
	MultidimensionalArray<double> lNewEmissionMatrix(2, { lNumberOfHiddenStates, lNumberOfEmissionStates });
	for (unsigned lCountN1 = 0; lCountN1 < lNumberOfHiddenStates; lCountN1++)
	{
		lNewInitialDistribution.setElement(lCountN1, lGamma.getElement({ 0, lCountN1 }));
		for (unsigned lCountN2 = 0; lCountN2 < lNumberOfHiddenStates; lCountN2++)
		{
			// if there are never any transisitons from the state then it can not transition to another state
			if (lFrequency4.getElement(lCountN1) == 0.0)
			{
				lNewTransitionMatrix.setElement({ lCountN1, lCountN2 }, 0.0);
			}
			else
			{
				lNewTransitionMatrix.setElement({ lCountN1, lCountN2 }, TransitionFrequency2.getElement({ lCountN1, lCountN2 }) / lFrequency4.getElement(lCountN1));
			}
		}
		for (unsigned lCountM = 0; lCountM < lNumberOfEmissionStates; lCountM++)
		{
			// if there are never any transitions from the state it can not emit an ovservation
			if (TransitionFrequency1.getElement(lCountN1) == 0.0)
			{
				lNewEmissionMatrix.setElement({ lCountN1, lCountM }, 0.0);
			}
			else
			{
				lNewEmissionMatrix.setElement({ lCountN1, lCountM }, lTransitionFrequency3.getElement({ lCountN1, lCountM }) / TransitionFrequency1.getElement(lCountN1));
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
	if (!lResult.checkParameters()) DebugLogger::getInstance().logMessage(MessageLoggerType::Error, __FUNCTION__, "The new model parameters do not make sense");

	return HMMModelParameters();
}