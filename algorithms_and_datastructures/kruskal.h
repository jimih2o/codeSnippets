#ifndef KRUSKAL_H
#define KRUSKAL_H
/*
author Jimi Huard
assign kruskal, disjoint subsets
file kruskal.h
about includes templatized kruskal algorithm definitions,
(1) inefficient using color vector, (2) efficient using
disjoint subsets class
*/
#include "disjoint_sets.h"
#include "graph.h"
// #include <list>
#include <deque>
#include <algorithm>

///
// efficient version, using disjoint subsets
/// 
template <typename Vertex, typename Edge>
std::vector<Edge> kruskal_disjoint_subsets(Graph<Vertex,Edge> const& g)
{
	unsigned const lengthVMinus1 = g.Size() - 1 ;
	
	std::vector < Edge > minSpanTree ;
	minSpanTree.reserve(lengthVMinus1) ; 
	
	// initialize array of "colors"
	DisjointSets djs(g.Size()) ;
	for (unsigned i = 0; i < g.Size(); ++i) djs.Make() ;

	// create sorted edge structure
	std::deque < Edge > edges(g.GetEdges().begin(), g.GetEdges().end()) ;
	//edges.sort() ;
	std::sort(edges.begin(), edges.end()) ;
	
	// while (edge structure is not empty and minSpanTree.size() < |V| - 1)
	while (minSpanTree.size() < lengthVMinus1 && !edges.empty()) {
		// e = edges.pop()
		Edge e = edges.front() ;
		edges.pop_front() ;

		// if (minSpanTree U e has no cycles)
		if (djs.GetRepresentative(e.ID1()) != djs.GetRepresentative(e.ID2())) {
			// update djs
			djs.Join(e.ID1(), e.ID2()) ;
			
			// add e to minSpanTree
			minSpanTree.push_back(e) ;
		}
	}
	
	return minSpanTree ;
}

/// 
// inefficient version, using array or vector of "colors"
///
template <typename Vertex, typename Edge>
std::vector<Edge> kruskal_vector(Graph<Vertex,Edge> const& g)
{
	unsigned const lengthVMinus1 = g.Size() - 1 ;
	unsigned const edgeCount = g.GetEdges().size() ;
	
	typedef std::vector < int > ColorSet ;
	
	struct CycleDetect {
		bool hasCycle ;
		CycleDetect(ColorSet const &colors, int v1, int v2) :
			hasCycle(colors[v1] == colors[v2]) {}
		
		operator bool() {
			return hasCycle ;
		}
	};
	
	std::vector < Edge > minSpanTree ;
	minSpanTree.reserve(lengthVMinus1) ; 
	
	// initialize array of "colors"
	ColorSet colors(edgeCount) ;
	for (unsigned i = 0; i < colors.size(); ++i) colors[i] = static_cast<int>(i) ;
	
	// create sorted edge structure
	std::deque < Edge > edges(g.GetEdges().begin(), g.GetEdges().end()) ;
	//edges.sort() ;
	std::sort(edges.begin(), edges.end()) ;
	
	// while (edge structure is not empty and minSpanTree.size() < |V| - 1)
	while (minSpanTree.size() < lengthVMinus1 && !edges.empty()) {
		// e = edges.pop()
		Edge e = edges.front() ;
		edges.pop_front() ;

		// if (minSpanTree U e has no cycles)
		if (CycleDetect(colors, e.ID1(), e.ID2()) == false) {
			// update colors
			// choose to change color 2 to color 1
			int c1 = colors[e.ID1()] ;
			int c2 = colors[e.ID2()] ;
			
			for (unsigned i = 0; i < colors.size(); ++i) {
				if (colors[i] == c2) colors[i] = c1 ;
			}
			
			// add e to minSpanTree
			minSpanTree.push_back(e) ;
		}
	}
	
	return minSpanTree ;
}

#endif
