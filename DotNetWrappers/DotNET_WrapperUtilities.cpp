
#include "Stdafx.h"
#include "DotNET_WrapperUtilities.h"

/** Converts a unmanaged parents properties to the managed wrapper version
*/
array<MyLibrary::ParentPropertyBaseWrapper^>^ ConvertPropertyToWrapper(std::vector<std::shared_ptr<ParentPropertyBase>> aProperties)
{
	array<MyLibrary::ParentPropertyBaseWrapper^>^ lResult = gcnew array<MyLibrary::ParentPropertyBaseWrapper^>(aProperties.size());

	for (int lCount = 0; lCount < lResult->Length; lCount++)
	{
		if (aProperties[lCount]->Type() == PropertyType::Double)
		{
			ParentPropertyDouble lTemp = dynamic_cast<ParentPropertyDouble&>(*aProperties[lCount]);
			lResult[lCount] = gcnew MyLibrary::ParentPropertyDoubleWrapper(lTemp.getMax(), lTemp.getMin());
		}
	}

	return lResult;
}

/** Converts a unmanaged parents properties to the managed wrapper version
*/
array<MyLibrary::ParentPropertyBaseWrapper^>^ ConvertPropertyToWrapper(const std::vector<std::unique_ptr<ParentPropertyBase>>& aProperties)
{
	array<MyLibrary::ParentPropertyBaseWrapper^>^ lResult = gcnew array<MyLibrary::ParentPropertyBaseWrapper^>(aProperties.size());

	for (int lCount = 0; lCount < lResult->Length; lCount++)
	{
		if (aProperties[lCount]->Type() == PropertyType::Double)
		{
			ParentPropertyDouble lTemp = dynamic_cast<ParentPropertyDouble&>(*aProperties[lCount]);
			lResult[lCount] = gcnew MyLibrary::ParentPropertyDoubleWrapper(lTemp.getMax(), lTemp.getMin());
		}
	}

	return lResult;
}

/** Converts a managed wrapper parents properties to the unmanaged version
*/
std::vector<std::shared_ptr<ParentPropertyBase>> ConvertWrapperToProperty(array<MyLibrary::ParentPropertyBaseWrapper^>^ aProperties)
{
	std::vector<std::shared_ptr<ParentPropertyBase>> lResult = std::vector<std::shared_ptr<ParentPropertyBase>>(aProperties->Length);

	for (int lCount = 0; lCount < aProperties->Length; lCount++)
	{
		if (aProperties[lCount]->Type() == PropertyType::Double)
		{
			MyLibrary::ParentPropertyDoubleWrapper^ lTemp = safe_cast<MyLibrary::ParentPropertyDoubleWrapper^>(aProperties[lCount]);
			lResult[lCount].reset(new ParentPropertyDouble(lTemp->getMax(), lTemp->getMin()));
		}
	}

	return lResult;
}
