#ifndef __HUFFMAN_H
#define __HUFFMAN_H
// huffman encode or decode a file

// headers
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "fileio.h"
#include "array.h"

// local definitions
#define BitsInByte 8
#define NumberOfCodes 256
#define SizeOfBitBuffer BUFSIZ

// forward declarations
class HuffmanTableEntry;
class HuffmanTable;
class HuffmanNode;
class HuffmanPQ;
class Huffman;

// internal data structures
class HuffmanTableEntry {
private:
	// friends
	friend class HuffmanTable;
	friend class HuffmanNode;
	friend class Huffman;

	// constructor
	HuffmanTableEntry():
		inuse(0), data(0), nbits(0), code(0) {
		// do nothing
	}

	// sanity check
	int inUse() const {
		return(inuse);
	}

	// internal data
	unsigned char inuse;
	unsigned char data;
	unsigned char nbits;
	unsigned short code;
};

class HuffmanTable {
private:
	// friends
	friend class HuffmanNode;
	friend class Huffman;

	// constructors
	HuffmanTable():
		nentries(0) {
		// do nothing
	}
	~HuffmanTable() {
		// do nothing
	}

	// descend tree and fill in table
	void fillInTable(const HuffmanNode *);
	void fillInTable(const HuffmanNode *, 
		unsigned char &, unsigned short &);

	// internal data
	unsigned short nentries;
	HuffmanTableEntry data[NumberOfCodes];
};

// huffman tree node class
class HuffmanNode {
public:
	// constructors and destructor
	HuffmanNode(unsigned char d, unsigned long c):
		data(d), counts(c), code(-1),
		left(NULL), right(NULL), parent(NULL) {
		// do nothing
	}
	~HuffmanNode() {
		if (left != NULL)
			delete left;
		left = NULL;
		if (right != NULL)
			delete left;
		right = NULL;
	}

	// access data fields
	HuffmanNode *getLeft() const {
		return(left);
	}
	HuffmanNode *getRight() const {
		return(right);
	}
	HuffmanNode *getParent() const {
		return(parent);
	}
	unsigned long getCounts() const {
		return(counts);
	}
	char getCode() const {
		return(code);
	}
	unsigned char getData() const {
		return(data);
	}
	void setLeft(HuffmanNode *l) {
		left = l;
	}
	void setRight(HuffmanNode *r) {
		right = r;
	}
	void setParent(HuffmanNode *p) {
		parent = p;
	}
	void setCounts(unsigned long c) {
		counts = c;
	}
	void setCode(char c) {
		code = c;
	}

	// some basic checks
	int isALeaf() const {
		return(left == NULL && right == NULL);
	}
	int isNotALeaf() const {
		return(left != NULL && right != NULL);
	}
	int isValid() const {
		return(isALeaf() || isNotALeaf());
	}

	// comparison operators
	int operator==(const HuffmanNode &hn) const {
		return(counts == hn.counts);
	}
	int operator!=(const HuffmanNode &hn) const {
		return(counts != hn.counts);
	}
	int operator<=(const HuffmanNode &hn) const {
		return(counts <= hn.counts);
	}
	int operator>=(const HuffmanNode &hn) const {
		return(counts >= hn.counts);
	}
	int operator<(const HuffmanNode &hn) const {
		return(counts < hn.counts);
	}
	int operator>(const HuffmanNode &hn) const {
		return(counts > hn.counts);
	}

	// utility functions
	int numberOfBits(int bytes) const {
		return(BitsInByte*bytes);
	}

protected:
	// internal data
	char code;
	unsigned char data;
	unsigned long counts;
	HuffmanNode *left;
	HuffmanNode *right;
	HuffmanNode *parent;
};

// huffman priority queue class
class HuffmanPQ {
public:
        // constructors and destructor
        HuffmanPQ(int sz):
		size(sz), last(0), array(1, sz) {
        	// do nothing
	}
	HuffmanPQ(const HuffmanPQ &pq): 
		size(pq.size), last(pq.last), 
		array(pq.array) {
        	// do nothing
	}
	~HuffmanPQ() {
        	// do nothing
	}

        // assignment
	HuffmanPQ &operator=(const HuffmanPQ &pq) {
        	if (this != &pq)
		{
                	size = pq.size;
                	last = pq.last;
                	array = pq.array;
		}
        	return(*this);
	}

        // priority queue operations
        void clear() {
		last = 0;
		return;
	}
	int enqueue(HuffmanNode *data)
	{
		// check if array is large enough
		if (last >= size)
		{
			size = 2*size;
			array.setBounds(1, size);
		}

		// insert new member in priority queue
		array[++last] = data;
		for (int i = last; 
		     i > 1 && (*array[i] < *array[parentOf(i)]); 
		     i = parentOf(i))
		{
			HuffmanNode *tmp = array[i];
			array[i] = array[parentOf(i)];
			array[parentOf(i)] = tmp;
		}
		return(OK);
	}
	int dequeue(HuffmanNode *&data)
	{
		// is queue empty
		if (isEmpty())
			return(NOTOK);

		// get highest priority item from queue
		data = array[1];

		// delete item from queue
		array[1] = array[last--];

		// float to correct position
		for (int parent = 1; parent <= parentOf(last); )
		{
			// get left and right children
			int min = 0;
			int left = leftChildOf(parent);
			int right = rightChildOf(parent);

			// float to correct position
			if ((*array[left] < *array[right]) || (left == last))
				min = left;
			else
				min = right;
			if (*array[parent] > *array[min])
			{
				HuffmanNode *tmp = array[parent];
				array[parent] = array[min];
				array[min] = tmp;
				parent = min;
			}
			else
				break;
		}
		return(OK);
	}
	int front(HuffmanNode *&data) const
	{
		// is queue empty
		if (isEmpty())
			return(NOTOK);

		// return first data item
		data = array[1];
		return(OK);
	}
	int isEmpty() const
	{
		return(last <= 0);
	}

protected:
	// internal use only
	inline int parentOf(int node) const {
		return(node/2);
	}
	inline int leftChildOf(int node) const {
		return(2*node);
	}
	inline int rightChildOf(int node) const {
		return(2*node+1);
	}

protected:
        // data
	int size, last;
        Array<HuffmanNode * > array;
};
 
// huffman class
class Huffman {
public:
	// constructors and destructor
	Huffman(const char *ifn, const char *ofn):
		infile(NULL), outfile(NULL), infd(), outfd(),
		phroot(NULL)
	{
		// sanity checks
		MustBeTrue(ifn != NULL && *ifn != '\0');
		MustBeTrue(ofn != NULL && *ofn != '\0');

		// save file names
		infile = new char [strlen(ifn)+1];
		MustBeTrue(infile != NULL);
		strcpy(infile, ifn);
		outfile = new char [strlen(ofn)+1];
		MustBeTrue(outfile != NULL);
		strcpy(outfile, ofn);

		// clear bit buffer data
		nextbit = 0;
		maxbit = BitsInByte*SizeOfBitBuffer*sizeof(unsigned long);
		memset(bitbuf, 0, SizeOfBitBuffer*sizeof(unsigned long));
	}
	~Huffman() {
		delete [] infile;
		infile = NULL;
		delete [] outfile;
		outfile = NULL;
		if (phroot != NULL)
			delete phroot;
		phroot = NULL;
		nextbit = 0;
	}

	// encoding or decoding of files
	void encode() {
		// open input file for read 
		infd.open(infile, O_RDONLY);

		// we have to read in the input file and 
		// build a huffman tree for it.
		buildTree();

		// convert tree to a table for easier access
		convertToTable();

		// open output file now that we are ready
		outfd.open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0664);

		// now write out header
		writeHeader();

		// encode and write out the fil
		encodeFile();
	}
	void decode() {
		// open input file for read 
		infd.open(infile, O_RDONLY);

		// read in huffman codes from input file.
		readHeader();

		// open output file, we are ready to use it
		outfd.open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0664);

		// decode file and write it out.
		decodeFile();
	}

protected:
	// build a huffman tree for the input file
	void buildTree() {
		// read in file and build frequency distribution
		frequencyDist();

		// start huffman algorithm, insert data
		// into a priority queue.
		//
		int numberInserted = 0;
		HuffmanPQ hpq(2*NumberOfCodes);
		for (int b=0; b<NumberOfCodes; b++)
		{
			// skip codes that were not found
			if (frequency[b][1] == 0)
				continue;

			// create a node for codes that were found
			HuffmanNode *phn = 
				new HuffmanNode((unsigned char)b, 
						frequency[b][1]);
			MustBeTrue(phn != NULL);

			// sort by frequency, insert into a priority queue
			MustBeTrue(hpq.enqueue(phn) == OK);
			numberInserted++;
		}

		// start huffman algorithm to generate new codes
		for (int i=1; i<=(numberInserted-1); i++)
		{
			// get two least frequent nodes
			HuffmanNode *pleft = NULL;
			MustBeTrue(hpq.dequeue(pleft) == OK);
			HuffmanNode *pright = NULL;
			MustBeTrue(hpq.dequeue(pright) == OK);
	
			// allocate new node to combine these
			HuffmanNode *pnew = 
				new HuffmanNode((unsigned char)-1, 
				pleft->getCounts()+pright->getCounts());
			MustBeTrue(pnew != NULL);

			// set fields
			pnew->setLeft(pleft);
			pnew->setRight(pright);
			pleft->setParent(pnew);
			pright->setParent(pnew);
			pleft->setCode('0');
			pright->setCode('1');

			// insert into the priority queue
			MustBeTrue(hpq.enqueue(pnew) == OK);
		}

		// get root node of huffman encoding tree
		MustBeTrue(hpq.dequeue(phroot) == OK);
		MustBeTrue(hpq.isEmpty());

		// root node has NO parent
		phroot->setParent(NULL);
		return;
	}

	// convert huffman tree to table for easier access
	void convertToTable() {
		// sanity checks
		MustBeTrue(phroot != NULL);

		// create table
		table.fillInTable(phroot);
	}

	// write in huffman table to output file
	void writeHeader() {
		// rewind file
		outfd.rewind();

		// write the number of entries
		MustBeTrue(outfd.write(&table.nentries, 
			sizeof(unsigned short)) == sizeof(unsigned short));

		// write out entries in use
		for (int i=0; i<NumberOfCodes; i++)
		{
			// skip entries not in use
			if (!table.data[i].inuse)
				continue;

			// write out the data
			MustBeTrue(outfd.write(&table.data[i].data, 1) == 1);
			MustBeTrue(outfd.write(&table.data[i].nbits, 1) == 1);
			MustBeTrue(outfd.write(&table.data[i].code, 
				sizeof(unsigned short)) == 
				sizeof(unsigned short));
		}
	}

	// encode input file using huffman table
	void writeToFile(unsigned char nbits, unsigned short code)
	{
		// sanity checks
		MustBeTrue(0 < nbits && 
			   nbits <= (sizeof(unsigned short)*BitsInByte));

		// mask out unused bits
		unsigned long ulcode = code&(~(((unsigned long)-1)<<nbits));

		// check if buffer is full
		if (nextbit >= maxbit)
		{
			MustBeTrue(outfd.write(&bitbuf, 
				SizeOfBitBuffer*sizeof(unsigned long)) ==
				SizeOfBitBuffer*sizeof(unsigned long));
			memset(bitbuf, 0, 
				SizeOfBitBuffer*sizeof(unsigned long));
			nextbit = 0;
		}

		// check if we can store all the data in buffer
		if ((nextbit+nbits) < maxbit)
		{
			// get long that contains the bit
			unsigned long mlong = 
				nextbit/(BitsInByte*sizeof(unsigned long));
			unsigned long mbit = 
				nextbit%(BitsInByte*sizeof(unsigned long));

			// shift bits in correct place
			ulcode <<= 
		
			// shift in new bits
			bitbuf[mlong] 
			
		}

		// check if room to store part of the data
		if ((nextbit+nbits) >= maxbit)
		{
			// store part of the data in bit buffer,
			// write the buffer, and write the remaining
			// bits in the buffer
			int numberBitsToStore = maxbit-nextbit;
			unsigned short bitsToStore =
				code
			
		}
	
	}
	void encodeFile() {
		// rewind input file
		infd.rewind();

		// read and encode file
		for (unsigned char c; infd.bgetc(c) > 0; )
		{
			writeToFile(table.data[c].nbits, table.data[c].code);
		}
		flushToFile();
	}

	// read in huffman table from input file
	void readHeader() {
	}

	// decode file using huffman table from input file
	void decodeFile() {
	}

	// read in file and get code distribution.
	void frequencyDist() {
		// initialize table
		for (int b = 0; b < NumberOfCodes; b++)
		{
			frequency[b][0] = b;
			frequency[b][1] = 0;
		}

		// read and populate frequency table
		unsigned char c;
		MustBeTrue(infd.bgetc(c) > 0);
		frequency[c][1]++;
		while (infd.bgetc(c) > 0)
		{
			frequency[c][1]++;
		}

		// rewind file for next read
		infd.seek(0, SEEK_SET);
		return;
	}

protected:
	// internal data
	char *infile;
	char *outfile;
	File infd;
	File outfd;

	// transient data
	mutable unsigned long frequency[NumberOfCodes][2];
	mutable HuffmanNode *phroot;
	mutable HuffmanTable table;
	mutable unsigned long bitbuf[SizeOfBitBuffer];
	mutable unsigned long nextbit;
	mutable unsigned long maxbit;
};

// function definitions
void 
HuffmanTable::fillInTable(const HuffmanNode *pr)
{
	// new code string
	unsigned char nbits = 0;
	unsigned short newcode = 0L;

	// descend tree and store data in table
	MustBeTrue(pr->getLeft() != NULL);
	fillInTable(pr->getLeft(), nbits, newcode);
	MustBeTrue(pr->getRight() != NULL);
	fillInTable(pr->getRight(), nbits, newcode);
}

void 
HuffmanTable::fillInTable(const HuffmanNode *pn, 
		unsigned char &nbits, unsigned short &newcode)
{
	// sanity checks
	MustBeTrue(pn->isValid());
	MustBeTrue(pn->getCode() == '0' || pn->getCode() == '1');

	// set bit in newcode,
	nbits++;
	MustBeTrue(nbits <= 
		pn->numberOfBits(sizeof(unsigned short)));
	newcode <<= 1;
	if (pn->getCode() == '1')
		newcode |= 1;

	// descend down tree
	if (pn->isNotALeaf())
	{
		fillInTable(pn->getLeft(), nbits, newcode);
		fillInTable(pn->getRight(), nbits, newcode);
	}
	else
	{
		unsigned char datum = pn->getData();
		data[datum].inuse = 1;
		data[datum].data = datum;
		data[datum].nbits = nbits;
		data[datum].code = newcode;
		nentries++;
	}

	// undo new code
	nbits--;
	newcode >>= 1;
	return;
}

#endif

