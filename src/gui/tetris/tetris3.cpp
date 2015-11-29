// rotation coordinates for tetrominoes.

// board
//	  0 1 2 3 ...
//	0 +-------------+ X
//	1 |             |
//	2 |             |
//	3 |             |
//	. |             |
//	. |             |
//	. |             |
//	  |             |
//	  +-------------+
//      Y

// headers
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <map>

#include <logging/Logging.h>

using namespace ombt;

// color map
class ColorMap {
public:
	ColorMap() {
		addcolor(" ", "black");
		addcolor("I", "cyan");
		addcolor("J", "blue");
		addcolor("L", "orange");
		addcolor("O", "yellow");
		addcolor("S", "green");
		addcolor("T", "purple");
		addcolor("Z", "red");
	}
	~ColorMap() { }

	const std::string &operator[](const std::string &color) {
		return(map_[color]);
	}

private:
	void addcolor(const std::string &letter, 
		      const std::string &color) {
		map_[color] = letter;
	}

private:
	std::map<std::string, std::string> map_;
};

static ColorMap colormap;

// a block in the board
class Block {
public:
	friend std::ostream &operator<<(std::ostream &os, const Block &b);

	// ctor
	Block(const std::string &color = "black"):
		color_(color) { }
	Block(const Block &src):
		x_(src.x_), y_(src.y_), color_(src.color_) { }
	~Block() { }

	// assignment
	Block &operator=(const Block &rhs) {
		if (this != &rhs)
		{
			x_ = rhs.x_;
			y_ = rhs.y_;
			color_ = rhs.color_;
		}
		return(*this);
	}

	// operations
	void setcolor(const std::string &color) {
		color_ = color;
	}
	void getcolor(std::string &color) const {
		color = color_;
	}
	void setxy(int newx, int newy) {
		x_ = newx;
		y_ = newy;
	}
	void getxy(int &x, int &y) const {
		x = x_;
		y = y_;
	}
	void rotate90CW() {
		int newx = y_;
		int newy = -x_;
		x_ = newx;
		y_ = newy;
	}
	void rotate90CCW() {
		int newx = -y_;
		int newy = x_;
		x_ = newx;
		y_ = newy;
	}

private:
	// data
	int x_, y_;
	std::string color_;
};

std::ostream &operator<<(std::ostream &os, const Block &b)
{
	os << "(X,Y) = (" << b.x_ << "," << b.y_ << ")";
	return(os);
}

// tetromino
class Tetromino {
public:
	friend std::ostream &operator<<(std::ostream &os, const Tetromino &b);

	// ctor and dtor
	Tetromino() { } 
	Tetromino(const std::string letter, 
		  const std::string color): 
			center_(0), letter_(letter), color_(color) {
		blocks_[0].setcolor(color_);
		blocks_[1].setcolor(color_);
		blocks_[2].setcolor(color_);
		blocks_[3].setcolor(color_);
	}
	Tetromino(const Tetromino &src):
		center_(src.center_),
		letter_(src.letter_),
		color_(src.color_) {
		for (int i=0; i<4; ++i)
		{
			blocks_[i] = src.blocks_[i];
		}
	}
	virtual ~Tetromino() { }

	// assignment
	Tetromino &operator=(const Tetromino &rhs) {
		if (this != &rhs)
		{
			center_ = rhs.center_;
			letter_ = rhs.letter_;
			color_ = rhs.color_;
			for (int i=0; i<4; ++i)
			{
				blocks_[i] = rhs.blocks_[i];
			}
		}
		return(*this);
	}

	// operations
	std::string getletter() const { return(letter_); }
	std::string getcolor() const { return(color_); }
	virtual void rotate90CW() {
		for (int i=0; i<4; ++i)
		{
			blocks_[i].rotate90CW();
		}
	}
	virtual void rotate90CCW() {
		for (int i=0; i<4; ++i)
		{
			blocks_[i].rotate90CCW();
		}
	}
	void getcenter(int &center) const {
		center = center_;
	}
	void getoffset(int block, int &x, int &y) const {
		assert(0 <= block && block <= 3);
		blocks_[block].getxy(x, y);
	}
	Block getblock(int block) const {
		return(blocks_[block]);
	}
	void sanecoords(int &finalx, int &finaly, int x, int y, 
			int minx, int maxx, int miny, int maxy) {
		if (x<minx)
			x = minx;
		if (x>=maxx)
			x = maxx-1;
		if (y<miny)
			y = miny;
		if (y>=maxy)
			y = maxy-1;

		for (int i=0; i<4; ++i)
		{
			int xoff, yoff;
			//blocks_[i].getxy(xoff, yoff);
			getoffset(i, xoff, yoff);
			if ((x+xoff)<minx)
				x = minx-xoff;
			if ((x+xoff)>=maxx)
				x = maxx-xoff-1;
			if ((y+yoff)<miny)
				y = miny-yoff;
			if ((y+yoff)>=maxy)
				y = maxy-yoff-1;
		}
		finalx = x;
		finaly = y;
	}

protected:
	int center_;
	std::string letter_;
	std::string color_;
	Block blocks_[4];
};

std::ostream &operator<<(std::ostream &os, const Tetromino &t)
{
	os << "Letter: " << t.letter_ << std::endl;
	os << "Color : " << t.color_ << std::endl;
	for (int i=0; i<4; ++i)
	{
		os << i << ": " << t.blocks_[i] << std::endl;
	}
	return(os);
}

// 7 types of tetrominoes
class ITetromino: public Tetromino {
public:
	ITetromino(): Tetromino("I", "cyan")
	{
		center_ = 1;
		blocks_[0].setxy(-1, 0);
		blocks_[1].setxy(0, 0);
		blocks_[2].setxy(1, 0);
		blocks_[3].setxy(2, 0);
	}
	~ITetromino() { }
};

class JTetromino: public Tetromino {
public:
	JTetromino(): Tetromino("J", "blue")
	{
		center_ = 1;
		blocks_[0].setxy(0, -1);
		blocks_[1].setxy(0, 0);
		blocks_[2].setxy(0, 1);
		blocks_[3].setxy(-1, 1);
	}
	~JTetromino() { }
};

class LTetromino: public Tetromino {
public:
	LTetromino(): Tetromino("L", "orange")
	{
		center_ = 1;
		blocks_[0].setxy(0, -1);
		blocks_[1].setxy(0, 0);
		blocks_[2].setxy(0, 1);
		blocks_[3].setxy(1, 1);
	}
	~LTetromino() { }
};

class OTetromino: public Tetromino {
public:
	OTetromino(): Tetromino("O", "yellow")
	{
		center_ = 3;
		blocks_[0].setxy(0, -1);
		blocks_[1].setxy(1, -1);
		blocks_[2].setxy(1, 0);
		blocks_[3].setxy(0, 0);
	}
	~OTetromino() { }

	virtual void rotate90CW() { }
	virtual void rotate90CCW() { }
};

class STetromino: public Tetromino {
public:
	STetromino(): Tetromino("S", "green")
	{
		center_ = 2;
		blocks_[0].setxy(1, -1);
		blocks_[1].setxy(0, -1);
		blocks_[2].setxy(0, 0);
		blocks_[3].setxy(-1, 0);
	}
	~STetromino() { }
};

class TTetromino: public Tetromino {
public:
	TTetromino(): Tetromino("T", "purple")
	{
		center_ = 1;
		blocks_[0].setxy(-1, 0);
		blocks_[1].setxy(0, 0);
		blocks_[2].setxy(1, 0);
		blocks_[3].setxy(0, 1);
	}
	~TTetromino() { }
};

class ZTetromino: public Tetromino {
public:
	ZTetromino(): Tetromino("Z", "red")
	{
		center_ = 2;
		blocks_[0].setxy(-1, -1);
		blocks_[1].setxy(0, -1);
		blocks_[2].setxy(0, 0);
		blocks_[3].setxy(1, 0);
	}
	~ZTetromino() { }
};

// board
class Board {
public:
	friend std::ostream &operator<<(std::ostream &os, const Board &b);

	// ctors and dtor
	Board(int maxx, int maxy):
		done_(false), maxx_(maxx), maxy_(maxy) {
		assert((maxx_ > 0) && (maxy_ > 0));
		board_ = new Block [maxx_*maxy_];
		for (int x=0; x<maxx_; ++x)
		{
			for (int y=0; y<maxy_; ++y)
			{
				board_[x*maxy_ + y].setxy(x,y);
				board_[x*maxy_ + y].setcolor("black");
			}
		}
	}
	~Board() {
		delete [] board_;
		board_ = NULL;
	}

	// access
	Block &operator()(int x, int y) {
		assert(0 <= x && x < maxx_);
		assert(0 <= y && y < maxy_);
		return(board_[x*maxy_ + y]);
	}

	Block &operator()(int x, int y) const {
		assert(0 <= x && x < maxx_);
		assert(0 <= y && y < maxy_);
		return(board_[x*maxy_ + y]);
	}

	// tetromino operations
	int add(Tetromino &letter, int x, int y, int orientation) {
		if (x<0 || y<0 || maxx_<=x || maxy_<=0) return(-1);
		letter_ = letter;
		if (orientation > 0)
		{
			for ( ; orientation>0; --orientation)
			{
				letter_.rotate90CCW();
			}
		}
		letter_.sanecoords(x_, y_, x, y, 0, maxx_, 0, maxy_);
		setlettercolor(letter_.getcolor());
		return(0);
	}
	int rotate90CW() {
		setlettercolor("black");
		letter_.rotate90CW();
		setlettercolor(letter_.getcolor());
		return(0);
	}
	int rotate90CCW() {
		setlettercolor("black");
		letter_.rotate90CCW();
		setlettercolor(letter_.getcolor());
		return(0);
	}
	int move(int delx, int dely) {
		setlettercolor("black");
		x_ += delx;
		y_ += dely;
		setlettercolor(letter_.getcolor());
		return(0);
	}

	// misc
	void clear() {
		done_ = false;
		for (int x=0; x<maxx_; ++x)
		{
			for (int y=0; y<maxy_; ++y)
			{
				board_[x*maxy_ + y].setcolor("black");
			}
		}
	}
	bool done() const { return(done_); }

private:
	// utilities
	void setlettercolor(const std::string &color) {
		for (int ib=0; ib<4; ++ib)
		{
			int xoff, yoff;
			letter_.getoffset(ib, xoff, yoff);
			if (!(0 <= (x_+xoff) && (x_+xoff) < maxx_) ||
			    !(0 <= (y_+yoff) && (y_+yoff) < maxy_))
			{
				done_ = true;
				return;
			}
			(*this)(x_+xoff,y_+yoff).setcolor(color);
			
		}
	}

private:
	// not allowed
	Board();
	Board(const Board &src);
	Board &operator=(const Board &rhs);

private:
	// data
	bool done_;
	int x_;
	int y_;
	Tetromino letter_;
	int maxx_;
	int maxy_;
	Block *board_;
};

std::ostream &operator<<(std::ostream &os, const Board &b)
{
	os << std::endl;
	os << " ";
	for (int x=0; x<b.maxx_; ++x)
	{
		os << x%10;
	}
	os << std::endl;
	for (int y=0; y<b.maxy_; ++y)
	{
		os << y%10;
		for (int x=0; x<b.maxx_; ++x)
		{
			std::string color;
			b(x,y).getcolor(color);
			os << colormap[color];
		}
		os << y%10 << std::endl;
	}
	os << " ";
	for (int x=0; x<b.maxx_; ++x)
	{
		os << x%10;
	}
	os << std::endl;
	return(os);
}

// test letter
void
testTetromino(Board &board, Tetromino &letter, int maxx)
{
	board.clear();
	board.add(letter, rand()%maxx, 0, rand()%4);
	std::cerr << board << std::endl;
	while (!board.done())
	{
		int choice = rand()%3;
		switch (choice)
		{
		case 0:
			board.move(0,1);
			break;
		case 1:
			board.rotate90CW();
			break;
		case 2:
			board.rotate90CCW();
			break;
		default:
			continue;
		}
		std::cerr << board << std::endl;
	}
}

// main entry point
int
main(int argc, char **argv)
{
	int maxx = 10;
	int maxy = 18;

	if (argc == 3)
	{
		int arg = 0;
		maxx = atoi(argv[++arg]);
		maxy = atoi(argv[++arg]);
	}

	std::cerr << "MaxX: " << maxx << std::endl;
	std::cerr << "MaxY: " << maxy << std::endl;

	Board b(maxx, maxy);
	Tetromino tetromino;

	srand(time(NULL));

	tetromino = ITetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = JTetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = LTetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = OTetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = STetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = TTetromino();
	testTetromino(b, tetromino, maxx);

	tetromino = ZTetromino();
	testTetromino(b, tetromino, maxx);

	return(0);
}

