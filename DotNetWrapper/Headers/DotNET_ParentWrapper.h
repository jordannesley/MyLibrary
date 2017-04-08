#pragma once

#include "Parent.h"
#include "DotNET_WrapperUtilities.h"

using namespace System;

namespace MyLibrary {
	ref class ParentWrapper
	{
		private:
			Parent* m_Parent;
		public:
			ParentWrapper();
			~ParentWrapper();

			double getFitness();
			unsigned getRank();
			array<ParentPropertyBaseWrapper^>^ getProperties();

	};
}