/*!
\file   ChHashTable.h
\author Jimi Huard
\par    email: jimi.e\@digipen.edu
\par    DigiPen login: jimi.e
\par    Course: CS280
\par    Assignment #5
\date   3/17/2014
\brief  
  This is the delcaration file for chHashTable
  
  Hours Spent:
  ~3

*/

//---------------------------------------------------------------------------
#ifndef CHHASHTABLEH
#define CHHASHTABLEH
//---------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable: 4290 ) // Suppress warning: C++ Exception Specification ignored
#endif

#include <string>
#include "ObjectAllocator.h"
#include "support.h"

// client-provided hash function: takes a key and table size,
// returns an index in the table.
typedef unsigned (*HASHFUNC)(const char *, unsigned);

// Max length of our "string" keys
const unsigned MAX_KEYLEN = 10;

class HashTableException
{
  private:  
    int error_code_;
    std::string message_;

  public:
    HashTableException(int ErrCode, const std::string& Message) :
        error_code_(ErrCode), message_(Message) {};

    virtual ~HashTableException() {
    }

    virtual int code(void) const { 
      return error_code_; 
    }

    virtual const char *what(void) const {
      return message_.c_str();
    }
    enum HASHTABLE_EXCEPTION {E_ITEM_NOT_FOUND, E_DUPLICATE, E_NO_MEMORY};
};


// HashTable statistical info
struct HTStats
{
  HTStats(void) : Count_(0), TableSize_(0), Probes_(0), Expansions_(0),
                    HashFunc_(0) {};
  unsigned Count_;      // Number of elements in the table
  unsigned TableSize_;  // Size of the table (total slots)
  unsigned Probes_;     // Number of probes performed
  unsigned Expansions_; // Number of times the table grew
  HASHFUNC HashFunc_;   // Pointer to primary hash function
};

template <typename T>
/**=================================================================================================
 * \class ChHashTable
 *
 * \brief Hash table.
 *
 *-----------------------------------------------------------------------------------------------**/
class ChHashTable
{
  public:

    typedef void (*FREEPROC)(T); // client-provided free proc (we own the data)

    struct HTConfig
    {
        HTConfig(unsigned InitialTableSize, 
                 HASHFUNC HashFunc, 
                 double MaxLoadFactor = 3.0,
                 double GrowthFactor = 2.0, 
                 FREEPROC FreeProc = 0) :

        InitialTableSize_(InitialTableSize), 
        HashFunc_(HashFunc), 
        MaxLoadFactor_(MaxLoadFactor), 
        GrowthFactor_(GrowthFactor),
        FreeProc_(FreeProc) {}

      unsigned InitialTableSize_;
      HASHFUNC HashFunc_; 
      double MaxLoadFactor_;
      double GrowthFactor_;
      FREEPROC FreeProc_;
    };
      
      // Nodes that will hold the key/data pairs
    struct ChHTNode
    {
      char Key[MAX_KEYLEN]; // Key is a string
      T Data;               // Client data
      ChHTNode *Next;
      /**=================================================================================================
       *!
       *
       * \brief Constructor.
       *
       *
       * \param data  The data.
       *-----------------------------------------------------------------------------------------------**/
      ChHTNode(const T& data) : Data(data) {}; // constructor
    };

      // Each list has a special head pointer
    struct ChHTHeadNode
    {
      ChHTNode *Nodes;
      /**=================================================================================================
       *!
       *
       * \brief Default constructor.
       *
       *-----------------------------------------------------------------------------------------------**/
      ChHTHeadNode(void) : Nodes(0), Count(0) {};
      int Count; // For testing
    };

    /**=================================================================================================
     *!
     *
     * \brief Constructor.
     *
     * \param Config    The configuration.
     * \param allocator (optional) [in,out] If non-null, the allocator.
     *-----------------------------------------------------------------------------------------------**/
    ChHashTable(const HTConfig& Config, ObjectAllocator* allocator = 0);
    /**=================================================================================================
     *!
     *
     * \brief Destructor.
     *
     * \author  Jimi
     *-----------------------------------------------------------------------------------------------**/
    ~ChHashTable();

    /**=================================================================================================
     *!
     *
     * \brief Insert a key/data pair into table. Throws an exception if the insertion is unsuccessful.
     *
     *
     * \exception HashTableException  Thrown when a Hash Table error condition occurs.
     *
     * \param Key   The key.
     * \param Data  The data.
     *-----------------------------------------------------------------------------------------------**/
    void insert(const char *Key, const T& Data) throw(HashTableException);

    /**=================================================================================================
     *!
     *
     * \brief Delete an item by key. Throws an exception if the key doesn't exist. Compacts the table
     *        by moving key/data pairs, if necessary.
     *
     * \exception HashTableException  Thrown when a Hash Table error condition occurs.
     *
     * \param Key The key to remove.
     *-----------------------------------------------------------------------------------------------**/
    void remove(const char *Key) throw(HashTableException);

    /**=================================================================================================
     *!
     *
     * \brief Find and return data by key (returns 0 if not found)
     *
     *
     * \exception HashTableException  Thrown when a Hash Table error condition occurs.
     *
     * \param Key The key.
     *
     * \return  value
     *-----------------------------------------------------------------------------------------------**/
    const T& find(const char *Key) const throw(HashTableException);

    /**=================================================================================================
     *!
     *
     * \brief Removes all items from the table (Doesn't deallocate table)
     *
     *-----------------------------------------------------------------------------------------------**/
    void clear(void);

    /**=================================================================================================
     *!
     *
     * \brief Allow the client to peer into the data.
     *
     * \return  The stats.
     *-----------------------------------------------------------------------------------------------**/
    HTStats GetStats(void) const;
    /**=================================================================================================
     *!
     *
     * \brief Gets the table.
     *
     * \return  null if it fails, else the table.
     *-----------------------------------------------------------------------------------------------**/
    const ChHTHeadNode *GetTable(void) const;

  private:
    /**=================================================================================================
     * \struct  Table
     *
     * \brief is for automated table creation/growing/destruction.
     *
     *-----------------------------------------------------------------------------------------------**/
    struct Table {
      /**=================================================================================================
       *!
       *
       * \brief Constructor.
       *
       * \param allocator If non-null, the allocator.
       *-----------------------------------------------------------------------------------------------**/
      Table(ObjectAllocator *allocator) ;
      /**=================================================================================================
       *!
       *
       * \brief Destructor.
       *
       *-----------------------------------------------------------------------------------------------**/
      ~Table() ;

      /**=================================================================================================
       *!
       *
       * \brief Grows the given nsize.
       *
       *
       * \param nsize The nsize.
       *-----------------------------------------------------------------------------------------------**/
      void Grow(unsigned nsize) ;
      /**=================================================================================================
       *!
       *
       * \brief Swaps the given other.
       *
       *
       * \param [in,out]  other The other.
       *-----------------------------------------------------------------------------------------------**/
      void Swap(Table &other) ;
      /**=================================================================================================
       *!
       *
       * \brief Gets the size.
       *
       *
       * \return  size
       *-----------------------------------------------------------------------------------------------**/
      unsigned Size() const ;
      /**=================================================================================================
       *!
       *
       * \brief Gets the data.
       *
       * \return  null if it fails, else.
       *-----------------------------------------------------------------------------------------------**/
      ChHTHeadNode const *Data() const ;

      /**=================================================================================================
       *!
       *
       * \brief get.
       *
       * \param index Zero-based index of the.
       *
       * \return  The indexed value.
       *-----------------------------------------------------------------------------------------------**/
      ChHTHeadNode const &operator[](unsigned index) const ;

      /**=================================================================================================
       *!
       * \brief set.
       *
       * \param index Zero-based index of the.
       *
       * \return  The indexed value.
       *-----------------------------------------------------------------------------------------------**/
      ChHTHeadNode &operator[](unsigned index) ;

    private:      
      ObjectAllocator *mAlloc ;
      ChHTHeadNode *mTable ;
      unsigned mSize ;
    } mTable ;
    
    // aggregates
    HTConfig mConfig ;
    mutable HTStats mStats ;
    ObjectAllocator *mAlloc ;
    
    /**=================================================================================================
     *!
     * \brief methods.
     *
     * \param key   The key.
     * \param data  The data.
     *
     * \return  null if it fails, else.
     *-----------------------------------------------------------------------------------------------**/
    ChHTNode *NewNode(char const *key, T const &data) ;
    /**=================================================================================================
     *!
     * \brief Pushes a front.
     *
     *
     * \param node  The node.
     * \param key             The key.
     * \param data            The data.
     * \param check_dup       true to check duplicate.
     *-----------------------------------------------------------------------------------------------**/
    void PushFront(ChHTHeadNode &node, char const *key, T const &data, bool check_dup) ;
    /**=================================================================================================
     *!
     *
     * \brief Clears the node described by node.
     *
     *
     * \param  node  The node.
     *-----------------------------------------------------------------------------------------------**/
    void ClearNode(ChHTHeadNode &node) ;
};

#include "ChHashTable.cpp"

#endif
