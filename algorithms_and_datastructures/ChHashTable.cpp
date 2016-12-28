/*!
\file   ChHashTable.cpp
\author Jimi Huard
\par    email: jimi.e\@digipen.edu
\par    DigiPen login: jimi.e
\par    Course: CS280
\par    Assignment #5
\date   3/17/2014
\brief  
  This is the definition file for chHashTable
  
  Hours Spent:
  ~3

*/

#include <cmath> // std::ceil,
#include <cstring> // std::strncpy, std::strcmp

//******************************************************************************
// Public Interface
/**=================================================================================================
 *!
 *
 * \brief Constructor.
 *
 * \tparam  T Generic type parameter.
 * \param Config              The configuration.
 * \param allocator If non-null, the allocator.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
ChHashTable<T>::ChHashTable(const HTConfig& Config, ObjectAllocator* allocator):
mTable(allocator),
mConfig(Config),
mAlloc(allocator)
{
  mStats.HashFunc_ = mConfig.HashFunc_ ;
  mStats.TableSize_ = GetClosestPrime(mConfig.InitialTableSize_) ;
  mTable.Grow(mStats.TableSize_) ;
}

/**=================================================================================================
 *!
 *
 * \brief Destructor.
 *
 *
 * \tparam  T Generic type parameter.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
ChHashTable<T>::~ChHashTable() {
  // dtor
}

// Insert a key/data pair into table. Throws an exception if the
// insertion is unsuccessful.
/**=================================================================================================
 *!
 * \brief Inserts.
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param Key   The key.
 * \param Data  The data.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::insert(const char *Key, const T& Data) throw(HashTableException) {
  // insert
  
  // check load factor, grow if necessary
  double loadFactor = static_cast<double>(mStats.Count_ + 1) / static_cast<double>(mStats.TableSize_) ;
  if (loadFactor > mConfig.MaxLoadFactor_) {
    typename ChHashTable<T>::Table temp(mAlloc) ; 
    
    // allocate larger table
    double dnew_size = mConfig.GrowthFactor_ * mStats.TableSize_ ;
    unsigned new_size = static_cast<unsigned>(std::ceil(dnew_size)) ;
    new_size = GetClosestPrime(new_size) ;
    
    temp.Grow(new_size) ;
    mStats.TableSize_ = new_size ;
    
    for (unsigned i = 0; i < mTable.Size(); ++i) {
      for (
        typename ChHashTable<T>::ChHTNode 
          *node = mTable[i].Nodes, 
          *next ;
        node ;
        ) {
        // hash & insert
        unsigned hash = mConfig.HashFunc_(node->Key, mStats.TableSize_) ;
        PushFront(temp[hash], node->Key, node->Data, true) ;
        next = node->Next ;
        
        if (mConfig.FreeProc_) mConfig.FreeProc_(node->Data) ;
        
        if (mAlloc) mAlloc->Free(node) ;
        else        delete node ;
        
        node = next ;
      }
    }
    mStats.Expansions_ ++ ;
    
    mTable.Swap(temp) ;
  }
  
  // do insert
  unsigned hash = mConfig.HashFunc_(Key, mStats.TableSize_) ;
  mStats.Count_ ++ ;
  PushFront(mTable[hash], Key, Data, true) ;
}

// Delete an item by key. Throws an exception if the key doesn't exist.
/**=================================================================================================
 *!
 *
 * \brief Removes the given Key.
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param Key The key to remove.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::remove(const char *Key) throw(HashTableException) {
  // remove
  unsigned hash = mConfig.HashFunc_(Key, mStats.TableSize_) ;
  typename ChHashTable<T>::ChHTNode *node, *prev = 0 ;
  typename ChHashTable<T>::ChHTHeadNode &list = mTable[hash] ;
  node = list.Nodes ;
  for (; node; prev = node, node = node->Next) {
    mStats.Probes_ ++ ;
    if (std::strcmp(node->Key, Key) == 0) {
      mStats.Count_ -- ;
      list.Count -- ;
      // remove
      if (prev) {
        prev->Next = node->Next ;
      } else {
        list.Nodes = node->Next ;
      }
      
      if (mConfig.FreeProc_) mConfig.FreeProc_(node->Data) ;
      
      if (mAlloc) mAlloc->Free(node) ;
      else        delete node ;
      
      return ;
    }
  }
  throw HashTableException(HashTableException::E_ITEM_NOT_FOUND, "could not find item.") ;
}

/**=================================================================================================
 *!
 *
 * \brief Searches for the first match for the given constant character *.
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param Key The key.
 *
 * \return  value
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
const T& ChHashTable<T>::find(const char *Key) const throw(HashTableException) {
  // find
  unsigned hash = mConfig.HashFunc_(Key, mStats.TableSize_) ;
  for (
    typename ChHashTable<T>::ChHTNode *node = mTable[hash].Nodes ;
    node ;
    node = node->Next
    ) {
    mStats.Probes_ ++ ;
    if (std::strcmp(node->Key, Key) == 0) {
      return node->Data ;
    }
  }
  throw HashTableException(HashTableException::E_ITEM_NOT_FOUND, "could not find item.") ;
}

// Removes all items from the table (Doesn't deallocate table)
/**=================================================================================================
 *!
 *
 * \brief Clears this object to its blank/initial state.
 *
 * \tparam  T Generic type parameter.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::clear(void) {
  // clear
  for (unsigned i = 0; i < mTable.Size(); ++i)
    ClearNode(mTable[i]) ;
  mStats.Count_ = 0 ;
}

// Allow the client to peer into the data
/**=================================================================================================
 *!
 *
 * \brief Gets the stats.
 *
 * \tparam  T Generic type parameter.
 *
 * \return  The stats.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
HTStats ChHashTable<T>::GetStats(void) const {
  return mStats ;
}

/**=================================================================================================
 *!
 * \brief Gets the table.
 *
 *
 * \tparam  T Generic type parameter.
 *
 * \return  null if it fails, else the table.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
const typename ChHashTable<T>::ChHTHeadNode *ChHashTable<T>::GetTable(void) const {
  return mTable.Data() ;
}

//******************************************************************************
// Implementation
/**=================================================================================================
 *!
 * \brief Creates a new node.
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param key   The key.
 * \param data  The data.
 *
 * \return  null if it fails, else.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
typename ChHashTable<T>::ChHTNode *ChHashTable<T>::NewNode(char const *key, T const &data) {
  typename ChHashTable<T>::ChHTNode *node ;
  if (mAlloc) {
    try {
      node = new (mAlloc->Allocate()) typename ChHashTable<T>::ChHTNode(data) ;
      std::strncpy(node->Key, key, std::strlen(key) + 1) ;
    } catch (OAException const &e) {
      throw HashTableException(HashTableException::E_NO_MEMORY, e.what()) ;
    }
  } else {
    try {
      node = new typename ChHashTable<T>::ChHTNode(data) ;
      std::strncpy(node->Key, key, std::strlen(key) + 1) ;
    } catch (std::bad_alloc&) {
      throw HashTableException(HashTableException::E_NO_MEMORY, "Out of memory!") ;
    }
  }
  return node ;
}

/**=================================================================================================
 *!
 * \brief Pushes a front.
 *
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param [in,out]  node  The node.
 * \param key             The key.
 * \param data            The data.
 * \param check_dup       true to check duplicate.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::PushFront(typename ChHashTable<T>::ChHTHeadNode &node, char const *key, T const &data, bool check_dup) {
  // check duplicate
  if (check_dup)
  for (
    typename ChHashTable<T>::ChHTNode *cur = node.Nodes ;
    cur;
    cur = cur->Next
    ) {
    mStats.Probes_ ++ ;
    if (std::strcmp(cur->Key, key) == 0) {
      throw HashTableException(HashTableException::E_DUPLICATE, "duplicate inserted.") ;
    }
  }
  
  
  mStats.Probes_ ++ ;
  typename ChHashTable<T>::ChHTNode *n = NewNode(key, data) ;
  n->Next = node.Nodes ;
  node.Nodes = n ;
  ++node.Count ;
}

/**=================================================================================================
 *!
 * \brief Clears the node described by node.
 *
 * \tparam  T Generic type parameter.
 * \param [in,out]  node  The node.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::ClearNode(typename ChHashTable<T>::ChHTHeadNode &node) {
  while (node.Nodes) {
    typename ChHashTable<T>::ChHTNode *n = node.Nodes->Next ;
    
    if (mConfig.FreeProc_) mConfig.FreeProc_(n->Data) ;
    c   if (mAlloc) mAlloc->Free(node.Nodes) ;
    else        delete node.Nodes ;
    
    node.Nodes = n ;
  }
  node.Count = 0 ;
}

////////////////////////////////////////////////////////////////////////////////
// Table
/**=================================================================================================
 *!
 * \brief Constructor.
 *
 *
 * \tparam  T Generic type parameter.
 * \param [in,out]  allocator If non-null, the allocator.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
ChHashTable<T>::Table::Table(ObjectAllocator *allocator) 
: mAlloc(allocator), mTable(0), mSize(0) {
}

/**=================================================================================================
 *!
 * \brief Destructor.
 *
 * \tparam  T Generic type parameter.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
ChHashTable<T>::Table::~Table() {
  delete []mTable 
}

/**=================================================================================================
 *!
 * \brief Grows the given nsize.
 *
 * \exception HashTableException  Thrown when a Hash Table error condition occurs.
 *
 * \tparam  T Generic type parameter.
 * \param nsize The nsize.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
void ChHashTable<T>::Table::Grow(unsigned nsize) {
  delete []mTable ;
  mSize = nsize ;
  try {
    mTable = new typename ChHashTable<T>::ChHTHeadNode[nsize] ;
  } catch (std::bad_alloc&) {
    throw HashTableException(HashTableException::E_NO_MEMORY, "Out of memory!") ;
  }
}

template < typename T >
/**=================================================================================================
 *!
 * \brief Swaps the given other.
 *
 *
 * \tparam  T Generic type parameter.
 * \param [in,out]  other The other.
 *-----------------------------------------------------------------------------------------------**/
void ChHashTable<T>::Table::Swap(typename ChHashTable<T>::Table &other) {
  std::swap(mSize, other.mSize) ;
  std::swap(mTable, other.mTable) ;
  std::swap(mAlloc, other.mAlloc) ;
}

template < typename T >
/**=================================================================================================
 *!
 * \brief Gets the size.
 *
 *
 * \tparam  T Generic type parameter.
 *
 * \return  size
 *-----------------------------------------------------------------------------------------------**/
unsigned ChHashTable<T>::Table::Size() const {
  return mSize ;
}

/**=================================================================================================
 *!
 * \brief Gets the data.
 *
 *
 * \tparam  T Generic type parameter.
 *
 * \return  null if it fails, else.
 *-----------------------------------------------------------------------------------------------**/
template < typename T >
typename ChHashTable<T>::ChHTHeadNode const *ChHashTable<T>::Table::Data() const {
  return mTable ; 
}

// documented in header file as doxygen could not figure this out
template < typename T >
typename ChHashTable<T>::ChHTHeadNode const &ChHashTable<T>::Table::operator[](unsigned index) const {
  return mTable[index] ;
}

// documented in header file as doxygen could not figure this out
template < typename T >
typename ChHashTable<T>::ChHTHeadNode &ChHashTable<T>::Table::operator[](unsigned index) {
  return mTable[index] ;
}