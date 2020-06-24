#ifndef __MAP_H
#define __MAP_H
// map class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#include "hash.h"

// forward declarations
class MapDataItem;
class Map;

// domain and range types
typedef DataItem MapDomain;
typedef DataItem MapRange;

// definitions
#define DEFAULTMAPSIZE 1021

// data class definition
class MapDataItem: public DataItem {
public:
        // friend class and functions
        friend class Map;

        // constructors and destructor
        MapDataItem(MapDomain *, MapRange *);
        MapDataItem(const MapDataItem &);
        ~MapDataItem();

        // assignment and equivalence
        DataItem &operator=(const DataItem &);
        int operator==(const DataItem &m) const;
        int operator!=(const DataItem &m) const;
        int operator<(const DataItem &m) const;
        int operator>(const DataItem &m) const;
        int operator<=(const DataItem &m) const;
        int operator>=(const DataItem &m) const;

        // printing
        void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const MapDataItem &);

        // hashing function
        friend int hasher(MapDomain *, int);

private:
        // not allowed
        MapDataItem();

protected:
        // internal data
        MapDomain *domain;
        MapRange *range;
};

// map class definition
class Map {
public:
        // constructors and destructor
        Map(int, HashFn);
        Map(const Map &);
        ~Map();

        // assignment
        Map &operator=(const Map &);

        // map operations
        friend int makeNull(Map &);
        friend int assign(Map &, MapDomain *, MapRange *);
        friend int compute(Map &, MapDomain *, MapRange *&);

private:
        // not allowed
        Map();

protected:
        // internal data
        HashTable map;
};

#endif
