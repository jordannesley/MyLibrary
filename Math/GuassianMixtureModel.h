/**
*  @file    GuassianMixtureModel.h
*  @author  Jordan Nesley
**/

#ifndef GUASSIANMIXTUREMODEL_H
#define GUASSIANMIXTUREMODEL_h

#include "MultidimensionalArray.h"
#include "Array.h"
#include <algorithm>    // std::copy
#include <float.h> //DBL_MAX
#include <math.h>

/** A class that will take a continuous set of data and will turn it into a set of discrete states.
*   The class uses hard clustering.
*/
class GuassianMixtureModel
{
	private:
		double m_Max;
		double m_Min;
		double m_Tau;
		double m_Increment;
		
		unsigned m_GMMStates;
		unsigned m_DataLength;

		Array<double> m_InputData;
		Array<double> m_ScaledData;

		MultidimensionalArray<double> m_States;

		const static double ZERO;

		static Array<double> scaleData(const Array<double> aData, double* aMax, double* aMin);
		static void calculateMaxAndMin(const Array<double> aData, double* aMax, double* aMin);
		static double calculateIncrement(const double& aMax, const double& aMin, const int& aGMMStates);
		static double calculateTau(const double& aIncrement);
		static MultidimensionalArray<double> calculateDiscreteStates(const Array<double> aData, const double& aMin, const unsigned& aGMMStates, const double& aIncrement, const double& aTau);
		static Array<double> inverseGuassianMixtureModel(const MultidimensionalArray<double>& aDiscreteStates, const double& aMax, const double& aMin, const unsigned& aGMMStates);
		
	public:
		GuassianMixtureModel();
		GuassianMixtureModel(const unsigned& aGMMStates, const Array<double> aData);
		Array<double> inverseGuassianMixtureModel() const;
		MultidimensionalArray<double> getStates() const;
};

#endif