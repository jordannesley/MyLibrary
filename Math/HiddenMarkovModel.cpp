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
	Array<double> TransitionFrequency1 = Array<double>(lNumberOfHiddenStates);

	//TransitionFrequency2 = the expected number of transitions from hidden state i to hidden state j
	MultidimensionalArray<double> TransitionFrequency2 = MultidimensionalArray<double>(2, { lNumberOfHiddenStates, lNumberOfHiddenStates });

	//lTransitionFrequency3 = the expected number of times in state i and observing symbol m
	MultidimensionalArray<double> lTransitionFrequency3 = MultidimensionalArray<double>(2, { lNumberOfHiddenStates, lNumberOfEmissionStates });

	Array<double> lFrequency4 = Array<double>(lNumberOfHiddenStates);

	// The total probability of the total observation sequence happening
	double lProbabilityOfObservationSequence = 0.0;

	for (unsigned lTcount = 0; lTcount < lTimeIterations; lTcount++)
	{
		lAlpha.setRow({ lTcount }, ForwardAlgorithm(aObservations, aModelParameters, lTcount));
		lBeta.setRow({ lTcount }, BackwardAlgorithm(aObservations, aModelParameters, lTcount));
	}
	return HMMModelParameters();
}