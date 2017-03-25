// MyLibrary.h

#pragma once

#include "DotNET_GeneticAlgorithmParametersWrapper.h"
#include "GeneticAlgorithm.h"

using namespace System;
using namespace System::Runtime::InteropServices;

#pragma unmanaged

typedef double(__stdcall *UNMANAGED_FITNESS_FUNCTION)(std::vector<std::unique_ptr<ParentPropertyBase>>&& aParentProperties);

class UnmanagedFitnessFunctionClass
{
public:
	UNMANAGED_FITNESS_FUNCTION FitnessFunction;
};

#pragma managed

namespace MyLibrary {

	/** Delegate of fitness function for the genetic algorithm
	*/
	public delegate double FitnessFunction(array<ParentPropertyBaseWrapper^>^);

	/** A wrapper for the GeneticAlgorithm.
	*/
	public ref class GeneticAlgorithmWrapper
	{
		private:
			delegate double UnmanagedFitnessFunctionDelegate(const std::vector<std::unique_ptr<ParentPropertyBase>>& aParentProperties);

			GeneticAlgorithm* m_GenAlg;
			UnmanagedFitnessFunctionClass* m_FitnessFunctionClass;
			FitnessFunction^ ManagedFitnessFunction;
			GCHandle m_gc;

			double MyLibrary::GeneticAlgorithmWrapper::UnmanagedFitnessFunction(const std::vector<std::unique_ptr<ParentPropertyBase>>& aParentProperties);
		public:
			GeneticAlgorithmWrapper(const unsigned% aSeed, MyLibrary::GeneticAlgorithmParametersWrapper^ test, FitnessFunction^ aFitnessFunction);
			~GeneticAlgorithmWrapper();
			void Start();
			
	};
}



