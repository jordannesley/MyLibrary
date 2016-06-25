/**
*  @file    GuassianMixtureModel.h
*  @author  Jordan Nesley
**/

#ifndef GUASSIANMIXTUREMODEL_H
#define GUASSIANMIXTUREMODEL_h

#include "TwoD_Matrix.h"
#include "Array.h"

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

		TwoD_Matrix_D m_States;

		const static double ZERO;

		static Array<double> scaleData(const Array<double> aData, double* aMax, double* aMin);
		static void calculateMaxAndMin(const Array<double> aData, double* aMax, double* aMin);
		static double calculateIncrement(const double& aMax, const double& aMin, const int& aGMMStates);
		static double calculateTau(const double& aIncrement);
		static TwoD_Matrix_D calculateDiscreteStates(const Array<double> aData, const double& aMin, const int& aGMMStates, const double& aIncrement, const double& aTau);
		static Array<double> inverseGuassianMixtureModel(const TwoD_Matrix_D& aDiscreteStates, const double& aMax, const double& aMin, const int& aGMMStates);
		
	public:
		GuassianMixtureModel();
		GuassianMixtureModel(const unsigned& aGMMStates, const Array<double> aData);
		Array<double> inverseGuassianMixtureModel() const;
		TwoD_Matrix_D getStates() const;
};

#endif