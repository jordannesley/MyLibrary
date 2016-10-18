/**
*  @file    ParentPropertyBase.h
*  @author  Jordan Nesley
**/

#ifndef PARENTPROPERTYBASE_H
#define PARENTPROPERTYBASE_H

#include <vector>
#include <memory>

class ParentPropertyBase
{
	public:
		ParentPropertyBase() {}
		virtual ~ParentPropertyBase() {}
		ParentPropertyBase(const ParentPropertyBase& aCopy) {}

		virtual ParentPropertyBase* Clone() = 0;


		virtual ParentPropertyBase* Crossover(const ParentPropertyBase * const aParentProperty, const unsigned aSeed) const = 0;
		virtual void Randomize(const unsigned aSeed) = 0;
};

#endif