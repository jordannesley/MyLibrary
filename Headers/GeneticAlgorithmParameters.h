/**
*  @file    GeneticAlgorithmParameters.h
*  @author  Jordan Nesley
**/

#ifndef GENETICALGORITHMPARAMETERS_H
#define GENETICALGORITHMPARAMETERS_H

#include "ParentPropertyBase.h"

class GeneticAlgorithmParameters
{
	private:
		unsigned int m_NumberOfGenerations;
		unsigned int m_NumberOfParents;
		double m_RandomParentRatio;
		std::vector<std::shared_ptr<ParentPropertyBase>> m_ParentTemplate;

	public:
		GeneticAlgorithmParameters();
		GeneticAlgorithmParameters(const unsigned int aNumberOfGenerations, const unsigned int aNumberOfParents, const double aRandomParentRatio, std::vector<std::shared_ptr<ParentPropertyBase>> aParentPropertyTemplate);
		GeneticAlgorithmParameters(const GeneticAlgorithmParameters& aCopy);
		GeneticAlgorithmParameters(GeneticAlgorithmParameters&& aMove);

		void swap(GeneticAlgorithmParameters& aFirst, GeneticAlgorithmParameters& aSecond);

		unsigned int getNumberOfGenerations() const;
		unsigned int getNumberOfParents() const;
		double getRandomParentRatio() const;
		std::vector<std::shared_ptr<ParentPropertyBase>> getParentTemplate();

		GeneticAlgorithmParameters& operator=(const GeneticAlgorithmParameters& aRight);

};

#endif
