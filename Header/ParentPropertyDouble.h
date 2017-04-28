/**
*  @file    ParentPropertyDouble.h
*  @author  Jordan Nesley
**/

#ifndef PARENTPROPERTYDOUBLE_H
#define PARENTPROPERTYDOUBLE_H

#include "ParentPropertyBase.h"
#include "Utilities.h"
#include "DebugLogger.h"

#pragma unmanaged

class ParentPropertyDouble : public ParentPropertyBase
{
	private:
		double m_Value;
		double m_MaxValue;
		double m_MinValue;

	public:
		ParentPropertyDouble();
		ParentPropertyDouble(const double aMaxValue, const double aMinValue);
		ParentPropertyDouble(const double aValue, const double aMaxValue, const double aMinValue);
		ParentPropertyDouble(const ParentPropertyDouble & aNew);

		~ParentPropertyDouble() override;

		ParentPropertyBase* Clone() override;
		PropertyType Type() override;

		double getValue() const;
		double getMax() const;
		double getMin() const;

		ParentPropertyBase* Crossover(const ParentPropertyBase * const aParentProperty, const unsigned aSeed) const;
		void Randomize(const unsigned aSeed);
};

#endif