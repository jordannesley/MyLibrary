/**
*  @file    Utilities.h
*  @author  Jordan Nesley
**/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>

class Utilities
{
public:
	static double Abs(const double A);
	static int Abs(const int A);
	static double RandomNumber(const unsigned aSeed, const double aMaxValue, const double aMinValue);
	static std::vector<double> RandomNumber(const unsigned aSeed, const unsigned aNumberOfRandomValues, const double aMaxValue, const double aMinValue);
	static double MapToNewRange(const double aValue, const double aOldMax, const double aOldMin, const double aNewMax, const double aNewMin);
};

#endif
