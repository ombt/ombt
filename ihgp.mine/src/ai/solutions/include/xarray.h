/*
 * Copyright (C) 1995, 1996 Peter Bouthoorn.
 *
 * This software may be freely distributed and modified provided
 * this copyright message is left intact. The copyright message must be
 * included both with this (the original) software and with any modified
 * copies of this software or with any new software based on this software.
 * Furthermore any modified copies of this software must carry prominent
 * notices stating the software was changed and the date of any change.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event
 * will the copyright holder be liable for any damage arising out of
 * the use of this software.
 *
 * As a matter of courtesy, the author requests to be informed about
 * any bugs found in this software and about improvements that may be of
 * general interest.
 *
 * Peter Bouthoorn
 * peter@obelix.icce.rug.nl
 */

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdio.h>

#define NoDel 0
#define DoDel 1
#define DontManage 0
#define DoManage 1

template <class T>
class Array
{
    public:
	Array();
	Array(int);
	~Array();
	Array(const Array<T> &);
	Array<T> &operator=(const Array<T> &);
	int operator==(const Array<T> &) const;
	int operator!=(const Array<T> &) const;
	T &operator[](int);
	const T &operator[](int) const;
	void operator+=(const T &);
	int getsize() const;
	void grow(int);
	int find(const T &) const;
	void clear();
    private:
	void copy_arr(const Array<T> &);
	int size;
	T *array;
};

template <class T>
class PtrArray
{
    public:
	PtrArray(int sz = 0, int do_manage = 1);
	~PtrArray();
	PtrArray(const PtrArray<T> &);
	PtrArray<T> &operator=(const PtrArray<T> &);
	int operator==(const PtrArray<T> &) const;
	int operator!=(const PtrArray<T> &) const;
	T *&operator[](int);
	const T *operator[](int) const;
	void operator+=(T *);
	int getsize() const;
	void grow(int);
	int find(const T *) const;
	void clear(int);
    private:
        void copy_arr(const PtrArray<T> &);
	int manage;
	int size;
	T **array;
};


template <class T>
inline Array<T>::Array()
{
    array = 0;
    size = 0;
}

template <class T>
inline Array<T>::Array(int sz)
{
    size = sz;
    if (size)
        array = new T[size];
    else
	array= 0;
}

template <class T>
inline Array<T>::~Array()
{
    delete [] array;
}

template <class T>
Array<T>::Array(const Array<T> &other)
{
    copy_arr(other);
}

template <class T>
Array<T> &Array<T>::operator=(const Array<T> &other)
{
    if (this != &other)
    {
	delete [] array;
	copy_arr(other);
    }
    return(*this);
}

template <class T>
void Array<T>::copy_arr(const Array<T> &other)
{
    size = other.size;
    if (size)
    {
        array = new T[size];
        for (int i= 0; i < size; i++)
	    array[i] = other.array[i];
    }
    else
	array = 0;
}

template <class T>
int Array<T>::operator==(const Array<T> &other) const
{
    if (this == &other)
	return(1);
    if (size != other.size)
	return(0);
    for (int i = 0; i < size; i++)
	if (!(array[i] == other.array[i]))
	    return(0);
    return(1);
}

template <class T>
inline int Array<T>::operator!=(const Array<T> &other) const
{
    return(!(*this == other));
}

template <class T>
inline T &Array<T>::operator[](int i)
{
    return(array[i]);
}

template <class T>
inline const T &Array<T>::operator[](int i) const
{
    return(array[i]);
}

template <class T>
inline void Array<T>::operator+=(const T &addme)
{
    grow(1);
    array[size - 1] = addme;
}

template <class T>
inline int Array<T>::getsize() const
{
    return(size);
}

template <class T>
void Array<T>::grow(int gz)
{
    T *oldarray = array;
    int newsize = size + gz;

    array = new T[newsize];
    for (int i = 0; i < size; i++)
	array[i] = oldarray[i];
    size = newsize;
    delete [] oldarray;
}

template <class T>
int Array<T>::find(const T &findme) const
{
    for (int i = 0; i < size; i++)
	if (array[i] == findme)
	    return(i);
    return(-1);
}

template <class T>
void Array<T>::clear()
{
    delete [] array;
    size = 0;
}

template <class T>
PtrArray<T>::PtrArray(int sz, int do_manage)
{
    size = sz;
    manage = do_manage;
    if (size)
    {
	array = new T*[size];
	for (int i = 0; i < size; i++)
	    array[i] = 0;
    }
    else
	array = 0;
}

template <class T>
PtrArray<T>::~PtrArray()
{
    clear(manage);
}

template <class T>
PtrArray<T>::PtrArray(const PtrArray<T> &other)
{
    copy_arr(other);
}

template <class T>
PtrArray<T> &PtrArray<T>::operator=(const PtrArray<T> &other)
{
    if (this != &other)
    {
	clear(manage);
        copy_arr(other);
    }
    return(*this);
}

template <class T>
void PtrArray<T>::copy_arr(const PtrArray<T> &other)
{
    size = other.size;
    manage = other.manage;

    if (size)
    {
	int i;

        array = new T*[size];
	if (manage)
	    for (i = 0; i < size; i++)
	        array[i] = other.array[i] ? other.array[i]->clone() : 0;
	else
	    for (i = 0; i < size; i++)
		array[i] = other.array[i];
    }
    else
	array = 0;
}

/*
 * operator== assumes that both arrays don't contain gaps
 * (i.e no 0 pointers)
 */
template <class T>
int PtrArray<T>::operator==(const PtrArray<T> &other) const
{
    if (this == &other)
	return(1);
    if (size != other.size)
	return(0);
    for (int i = 0; i < size; i++)
	if (!(*array[i]  == *other.array[i]))
	    return(0);
    return(1);
}

template <class T>
inline int PtrArray<T>::operator!=(const PtrArray<T> &other) const
{
    return(!(*this == other));
}

template <class T>
inline T *&PtrArray<T>::operator[](int i)
{
    return(array[i]);
}

template <class T>
inline const T *PtrArray<T>::operator[](int i) const
{
    return(array[i]);
}

template <class T>
inline void PtrArray<T>::operator+=(T *addme)
{
    grow(1);
    array[size - 1] = addme;
}

template <class T>
inline int PtrArray<T>::getsize() const
{
    return(size);
}

template <class T>
void PtrArray<T>::grow(int gz)
{
    T **oldarray = array;
    int i, newsize = size + gz;

    array = new T*[newsize];
    for (i = 0; i < size; i++)
	array[i] = oldarray[i];
    for (; i < newsize; i++)
	array[i] = 0;
    size = newsize;
    delete [] oldarray;
}

template <class T>
int PtrArray<T>::find(const T *find_me) const
{
    for (int i = 0; i < size; i++)
	if (array[i])
	    if (*array[i] == *find_me)
	        return(i);
    return(-1);
}

template <class T>
void PtrArray<T>::clear(int clr)
{
    if (clr)
	for (int i = 0; i < size; i++)
	    delete array[i];
    delete [] array;
    array = 0;
    size = 0;
}

#endif
