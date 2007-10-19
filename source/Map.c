#include "xtree/Map.h"


/**
 * Knuth's "The Art of Computer Programming", volume 3 "Sorting and Searching",
 * chapter 6.4.
 */
PRIVFUNC(unsigned int) _xtree_Map_hashKnuth(xtree_Map *map, const char *key)
{
    char *c = (char*)&key[0];
    int hash = 0, len = strlen(key);
    for (hash=len; len--;)
        hash = ((hash << 5) ^ (hash >> 27)) ^ *c++;
    return hash % map->numSlots;
}


PUBFUNC(xtree_Map*) xtree_Map_construct(unsigned int numSlots)
{
    size_t mapSize;
    xtree_Map* map = (xtree_Map*)malloc(sizeof(xtree_Map));
    /* TODO - check memory */
    
    map->numSlots = numSlots <= 0 ? XTREE_MAP_DEFAULT_SLOTS : numSlots;
    mapSize = sizeof(xtree_List*) * map->numSlots;
    map->slots = (xtree_List**)malloc(mapSize);
    
    /* we will lazy fill the slots, so don't construct lists now */
    memset(map->slots, 0, mapSize);
    
    map->first = map->last = NULL;
    map->size = 0;
    
    map->hashFunc = &_xtree_Map_hashKnuth;
    return map;
}

PUBFUNC(void) xtree_Map_destruct(xtree_Map **map)
{
    int i;
    if (*map)
    {
        for(i = 0; (*map)->slots && i < (*map)->numSlots; ++i)
        {
            if ((*map)->slots[i])
            {
                xtree_Pair *pair = NULL;
                xtree_MapNode *node = NULL;
                xtree_ListIterator iter, end;
                iter = xtree_List_begin((*map)->slots[i]);
                end = xtree_List_end((*map)->slots[i]);
                while(!xtree_ListIterator_equals(&iter, &end))
                {
                    node = (xtree_MapNode*)xtree_List_remove((*map)->slots[i], &iter);
                    pair = node ? (xtree_Pair*)node->data : NULL;
                    if (node) xtree_MapNode_destruct(&node);
                    if (pair) xtree_Pair_destruct(&pair);
                    /* TODO - the data won't get free'd
                     * if we add an ownership policy, this will change */
                }
                xtree_List_destruct(&(*map)->slots[i]);
            }
        }
        if ((*map)->slots) free((*map)->slots);
        free(*map);
    }
    *map = NULL;
}

PUBFUNC(size_t) xtree_Map_getSize(xtree_Map *map)
{
    return map ? map->size : 0;
}


PUBFUNC(void) xtree_Map_set(xtree_Map *map, const char *key, void *data)
{
    xtree_Pair *pair = NULL;
    xtree_MapNode *node = NULL;
    void *removedData = NULL;
    unsigned int slot = map->hashFunc(map, key);
    
    /* since we lazy initialize, check to see if we need a new list */
    if (!map->slots[slot])
    {
        map->slots[slot] = xtree_List_construct();
        /* TODO - check for mem err */ 
    }
    
    /* first, we need to remove the object, in case it already exists */
    removedData = xtree_Map_remove(map, key);
    if (removedData)
    {
        /* TODO - what should we do with the removed data?
         * for now it's OK, becuase we assume the user owns the data
         * However, if we add an ownership policy, this might change!
         */
    }
    
    pair = xtree_Pair_construct(key, data);
    node = xtree_MapNode_construct(map->last, NULL, pair);
    if (!map->first)
        map->first = node;
    if (!map->last)
        map->last = node;
    else
    {
        map->last->next = node;
        node->prev = map->last;
        map->last = node;
    }
    
    /* add the node to the right slot */
    xtree_List_pushBack(map->slots[slot], node);
    map->size++;
}

PUBFUNC(xtree_MapIterator) xtree_Map_find(xtree_Map *map, const char *key)
{
    xtree_Pair *pair = NULL;
    xtree_MapNode *node = NULL;
    xtree_MapIterator mapIter;
    xtree_ListIterator iter, end;
    unsigned int slot = map->hashFunc(map, key);
    
    if (!map->slots[slot]) goto EXCEPTION;
    
    iter = xtree_List_begin(map->slots[slot]);
    end = xtree_List_end(map->slots[slot]);
    while(!xtree_ListIterator_equals(&iter, &end))
    {
        node = (xtree_MapNode*)xtree_ListIterator_get(&iter);
        pair = node ? (xtree_Pair*)node->data : NULL;
        if (pair && strcmp(pair->first, key) == 0)
        {
            mapIter.node = node;
            return mapIter;
        }
        xtree_ListIterator_increment(&iter);
    }
  EXCEPTION:
    return xtree_Map_end(map);
}


PUBFUNC(void*) xtree_Map_remove(xtree_Map *map, const char *key)
{
    xtree_Pair *pair = NULL;
    xtree_MapNode *node = NULL;
    void *data = NULL;
    xtree_ListIterator iter, end;
    unsigned int slot = map->hashFunc(map, key);
    
    if (!map->slots[slot]) goto EXCEPTION;
    
    iter = xtree_List_begin(map->slots[slot]);
    end = xtree_List_end(map->slots[slot]);
    while(!xtree_ListIterator_equals(&iter, &end))
    {
        node = (xtree_MapNode*)xtree_ListIterator_get(&iter);
        pair = node ? (xtree_Pair*)node->data : NULL;
        if (pair && strcmp(pair->first, key) == 0)
        {
            if (node->prev)
                node->prev->next = node->next;
            if (node->next)
                node->next->prev = node->prev;
            if (node == map->first)
                map->first = node->next;
            if (node == map->last)
                map->last = node->prev;
            
            data = pair->second;
            xtree_MapNode_destruct(&node);
            xtree_Pair_destruct(&pair);
            xtree_List_remove(map->slots[slot], &iter);
            map->size--;
            break;
        }
        xtree_ListIterator_increment(&iter);
    }
    return data;

  EXCEPTION:
    return NULL;
}


PUBFUNC(xtree_MapIterator) xtree_Map_begin(xtree_Map *map)
{
    xtree_MapIterator iter;
    assert(map);
    iter.node = map ? map->first : NULL;
    iter.index = 0;
    return iter;
}

PUBFUNC(xtree_MapIterator) xtree_Map_end(xtree_Map *map)
{
    xtree_MapIterator iter;
    assert(map);
    iter.node = NULL;
    iter.index = 0;
    return iter;
}

PUBFUNC(void) xtree_MapIterator_increment(xtree_MapIterator *iter)
{
    if (iter)
    {
        iter->node = iter->node ? iter->node->next : NULL;
        iter->index = iter->node ? iter->index + 1 : 0;
    }
}

PUBFUNC(xtree_Pair*) xtree_MapIterator_get(xtree_MapIterator *iter)
{
    return iter && iter->node && iter->node->data ?
        (xtree_Pair*)iter->node->data : NULL;
}

PUBFUNC(XTREE_BOOL) xtree_MapIterator_equals(xtree_MapIterator *first,
                                             xtree_MapIterator *second)
{
    return first == second || (first && second && first->node == second->node);
}

PRIVFUNC(void) _xtree_Map_remap(xtree_Map *map)
{
    xtree_Pair *pair = NULL;
    xtree_List **tempSlots = NULL;
    xtree_Map *newMap = xtree_Map_construct(map->numSlots);
    newMap->hashFunc = map->hashFunc;
    
    
    /* first, go through and re-map everything -- in order! */
    xtree_MapIterator mapIter, mapEnd;
    mapIter = xtree_Map_begin(map);
    mapEnd = xtree_Map_end(map);
    while(!xtree_MapIterator_equals(&mapIter, &mapEnd))
    {
        pair = xtree_MapIterator_get(&mapIter);
        if (pair)
            xtree_Map_set(newMap, pair->first, pair->second);
        /* might be able to remove the item here, to cut down on memory
         * usage... however, it will cause more Big-O complexity to the algorithm */
        xtree_MapIterator_increment(&mapIter);
    }
    
    tempSlots = newMap->slots;
    newMap->slots = map->slots;
    map->slots = tempSlots;
    
    /* copy over the stuff we want */
    map->size = newMap->size;
    map->first = newMap->first;
    map->last = newMap->last;
    
    /* NULL out some of the newMap values, and destroy */
    newMap->first = newMap->last = NULL;
    newMap->slots = NULL;
    newMap->size = 0;
    xtree_Map_destruct(&newMap);
}

PUBFUNC(void) xtree_Map_setHashFunc(xtree_Map *map, XTREE_MAP_FUNC func)
{
    map->hashFunc = func;
    _xtree_Map_remap(map);
}

PUBFUNC(void) xtree_Map_setNumSlots(xtree_Map *map, unsigned int numSlots)
{
    map->numSlots = numSlots <= 0 ? XTREE_MAP_DEFAULT_SLOTS : numSlots;
    _xtree_Map_remap(map);
}
