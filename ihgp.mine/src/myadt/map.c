// functions for map class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <assert.h>

// local headers
#include "map.h"

// map item constructors and destructor
MapDataItem::MapDataItem(): 
	DataItem(), domain(0), range(0)
{
	// should not use this one
	assert(domain != (MapDomain *)0 && range != (MapRange *)0);
}

MapDataItem::MapDataItem(MapDomain *d, MapRange *r):
	DataItem(), domain(d), range(r)
{
	assert(domain != (MapDomain *)0 && range != (MapRange *)0);
}

MapDataItem::MapDataItem(const MapDataItem &m):
	DataItem(m), domain(m.domain), range(m.range)
{
	assert(domain != (MapDomain *)0 && range != (MapRange *)0);
}

MapDataItem::~MapDataItem()
{
	domain = (MapDomain *)0;
	range = (MapRange *)0;
}

// assignment and comparison
DataItem &
MapDataItem::operator=(const DataItem &m)
{
	// check for self-assignment
	if (this != &m) return(*this);

	// copy base data 
	DataItem::operator=(m);

	// copy data
	MapDataItem *d = (MapDataItem *)&m;
	domain = d->domain;
	range = d->range;

	// all done
	return(*this);
}

int
MapDataItem::operator==(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *) &m;
	return(*range == *d->range);
}

int
MapDataItem::operator!=(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *)&m;
	return(*range != *d->range);
}

int
MapDataItem::operator<(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *)&m;
	return(*range < *d->range);
}

int
MapDataItem::operator>(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *)&m;
	return(*range > *d->range);
}

int
MapDataItem::operator<=(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *)&m;
	return(*range <= *d->range);
}

int
MapDataItem::operator>=(const DataItem &m) const
{
	const MapDataItem *d = (const MapDataItem *)&m;
	return(*range >= *d->range);
}

// printing
void
MapDataItem::dump(ostream &os) const
{
	os << "(domain, range) = (";
	os << *domain << "," << *range;
	os << ")";
	return;
}

ostream &
operator<<(ostream &os, const MapDataItem &m)
{
	m.dump(os);
	return(os);
}

// map class constructors and destructor
Map::Map(int mapsz, HashFn hfn): 
	map(mapsz, hfn)
{
	// do nothing
}

Map::Map(const Map &m): 
	map(m.map)
{
	// do nothing
}

Map::~Map()
{
	// do nothing
}

// assignment
Map &
Map::operator=(const Map &m)
{
	if (this != &m) 
		map = m.map;
	return(*this);
}

// map operations
int
makeNull(Map &m)
{
	return(makeNull(m.map));
}

int
assign(Map &m, MapDomain *d, MapRange *r)
{
	MapDataItem *mdi = new MapDataItem(d, r);
	assert(mdi != (MapDataItem *)0);
	return(insert(m.map, mdi));
}

int
compute(Map &m, MapDomain *d, MapRange *&r)
{
	MapDataItem mdi(d, r);
	DataItem *pmdi = &mdi;
	if (retrieve(m.map, pmdi) != OK)
		return(NOTOK);
	else
	{
		remove(m.map, pmdi);
		assign(m, d, r);
		return(OK);
	}
}
