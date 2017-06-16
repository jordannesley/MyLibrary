

#ifndef MYLIST_H
#define MYLIST_H


#pragma unmanaged

template<typename T> class MyListIterator;

template<typename T>
struct Node
{
	T Data;
	Node<T>* Next;
	Node<T>* Previous;
};

/**
*
*/
template<typename T>
class MyList
{
	friend class MyListIterator<T>;

private:


	Node<T>* m_Front;
	Node<T>* m_Back;
	int m_NumberOfElements;


public:
	MyList();
	MyList(const MyList<T>& aCopy);
	~MyList();

	void pushBack(const T& aData);
	void pushFront(const T& aData);
	MyListIterator<T> insert(const MyListIterator<T>& aLoc, const T& aData);
	void popFront();
	void popBack();
	void erase(MyListIterator<T> aLoc);

	int size() const;
	MyListIterator<T> find(MyListIterator<T> aStart, MyListIterator<T> aEnd, const T& aValue) const;

	MyListIterator<T> begin() const;
	MyListIterator<T> end() const;
};

////////////////////////////////////////////////////////////////////////////////

template<typename T> 
class MyListIterator
{
	friend class MyList<T>;

private:
	Node<T>* m_Ptr;
	MyListIterator(Node<T>* aNode);
public:
	MyListIterator();
	MyListIterator(const MyListIterator<T>& aCopy);

	MyListIterator<T> next() const;
	MyListIterator<T> prev() const;

	T& operator*();
	bool operator==(const MyListIterator<T>& aRight);
	bool operator!=(const MyListIterator<T>& aRight);
	MyListIterator<T>& operator++();
	MyListIterator<T> operator++(int);
	MyListIterator<T>& operator--();
	MyListIterator<T> operator--(int);
	MyListIterator<T>& operator=(const MyListIterator<T>& aRight);
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
MyList<T>::MyList()
{
	this->m_Front = new Node<T>;
	this->m_Front->Next = nullptr;
	this->m_Front->Previous = nullptr;
	this->m_Back = m_Front;
	this->m_NumberOfElements = 0;
}

template<typename T>
MyList<T>::MyList(const MyList<T>& aCopy)
{
	Node<T>* current = aCopy.m_Front;

	this->m_Front = new Node<T>;
	this->m_Front->Next = nullptr;
	this->m_Front->Previous = nullptr;
	this->m_Back = m_Front;
	this->m_NumberOfElements = 0;

	while (current != nullptr)
	{
		this->pushBack(current->Data);
		current = current->Next;
	}
}

template<typename T>
MyList<T>::~MyList()
{
	Node<T>* temp;
	while (this->m_Front != nullptr)
	{
		temp = this->m_Front;
		this->m_Front = m_Front->Next;
		delete temp;
	}
	this->m_Front = nullptr;
	this->m_Back = nullptr;
}

template<typename T>
void MyList<T>::pushBack(const T& aData)
{
	Node<T>* NewNode = new Node<T>;
	NewNode->Data = aData;
	NewNode->Next = this->m_Back;
	NewNode->Previous = this->m_Back->Previous;
	this->m_Back->Previous = NewNode;

	this->m_NumberOfElements++;

	if (this->m_NumberOfElements == 1)
	{
		this->m_Front = this->m_Back->Previous;
	}
}

template<typename T>
void MyList<T>::pushFront(const T& aData)
{
	Node<T>* NewNode = new Node<T>;
	NewNode->Data = aData;
	NewNode->Next = this->m_Front;
	NewNode->Previous = nullptr;

	this->m_Front->Previous = NewNode;
	this->m_Front = NewNode;
	
	this->m_NumberOfElements++;
}

template<typename T>
MyListIterator<T> MyList<T>::insert(const MyListIterator<T>& aLoc, const T& aData)
{
	if (aLoc.m_Ptr == this->m_Front)
	{
		this->pushFront(aData);
		return this->begin();
	}
	else if (aLoc.m_Ptr == this->m_Back)
	{
		this->pushBack(aData);
		return (this->end())--;
	}
	else
	{
		Node<T>* CurrentNode = this->m_Front->Next;
		while (CurrentNode != aLoc.m_Ptr)
		{
			CurrentNode = CurrentNode->Next;
		}

		Node<T>* NewNode = new Node<T>;
		NewNode->Data = aData;
		NewNode->Next = CurrentNode;
		NewNode->Previous = CurrentNode->Previous;

		CurrentNode->Previous->Next = NewNode;
		CurrentNode->Previous = NewNode;

		this->m_NumberOfElements++;

		return MyListIterator<T>(NewNode);
	}
}

template<typename T>
void MyList<T>::popFront()
{
	if (this->m_NumberOfElements == 0) return;

	Node<T>* NewFront = this->m_Front->Next;
	NewFront->Previous = nullptr;
	delete this->m_Front;
	this->m_Front = NewFront;

	this->m_NumberOfElements--;
}

template<typename T>
void MyList<T>::popBack()
{
	if (this->m_NumberOfElements == 0) return;
	
	if (this->m_Back->Previous == this->m_Front)
	{
		this->m_Front = this->m_Back;
	}
	else
	{
		Node<T>* NewBack = this->m_Back->Previous->Previous;
		NewBack->Next = this->m_Back;
	}

	delete this->m_Back->Previous;
	this->m_Back->Previous = nullptr;

	this->m_NumberOfElements--;
}

template<typename T>
void MyList<T>::erase(MyListIterator<T> aLoc)
{
	if (aLoc.m_Ptr == this->m_Front)
	{
		this->popFront();
	}
	else if (aLoc.m_Ptr == this->m_Back->Previous)
	{
		this->popBack();
	}
	else
	{
		Node<T>* RemoveNode = aLoc.m_Ptr;
		RemoveNode->Previous->Next = RemoveNode->Next;
		RemoveNode->Next->Previous = RemoveNode->Previous;
		delete RemoveNode;
	}
}

template<typename T>
int MyList<T>::size() const
{
	return this->m_NumberOfElements;
}

template<typename T>
MyListIterator<T> MyList<T>::find(MyListIterator<T> aStart, MyListIterator<T> aEnd, const T& aValue) const
{
	for (MyListIterator<T> it = aStart; it != aEnd; it++)
	{
		if (*it == aValue)
		{
			return it;
		}
	}
	return aEnd;
}

template<typename T>
MyListIterator<T> MyList<T>::begin() const
{
	return MyListIterator<T>(this->m_Front);
}

template<typename T>
MyListIterator<T> MyList<T>::end() const
{
	return MyListIterator<T>(this->m_Back);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

template<typename T>
MyListIterator<T>::MyListIterator()
{
	this->m_Ptr = nullptr;
}

template<typename T>
MyListIterator<T>::MyListIterator(const MyListIterator<T>& aCopy)
{
	this->m_Ptr = aCopy.m_Ptr;
}

template<typename T>
MyListIterator<T>::MyListIterator(Node<T>* aNode)
{
	this->m_Ptr = aNode;
}
template<typename T>
MyListIterator<T> MyListIterator<T>::next() const
{
	return MyListIterator<T>(this->m_Ptr->Next);
}

template<typename T>
MyListIterator<T> MyListIterator<T>::prev() const
{
	return MyListIterator<T>(this->m_Ptr->Previous);
}

template<typename T>
T& MyListIterator<T>::operator*()
{
	return this->m_Ptr->Data;
}

template<typename T>
bool MyListIterator<T>::operator==(const MyListIterator<T>& aRight)
{
	return (this->m_Ptr == aRight.m_Ptr);
}

template<typename T>
bool MyListIterator<T>::operator!=(const MyListIterator<T>& aRight)
{
	return !(*this == aRight);
}

template<typename T>
MyListIterator<T>& MyListIterator<T>::operator++()
{
	this->m_Ptr = this->m_Ptr->Next;
	return *this;
}

template<typename T>
MyListIterator<T> MyListIterator<T>::operator++(int)
{
	MyListIterator<T> Result(*this);
	++(*this);
	return Result;
}

template<typename T>
MyListIterator<T>& MyListIterator<T>::operator--()
{
	this->m_Ptr = this->m_Ptr->Previous;
	return *this;
}

template<typename T>
MyListIterator<T> MyListIterator<T>::operator--(int)
{
	MyListIterator<T> Result(*this);
	--(*this);
	return Result;
}

template<typename T>
MyListIterator<T>& MyListIterator<T>::operator=(const MyListIterator<T>& aRight)
{
	this->m_Ptr = aRight.m_Ptr;
	return *(this);
}

#endif