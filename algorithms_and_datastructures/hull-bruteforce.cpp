/*!
\file	hull-bruteforce.cpp
\author	Jimi Huard (jimi.e@digipen.edu)
\brief
	Contains the definitions for hullBruteForce
	and hullBruteForce2.

DigiPen Institute of Technology, all rights reserved. */
#include "hull-bruteforce.h"
#include <algorithm> // std::make_pair
#include <set>       // std::set
#include <iostream>  // iostreams (cout)

/// 
/// Check 1:(x,y) == 2:(x,y)
/// 
bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

/// 
/// Stream output operator (insertion)
/// 
std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

///
/// Stream input operator (extraction)
///
std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y)
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		)
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

///
/// returns a set of indices of points that form convex hull
/// Uses a simple O(n^3) brute force algorithm.
/// 
std::set<int> hullBruteForce ( std::vector< Point > const& points )
{
	unsigned numPoints = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( numPoints < 3 ) throw "bad number of points";

	std::set<int> hullIndices;

	// for each pair of points (a, b)
	//	if all other points are on one side of the line formed between (a, b)
	//		add indices (i, j) to hull
	for (unsigned i = 0; i < numPoints; ++i) {
		for (unsigned j = i + 1; j < numPoints; ++j) {
			std::pair<bool, bool> side ;
			bool firstTest = true ;
			unsigned k ;
			for (k = 0; k < numPoints; ++k) {
				if (k != j && k != i) {
					// test point
					if (firstTest) {
						side = get_location(
								points[i].x,
								points[i].y,
								points[j].x,
								points[j].y,
								points[k].x,
								points[k].y) ;
						firstTest = false ;
					} else {
						if (side == get_location(
								points[i].x,
								points[i].y,
								points[j].x,
								points[j].y,
								points[k].x,
								points[k].y)) continue ;
						else                          break ;
					}
				}
			}

			if (k == numPoints) {
				hullIndices.insert(i) ;
				hullIndices.insert(j) ;
			}
		}
	}

	return hullIndices ;
}

/// 
/// returns a counter-clockwise vector of indices into 
/// the std::vector<Point> points structure, forming a 
/// convex hull. It does this by tracing the outside of the
/// convex hull as it considers the edges, rather than 
/// blindly checking every combination. 
/// 
std::vector<int> hullBruteForce2 ( std::vector< Point > const& points )
{
	int numPoints = points.size() ;
	if ( numPoints < 3 ) throw "bad number of points";
	
	std::set<int> remaining ;
	std::vector<int> hullIndices ;
	
	for (int i = 0; i < numPoints; ++i)
		remaining.insert(i) ;

	// find first point in hull (min x coord)
	class {
	public:
		bool operator()(Point const &a, Point const &b) {
			return a.x < b.x ;
		}
	} lessThan ;
	
	// std::vector<Point>::const_iterator firstElementIt = std::min_element(points.begin(), points.end(), PointMinXPred()) ;
	std::vector<Point>::const_iterator firstElementIt = points.begin() ;
	for (std::vector<Point>::const_iterator e = points.begin(); e != points.end(); ++e) 
		if (lessThan(*e, *firstElementIt))
			firstElementIt = e ;

	int currentElement = firstElementIt - points.begin() ; // points.begin() - firstElementIt ;
	hullIndices.push_back(currentElement) ;
	remaining.erase(currentElement) ;

	// find next vertex of hull in counter-clockwise order
		// consider all lines through the previous vertex requiring that there are no points to the right of it
	for (std::set<int>::iterator i = remaining.begin(); i != remaining.end();) {
		class AllPointsRight {
			bool mGoodSoFar ;
			Point const & mCurrentElement ;
			Point const & mNextElement ;
		public:
			AllPointsRight(Point const & c, Point const & n): 
				mGoodSoFar(true),
				mCurrentElement(c),
				mNextElement(n) {}

			void operator()(Point const &test) {
				if (test == mCurrentElement || test == mNextElement) return ;
				std::pair<bool,bool> res = get_location(
					mCurrentElement.x,
					mCurrentElement.y,
					mNextElement.x,
					mNextElement.y,
					test.x,
					test.y) ;

				if (res.second && !res.first)
					mGoodSoFar = false ;
			}

			bool Good() const { return mGoodSoFar; }
		} /* AllPointsRight */ pred(points[currentElement], points[*i]) ;
		
		for (int j = 0; j < numPoints && pred.Good(); ++j) {
			pred(points[j]) ;
		}

		if (pred.Good()) {
			// found et
			currentElement = *i ;
			remaining.erase(i) ;
			i = remaining.begin() ;
			hullIndices.push_back(currentElement) ;
		} else ++i ;
	}

	return hullIndices ;
}
