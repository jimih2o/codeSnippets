/*!
\file ALGraph.cpp
\author Jimi Huard
\date 4/6/14
\brief
  Hours: ~3
  Notes:
    I liked this assignment. Again, wish I had more time so that
    I can implement the extra credit. 
    
    - Aside,
    a master diff file (all tests) would had been nice to have.
*/

#include "ALGraph.h"

#include <queue> // std::priority_queue

////////////////////////////////////////////////////////////////////////////////
// Interface

/*!
\brief
  constructor.
  size=how many nodes in the graph (mxm graph, where m=size)
\param size
  the size of the graph
*/
ALGraph::ALGraph(unsigned size) {
  mAList.reserve(size) ;
  for (unsigned i = 0; i < size; ++i)
    mAList.push_back(ALIST::value_type()) ;
}

/*!
\brief destructor.
*/
ALGraph::~ALGraph(void) {
}

/*!
\brief 
  Adds a directed edge to the graph
\param source
  where the edge comes from
\param destination
  where the edge goes to
\param weight
  weight of the edge
*/
void ALGraph::AddDEdge(unsigned source, unsigned destination, unsigned weight) {
  // update adjacency list
  // sort by weight, then by node id
  AdjacencyInfo info = {destination, weight} ;
  ALIST::value_type &list = mAList.at(source - 1) ;
  ALIST::value_type::iterator it ;
  for (it = list.begin(); it != list.end(); ++it) {
    if (info.weight < it->weight) {
      list.insert(it, info) ;
      return ;
    } else if (info.weight == it->weight) {
      if (info.id < it->id) {
        list.insert(it, info) ;
        return ;
      } // could handle id==id here, but Mead said in class that this is
        // not necessary
    }
  }

  list.push_back(info) ;
}

/*!
\brief
  usability function
\param node1
  node 1 in the edge
\param node2
  node 2 in the edge
\param weight
  the weight of hte edge
*/
void ALGraph::AddUEdge(unsigned node1, unsigned node2, unsigned weight) {
  AddDEdge(node1, node2, weight) ;
  AddDEdge(node2, node1, weight) ;
}

/*!
\brief
  extra credit
*/
std::vector<unsigned> ALGraph::SearchFrom(unsigned, TRAVERSAL_METHOD) const {
  std::vector<unsigned> ret ;
  
  return ret ;
}

/*!/////////////////////////////////////////////////////////////////////////////
///  \brief
/// Choose a node to be the source or starting point.
/// Initialize source to 0 cost and mark as evaluated.
/// Initialize all nodes to infinite cost from the source.
/// 
/// For each node, y, adjacent to source
///   1. Relax the node. That is, set y's cost to the cost of all edges from source to y.
///   2. Place y into a priority queue based on its total cost. (Lower is better)
///   3. Add source node as predecessor of y.
/// End For
/// 
/// While there are nodes in the graph that haven't been evaluated
///   Remove a node, x, from the PQ (lowest total cost)
///   Mark x as evaluated.
///   For each neighbor, y, of x
///     Relax y
///     If new cost to reach y is less
///       Update list of nodes (path) to y from source.
///       Place y in the PQ.
///     End If
///   End For
/// End While
/// \param startNode
///   The node where to start Dijkstra's algorithm
/// \returns
///   The set of paths leading from this node to all others.
//////////////////////////////////////////////////////////////////////////////*/
std::vector<DijkstraInfo> ALGraph::Dijkstra(unsigned startNode) const {
  typedef std::priority_queue<NodePtr> QueueType ;
  std::vector<DijkstraInfo> result ;
  QueueType queue ;
  std::vector<Node> nodes ;
  nodes.reserve(mAList.size()) ;
  result.reserve(mAList.size()) ;

  // SECTION 1: Build the Graph
  for (unsigned i = 1; i < mAList.size() + 1; ++i) {
    Node node ;
    if (i != startNode) {
      node.mEval = false ;
      node.mInfo.cost = static_cast<unsigned>(Infinity) ;
    } else {
      node.mEval = true ;
      node.mInfo.cost = 0 ;
      node.mInfo.path.push_back(startNode) ;
    }

    node.mNode = i ;
    nodes.push_back(node) ;
  }

  // SECTION 2: Seed the queue
  // get adjacency list
  AList const &startList = GetAdj(startNode) ;

  // for each node in list
  for (unsigned i = 0; i < startList.size(); ++i) {
    // update nodes cost to edge cost
    AdjacencyInfo const &info = startList.at(i) ;
    Node &node = nodes[(info.id - 1)] ;
    node.mInfo.cost = info.weight ;

    // place node in queue
    queue.push(&node) ;

    // node predecessor = start node
    node.mInfo.path.push_back(startNode) ;
  }

  // SECTION 3: Traverse
  // while nodes left to be evaluated
  while (!queue.empty()) {
    Node &top = *queue.top().mNode ;
    queue.pop() ;

    top.mEval = true ;
    top.mInfo.path.push_back(top.mNode) ;

    // get adj list
    AList const &list = GetAdj(top.mNode) ;

    // for each node in list
    for (unsigned i = 0; i < list.size(); ++i) {
      AdjacencyInfo const &edgeInfo = list.at(i) ;
      Node &node = nodes.at(edgeInfo.id - 1) ;

      // relax node 
      unsigned newCost = edgeInfo.weight + top.mInfo.cost ;

      // if new cost < old cost
      if (newCost < node.mInfo.cost) {
        node.mInfo.cost = newCost ;

        // update path
        node.mInfo.path = top.mInfo.path ;
        if (!top.mEval) node.mInfo.path.push_back(top.mNode) ;

        // put in queue
        queue.push(&node) ;
      }
    }
  }

  // SECTION 4: Meadify
  // for each node in nodes
  for (unsigned i = 0; i < nodes.size(); ++i) {
    // add path to result
    result.push_back(nodes.at(i).mInfo) ;
  }

  return result ;
}

/*!
\brief
  Driver function (extends driver debugability)
\returns
  the Adjacency List
*/
ALIST ALGraph::GetAList(void) const {
  return mAList ;
}

////////////////////////////////////////////////////////////////////////////////
// Implementation

/*!
\brief
  less than operator for nodes
\param other
  what to compare against
\returns
  whether or not this one is less than the other
*/
bool ALGraph::Node::operator<(Node const &other) const {
  return mInfo.cost < other.mInfo.cost ;
}

/*!
\brief
  default constructor
*/
ALGraph::NodePtr::NodePtr() : mNode(0) {
}

/*!
\brief
  assignment constructor
\param node
  the node to assign to
*/
ALGraph::NodePtr::NodePtr(Node *node) : mNode(node) {
}
  
/*!
\brief
  assignment operator
\param node
  the node to assign to
\returns
  this
*/
ALGraph::NodePtr &ALGraph::NodePtr::operator=(Node *node) {
  mNode = node ;
  return *this ;
}

/*!
\brief
  dereference operator
\returns
  node
*/
ALGraph::Node &ALGraph::NodePtr::operator*() const {
  return *mNode ;
}

/*!
\brief
  dereference operator
\returns
  node
*/
ALGraph::Node *ALGraph::NodePtr::operator->() const {
  return mNode ;
}

/*!
\brief
  compares two node pointers
\param other
  the node to compare against
\returns
  less than
*/
bool ALGraph::NodePtr::operator<(NodePtr const &other) const {
  return *mNode < *other.mNode ;
}

/*!
\brief
  Gets the adjacency list for the given node
\param id
  the node id
\returns
  The list
*/
ALGraph::AList const &ALGraph::GetAdj(unsigned id) const {
  return mAList.at(id - 1) ;
}

////////////////////////////////////////////////////////////////////////////////
// Extra
/*!
\brief
  Whether or not to test the search functions
\returns
  bool: whether or not they are implemented
*/
bool ALGraph::ImplementedSearches(void) { return false; }