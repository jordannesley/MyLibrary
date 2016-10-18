/**
*  @file    Parent.h
*  @author  Jordan Nesley
**/

#ifndef PARENT_H
#define PARENT_H

#include "ParentPropertyBase.h"
#include "DebugLogger.h"
#include <vector>
#include <memory> // std::shared_ptr

class Parent
{
	private:
		double m_Fitness;
		std::vector<std::unique_ptr<ParentPropertyBase>> m_ParentProperties;
		unsigned m_Rank;

	public:
		Parent();
		Parent(std::vector<std::shared_ptr<ParentPropertyBase>> aParentProperties);
		Parent(const Parent& aNew);
		Parent(Parent&& aNew);

		~Parent();

		void swap(Parent& aSwap);

		double getFitness() const;
		void setFitness(const double aFitness);
		unsigned getRank() const;
		void setRank(const unsigned aRank);
		std::vector<std::unique_ptr<ParentPropertyBase>> getProperties() const;

		Parent Crossover(const Parent aMate, const unsigned aSeed) const;
		void Randomize(const unsigned aSeed);

		Parent& operator = (Parent& aRight);
};

#endif