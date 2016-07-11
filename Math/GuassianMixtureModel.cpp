/**
*  @file    GuassianMixtureModel.cpp
*  @author  Jordan Nesley
**/

#include "GuassianMixtureModel.h"

const double GuassianMixtureModel::ZERO = 0.0000000000001;

/** Defualt constructor for GuassianMixtureModel
*/
GuassianMixtureModel::GuassianMixtureModel()
{
	this->m_GMMStates = 0;
	this->m_InputData = Array<double>();
	this->m_ScaledData = Array<double>();
	this->m_Min = 0.0;
	this->m_Max = 0.0;
}

/** Constructor for the GuassianMixtureModel
* @param aGMMStates The number of states for the output.
* @param aData The input data that will be sorted into discrete states.
*/
GuassianMixtureModel::GuassianMixtureModel(const unsigned& aGMMStates, const Array<double> aData)
{
	this->m_GMMStates = aGMMStates;
	this->m_DataLength = aData.getNumberOfElements();
	this->m_InputData = aData;

	// calcualte the scaled data
	this->m_ScaledData = GuassianMixtureModel::scaleData(this->m_InputData, &(this->m_Max), &(this->m_Min));

	// calculate increment
	this->m_Increment = GuassianMixtureModel::calculateIncrement(this->m_Max, this->m_Min, this->m_GMMStates);

	// calculate tau
	this->m_Tau = GuassianMixtureModel::calculateTau(this->m_Increment);

	// Calculate the discrete states
	this->m_States = GuassianMixtureModel::calculateDiscreteStates(this->m_ScaledData, 0.0, this->m_GMMStates, this->m_Increment, this->m_Tau);
}

/** Scales the input data to be between 0.0 and 1.0. Also keeps track of the max and min values so that the data can be "un-scaled".
* @param aDataLength The length of the data to be scaled.
* @param aData The data to be scaled.
* @param aMax Will be returned with the maximum value of the data.
* @param aMin Will be returned with the minimum value of the data.
*/
Array<double> GuassianMixtureModel::scaleData(const Array<double> aData, double* aMax, double* aMin)
{
	Array<double> lResult = Array<double>(aData.getNumberOfElements());

	calculateMaxAndMin(aData, aMax, aMin);

	for (unsigned lCount = 0; lCount < aData.getNumberOfElements(); lCount++)
	{
		lResult.setElement(lCount, (aData.getElement(lCount) - *aMin) / (*aMax - *aMin));
	}

	return lResult;
}

/** Finds the max and min values in an array
* @param aMax Will be returned with the maximum value of the data.
* @param aMin Will be returned with the minimum value of the data.
*/
void GuassianMixtureModel::calculateMaxAndMin(const Array<double> aData, double* aMax, double* aMin)
{
	*aMax = -DBL_MAX;
	*aMin = DBL_MAX;

	double lTemp;

	for (unsigned lCount = 0; lCount < aData.getNumberOfElements(); lCount++)
	{
		lTemp = aData.getElement(lCount);
		if (lTemp > *aMax) *aMax = lTemp;
		if (lTemp < *aMin) *aMin = lTemp;
	}
}

/** Calculates the increment parameter for GuassianMixtureModel
* @param aMax The max parameter.
* @param aMin The min parameter.
* @param aGMMStates The number of discrete states.
*/
double GuassianMixtureModel::calculateIncrement(const double& aMax, const double& aMin, const int& aGMMStates)
{
	return (aMax - aMin) / (aGMMStates - 1);
}

/** Calculates the tau parameter for the GuassianMixtureModel
* @param aIncrement The increment parameter of GuassianMixtureModel.
*/
double GuassianMixtureModel::calculateTau(const double& aIncrement)
{
	return -pow(-aIncrement, 2.0) / log(GuassianMixtureModel::ZERO);
}

/** Calculates the discrete statesn using hard clustering
* @param aDataLength The length of the data to be scaled.
* @param aData The scaled data to be scaled.
* @param aMin The min parameter.
* @param aGMMStates The number of discrete states.
* @param aIncrement The increment parameter of GuassianMixtureModel.
* @param aTau The tau parameter of GuassianMixtureModel.
*/
MultidimensionalArray<double> GuassianMixtureModel::calculateDiscreteStates(const Array<double> aData, const double& aMin, const unsigned& aGMMStates, const double& aIncrement, const double& aTau)
{
	double lLocation;
	double temp;
	MultidimensionalArray<double> lResult = MultidimensionalArray<double>(2, { aData.getNumberOfElements(), aGMMStates});
	double lMax;

	unsigned lSelectedState = 0;
	for (unsigned lCount1 = 0; lCount1 < aData.getNumberOfElements(); lCount1++)
	{
		lMax = -DBL_MAX;
		for (unsigned lCount2 = 0; lCount2 < aGMMStates; lCount2++)
		{
			// Calculate the location of the center of the guassian function (the peak)
			lLocation = aMin + lCount2 * aIncrement;

			// calculate the probability of the data point belonging to the Guassian Mixture Model State.
			
			temp = exp(-pow(lLocation - aData.getElement(lCount1), 2.0) / aTau);
			if (temp > lMax)
			{
				lMax = temp;
				lSelectedState = lCount2;
			}
		}

		// set the selected state of the observation to 1.0
		lResult.setElement({ lCount1, lSelectedState }, 1.0);
	}

	return lResult;
}

/** undoes the guassian mixture model and returns the an array of data
* @param aDiscreteStates The discrete states of the GusassianMixtureModel.
* @param aMax The max parameter.
* @param aMax The min parameter.
* @param aGMMStates The number of discrete states.
*/
Array<double> GuassianMixtureModel::inverseGuassianMixtureModel(const MultidimensionalArray<double>& aDiscreteStates, const double& aMax, const double& aMin, const unsigned& aGMMStates)
{
	unsigned lDataLength = aDiscreteStates.getNumberOfElementsOfDimension(0);
	double lIncrement = GuassianMixtureModel::calculateIncrement(aMax, aMin, aGMMStates);

	Array<double> lResult = Array<double>(lDataLength);

	for (unsigned lCount1 = 0; lCount1 < lDataLength; lCount1++)
	{
		lResult.setElement(lCount1, 0.0);

		// combine the discrete states by calculating the expectation value for each row
		for (unsigned lCount2 = 0; lCount2 < aGMMStates; lCount2++)
		{
			lResult.setElement(lCount1, lResult.getElement(lCount1) + (aMin + lIncrement * lCount2)*aDiscreteStates.getElement({ lCount1, lCount2}));
		}
	}

	return lResult;
}

/** undoes the guassian mixture model and returns the an array of data
*/
Array<double> GuassianMixtureModel::inverseGuassianMixtureModel() const
{
	return GuassianMixtureModel::inverseGuassianMixtureModel(this->m_States, this->m_Max, this->m_Min, this->m_GMMStates);
}

/** Returns the discrete states
*/
MultidimensionalArray<double> GuassianMixtureModel::getStates() const
{
	return this->m_States;
}