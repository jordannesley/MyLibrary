/**
*  @file    Utilities.cpp
*  @author  Jordan Nesley
**/

#include "Utilities.h"

#pragma unmanaged

/** Calculates the absolute value of the number.
* @param A The number.
* @return The absolute value of the number.
*/
double Utilities::Abs(const double A)
{
	return (A < 0.0 ? A*-1.0 : A);
}

/** Calculates the absolute value of the number.
* @param A The number.
* @return The absolute value of the number.
*/
int Utilities::Abs(const int A)
{
	return (A < 0 ? A*-1 : A);
}

/** Creates a random number based on a seed number.
* @param aSeed The seed number.
* @return The random number.
*/
double Utilities::RandomNumber(const unsigned aSeed, const double aMaxValue, const double aMinValue)
{
	return Utilities::RandomNumber(aSeed, 100, aMaxValue, aMinValue)[99];
}

/** Creates a set of random numbers based on a seed number.
* @param aSeed The seed number.
* @return The random numbers.
*/
std::vector<double> Utilities::RandomNumber(const unsigned aSeed, const unsigned aNumberOfRandomValues, const double aMaxValue, const double aMinValue)
{
	std::vector<double> lResult(aNumberOfRandomValues);

	int a = 16807, b = 1, m = 2147483647;
	std::vector<double> rNumber(aNumberOfRandomValues + 6);

	rNumber[0]= aSeed;
	for (unsigned lCount = 1; lCount < rNumber.size(); lCount++)
	{
		rNumber[lCount] = (a * (int)rNumber[lCount - 1] + b) % m;
	}

	for (unsigned lCount = 5; lCount< rNumber.size(); lCount++)
	{
		rNumber[lCount - 5] = rNumber[lCount];
	}

	for (unsigned lCount = 0; lCount < lResult.size(); lCount++)
	{
		lResult[lCount] = Utilities::MapToNewRange(Utilities::Abs(rNumber[lCount]), (double)m, 0.0, aMaxValue, aMinValue);
	}

	return lResult;
}

/** Maps a value on a number line (in between some max and min value) to a new number line with a new max an min.
* @param aValue The value to map.
* @param aOldMax The maximum value on the old number line.
* @param aOldMin The minimum value on the old number line.
* @param aNewMax The maximum value on the new number line.
* @param aNewMin The minimum value on the new number line.
* @return The value mapped to the new number line.
*/
double Utilities::MapToNewRange(const double aValue, const double aOldMax, const double aOldMin, const double aNewMax, const double aNewMin)
{
	return aNewMin + (aValue - aOldMin) * (aNewMax - aNewMin) / (aOldMax - aOldMin);
}

