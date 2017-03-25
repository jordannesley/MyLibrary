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

#pragma unmanaged

typedef double(__stdcall *UNMANAGED_FITNESS_FUNCTION)(std::vector<std::unique_ptr<ParentPropertyBase>>&& aParentProperties);

class GeneticAlgorithm
{
private:
	unsigned int m_Seed;
	std::vector<Parent> m_ParentArray;
	Parent m_BestParent;
	GeneticAlgorithmParameters m_GAParameters;

	//The function to test
	UNMANAGED_FITNESS_FUNCTION m_Function;

	void static rankParents(std::vector<Parent>& aParentArray);
	std::vector<Parent> static breed(const std::vector<Parent>& aParentArray, unsigned aSeed);

	public:
		GeneticAlgorithm(const unsigned int aSeed, const GeneticAlgorithmParameters aGAParameters, UNMANAGED_FITNESS_FUNCTION aFitnessFunction);

		void Start();

		Parent GetBestParent();
};

#endif
