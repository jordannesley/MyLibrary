// GeneticAlgorithmParameters.h

#pragma once

#include "GeneticAlgorithmParameters.h"
#include "DotNET_WrapperUtilities.h"

using namespace System;

namespace MyLibrary {

	/** A wrapper for the GeneticAlgorithmParameters.
	*/
	public ref class GeneticAlgorithmParametersWrapper
	{
		private:
			GeneticAlgorithmParameters* m_GAParameters;

		public:
			GeneticAlgorithmParametersWrapper();
			GeneticAlgorithmParametersWrapper(const unsigned aNumberOfGenerations, const unsigned aNumberOfParents, const double aRandomParentRatio, array<ParentPropertyBaseWrapper^>^ aParentPropertyTemplate);
			~GeneticAlgorithmParametersWrapper();

			unsigned getNumberOfGenerations();
			unsigned getNumberOfParents();
			double getRandomParentRatio();
			array<ParentPropertyBaseWrapper^>^ getParentTemplate();
	};
}

