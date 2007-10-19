#ifndef __XTREE_MAP_H__
#define __XTREE_MAP_H__

#include "xtree/System.h"
#include "xtree/List.h"
#include "xtree/Pair.h"

/* set the default number of slots to be a *reasonable* prime number */
#define XTREE_MAP_DEFAULT_SLOTS 43

/* Setup the alias: MapNode -> ListNode */
typedef xtree_ListNode              xtree_MapNode;
#define xtree_MapNode_construct     xtree_ListNode_construct
#define xtree_MapNode_destruct      xtree_ListNode_destruct

/**
 * A sparse Map class.
 * 
 * There are a few properties a Map has:
 * - sparse; it is lazily loaded
 * - in-order; the order that elements are added is kept intact.
 * - the iterator will iterate the map in the order elements were added
 */
typedef struct _xtree_Map
{
    size_t numSlots;
    xtree_List **slots;
    xtree_MapNode *first;
    xtree_MapNode *last;
    size_t size;
    unsigned int (*hashFunc) (struct _xtree_Map*, const char*);
    
}xtree_Map;

typedef unsigned int (*XTREE_MAP_FUNC)(xtree_Map*, const char*);

typedef struct _xtree_MapIterator
{
    xtree_MapNode *node;    /** the current node */
    size_t index;
}xtree_MapIterator;


/**
 * Constructs a new Map
 */
PUBFUNC(xtree_Map*) xtree_Map_construct(unsigned int numSlots);

/**
 * Destroys the map
 */
PUBFUNC(void) xtree_Map_destruct(xtree_Map **map);

/**
 * Returns the size of the map: O(1)
 */
PUBFUNC(size_t) xtree_Map_getSize(xtree_Map *map);

/**
 * Sets a value in the Map. If the value already exists, the existing entry is
 * removed and the new one is added.
 * \param map   the map
 * \param key   the lookup key
 * \param data  the data to store
 */
PUBFUNC(void) xtree_Map_set(xtree_Map *map, const char *key, void *data);

/**
 * Returns a MapIterator. If the key was not found, the iterator returned will
 * equal the iterator returned from xtree_Map_end()
 * \param map   the map
 * \param key   the key to lookup
 * \return a MapIterator
 */
PUBFUNC(xtree_MapIterator) xtree_Map_find(xtree_Map *map, const char *key);

/**
 * Returns the data stored in the map at the given entry location.
 * \param map   the map
 * \param key   the key to lookup
 * \return the user data, or NULL if not found
 */
PUBFUNC(void*) xtree_Map_remove(xtree_Map *map, const char *key);

PUBFUNC(xtree_MapIterator) xtree_Map_begin(xtree_Map *map);

PUBFUNC(xtree_MapIterator) xtree_Map_end(xtree_Map *map);

PUBFUNC(void) xtree_MapIterator_increment(xtree_MapIterator *iter);

PUBFUNC(xtree_Pair*) xtree_MapIterator_get(xtree_MapIterator *iter);

PUBFUNC(XTREE_BOOL) xtree_MapIterator_equals(xtree_MapIterator *first,
                                             xtree_MapIterator *second);

/**
 * Sets the hash function to a function of your liking.
 * This will cause a re-hash of the items in the Map.
 * \param map   the map
 * \param func  the new hashing function
 */
PUBFUNC(void) xtree_Map_setHashFunc(xtree_Map *map, XTREE_MAP_FUNC func);

/**
 * Re-sets the number of slots
 * This will cause a re-hash of the items in the Map.
 * \param map       the map
 * \param numSlots  the new number of slots
 */
PUBFUNC(void) xtree_Map_setNumSlots(xtree_Map *map, unsigned int numSlots);



#endif
