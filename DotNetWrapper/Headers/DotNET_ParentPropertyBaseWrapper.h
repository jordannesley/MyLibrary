#pragma once

using namespace System;

#include "ParentPropertyBase.h"
#include "ParentPropertyDouble.h"

#pragma managed

namespace MyLibrary {

	/** A wrapper for the ParentPropertyBase.
	*/
	public ref class ParentPropertyBaseWrapper abstract
	{
		public:
			virtual PropertyType Type() = 0;

			virtual ParentPropertyBaseWrapper^ Crossover(const ParentPropertyBaseWrapper^ aParentProperty, const unsigned aSeed) { return nullptr; }
			virtual void Randomize(const unsigned aSeed) = 0;
	};
}