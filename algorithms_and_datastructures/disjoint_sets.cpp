////
/// assign: MST - disjoint sets + color array
/// file: disjoint_sets.cpp
/// student: Jimi Huard
/// about: implements disjoint sets object and associated
/// objects
///
#include "disjoint_sets.h"

//class Node implementation
Node::Node( size_t const& value ) : value(value), next() {}

Node* Node::Next() const { return next; }

void  Node::SetNext( Node* new_next ) { next = new_next; }

size_t Node::Value() const { return value; }

std::ostream& operator<< (std::ostream& os, Node const& node) {
	os << "(" << node.value << " ) -> ";
	return os;
}

//class Head implementation
Head::Head( ) : counter(), first(), last() {} 

Head::~Head()
{
	while (first != last) {
		Node *temp = first ;
		first = first->Next() ;
		delete temp ;
	}

	delete last ;
}

size_t Head::Size() const      { return counter; }

void   Head::Reset()           { counter = 0; last = first = NULL; }

Node*  Head::GetFirst() const  { return first; }

Node*  Head::GetLast()  const  { return last; }

void   Head::Init( size_t value ) {
	first = last = new Node( value );
	counter = 1;
}

/*
	this one += that one
	[this]first1    ---   last1
	[that]first2    ---   last2
	...
	[this]first1 --- last2
	[that] 0
*/
void Head::Join(Head* pHead2)
{
	// first = first
	last->SetNext(pHead2->first) ;
	last = pHead2->last ;

	counter += pHead2->counter ;
	
	pHead2->Reset() ;
}

std::ostream& operator<< (std::ostream& os, Head const& head) {
	os << "[" << head.counter << " ] -> ";
	return os;
}

//class DisjointSets implementation
DisjointSets::DisjointSets( size_t const& capacity ) : 
	size(0),
	capacity(capacity),
	representatives(new size_t[capacity]),
	heads          (new Head[capacity])
{ }

DisjointSets::~DisjointSets() {
	delete [] heads;
	delete [] representatives;
}

void DisjointSets::Make( ) {
	if ( size == capacity ) throw "DisjointSets::Make(...) out of space";
	heads[size].Init( size );
	representatives[size] = size;
	++size;
}

void DisjointSets::Join(size_t const& id1, size_t const& id2)
{
	// assumes id1 != id2, color1 != color2
	size_t rep1 = representatives[id1] ;
	size_t rep2 = representatives[id2] ;
	
	if (rep1 == rep2) return ; // nothing to do

	// update colors
	Head &head1 = heads[rep1] ;
	Head &head2 = heads[rep2] ;
	if (head2.Size() >= head1.Size()) {
		for (Node const *cur = head1.GetFirst(); cur != head1.GetLast(); cur = cur->Next())
			representatives[cur->Value()] = rep2 ;
	
		// sanity check, what if last=first=0?
		if (head1.GetLast()) representatives[head1.GetLast()->Value()] = rep2 ;

		head2.Join(&head1) ;
	} else {
		for (Node const *cur = head2.GetFirst(); cur != head2.GetLast(); cur = cur->Next())
			representatives[cur->Value()] = rep1 ;
	
		// sanity check, what if last=first=0?
		if (head2.GetLast()) representatives[head2.GetLast()->Value()] = rep1 ;

		head1.Join(&head2) ;
	}
}

size_t DisjointSets::GetRepresentative( size_t const& id ) const {
	return representatives[id];
}

size_t DisjointSets::operator[]( size_t const& id ) const {
	return representatives[id];
}

std::ostream& operator<< (std::ostream& os, DisjointSets const& ds) {
	for (size_t i=0; i<ds.size; ++i ) {
		os << i << ":  ";
		Head *p_head = &ds.heads[i];
		os << *p_head;
		Node* p_node = p_head->GetFirst();
		while ( p_node ) {
			os << *p_node;
			p_node = p_node->Next();
		}
		os << "NULL (representative " << ds.representatives[i] << ")\n";
	}
	return os;
}
