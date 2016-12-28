/*!
  \file   disjoint_sets.h
  \author Dmitri Volper
  \par    email: dvolper\@digipen.edu
  \date   Mon 28 Jun 2010 06:23:40 AM PDT
  \brief  Disoint sets using quick find implementation (linked lists and 
  a look-up table)

Implements:
	ctor (size)
	Make( id )      make a single element set
	Join( id,id )   join 2 sets
	GetRepresentative( id )

Rationale:
	elements of the set are assumed to be contiguous 0,1,2,3,....
*/

#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H
#include <cstdlib>
#include <fstream> //ostream, istream

////////////////////////////////////////////////////////////
//disjoint subsets
//   head node     node      node      node
//   +--+---+--+   +--+--+   +--+--+   +--+--+
// 1 | 3|   |  |-->| 1|  |-->| 3|  |-->| 4| 0|
//   +--+-+-+--+   +--+--+   +--+--+   +--+--+
//        |       tail pointer          ^
//        +-----------------------------+
//   
//   +--+---+--+  +--+--+
// 2 | 1|   |  |->| 2| 0|
//   +--+-+-+--+  +--+--+
//        |        ^
//        +--------+
//   
//   +--+---+--+
// 3 | 0| 0 | 0|
//   +--+-+-+--+
//   
//   +--+---+--+
// 4 | 0| 0 | 0|
//   +--+-+-+--+
//   



///////////////////////////////////////
// Node for disjoint sets
class Node {
	public:
		Node( size_t const& value );
		Node*  Next() const;
		void   SetNext( Node* new_next );
		size_t Value() const;
		friend std::ostream& operator<< (std::ostream& os, Node const& node);
	private:
		size_t value;
		Node*  next;
		//do not need this 2
		Node( Node const& );
		Node& operator=( Node const& );
};

////////////////////////////////////////////////////////////
// Head node for disjoint sets
class Head {
	public:
		Head( );
		~Head();
		size_t Size() const;
		void   Reset();
		Node*  GetFirst() const;
		Node*  GetLast()  const;
		void   Init( size_t value );
		void   Join( Head* pHead2 );
		friend std::ostream& operator<< (std::ostream& os, Head const& head);
	private:
		size_t counter;
		Node * first;
		Node * last;
		//do not need this 2
		Head( Head const& );
		Head& operator=( Head const& );
};

////////////////////////////////////////////////////////////
class DisjointSets {
	public:
		DisjointSets( size_t const& capacity );
		~DisjointSets();
		void   Make( );
		void   Join( size_t const& id1, size_t const& id2 );
		size_t GetRepresentative( size_t const& id ) const;
		size_t operator[]( size_t const& id ) const;
		friend std::ostream& operator<< (std::ostream& os, DisjointSets const& ds);
	private:
		size_t  size;            // current size
		size_t  capacity;        // maimum capacity
		size_t *representatives; // look-up table ID->ID
		Head   *heads;           // lists' heads
		//do not need this 2
		DisjointSets( DisjointSets const& );
		DisjointSets& operator=( DisjointSets const& );
};


#endif
