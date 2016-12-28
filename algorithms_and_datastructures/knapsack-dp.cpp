/*
File       knapsack-dp.cpp
Author     Jimi Huard
Assignment Knapsack, Dynamic Programming
Description
	Contains the definitions for the knapsack dynamic
	programming assignment functions, including the recursive
	and dynamic programmed implementations. 
*/

#include "knapsack-dp.h"
#include <iostream>
#include <numeric>
#include <algorithm>

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value )
: weight(weight), value(value)
{
}

////////////////////////////////////////////////////////////
	Item::Item( Item const& original )
: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item)
{
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item)
{
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W )
{
	int num_items = items.size();

	std::vector< std::vector< int > > table(W + 1, std::vector<int>(num_items + 1, 0)) ;

/* taken care of in constructor
	for (int i = 0; i <= std::max(num_items, W); ++i) {
		if (i <= num_items) table[0][i] = 0 ;
		if (i <= W) table[i][0] = 0 ;
	}
*/

	for (int i = 1; i <= num_items; ++i) {
		for (int j = 1; j <= W; ++j) {
			int v1 = -1 ;
			int weight = items[i - 1].weight ;

			if (j >= weight)
				v1 = table[j - weight][i - 1] + items[i - 1].value ;

			table[j][i] = std::max(v1, table[j][i - 1]) ;
		}
	}

	//print final table - for debugging?
	//do not delete this code
	if ( num_items + W < 50 ) { //print only if table is not too big
		std::cout << "   ";
		for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
		std::cout << "  items\n        ";
		for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
		std::cout << "\n   ";
		for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
		std::cout << std::endl;

		for ( int w=0; w<=W; ++w) {
			std::cout << w << "| ";
			for ( int n=0; n<=num_items; ++n) {
				std::cout << table[w][n] << "     ";
			}
			std::cout << std::endl;
		}
	}
	//end do not delete this code


	//figure out which items are in the bag based on the table
	std::vector<int> bag;
	// item either came from table[W - w][n - 1] + v
	// or table[W][n - 1]
	// i.e. take/not take.
	int max = table[W][num_items] ;
	int wRem = W ;

	for (int i = num_items; i > 0; --i) {
		int wi = items[i - 1].weight ;
		int vi = items[i - 1].value ;

		if (max == table[wRem][i - 1]) {
			// not take
		} else {
			// take
			bag.push_back(i - 1) ;
			max -= vi ;
			wRem -= wi ;
		}
	}

//	std::reverse(bag.begin(), bag.end()) ;

	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag )
{
	std::vector<int>::const_iterator it   = bag.begin(),
		it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) {
		accum += items[ *it ].value;
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

typedef std::vector < std::vector < int > > Table ;

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W )
{
	int num_items = items.size();

	std::vector< std::vector< int > > table(W + 1, std::vector<int>(num_items + 1, -1)) ;
	
	// initial conditions (as per output, to get 0's in some places where recursion is not actually necessary)
	for (int i = 0; i <= std::max(num_items, W); ++i) {
		if (i <= num_items) table[0][i] = 0 ;
		if (i <= W) table[i][0] = 0 ;
	}

	int max = knapsackRecMemAux(items, W, num_items, table) ;

	//print table - debugging?
	//do not delete this code
	if ( num_items + W < 50 ) { //print only if table is not too big
		std::cout << "   ";
		for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
		std::cout << "  items\n        ";
		for ( int n=0; n<num_items; ++n) {
			std::cout << items[n].weight << "," << items[n].value<<"   ";
		}
		std::cout << "\n   ";
		for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
		std::cout << std::endl;

		for ( int w=0; w<=W; ++w) {
			std::cout << w << "| ";
			for ( int n=0; n<=num_items; ++n) {
				std::cout << table[w][n] << "     ";
			}
			std::cout << std::endl;
		}
	}
	//end do not delete this code

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
	
	int wRem = W ;

	for (int i = num_items; i > 0; --i) {
		int wi = items[i - 1].weight ;
		int vi = items[i - 1].value ;

		if (max == table[wRem][i - 1]) {
			// not take
		} else {
			// take
			bag.push_back(i - 1) ;
			max -= vi ;
			wRem -= wi ;
		}
	}

	return bag;
}

static bool InTable(Table const &table, int w, int i)
{
	return table[w][i] > 0 ;
}

////////////////////////////////////////////////////////////
//the real recursive function
int
knapsackRecMemAux(std::vector<Item> const& items, int const& W, int index, Table & table)
{
	if (InTable(table, W, index)) return table[W][index] ;
	
	int wi, vi ;	
	
	if (index == 0 || W == 0) {
		table[W][index] = 0 ;
		return 0 ;
	}

	wi = items[index - 1].weight ;
	vi = items[index - 1].value ;

	int v1, v2 ;

	// take
	if (wi <= W) {
		// memoize
		if (InTable(table, W - wi, index - 1)) {
			v1 = table[W - wi][index - 1] + vi ;
		} else {
			v1 = knapsackRecMemAux(items, W - wi, index - 1, table) + vi ;
//			table[W - wi][index - 1] = v1 ;
		}
	} else {
		v1 = 0 ;
	}

	// not take

	// memoize
	if (InTable(table, W, index - 1)) v2 = table[W][index - 1] ;
	else {
		v2 = knapsackRecMemAux(items, W, index - 1, table) ;
//		table[W][index - 1] = v2 ;
	}

	// memoize
	table[W][index] = std::max(v1, v2) ;
	return table[W][index] ;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
