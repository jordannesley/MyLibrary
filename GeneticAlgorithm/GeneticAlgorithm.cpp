/**
*  @file    GeneticAlgorithm.cpp
*  @author  Jordan Nesley
**/

#include "GeneticAlgorithm.h"

/** Constructor for Genetic Algorithm.
* @param aSeed The seed number to use for randomization.
* @param aGAParameters The parameters that define the genetic algorithm.
* @param aFunction The function that defines the fitness for each parent.
*/
GeneticAlgorithm::GeneticAlgorithm(const unsigned int aSeed, const GeneticAlgorithmParameters aGAParameters, double(*aFunction)(std::vector<std::unique_ptr<ParentPropertyBase>>&&))
{
	this->m_Seed = aSeed;
	this->m_GAParameters = aGAParameters;
	this->Function = aFunction;

	this->m_BestParent = Parent();
	this->m_BestParent.setFitness(DBL_MAX);
}

/** Start the genetic algorithm.
*/
void GeneticAlgorithm::Start()
{
	this->m_ParentArray = std::vector<Parent>(this->m_GAParameters.getNumberOfParents());
	for (unsigned lCount = 0; lCount < this->m_ParentArray.size(); lCount++)
	{
		this->m_ParentArray[lCount] = Parent(this->m_GAParameters.getParentTemplate());
		this->m_ParentArray[lCount].Randomize(this->m_Seed++);
	}

	for (unsigned lGenCount = 0; lGenCount < this->m_GAParameters.getNumberOfGenerations(); lGenCount++)
	{
		for (unsigned lParentCount = 0; lParentCount < this->m_ParentArray.size(); lParentCount++)
		{
			this->m_ParentArray[lParentCount].setFitness(this->Function(this->m_ParentArray[lParentCount].getProperties()));
		}

		rankParents(this->m_ParentArray);

		if (this->m_ParentArray[0].getFitness() < this->m_BestParent.getFitness())
		{
			this->m_BestParent = this->m_ParentArray[0];
		}

		if (lGenCount != this->m_GAParameters.getNumberOfGenerations())
		{
			this->m_ParentArray = breed(this->m_ParentArray, this->m_Seed++);
		}
	}

}

/** Sorts the array of parents based on the fitness score. Position in the array is equal to the ranking.
* @param aParentArray The array of parents to sort. Note: The array will be modified.
*/
void GeneticAlgorithm::rankParents(std::vector<Parent>& aParentArray)
{
	// sorts the array of parents based on the fitness score. Position in the array is equal to the ranking.
	for (unsigned lCount1 = 0; lCount1 < aParentArray.size()-1; lCount1++)
	{
		Parent lTemp = aParentArray[lCount1 + 1];
		for (int lCount2 = lCount1; lCount2 >= 0; lCount2--)
		{
			if (aParentArray[lCount2].getFitness() > lTemp.getFitness())
			{
				aParentArray[lCount2 + 1] = aParentArray[lCount2];
				aParentArray[lCount2] = lTemp;
			}
		}
	}

	for (unsigned lCount = 0; lCount < aParentArray.size(); lCount++)
	{
		aParentArray[lCount].setRank(lCount);
	}

}

/** Breeds the parents to make a new generation of parents.
* @param aParentArray The parent array to breed.
* @param aNumberToSelect The number of parents to select for breeding.
*/
std::vector<Parent> GeneticAlgorithm::breed(const std::vector<Parent>& aParentArray, unsigned aSeed)
{
	std::vector<Parent> lNewGeneration(aParentArray.size());
	std::vector<std::tuple<unsigned, unsigned>> lCouple(aParentArray.size());
	double lSum = pow(aParentArray.size(), 2.0);
	double temp;
	std::vector<double> lRandomNumbers1 = Utilities::RandomNumber(aSeed++, aParentArray.size(), 0.0, lSum);
	std::vector<double> lRandomNumbers2 = Utilities::RandomNumber(aSeed++, aParentArray.size(), 0.0, lSum);

	for (unsigned lCount = 0; lCount < lNewGeneration.size(); lCount++)
	{
		for (unsigned lCount2 = 0; lCount2 < lNewGeneration.size(); lCount2++)
		{
			if (lRandomNumbers1[lCount] <= pow(aParentArray.size() - lCount2, 2.0) && lRandomNumbers1[lCount] > pow(aParentArray.size() - (lCount2 + 1), 2.0))
			{
				std::get<0>(lCouple[lCount]) = lCount2;
			}
			if (lRandomNumbers2[lCount] <= pow(aParentArray.size() - lCount2, 2.0) && lRandomNumbers2[lCount] > pow(aParentArray.size() - (lCount2 + 1), 2.0))
			{
				std::get<1>(lCouple[lCount]) = lCount2;
			}
		}

		// if the two selected parents are the same then increment one of them so they are different.
		if (std::get<0>(lCouple[lCount]) == std::get<1>(lCouple[lCount]))
		{
			if (std::get<0>(lCouple[lCount]) == lCouple.size() - 1)
			{
				std::get<0>(lCouple[lCount]) = 0;
			}
			else
			{
				std::get<0>(lCouple[lCount]) = std::get<0>(lCouple[lCount]) + 1;
			}
		}

		// cross over the parents .... get freaky!
		lNewGeneration[lCount] = aParentArray[std::get<0>(lCouple[lCount])].Crossover(aParentArray[std::get<1>(lCouple[lCount])], aSeed++);
	}

	return lNewGeneration;
}
