/**
*  @file    GeneticAlgorithm.h
*  @author  Jordan Nesley
**/

#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "ParentPropertyBase.h"
#include "GeneticAlgorithmParameters.h"
#include "Parent.h"
#include "Utilities.h"
#include <vector>
#include <tuple>
#include <math.h>
#include <cfloat>

class GeneticAlgorithm
{
private:
	unsigned int m_Seed;
	std::vector<Parent> m_ParentArray;
	Parent m_BestParent;
	GeneticAlgorithmParameters m_GAParameters;

	//The function to test
	double(*Function)(std::vector<std::unique_ptr<ParentPropertyBase>>&&);

	void static rankParents(std::vector<Parent>& aParentArray);
	std::vector<Parent> static breed(const std::vector<Parent>& aParentArray, unsigned aSeed);

	public:
		GeneticAlgorithm(const unsigned int aSeed, const GeneticAlgorithmParameters aGAParameters, double(*aFunction)(std::vector<std::unique_ptr<ParentPropertyBase>>&&));
		
		void Start();
};

#endif
