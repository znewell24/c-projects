/***********************************************************************
* Header:
*    Vector
************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

// forward declaration for VectorIterator
template <class T>
class VectorIterator;

/************************************************
 * VECTOR
 * A class that holds stuff
 ***********************************************/
template <class T>
class Vector
{
public:
   // default constructor : empty and kinda useless
   Vector() : numItems(0), cap(0), data(NULL) {}

   // copy constructor : copy it
   Vector(const Vector & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Vector(int capacity) throw (const char *);
   
   // destructor : free everything
   ~Vector() { if (cap) delete [] data; }
   
   // = operator assignment. copy data from one vector to another.
   Vector <T> & operator= (Vector & rhs) throw (const char *)
   {
	   if (rhs.size() == 0)
	   {
		   this->data = NULL;
		   this->cap = 0;
		   this->numItems = 0;
		   return *this;
	   }

	   try 
	   {
		   data = new T[rhs.capacity()];
	   }
	   catch (std::bad_alloc)
	   {
		   throw "Error: Unable to allocate a new buffer for Vector";
	   }

	   cap = rhs.capacity();
	   numItems = rhs.size();
	   for (int i = 0; i < cap ; i++)
	   {
		   data[i] = rhs.data[i];
	   }

	   return *this;
   }

   // [] operator assignment
   T & operator [] (int index) throw (const char *) 
   {
	   if (index < 0 || index > numItems)
	   {
		   throw "ERROR: Invalid index";
	   }

	   else
	   {
		   return data[index];
	   }
   }

   // [] operator assignment
   T operator [] (int index) const throw (const char *)
   {
	   if (index < 0 || index > numItems)
	   {
		   throw "ERROR: Invalid index";
	   }

	   else
	   {
		   return data[index];
	   }
   }

   // is the Vector currently empty
   bool empty() const  { return numItems == 0;}

   // remove all the items from the Vector
   void clear()        { numItems = 0;}

   // how many items are currently in the Vector?
   int size() const { return numItems;}

   // add an item to the Vector
   void insert(const T & t) throw (const char *);
   
   // capacity returns the size currently allocated
   int capacity() { return cap;}

   //push_back adds a new element to the end of the vector
   void push_back(T index) throw (const char *);

   // return an iterator to the beginning of the list
   VectorIterator <T> begin() { return VectorIterator<T>(data); }

   // return an iterator to the end of the list
   VectorIterator <T> end() { return VectorIterator<T>(data + numItems);}
   
private:
   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Vector?
   int cap;           // how many items can I put on the Vector before full?
};

/**************************************************
 * Vector ITERATOR
 * An iterator through Vector
 *************************************************/
template <class T>
class VectorIterator
{
  public:
   // default constructor
  VectorIterator() : p(NULL) {}

   // initialize to direct p to some item
  VectorIterator(T * p) : p(p) {}


   // not equals operator
   bool operator != (const VectorIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   bool operator == (const VectorIterator & rhs) const
   {
	   return rhs.p == this->p;
   }

   VectorIterator & operator = (const VectorIterator & rhs)
   {
	   this->p = rhs.p;

	   return *this;
   }

   // dereference operator
   T & operator * ()
   {
      return *p;
   }

   // prefix increment
   VectorIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   VectorIterator <T> operator++(int postfix)
   {
      VectorIterator tmp(*this);
      p++;
      return tmp;
   }
   
  private:
   T * p;
};

/*******************************************
 * Vector :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Vector <T> :: Vector(const Vector <T> & rhs) throw (const char *)
{
   assert(rhs.cap >= 0);
      
   // do nothing if there is nothing to do
   if (rhs.cap == 0)
   {
      cap = numItems = 0;
      data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[rhs.cap];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }
   
   // copy over the capacity and size
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.cap);
   cap = rhs.cap;
   numItems = rhs.numItems;

   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < numItems; i++)
      data[i] = rhs.data[i];

   // the rest needs to be filled with the default value for T
   for (int i = numItems; i < cap; i++)
      data[i] = T();
}

/**********************************************
 * Vector : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Vector to "capacity"
 **********************************************/
template <class T>
Vector <T> :: Vector(int capacity) throw (const char *)
{
   assert(capacity >= 0);
   
   // do nothing if there is nothing to do
   if (capacity == 0)
   {
      this->cap = this->numItems = 0;
      this->data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

      
   // copy over the stuff
   this->cap = capacity;
   this->numItems = 0;

   // initialize the Vector by calling the default constructor
   for (int i = 0; i < capacity; i++)
      data[i] = T();
}

/***************************************************
 * Vector :: INSERT
 * Insert an item on the end of the Vector
 **************************************************/
template <class T>
void Vector <T> :: insert(const T & t) throw (const char *)
{
   // do we have space?
   if (cap == 0 || cap == numItems)
      throw "ERROR: Insufficient space";
   
   // add an item to the end
   data[numItems++] = t;
}

/***************************************************
* Vector :: push_back
* Insert an item on the end of the Vector
**************************************************/
template <class T>
void Vector <T> ::push_back(T index) throw (const char *)
{
	//if cap is 0 set it to 1
	if (cap == 0)
		cap = 1;

	//if at cap, double cap
	if (cap == numItems)
	{
		cap = (cap * 2);
	}

	//if doubling the cap doesnt work, throw an error
	T *array = NULL;
	try
	{
		array = new T[cap];
		
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a new buffer for Vector";
	}

	for (int i = 0; i < numItems; i++)
	{
		array[i] = data[i];
	}
	delete[] data;
	data = NULL;
	data = array;
	data[numItems++] = index;
}

#endif // Vector_H

