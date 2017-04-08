
#include "Stdafx.h"
#include "DotNET_ParentWrapper.h"

#pragma managed

/** Default Constructor for ParentWrapper.
*/
MyLibrary::ParentWrapper::ParentWrapper()
{
	this->m_Parent = new Parent();
}

/** Descructor of ParentWrapper
*/
MyLibrary::ParentWrapper::~ParentWrapper()
{
	delete m_Parent;
}

/** Returns the fitness of the parent
* @return The fitness of the parent
*/
double MyLibrary::ParentWrapper::getFitness()
{
	return this->m_Parent->getFitness();
}

/** Returns the rank of the parent
* @return The rank of the parent
*/
unsigned MyLibrary::ParentWrapper::getRank()
{
	return this->m_Parent->getRank();
}

/** Returns the properties of the parent
* @return The properties of the parent
*/
array<MyLibrary::ParentPropertyBaseWrapper^>^ MyLibrary::ParentWrapper::getProperties()
{
	return ConvertPropertyToWrapper(this->m_Parent->getProperties());
}
