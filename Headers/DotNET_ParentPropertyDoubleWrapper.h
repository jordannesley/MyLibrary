#pragma once

using namespace System;

#include "DotNET_ParentPropertyBaseWrapper.h"
#include "ParentPropertyDouble.h"

#pragma managed

namespace MyLibrary {

	/** A wrapper for the ParentPropertyDoubleWrapper.
	*/
	public ref class ParentPropertyDoubleWrapper : public ParentPropertyBaseWrapper
	{
		private:	
			ParentPropertyDouble* m_Property;

		public:
			PropertyType Type() override;

			ParentPropertyDoubleWrapper();
			ParentPropertyDoubleWrapper(const double aMaxValue, const double aMinValue);
			ParentPropertyDoubleWrapper(const double aValue, const double aMaxValue, const double aMinValue);
			ParentPropertyDoubleWrapper(const ParentPropertyDoubleWrapper^ aNew);


			~ParentPropertyDoubleWrapper() override;

			double getValue();
			double getMax();
			double getMin();

			ParentPropertyBaseWrapper^ Crossover(const ParentPropertyBaseWrapper^ aParentProperty, const unsigned aSeed) override;
			void Randomize(const unsigned aSeed) override;
	};
}