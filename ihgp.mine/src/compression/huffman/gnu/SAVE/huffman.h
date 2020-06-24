#ifndef __HUFFMAN_H
#define __HUFFMAN_H
// huffman encode or decode a file

// headers
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "fileio.h"
#include "array.h"

// local definitions
#define BitsInByte 8
#define BitsInULong (BitsInByte*sizeof(unsigned long))
#define BitsInUShort (BitsInByte*sizeof(unsigned short))
#define BytesInULong sizeof(unsigned long)
#define BytesInUShort sizeof(unsigned short)
#define NumberOfCodes 256

// forward declarations
class HuffmanTableEntry;
class HuffmanIndex;
class HuffmanTable;
class HuffmanNode;
class HuffmanPQ;
class Huffman;

// internal data structures
class HuffmanIndex{
private:
	// friends
	friend class HuffmanTable;
	friend class HuffmanNode;
	friend class Huffman;

	// constructor
	HuffmanIndex():
		inuse(0), nbits(0), code(0), node(NULL) {
		// do nothing
	}

	// sanity check
	int inUse() const {
		return(inuse);
	}

	// internal data
	unsigned char inuse;
	unsigned char nbits;
	unsigned short code;
	HuffmanNode *node;
	HuffmanIndex *next;
};

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
	void setData(unsigned char d) {
		data = d;
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

	// print tree data
	void printIt() const {
		// sanity check
		MustBeTrue(isValid());

		// descend down the tree
		if (isNotALeaf())
		{
			left->printIt();
			right->printIt();
		}
		else
		{
			cerr << "code: ";
			int iscode=BUFSIZ-1;
			char scode[BUFSIZ];
			scode[iscode--] = '\0';
			for (const HuffmanNode *p = this;
				p != NULL; p = p->parent)
			{
				scode[iscode--] = (char)(p->code);
			}
			iscode += 2;
			cerr << scode+iscode;
			if (isalnum(data) || ispunct(data) || isprint(data))
				cerr << ", data: " << data;
			else
				cerr << ", data: " << (int)data;
			cerr << ", counts: " << counts << endl;
		}
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
		phroot(NULL), hindex(NULL)
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
		maxbit = BitsInULong;
		bitbuf = 0L;
	}
	~Huffman() {
		delete [] infile;
		infile = NULL;
		delete [] outfile;
		outfile = NULL;
		if (phroot != NULL)
			delete phroot;
		phroot = NULL;
		if (hindex != NULL)
			delete hindex;
		hindex = NULL;
		nextbit = 0;
	}

	// encoding or decoding of files
	void encode() {
		// open input file for read 
		infd.open(infile, O_RDONLY);

		// we have to read in the input file and 
		// build a huffman tree for it.
		buildTree();

		// print tree
		cerr << "HUFFMAN TREE: " << endl;
		phroot->printIt();

		// convert tree to a table for easier access
		convertToTable();

		// open output file now that we are ready
		outfd.open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0664);

		// now write out header
		writeHeader();

		// encode and write out the fil
		encodeFile();
	}
	void fast_decode(int bitsInIndexKey = BitsInByte) {
		// open input file for read 
		infd.open(infile, O_RDONLY);

		// read in huffman codes from input file.
		readHeader();

		// convert from table to a tree
		convertToTree();

		// print tree
		cerr << "HUFFMAN TREE: " << endl;
		phroot->printIt();

		// build huffman tree index for quick decoding
		buildIndex(bitsInIndexKey);

		// open output file, we are ready to use it
		outfd.open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0664);

		// decode file and write it out.
		fastDecodeFile();
	}
	void decode() {
		// open input file for read 
		infd.open(infile, O_RDONLY);

		// read in huffman codes from input file.
		readHeader();

		// convert from table to a tree
		convertToTree();

		// print tree
		cerr << "HUFFMAN TREE: " << endl;
		phroot->printIt();

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
		MustBeTrue(outfd.write(
			&table.nentries, BytesInUShort) == BytesInUShort);

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
				BytesInUShort) == BytesInUShort);
		}
	}

	// encode input file using huffman table
	void writeToFile(unsigned char nbits, unsigned short code)
	{
		// sanity check
		MustBeTrue(nbits <= BitsInUShort);
		MustBeTrue(nextbit <= maxbit);

		// mask and store code 
		unsigned long ulcode = (unsigned long)
			(code&(~(((unsigned short)-1)<<nbits)));

		// check if buffer is full
		if (nextbit == maxbit)
		{
			// write bit buffer 
			MustBeTrue(outfd.bputl(bitbuf) > 0);
			nextbit = 0;
			bitbuf = 0L;
		}

		// check if data fits in buffer
		if ((nextbit+nbits) <= maxbit)
		{
			// store data in bit buffer
			bitbuf |= ulcode<<(maxbit-nextbit-nbits);
			nextbit += nbits;
			return;
		}

		// only part of the bits fit, ugh.
		MustBeTrue(nextbit < maxbit && maxbit < (nextbit+nbits));

		// store the part that fits and write it,
		// then store the remaining part.
		//
		int topBits = maxbit-nextbit;
		MustBeTrue(topBits < nbits);
		int bottomBits = nbits-topBits;
		MustBeTrue(bottomBits > 0);
		bitbuf |= (ulcode>>bottomBits);
		MustBeTrue(outfd.bputl(bitbuf) > 0);
		bitbuf = ulcode<<(maxbit-bottomBits);
		nextbit = bottomBits;

		// all done
		return;
	}
	void flushToFile() {
		if (nextbit > 0)
		{
			MustBeTrue(outfd.bputl(bitbuf) > 0);
		}
		MustBeTrue(outfd.flush_bputl() > 0);
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
		// rewind file
		infd.rewind();

		// read the number of entries
		MustBeTrue(infd.read(
			&table.nentries, BytesInUShort) == BytesInUShort);
		MustBeTrue(table.nentries > 0);

		// read in codes 
		for (int i=0; i<table.nentries; i++)
		{
			// data to read in
			unsigned char uc;
			unsigned char nbits;
			unsigned short code;

			// read in the data
			MustBeTrue(infd.read(&uc, 1) == 1);
			MustBeTrue(infd.read(&nbits, 1) == 1);
			MustBeTrue(infd.read(&code, 
				BytesInUShort) == BytesInUShort);

			// store data in table
			table.data[uc].data = uc;
			table.data[uc].nbits = nbits;
			table.data[uc].code = code;
			table.data[uc].inuse = 1;
		}
	}

	// build huffman tree from input file header
	void buildTreePath(HuffmanNode *phn, unsigned currbit, 
		unsigned char data, unsigned char nbits, 
		unsigned short uscode)
	{
		// check if we are done
		if (currbit >= nbits)
		{
			// done with this path
			MustBeTrue(phn != NULL);
			phn->setData(data);
			return;
		}

		// continue setting path
		unsigned char bitval = (uscode>>(nbits-currbit-1))&1;
		if (bitval == 0)
		{
			// check if left node exists
			if (phn->getLeft() == NULL)
			{
				// create a new LEFT node.
				HuffmanNode *pleft = new HuffmanNode(
					(unsigned char)-1, 0);
				MustBeTrue(pleft != NULL);
				phn->setLeft(pleft);
				pleft->setParent(phn);
				pleft->setCode('0');
			}

			// build node
			buildTreePath(phn->getLeft(), currbit+1, 
					data, nbits, uscode);
		}
		else
		{
			// check if right node exists
			if (phn->getRight() == NULL)
			{
				// create a new LEFT node.
				HuffmanNode *pright = new HuffmanNode(
					(unsigned char)-1, 0);
				MustBeTrue(pright != NULL);
				phn->setRight(pright);
				pright->setParent(phn);
				pright->setCode('1');
			}

			// build node
			buildTreePath(phn->getRight(), currbit+1, 
					data, nbits, uscode);
		}
	}
	void convertToTree() {
		// get rid of old tree, if any.
		if (phroot != NULL)
		{
			delete phroot;
			phroot = NULL;
		}

		// allocate the root node
		phroot = new HuffmanNode((unsigned char)-1, 0);
		MustBeTrue(phroot != NULL);
		phroot->setParent(NULL);

		// cycle thru entries in table, and build a 
		// path in the tree for each code that was 
		// read in.
		for (int i=0; i<NumberOfCodes; i++)
		{
			// check if table entry is used.
			if (!table.data[i].inuse)
				continue;

			// recursively build path for this code
			unsigned char currbit = 0;
			unsigned char data =  table.data[i].data;
			unsigned char nbits = table.data[i].nbits;
			unsigned short uscode = table.data[i].code;
			unsigned char bitval = (uscode>>(nbits-currbit-1))&1;
			if (bitval == 0)
			{
				// check if left node exists
				if (phroot->getLeft() == NULL)
				{
					// create a new LEFT node.
					HuffmanNode *pleft = new HuffmanNode(
						(unsigned char)-1, 0);
					MustBeTrue(pleft != NULL);
					phroot->setLeft(pleft);
					pleft->setParent(phroot);
					pleft->setCode('0');
				}

				// build node
				buildTreePath(phroot->getLeft(), currbit+1, 
						data, nbits, uscode);
			}
			else
			{
				// check if right node exists
				if (phroot->getRight() == NULL)
				{
					// create a new RIGHT node.
					HuffmanNode *pright = new HuffmanNode(
						(unsigned char)-1, 0);
					MustBeTrue(pright != NULL);
					phroot->setRight(pright);
					pright->setParent(phroot);
					pright->setCode('1');
				}

				// build node
				buildTreePath(phroot->getRight(), currbit+1, 
						data, nbits, uscode);
			}
		}

		// all done
		return;
	}

	// convert Huffman tree to a indexed-tree structure. the
	// index points into the Huffman tree for fast access
	// when decoding a file. 
	//
	void dumpbits(int nbits, unsigned short data, const char *msg) {
		char buf[BUFSIZ];
		buf[nbits] = '\0';
		for (int b=1; b<=nbits; b++)
		{
			if (data&1)
				buf[nbits-b] = '1';
			else
				buf[nbits-b] = '0';
			data >>= 1;
		}
		cerr << msg << buf << endl;
	}
	void buildIndex(int maxbits) {
		// the maximum allowed bits in a Huffman code
		// is the number of bits in an unsigned short (my
		// choice).
		//
		MustBeTrue(0 < maxbits && maxbits <= BitsInUShort);

		// allocate a new Huffman tree index
		if (hindex != NULL)
		{
			delete [] hindex;
			hindex = NULL;
			hindexsz = 0;
		}
		hidxkeysz = maxbits;
		hindexsz = ((unsigned short)1)<<maxbits;
		hindex = new HuffmanIndex [hindexsz];
		MustBeTrue(hindex != NULL);

		// start filling in the index
		for (unsigned short i=0; i<hindexsz; i++)
		{
			unsigned short code = i;
			cerr << "code: " << code;
			HuffmanNode *pn = phroot;
			for (int i=0; i<maxbits; i++, code >>= 1)
			{
				// get current bit
				unsigned int bit = code&1;
				MustBeTrue(pn != NULL && pn->isValid());
				if (pn->isNotALeaf())
				{
					if (bit == 0)
						pn = pn->getLeft();
					else
						pn = pn->getRight();
				}
				else
				{
					cerr << ", char: " << (int)pn->getData();
					pn = phroot;
				}
			}
			MustBeTrue(pn != NULL && pn->isValid());
			if (pn->isNotALeaf())
				cerr << ", NOT a last char.";
			else
				cerr << ", lchar: " << (int)pn->getData();
			cerr << endl;
		}

		return;
	}

	// decode file using huffman table from input file
	void fastDecodeFile() {
		// nothing
		return;
	}
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
		infd.rewind();
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
	mutable unsigned long bitbuf;
	mutable unsigned long nextbit;
	mutable unsigned long maxbit;
	mutable unsigned short hidxkeysz;
	mutable unsigned short hindexsz;
	mutable HuffmanIndex *hindex;
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
	MustBeTrue(nbits <= pn->numberOfBits(BytesInUShort));
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

