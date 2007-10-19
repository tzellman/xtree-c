#include <stdio.h>
#include "xtree/Map.h"


int main(int argc, char **argv)
{
    xtree_MapIterator iter, end;
    xtree_Pair *pair = NULL;
    
    xtree_Map *map = xtree_Map_construct(XTREE_MAP_DEFAULT_SLOTS);
    
    
    printf("%d\n", map->hashFunc(map, "string"));
    printf("%d\n", map->hashFunc(map, "strings"));
    printf("%d\n", map->hashFunc(map, "tring"));
    printf("%d\n", map->hashFunc(map, "ttring"));
    
    printf("%d\n", map->hashFunc(map, "tom"));
    printf("%d\n", map->hashFunc(map, "zellman"));
    printf("%d\n", map->hashFunc(map, "tiffany"));
    printf("%d\n", map->hashFunc(map, "zell"));
    printf("%d\n", map->hashFunc(map, "ott"));
    
    xtree_Map_set(map, "tom", "zellman");
    xtree_Map_set(map, "tiffany", "ott");
    xtree_Map_set(map, "adam", "beauchamp");
    xtree_Map_set(map, "dan", "pressel");
    
    printf("Size: %d\n", xtree_Map_getSize(map));
    
    iter = xtree_Map_begin(map);
    end = xtree_Map_end(map);
    while(!xtree_MapIterator_equals(&iter, &end))
    {
        pair = xtree_MapIterator_get(&iter);
        printf("%d) %s: %s\n", iter.index + 1,
            pair->first, (pair->second ? (char*)pair->second : "<null>"));
        xtree_MapIterator_increment(&iter);
    }
    
    /* add some more values, including overriding some existing entries */
    xtree_Map_set(map, "tiffany", "zellman");
    xtree_Map_set(map, "jeff", "zellman");
    
    printf("\nNew map contents:\n-----------------\n");
    iter = xtree_Map_begin(map);
    end = xtree_Map_end(map);
    while(!xtree_MapIterator_equals(&iter, &end))
    {
        pair = xtree_MapIterator_get(&iter);
        printf("%d) %s: %s\n", iter.index + 1,
            pair->first, (pair->second ? (char*)pair->second : "<null>"));
        xtree_MapIterator_increment(&iter);
    }
    
    
    xtree_Map_destruct(&map);
    return 0;
}
