/*!
\file ALGraph.h
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
//---------------------------------------------------------------------------
#ifndef ALGRAPH_H
#define ALGRAPH_H
//---------------------------------------------------------------------------
#include <vector>

struct DijkstraInfo
{
  unsigned cost;
  std::vector<unsigned> path;
};

struct AdjacencyInfo
{
  unsigned id;
  unsigned weight;
};

typedef std::vector<std::vector<AdjacencyInfo> > ALIST;

/*!
\brief
  It's an Adjacency list that you can turn into a graph such that
  Dijkstras Algorithm works on it.
*/
class ALGraph
{
  public:
    enum TRAVERSAL_METHOD {DEPTH_FIRST, BREADTH_FIRST};

    ALGraph(unsigned size);
    ~ALGraph(void);
    void AddDEdge(unsigned source, unsigned destination, unsigned weight);
    void AddUEdge(unsigned node1, unsigned node2, unsigned weight);

    std::vector<unsigned> SearchFrom(unsigned start_node, TRAVERSAL_METHOD method) const;
    std::vector<DijkstraInfo> Dijkstra(unsigned start_node) const;
    ALIST GetAList(void) const;
    static bool ImplementedSearches(void);
        
  private:
    ALIST mAList ;
    typedef std::vector<AdjacencyInfo> AList ;

    // unfortunately this didn't do what I hoped it would. (Compilers decided it should be a signed value)
    enum {Infinity = (unsigned)(-1)};

    /*!
    \brief
      represents a node or vertex in the graph
    */
    struct Node {
      unsigned mNode ;
      bool mEval ;
      DijkstraInfo mInfo ;

      bool operator<(Node const &other) const ;
    } ;
    
    /*!
    \brief
      represents a pointer to a node or vertex in the graph
    */
    struct NodePtr {
      Node *mNode ;
      NodePtr() ;
      NodePtr(Node *node) ;
      
      NodePtr &operator=(Node *node) ;
      Node &operator*() const ;
      Node *operator->() const ;

      bool operator<(NodePtr const &other) const ;
    };

    AList const &GetAdj(unsigned id) const ;
};

#endif
