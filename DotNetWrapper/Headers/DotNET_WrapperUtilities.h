#pragma once

#include "DotNET_ParentPropertyBaseWrapper.h"
#include "DotNET_ParentPropertyDoubleWrapper.h"

#pragma managed

using namespace System;

array<MyLibrary::ParentPropertyBaseWrapper^>^ ConvertPropertyToWrapper(std::vector<std::shared_ptr<ParentPropertyBase>> aProperties);
																	 //std::vector<std::unique_ptr<ParentPropertyBase>>
array<MyLibrary::ParentPropertyBaseWrapper^>^ ConvertPropertyToWrapper(const std::vector<std::unique_ptr<ParentPropertyBase>>& aProperties);
std::vector<std::shared_ptr<ParentPropertyBase>> ConvertWrapperToProperty(array<MyLibrary::ParentPropertyBaseWrapper^>^ aProperties);
