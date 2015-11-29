// plot a 2-d graph

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <unistd.h>

// Xlib headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// local headers
#include "returns.h"
#include "debug.h"

// local definitions
#define MaxCharsInWindow 12
#define MaxCharsInFormat 12
#define VerticalSpace 2
#define HorizontalSpace 2
#define Space 10
#define MinimumMainWidth 500
#define MinimumMainHeight 300

// definitions for windows
#define X_Axis_Plot_Type 0
#define Y_Axis_Plot_Type 1
#define Reset 2
#define Cursor_Mode 3
#define X_Cross_Hair 4
#define Y_Cross_Hair 5
#define Integral 6
#define WeightedMean 7
#define Quit 8
#define Main 9
#define Graph 10
#define NumberOfButtons (Quit+1)
#define NumberOfWindows (NumberOfButtons+2)
#define NumberOfFixedLabels 9
#define NumberOfNonFixedLabels 14
#define NumberOfXHairLabels 20
#define NumberOfNonXHairLabels \
	(NumberOfFixedLabels+NumberOfNonFixedLabels)
#define NumberOfLabels \
	(NumberOfFixedLabels+NumberOfNonFixedLabels+NumberOfXHairLabels)
#define Direction0 0
#define Direction90 1
#define Direction180 2
#define Direction270 3

// event masks for main, graph, and  button windows
#define MainEventMask \
	(ExposureMask | \
	 StructureNotifyMask | \
	 KeyPressMask)

#define GraphEventMask \
	(ExposureMask | \
	 ButtonPressMask | \
	 ButtonReleaseMask | \
	 ButtonMotionMask)

#define ButtonEventMask \
	(ExposureMask | \
	 ButtonReleaseMask | \
	 ButtonPressMask)

// definitions for reading in data
#define StartOfOptions "++++ START OPTIONS ++++"
#define StartOfData "++++ START DATA ++++"
#define MinimumXValue "MINIMUM X VALUE"
#define MaximumXValue "MAXIMUM X VALUE"
#define MinimumYValue "MINIMUM Y VALUE"
#define MaximumYValue "MAXIMUM Y VALUE"
#define XAxisPlotType "X-AXIS PLOT TYPE"
#define YAxisPlotType "Y-AXIS PLOT TYPE"
#define XAxisLabel "X-AXIS LABEL"
#define YAxisLabel "Y-AXIS LABEL"
#define GraphTitle "GRAPH TITLE"
#define PointsToAllocate 100

// plot states
#define NoState 0
#define LinearPlotState 1
#define LogarithmPlotState 2
#define SquareRootPlotState 3
#define CrossHairState 4
#define ZoomerState 5
#define CrossHairWithLabelsState 6

// x-window command line options
class XCmdOpts
{
public:
	// constructor and destructor
	XCmdOpts();
	~XCmdOpts();

	// internal data
	char display[BUFSIZ];
	char bordercolor[BUFSIZ];
	char foreground[BUFSIZ];
	char background[BUFSIZ];
	char title[BUFSIZ];
	char name[BUFSIZ];
	char geometry[BUFSIZ];
	char font[BUFSIZ];
	int iconic;
};

// structure to hold spectrum data
class SpectrumData
{
public:
	// constructors and destructor
	SpectrumData();
	SpectrumData(const SpectrumData &sp);
	~SpectrumData();

	// assignment operator
	SpectrumData &operator=(const SpectrumData &sp);

	// raw data points
	int allocpts;
	int npts;
	double *x, *y;

	// actual limits for the data set.
	double xmin;
	double xmax;
	double ymin;
	double ymax;
};

// structure to hold spectrum data
class GraphPlotOptions
{
public:
	// constructors and destructor
	GraphPlotOptions();
	GraphPlotOptions(const GraphPlotOptions &sp);
	~GraphPlotOptions();

	// assignment operator
	GraphPlotOptions &operator=(const GraphPlotOptions &sp);

	// user given limits
	double user_xmin;
	double user_xmax;
	double user_ymin;
	double user_ymax;

	// user given plot types
	int user_xplottype;
	int user_yplottype;

	// graph limits
	double graph_xmin;
	double graph_xmax;
	double graph_ymin;
	double graph_ymax;

	// these flags are needed since there are no out-of-range values
	// for doubles
	int user_xmin_given;
	int user_xmax_given;
	int user_ymin_given;
	int user_ymax_given;

	// graph types
	int xplottype;
	int yplottype;

	// cursor state: cross hairs or zoom
	int cursor_state;

	// x,y cross hair values
	double x_cross_hair;
	double y_cross_hair;

	// graph labels
	char xlabel[BUFSIZ];
	char ylabel[BUFSIZ];
	char title[BUFSIZ];

	// graph origin
	int xorig;
	int yorig;

	// sum of y counts
	double ysum;

	// weighted x-average
	double xwmean;

	// peak labels
	int labelcount;
	double energies[NumberOfXHairLabels];
};

// structure to hold display data
struct DisplayData
{
	Window root_window;
	Display *display;
	unsigned int display_width;
	unsigned int display_height;
	int screen;
	GC gc;
	GC xor_gc;
	XFontStruct *font;
	int depth;
	Visual *visual;
	Colormap colormap;
	unsigned long background;
	unsigned long foreground;
	unsigned long black;
	unsigned long white;
} ;

// structure for labels
struct LabelData {
	char label[BUFSIZ];
	int x;
	int y;
	int width;
	int height;
	int direction;
	int reverse;
	Window window;
} labels[NumberOfLabels] = {
	{ "X PLOT TYPE", 0, 0, 0, 0, 0, 0, 0 },
	{ "Y PLOT TYPE", 0, 0, 0, 0, 0, 0, 0 },
	{ "RESET", 0, 0, 0, 0, 0, 0, 0 },
	{ "CURSOR MODE", 0, 0, 0, 0, 0, 0, 0 },
	{ "CROSS HAIR X", 0, 0, 0, 0, 0, 0, 0 },
	{ "CROSS HAIR Y", 0, 0, 0, 0, 0, 0, 0 },
	{ "TOTAL COUNTS", 0, 0, 0, 0, 0, 0, 0 },
	{ "WEIGHTED MEAN", 0, 0, 0, 0, 0, 0, 0 },
	{ "QUIT", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
	{ "", 0, 0, 0, 0, 0, 0, 0 },
};

// externs for windows
extern int x_plot_type(XEvent &);
extern int y_plot_type(XEvent &);
extern int reset(XEvent &);
extern int cursor_mode(XEvent &);
extern int x_cross_hairs(XEvent &);
extern int y_cross_hairs(XEvent &);
extern int total_counts(XEvent &);
extern int weighted_mean(XEvent &);
extern int quit_handler(XEvent &);
extern int main_handler(XEvent &);
extern int graph_handler(XEvent &);

// globals for windows
struct WindowInfo {
	int x;
	int y;
	int width;
	int height;
	Window window;
	int (*handler)(XEvent &);
} winfo[NumberOfWindows] = {
	{ 0, 0, 0, 0, 0, x_plot_type },
	{ 0, 0, 0, 0, 0, y_plot_type },
	{ 0, 0, 0, 0, 0, reset },
	{ 0, 0, 0, 0, 0, cursor_mode },
	{ 0, 0, 0, 0, 0, x_cross_hairs },
	{ 0, 0, 0, 0, 0, y_cross_hairs },
	{ 0, 0, 0, 0, 0, total_counts },
	{ 0, 0, 0, 0, 0, weighted_mean },
	{ 0, 0, 0, 0, 0, quit_handler },
	{ 0, 0, 0, 0, 0, main_handler },
	{ 0, 0, 0, 0, 0, graph_handler },
};

// structure for rubberbanding
struct RubberBandData {
	int start_x;
	int start_y;
	int last_x;
	int last_y;
};

// globals
XCmdOpts xopts;
SpectrumData usrd;
GraphPlotOptions popts;
DisplayData dd;
RubberBandData rbd;
RubberBandData xhd;
int done = 0;

// debugging
extern int _Xdebug;

// constructor and destructor for spectrum
GraphPlotOptions::GraphPlotOptions():
	user_xmin(0.0), user_xmax(0.0), 
	user_ymin(0.0), user_ymax(0.0),
	user_xplottype(LinearPlotState), user_yplottype(LinearPlotState),
	cursor_state(ZoomerState),
	x_cross_hair(0.0), y_cross_hair(0.0),
	graph_xmin(0.0), graph_xmax(0.0), 
	graph_ymin(0.0), graph_ymax(0.0),
	user_xmin_given(0), user_xmax_given(0), 
	user_ymin_given(0), user_ymax_given(0),
	xplottype(LinearPlotState), yplottype(LinearPlotState),
	xorig(0), yorig(0), ysum(0), xwmean(0.0),
	labelcount(0)
{
	strcpy(xlabel, "");
	strcpy(ylabel, "");
	strcpy(title, "NO TITLE");
	for (int ie = 0; ie < NumberOfXHairLabels; ie++)
	{
		energies[ie] = 0.0;
	}
}

GraphPlotOptions::GraphPlotOptions(const GraphPlotOptions &sp):
	user_xmin(sp.user_xmin), user_xmax(sp.user_xmax), 
	user_ymin(sp.user_ymin), user_ymax(sp.user_ymax),
	user_xplottype(sp.user_xplottype), user_yplottype(sp.user_yplottype),
	cursor_state(sp.cursor_state),
	x_cross_hair(sp.x_cross_hair), y_cross_hair(sp.y_cross_hair),
	graph_xmin(sp.graph_xmin), graph_xmax(sp.graph_xmax), 
	graph_ymin(sp.graph_ymin), graph_ymax(sp.graph_ymax),
	user_xmin_given(sp.user_xmin_given), 
		user_xmax_given(sp.user_xmax_given), 
	user_ymin_given(sp.user_ymin_given), 
		user_ymax_given(sp.user_ymax_given),
	xplottype(sp.xplottype), yplottype(sp.yplottype),
	xorig(sp.xorig), yorig(sp.yorig), 
	ysum(sp.ysum), xwmean(sp.xwmean),
	labelcount(sp.labelcount)
{
	strcpy(xlabel, sp.xlabel);
	strcpy(ylabel, sp.ylabel);
	strcpy(title, sp.title);
	for (int ie = 0; ie < NumberOfXHairLabels; ie++)
	{
		energies[ie] = sp.energies[ie];
	}
}

GraphPlotOptions::~GraphPlotOptions()
{
	// do nothing
}

// assignment operator
GraphPlotOptions &
GraphPlotOptions::operator=(const GraphPlotOptions &sp)
{
	if (this != &sp)
	{
		user_xmin = sp.user_xmin;
		user_xmax = sp.user_xmax;
		user_ymin = sp.user_ymin;
		user_ymax = sp.user_ymax;
		user_xplottype = sp.user_xplottype;
		user_yplottype = sp.user_yplottype;
		cursor_state = sp.cursor_state;
		x_cross_hair = sp.x_cross_hair;
		y_cross_hair = sp.y_cross_hair;
		graph_xmin = sp.graph_xmin;
		graph_xmax = sp.graph_xmax;
		graph_ymin = sp.graph_ymin;
		graph_ymax = sp.graph_ymax;
		user_xmin_given = sp.user_xmin_given;
		user_xmax_given = sp.user_xmax_given;
		user_ymin_given = sp.user_ymin_given;
		user_ymax_given = sp.user_ymax_given;
		xplottype = sp.xplottype;
		yplottype = sp.yplottype;
		strcpy(xlabel, sp.xlabel);
		strcpy(ylabel, sp.ylabel);
		strcpy(title, sp.title);
		xorig = sp.xorig;
		yorig = sp.yorig;
		ysum = sp.ysum;
		xwmean = sp.xwmean;
		labelcount = sp.labelcount;
		for (int ie = 0; ie < NumberOfXHairLabels; ie++)
		{
			energies[ie] = sp.energies[ie];
		}
	}
	return(*this);
}

// constructor and destructor for X options
XCmdOpts::XCmdOpts()
{
	strcpy(display, "");
	strcpy(bordercolor, "black");
	strcpy(foreground, "black");
	strcpy(background, "white");
	strcpy(title, "graph");
	strcpy(name, "graph");
	strcpy(geometry, "");
	strcpy(font, "fixed");
	iconic = 0;
}

XCmdOpts::~XCmdOpts()
{
	// do nothing
}

// constructor and destructor for spectrum
SpectrumData::SpectrumData():
	allocpts(PointsToAllocate), npts(0), 
	x(new double [PointsToAllocate]), y(new double [PointsToAllocate]),
	xmin(0.0), xmax(0.0), 
	ymin(0.0), ymax(0.0)
{
	MustBeTrue(x != NULL && y != NULL);
}

SpectrumData::SpectrumData(const SpectrumData &sp):
	allocpts(sp.allocpts), npts(sp.npts),
	x(new double [sp.allocpts]), y(new double [sp.allocpts]),
	xmin(sp.xmin), xmax(sp.xmax), 
	ymin(sp.ymin), ymax(sp.ymax)
{
	MustBeTrue(x != NULL && y != NULL);
	memcpy((void *)x, (void *)sp.x, sizeof(double)*sp.npts);
	memcpy((void *)y, (void *)sp.y, sizeof(double)*sp.npts);
}

SpectrumData::~SpectrumData()
{
	delete [] x;
	delete [] y;
}

// assignment operator
SpectrumData &
SpectrumData::operator=(const SpectrumData &sp)
{
	if (this != &sp)
	{
		allocpts = sp.allocpts;
		npts = sp.npts;
		x = new double [sp.allocpts];
		y = new double [sp.allocpts];
		MustBeTrue(x != NULL && y != NULL);
		memcpy((void *)x, (void *)sp.x, sizeof(double)*sp.npts);
		memcpy((void *)y, (void *)sp.y, sizeof(double)*sp.npts);
		xmin = sp.xmin;
		xmax = sp.xmax;
		ymin = sp.ymin;
		ymax = sp.ymax;
	}
	return(*this);
}

// usage message
void
usage(const char *cmd)
{
	cerr << "usage: " << cmd << " [-debug] [standard X options] [filename]" << endl;
	return;
}

// get command lines options
int 
CommandLineOptions(int argc, char **argv, int &nextarg)
{
	// get command line option
	for (nextarg = 1; nextarg < argc; nextarg++)
	{
		if (strcmp(argv[nextarg], "-display") == 0)
		{
			strcpy(xopts.display, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-bordercolor") == 0) ||
			 (strcmp(argv[nextarg], "-bd") == 0))
		{
			strcpy(xopts.bordercolor, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-foreground") == 0) ||
			 (strcmp(argv[nextarg], "-fg") == 0))
		{
			strcpy(xopts.foreground, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-background") == 0) ||
			 (strcmp(argv[nextarg], "-bg") == 0))
		{
			strcpy(xopts.background, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-title") == 0)
		{
			strcpy(xopts.title, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-name") == 0)
		{
			strcpy(xopts.name, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-iconic") == 0)
		{
			xopts.iconic = 1;
		}
		else if ((strcmp(argv[nextarg], "-help") == 0) ||
			 (strcmp(argv[nextarg], "-?") == 0))
		{
			usage(argv[0]);
			exit(0);
		}
		else if ((strcmp(argv[nextarg], "-geometry") == 0) ||
			 (strcmp(argv[nextarg], "-geom") == 0))
		{
			strcpy(xopts.geometry, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-font") == 0) ||
			 (strcmp(argv[nextarg], "-fn") == 0))
		{
			strcpy(xopts.font, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-debug") == 0)
		{
			_Xdebug = 1;
		}
		else if (strncmp(argv[nextarg], "-", 1) == 0)
		{
			// unknown option. error.
			return(NOTOK);
		}
		else
		{
			// finished with options
			break;
		}
	}

	// all done
	return(OK);
}

// specialized string functions
int
mystrncmp(char *string1, char *string2)
{
	return(strncmp(string1, string2, strlen(string2)));
}

int
mystrncpy(char *string1, char *string2, char *prefix)
{
	// length of prefix
	int plen = strlen(prefix);

	// is string longer than prefix
	if (strlen(string2) <= plen)
	{
		*string1 = 0;
		return(OK);
	}

	// skip prefix and blank spaces
	string2 += strlen(prefix);
	for ( ; 
		(*string2 == ' ' || *string2 == '\t') && *string2 != 0; 
		string2++) ;

	// check if anything left
	if (*string2 == 0)
	{
		*string1 = 0;
		return(OK);
	}

	// copy remaining string
	strcpy(string1, string2);

	// remove trailing newline
	int s1len = strlen(string1);
	if (string1[s1len-1] == '\n')
		string1[s1len-1] = 0;

	// all done
	return(OK);
}

// check if plot type is ok
int
CheckPlotType(int axis, int plottype)
{
	int status = NOTOK;
	switch (axis)
	{
	case X_Axis_Plot_Type:
		switch (plottype)
		{
		case LinearPlotState:
			status = OK;
			break;

		case LogarithmPlotState:
			if ((popts.graph_xmin > 0.0) &&
			    (popts.graph_xmax > 0.0))
				status = OK;
			break;

		case SquareRootPlotState:
			if ((popts.graph_xmin >= 0.0) &&
			    (popts.graph_xmax >= 0.0))
				status = OK;
			break;

		default:
			break;
		}
		break;

	case Y_Axis_Plot_Type:
		switch (plottype)
		{
		case LinearPlotState:
			status = OK;
			break;

		case LogarithmPlotState:
			if ((popts.graph_ymin > 0.0) &&
			    (popts.graph_ymax > 0.0))
				status = OK;
			break;

		case SquareRootPlotState:
			if ((popts.graph_ymin >= 0.0) &&
			    (popts.graph_ymax >= 0.0))
				status = OK;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
	return(status);
}

// read in data
int
ReadInData(int argc, char **argv, int nextarg, istream &in)
{
	// read in data: first any options
	char inbuf[BUFSIZ+1], tmp[BUFSIZ+1];
	for (in.getline(inbuf, BUFSIZ); 
	     ! in.eof(); 
	     in.getline(inbuf, BUFSIZ))
	{
		if (*inbuf == '#')
		{
			// skip comments
			continue;
		}
		else if (mystrncmp(inbuf, (char *)StartOfOptions) == 0)
		{
			continue;
		}
		else if (mystrncmp(inbuf, (char *)StartOfData) == 0)
		{
			// break out of options loop
			break;
		}
		else if (mystrncmp(inbuf, (char *)MinimumXValue) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)MinimumXValue);
			popts.user_xmin = strtod(tmp, NULL);
			popts.user_xmin_given = 1;
		}
		else if (mystrncmp(inbuf, (char *)MaximumXValue) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)MaximumXValue);
			popts.user_xmax = strtod(tmp, NULL);
			popts.user_xmax_given = 1;
		}
		else if (mystrncmp(inbuf, (char *)MinimumYValue) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)MinimumYValue);
			popts.user_ymin = strtod(tmp, NULL);
			popts.user_ymin_given = 1;
		}
		else if (mystrncmp(inbuf, (char *)MaximumYValue) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)MaximumYValue);
			popts.user_ymax = strtod(tmp, NULL);
			popts.user_ymax_given = 1;
		}
		else if (mystrncmp(inbuf, (char *)XAxisPlotType) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)XAxisPlotType);
			if (strcmp(tmp, "LOG") == 0)
				popts.user_xplottype = LogarithmPlotState;
			else if (strcmp(tmp, "SQRT") == 0)
				popts.user_xplottype = SquareRootPlotState;
			else
				popts.user_xplottype = LinearPlotState;
		}
		else if (mystrncmp(inbuf, (char *)YAxisPlotType) == 0)
		{
			mystrncpy(tmp, inbuf, (char *)YAxisPlotType);
			if (strcmp(tmp, "LOG") == 0)
				popts.user_yplottype = LogarithmPlotState;
			else if (strcmp(tmp, "SQRT") == 0)
				popts.user_yplottype = SquareRootPlotState;
			else
				popts.user_yplottype = LinearPlotState;
		}
		else if (mystrncmp(inbuf, (char *)XAxisLabel) == 0)
		{
			mystrncpy(popts.xlabel, inbuf, (char *)XAxisLabel);
		}
		else if (mystrncmp(inbuf, (char *)YAxisLabel) == 0)
		{
			mystrncpy(popts.ylabel, inbuf, (char *)YAxisLabel);
		}
		else if (mystrncmp(inbuf, (char *)GraphTitle) == 0)
		{
			mystrncpy(popts.title, inbuf, (char *)GraphTitle);
		}
		else
		{
			// unknown option
			ERRORD("SKIPPING unknown option in graph.",
				inbuf, EINVAL);
		}
	}

	// check if out of data
	if (in.eof())
	{
		ERROR("unexpected EOF. no data read in.", EINVAL);
		return(NOTOK);
	}

	// allocate space for a 100 points, first.
	usrd.x = new double[usrd.allocpts];
	MustBeTrue(usrd.x != NULL);
	usrd.y = new double[usrd.allocpts];
	MustBeTrue(usrd.y != NULL);

	// now read in data
	for (usrd.npts = 0; ! in.eof(); )
	{
		// read in x,y point
		double tmpx, tmpy;
		in >> tmpx >> tmpy;
	
		// store x,y point
		if (usrd.npts >= usrd.allocpts)
		{
			// need to reallocate 
			usrd.allocpts *= 2;
			double *px = new double [usrd.allocpts];
			MustBeTrue(px != NULL);
			double *py = new double [usrd.allocpts];
			MustBeTrue(py != NULL);
			for (int ipt = 0; ipt < usrd.npts; ipt++)
			{
				px[ipt] = usrd.x[ipt];
				py[ipt] = usrd.y[ipt];
			}
			delete [] usrd.x;
			delete [] usrd.y;
			usrd.x = px;
			usrd.y = py;
		}
		usrd.x[usrd.npts] = tmpx;
		usrd.y[usrd.npts++] = tmpy;
	}

	// find min and max limits for x values.
	usrd.xmin = usrd.x[0];
	for (int ipt = 1; ipt < usrd.npts; ipt++)
	{
		if (usrd.x[ipt] < usrd.xmin)
			usrd.xmin =  usrd.x[ipt];
		if (usrd.x[ipt] > usrd.xmax)
			usrd.xmax =  usrd.x[ipt];
		if (usrd.y[ipt] < usrd.ymin)
			usrd.ymin =  usrd.y[ipt];
		if (usrd.y[ipt] > usrd.ymax)
			usrd.ymax =  usrd.y[ipt];
	}

	// set graphing limits
	if (popts.user_xmin_given)
		popts.graph_xmin = popts.user_xmin;
	else
		popts.graph_xmin = usrd.xmin;
	if (popts.user_xmax_given)
		popts.graph_xmax = popts.user_xmax;
	else
		popts.graph_xmax = usrd.xmax;
	if (popts.user_ymin_given)
		popts.graph_ymin = popts.user_ymin;
	else
		popts.graph_ymin = usrd.ymin;
	if (popts.user_ymax_given)
		popts.graph_ymax = popts.user_ymax;
	else
		popts.graph_ymax = usrd.ymax;

	// set plot types
	if (CheckPlotType(X_Axis_Plot_Type, popts.user_xplottype) == OK)
		popts.xplottype = popts.user_xplottype;
	else
		popts.xplottype = popts.user_xplottype = LinearPlotState;
	if (CheckPlotType(Y_Axis_Plot_Type, popts.user_yplottype) == OK)
		popts.yplottype = popts.user_yplottype;
	else
		popts.yplottype = popts.user_yplottype = LinearPlotState;

	// close file buffer
	// in.close();

	// all done
	return(OK);
}

int
ReadInData(int argc, char **argv, int nextarg)
{

	// where to get data?
	if (nextarg < argc)
	{
		ifstream in;
		in.open(argv[nextarg]);
		return(ReadInData(argc, argv, nextarg, in));
	}
	else
		return(ReadInData(argc, argv, nextarg, cin));

}

// open and close connections to display
int
ConnectToServer(char *display, Display *&xdisplay)
{
	// check display variable
	if (display == NULL || *display == 0)
	{
		display = getenv("DISPLAY");
	}

	// open x server
	if ((xdisplay = XOpenDisplay(display)) == NULL)
	{
		fprintf(stderr, "cannot connect to X server %s.\n",
			XDisplayName(display));
		return(NOTOK);
	}

	// all done
	return(OK);
}

void
DisconnectFromServer(Display *xdisplay)
{
	XCloseDisplay(xdisplay);
}

// get data for physical display
int
GetDisplayData(Display *xdisplay, int &xscreen, 
	unsigned int &display_width, unsigned int &display_height)
{
	// default physical screen attributes
	xscreen = DefaultScreen(xdisplay);
	display_width = DisplayWidth(xdisplay, xscreen);
	display_height = DisplayHeight(xdisplay, xscreen);

	// all done
	return(OK);
}

// find handler for a window
int (*find_handler(Window &w))(XEvent &)
{
	for (int iw = 0; iw < NumberOfWindows; iw++)
	{
		if (winfo[iw].window == w)
			return(winfo[iw].handler);
	}
	return(NULL);
}

// plot labels or data
int
PlotLabel(int il)
{
	// check window
	if (labels[il].window == 0)
		return(OK);

	// get direction for writing string
	int delx, dely;
	switch (labels[il].direction)
	{
	case Direction0:
		delx = 1;
		dely = 0;
		break;
	case Direction90:
		delx = 0;
		dely = -1;
		break;
	case Direction180:
		delx = -1;
		dely = 0;
		break;
	case Direction270:
		delx = 0;
		dely = 1;
		break;
	default:
		ERROR("invalid direction.", EINVAL);
		return(NOTOK);
	}

	// set foreground color
	XSetForeground(dd.display, dd.gc, dd.foreground);

	// write string on screen
	if (labels[il].direction == Direction0)
	{
		// just write string
		XDrawString(dd.display, labels[il].window, dd.gc,
			labels[il].x, labels[il].y, 
			labels[il].label, strlen(labels[il].label));
	}
	else
	{
		// write string one character at a time
		char tmpString[2];
		char *pstr = labels[il].label;
		int height = dd.font->ascent + dd.font->descent;
		int x = labels[il].x;
		int y = labels[il].y;
		for ( ; *pstr != 0; pstr++)
		{
			// save character
			tmpString[0] = *pstr;
			tmpString[1] = (char)0;

			// write string
			XDrawString(dd.display, labels[il].window, dd.gc, 
				x, y, tmpString, 1);

			// calculate the next location
			x += delx*XTextWidth(dd.font, tmpString, 1);
			y += dely*height;
		}
	}

	// flush buffers
	XFlush(dd.display);

	// all done
	return(OK);
}

int
PlotLabels(Window &window)
{
	for (int il = 0; il < NumberOfLabels; il++)
	{
		// skip labels for other windows
		if (labels[il].window != window)
			continue;

		// plot individual label
		int status = PlotLabel(il);
		if (status != OK) 
			return(status);
	}
	return(OK);
}

void
ResetData()
{
	// set graphing limits
	if (popts.user_xmin_given)
		popts.graph_xmin = popts.user_xmin;
	else
		popts.graph_xmin = usrd.xmin;
	if (popts.user_xmax_given)
		popts.graph_xmax = popts.user_xmax;
	else
		popts.graph_xmax = usrd.xmax;
	if (popts.user_ymin_given)
		popts.graph_ymin = popts.user_ymin;
	else
		popts.graph_ymin = usrd.ymin;
	if (popts.user_ymax_given)
		popts.graph_ymax = popts.user_ymax;
	else
		popts.graph_ymax = usrd.ymax;

	// set plot types
	popts.xplottype = popts.user_xplottype;
	popts.yplottype = popts.user_yplottype;

	// set cursor state
	popts.cursor_state = ZoomerState;
	popts.x_cross_hair = 0;
	popts.y_cross_hair = 0;

	// set number of labels
	popts.labelcount = 0;
	return;
}

void
PlotAllLabels()
{
	for (int il = 0; il < NumberOfLabels; il++)
	{
		// plot individual label
		PlotLabel(il);
	}
	return;
}

void
ClearAllWindows()
{
	XSetBackground(dd.display, dd.gc, dd.background);
	for (int iw = 0; iw < NumberOfWindows; iw++)
	{
		XClearWindow(dd.display, winfo[iw].window);
	}
	return;
}

// clear crosshair data
void
ClearXHairData()
{
	for (int il = 0; il < NumberOfXHairLabels; il++)
	{
		popts.energies[il] = 0;
		labels[NumberOfNonXHairLabels+il].label[0] = 0;
		labels[NumberOfNonXHairLabels+il].window = 0;
	}
	popts.labelcount = 0;
	return;
}

// plot graph data 
int
PlotGraph()
{
	// allocate arrays to hold point to plot
	double xptsmin, xptsmax, yptsmin, yptsmax;
	double *xpts = new double [usrd.npts];
	MustBeTrue(xpts != NULL);
	double *ypts = new double [usrd.npts];
	MustBeTrue(ypts != NULL);

	// calculate x,y logs, if necessary
	if (popts.xplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_xmin > 0.0);
		MustBeTrue(popts.graph_xmax > 0.0);
		xptsmin = log10(popts.graph_xmin);
		xptsmax = log10(popts.graph_xmax);
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			if (usrd.x[ipt] > 0.0)
				xpts[ipt] = log10(usrd.x[ipt]);
			else
				xpts[ipt] = xptsmin;
		}
	}
	else if (popts.xplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_xmin >= 0.0);
		MustBeTrue(popts.graph_xmax >= 0.0);
		xptsmin = sqrt(popts.graph_xmin);
		xptsmax = sqrt(popts.graph_xmax);
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			if (usrd.x[ipt] >= 0.0)
				xpts[ipt] = sqrt(usrd.x[ipt]);
			else
				xpts[ipt] = xptsmin;
		}
	}
	else
	{
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			xpts[ipt] = usrd.x[ipt];
		}
		xptsmin = popts.graph_xmin;
		xptsmax = popts.graph_xmax;

		// plot x-axis, if visible.
		if (xptsmin < 0.0 && 0.0 < xptsmax)
		{
			int x0 = (0.0-xptsmin)*winfo[Graph].width/(xptsmax-xptsmin);
			XDrawLine(dd.display, winfo[Graph].window, dd.gc, 
				x0, 0, x0, winfo[Graph].height);
			XFlush(dd.display);
		}
	}
	if (popts.yplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_ymin > 0.0);
		MustBeTrue(popts.graph_ymax > 0.0);
		yptsmin = log10(popts.graph_ymin);
		yptsmax = log10(popts.graph_ymax);
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			if (usrd.y[ipt] > 0.0)
				ypts[ipt] = log10(usrd.y[ipt]);
			else
				ypts[ipt] = yptsmin;
		}
	}
	else if (popts.yplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_ymin >= 0.0);
		MustBeTrue(popts.graph_ymax >= 0.0);
		yptsmin = sqrt(popts.graph_ymin);
		yptsmax = sqrt(popts.graph_ymax);
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			if (usrd.y[ipt] >= 0.0)
				ypts[ipt] = sqrt(usrd.y[ipt]);
			else
				ypts[ipt] = yptsmin;
		}
	}
	else
	{
		for (int ipt = 0; ipt < usrd.npts; ipt++)
		{
			ypts[ipt] = usrd.y[ipt];
		}
		yptsmin = popts.graph_ymin;
		yptsmax = popts.graph_ymax;

		// plot y-axis, if visible.
		if (yptsmin < 0.0 && 0.0 < yptsmax)
		{
			int y0 = winfo[Graph].height*(1.0-(0.0-yptsmin)/(yptsmax-yptsmin));
			XDrawLine(dd.display, winfo[Graph].window, dd.gc, 
				0, y0, winfo[Graph].width, y0);
			XFlush(dd.display);
		}
	}

	// draw lines
	for (int ipt = 0; ipt < (usrd.npts-1); ipt++)
	{
		// fix out-or-range points
		double xp1, yp1, xp2, yp2;
		if (xpts[ipt] < xptsmin)
			xp1 = xptsmin;
		else if (xpts[ipt] > xptsmax)
			xp1 = xptsmax;
		else
			xp1 = xpts[ipt];
		if (xpts[ipt+1] < xptsmin)
			xp2 = xptsmin;
		else if (xpts[ipt+1] > xptsmax)
			xp2 = xptsmax;
		else
			xp2 = xpts[ipt+1];
		if (ypts[ipt] < yptsmin)
			yp1 = yptsmin;
		else if (ypts[ipt] > yptsmax)
			yp1 = yptsmax;
		else
			yp1 = ypts[ipt];
		if (ypts[ipt+1] < yptsmin)
			yp2 = yptsmin;
		else if (ypts[ipt+1] > yptsmax)
			yp2 = yptsmax;
		else
			yp2 = ypts[ipt+1];

		// calculate coordinates
		int x1 = (xp1-xptsmin)*winfo[Graph].width/(xptsmax-xptsmin);
		int x2 = (xp2-xptsmin)*winfo[Graph].width/(xptsmax-xptsmin);
		int y1 = winfo[Graph].height*
				(1.0-(yp1-yptsmin)/(yptsmax-yptsmin));
		int y2 = winfo[Graph].height*
				(1.0-(yp2-yptsmin)/(yptsmax-yptsmin));

		// draw line
		XDrawLine(dd.display, winfo[Graph].window, dd.gc, 
			x1, y1, x2, y2);
		XFlush(dd.display);
	}

	// delete arrays
	delete [] xpts;
	delete [] ypts;

	// all done
	return(OK);
}

// get geometry data from user
int
GetGeometryData()
{
	// set default values of window
	winfo[Main].x = 1;
	winfo[Main].y = 1;
	winfo[Main].width = MinimumMainWidth;
	winfo[Main].height = MinimumMainHeight;

	// get geometry that user gave.
	if (xopts.geometry != NULL && *xopts.geometry != 0)
	{
		// user specified some geometry
		int user_x, user_y;	
		unsigned int user_width, user_height;
		int status = XParseGeometry(xopts.geometry, 
					&user_x, &user_y, 
					&user_width, &user_height);

		if (status & WidthValue && user_width > winfo[Main].width) 
			winfo[Main].width = user_width;
		if (status & HeightValue && user_height > winfo[Main].height) 
			winfo[Main].height = user_height;
		if (status & XValue) winfo[Main].x = user_x;
		if (status & YValue) winfo[Main].y = user_y;
		if (status & XNegative) winfo[Main].x = dd.display_width - 
					winfo[Main].width + winfo[Main].x;
		if (status & YNegative) winfo[Main].y = dd.display_height -
					winfo[Main].height + winfo[Main].y;
	}

	// all done
	return(OK);
}

// get sum of y counts for spectrum
double
GetIntegral()
{
	double ysum = 0.0;
	for (int ipt = 0; ipt < usrd.npts; ipt++)
	{
		if ((popts.graph_xmin <= usrd.x[ipt]) &&
		    (usrd.x[ipt] <= popts.graph_xmax))
		{
			ysum += usrd.y[ipt];
		}
	}
	return(ysum);
}

// get weighted mean for x
double
GetWeightedMean()
{
	double xwmean = 0.0;
	double ysum = GetIntegral();
	if (ysum == 0.0) ysum = 1.0;
	for (int ipt = 0; ipt < usrd.npts; ipt++)
	{
		if ((popts.graph_xmin <= usrd.x[ipt]) &&
		    (usrd.x[ipt] <= popts.graph_xmax))
		{
			xwmean += usrd.x[ipt]*usrd.y[ipt]/ysum;
		}
	}
	return(xwmean);
}

// get size and location of all windows
int
GetWindowData()
{
	// maximum character widths and heights
	int max_char_width = 
		dd.font->max_bounds.width;
	int max_char_height = 
		dd.font->max_bounds.ascent + dd.font->max_bounds.descent;

	// maximum label width
	int max_label_width = 0;
	int il;
	for (il = 0; il < NumberOfFixedLabels; il++)
	{
		if (strlen(labels[il].label) > max_label_width)
		{
			max_label_width = strlen(labels[il].label);
		}
	}
	max_label_width *= max_char_width;

	// maximum button window width
	int max_window_width = MaxCharsInWindow*max_char_width;

	// add spacing
	max_window_width += 2*HorizontalSpace;
	max_label_width += 2*HorizontalSpace;
	int max_height = max_char_height + 2*VerticalSpace;

	// fixed label info 
	for (il = 0; il < NumberOfFixedLabels; il++)
	{
		labels[il].x = Space;
		labels[il].y = (il+1)*Space + (il+1)*max_height;
		labels[il].width = max_label_width;
		labels[il].height = max_height;
		labels[il].window = winfo[Main].window;
	}

	// fixed label window info
	for (il = 0; il < NumberOfFixedLabels; il++)
	{
		winfo[il].x = 2*Space + max_label_width;
		winfo[il].y = (il+1)*Space + il*max_height;
		winfo[il].width = max_window_width;
		winfo[il].height = max_height;
	}

	// x plot type window label
	if (popts.xplottype == LogarithmPlotState)
		strcpy(labels[il].label, "LOGARITHMIC");
	else if (popts.xplottype == SquareRootPlotState)
		strcpy(labels[il].label, "SQUARE ROOT");
	else
		strcpy(labels[il].label, "LINEAR");
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[X_Axis_Plot_Type].window;

	// y plot type window label
	if (popts.yplottype == LogarithmPlotState)
		strcpy(labels[il].label, "LOGARITHMIC");
	else if (popts.yplottype == SquareRootPlotState)
		strcpy(labels[il].label, "SQUARE ROOT");
	else
		strcpy(labels[il].label, "LINEAR");
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[Y_Axis_Plot_Type].window;

	// x-hair/zoom window label
	if (popts.cursor_state == CrossHairWithLabelsState)
		strcpy(labels[il].label, "X-HAIR LBLS");
	else if (popts.cursor_state == CrossHairState)
		strcpy(labels[il].label, "CROSS HAIR");
	else
		strcpy(labels[il].label, "ZOOM");
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[Cursor_Mode].window;

	// x cross-hair window label
	if ((popts.cursor_state == CrossHairState) ||
	    (popts.cursor_state == CrossHairWithLabelsState))
		sprintf(labels[il].label, "%8lg", popts.x_cross_hair);
	else
		strcpy(labels[il].label, "");
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[X_Cross_Hair].window;

	// y cross-hair window label
	if ((popts.cursor_state == CrossHairState) ||
	    (popts.cursor_state == CrossHairWithLabelsState))
		sprintf(labels[il].label, "%8lg", popts.y_cross_hair);
	else
		strcpy(labels[il].label, "");
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[Y_Cross_Hair].window;

	// y sum window label
	popts.ysum = GetIntegral();
	sprintf(labels[il].label, "%8lg", popts.ysum);
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[Integral].window;

	// x weighted-average window
	popts.xwmean = GetWeightedMean();
	sprintf(labels[il].label, "%8lg", popts.xwmean);
	labels[il].x = HorizontalSpace;
	labels[il].y = VerticalSpace+dd.font->max_bounds.ascent;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].reverse = 1;
	labels[il++].window = winfo[WeightedMean].window;

	// get width of format statement
	int max_format_width = MaxCharsInFormat*max_char_width;

	// get minimum height required for all buttons
	int min_window_height = 
		(NumberOfButtons+1)*Space + NumberOfButtons*max_height;

	// get coordinates to origins
	popts.xorig = 4*Space + max_label_width + 
			max_window_width + max_format_width;
	popts.yorig = winfo[Main].height - 3*max_char_height;

	// graph x,y coordinates
	winfo[Graph].x = popts.xorig;
	winfo[Graph].y = 0;

	// get graph width and height
	winfo[Graph].width = winfo[Main].width - popts.xorig;
	winfo[Graph].height = popts.yorig;
	
#if 0
	// get x,y coordinate for root window
	winfo[Main].x = (dd.display_width - winfo[Main].width)/3;
	if (winfo[Main].x < 0) winfo[Main].x = 0;
	winfo[Main].y = (dd.display_height - winfo[Main].height)/3;
	if (winfo[Main].y < 0) winfo[Main].y = 0;
#endif

	// y max window label
	sprintf(labels[il].label, "%8lg", popts.graph_ymax);
	labels[il].x = popts.xorig - (strlen(labels[il].label)+1)*max_char_width;
	labels[il].y = max_char_height;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// y min window label
	sprintf(labels[il].label, "%8lg", popts.graph_ymin);
	labels[il].x = popts.xorig - (strlen(labels[il].label)+1)*max_char_width;
	labels[il].y = popts.yorig;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// x max window label
	sprintf(labels[il].label, "%8lg", popts.graph_xmax);
	labels[il].x = 
		winfo[Main].width - (strlen(labels[il].label)+1)*max_char_width;
	labels[il].y = popts.yorig + max_char_height;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// x min window label
	sprintf(labels[il].label, "%8lg", popts.graph_xmin);
	labels[il].x = popts.xorig;
	labels[il].y = popts.yorig + max_char_height;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// y-axis label
	if (popts.yplottype == LogarithmPlotState)
		sprintf(labels[il].label, "%s (LOG)", popts.ylabel);
	else if (popts.yplottype == SquareRootPlotState)
		sprintf(labels[il].label, "%s (SQRT)", popts.ylabel);
	else
		sprintf(labels[il].label, "%s (LINEAR)", popts.ylabel);
	labels[il].x = popts.xorig - 2*max_char_width;
	labels[il].y = 
		popts.yorig/2 - strlen(labels[il].label)*max_char_height/2;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il].direction = Direction270;
	labels[il++].window = winfo[Main].window;

	// x-axis label
	if (popts.xplottype == LogarithmPlotState)
		sprintf(labels[il].label, "%s (LOG)", popts.xlabel);
	else if (popts.xplottype == SquareRootPlotState)
		sprintf(labels[il].label, "%s (SQRT)", popts.xlabel);
	else
		sprintf(labels[il].label, "%s (LINEAR)", popts.xlabel);
	labels[il].x = popts.xorig + winfo[Graph].width/2 - 
			strlen(labels[il].label)*max_char_width/2;
	labels[il].y = popts.yorig + 2*max_char_height;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// graph title
	strcpy(labels[il].label, popts.title);
	labels[il].x = popts.xorig + winfo[Graph].width/2 - 
			strlen(labels[il].label)*max_char_width/2;
	labels[il].y = popts.yorig + 3*max_char_height;
	labels[il].width = max_window_width;
	labels[il].height = max_height;
	labels[il++].window = winfo[Main].window;

	// all done
	return(OK);
}

// draw a box
void 
DrawBox(Display *display, Drawable drawable, GC gc, 
	int x1, int y1, int x2, int y2)
{
	int width = x2 - x1;
	if (width < 0)
	{
		width = -width;
		x1 = x2;
	}
	int height = y2 - y1;
	if (height < 0)
	{
		height = -height;
		y1 = y2;
	}
	XDrawRectangle(display, drawable, gc, x1, y1, width, height);
	return;
}

// update limits from rubberband box
void
UpdateLimits(int px1, int py1, int px2, int py2)
{
	// make sure point order is correct
	if (px2 < px1)
	{
		int tmpx = px1;
		px1 = px2;
		px2 = tmpx;
	}
	if (py2 < py1)
	{
		int tmpy = py1;
		py1 = py2;
		py2 = tmpy;
	}

	// local values
	int ph = winfo[Graph].height;
	int pw = winfo[Graph].width;

	// check ranges
	if (px1 < 0)
		px1 = 0;
	else if (px1 > pw)
		px1 = pw;
	if (px2 < 0)
		px2 = 0;
	else if (px2 > pw)
		px2 = pw;
	if (py1 < 0)
		py1 = 0;
	else if (py1 > ph)
		py1 = ph;
	if (py2 < 0)
		py2 = 0;
	else if (py2 > ph)
		py2 = ph;

	// calculate new graph limits
	if (popts.xplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_xmin > 0.0 && popts.graph_xmax > 0.0);
		double xmin = log10(popts.graph_xmin);
		double xmax = log10(popts.graph_xmax);
		double x1 = (xmax-xmin)*px1/pw+xmin;
		double x2 = (xmax-xmin)*px2/pw+xmin;
		popts.graph_xmin = pow(10.0, x1);
		popts.graph_xmax = pow(10.0, x2);
	}
	else if (popts.xplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_xmin >= 0.0 && popts.graph_xmax >= 0.0);
		double xmin = sqrt(popts.graph_xmin);
		double xmax = sqrt(popts.graph_xmax);
		double x1 = (xmax-xmin)*px1/pw+xmin;
		double x2 = (xmax-xmin)*px2/pw+xmin;
		popts.graph_xmin = x1*x1;
		popts.graph_xmax = x2*x2;
	}
	else
	{
		double xmin = popts.graph_xmin;
		double xmax = popts.graph_xmax;
		double x1 = (xmax-xmin)*px1/pw+xmin;
		double x2 = (xmax-xmin)*px2/pw+xmin;
		popts.graph_xmin = x1;
		popts.graph_xmax = x2;
	}
	if (popts.yplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_ymin > 0.0 && popts.graph_ymax > 0.0);
		double ymin = log10(popts.graph_ymin);
		double ymax = log10(popts.graph_ymax);
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		double y2 = (py2-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.graph_ymin = pow(10.0, y2);
		popts.graph_ymax = pow(10.0, y1);
	}
	else if (popts.xplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_xmin >= 0.0 && popts.graph_xmax >= 0.0);
		double ymin = sqrt(popts.graph_ymin);
		double ymax = sqrt(popts.graph_ymax);
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		double y2 = (py2-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.graph_ymin = y2*y2;
		popts.graph_ymax = y1*y1;
	}
	else
	{
		double ymin = popts.graph_ymin;
		double ymax = popts.graph_ymax;
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		double y2 = (py2-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.graph_ymin = y2;
		popts.graph_ymax = y1;
	}

	// all done
	return;
}

void
UpdateLimits(int px1, int py1)
{
	// local values
	int ph = winfo[Graph].height;
	int pw = winfo[Graph].width;

	// check ranges
	if (px1 < 0)
		px1 = 0;
	else if (px1 > pw)
		px1 = pw;
	if (py1 < 0)
		py1 = 0;
	else if (py1 > ph)
		py1 = ph;

	// calculate new graph limits
	if (popts.xplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_xmin > 0.0 && popts.graph_xmax > 0.0);
		double xmin = log10(popts.graph_xmin);
		double xmax = log10(popts.graph_xmax);
		double x1 = (xmax-xmin)*px1/pw+xmin;
		popts.x_cross_hair = pow(10.0, x1);
	}
	else if (popts.xplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_xmin >= 0.0 && popts.graph_xmax >= 0.0);
		double xmin = sqrt(popts.graph_xmin);
		double xmax = sqrt(popts.graph_xmax);
		double x1 = (xmax-xmin)*px1/pw+xmin;
		popts.x_cross_hair = x1*x1;
	}
	else
	{
		double xmin = popts.graph_xmin;
		double xmax = popts.graph_xmax;
		double x1 = (xmax-xmin)*px1/pw+xmin;
		popts.x_cross_hair = x1;
	}
	if (popts.yplottype == LogarithmPlotState)
	{
		MustBeTrue(popts.graph_ymin > 0.0 && popts.graph_ymax > 0.0);
		double ymin = log10(popts.graph_ymin);
		double ymax = log10(popts.graph_ymax);
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.y_cross_hair = pow(10.0, y1);
	}
	else if (popts.xplottype == SquareRootPlotState)
	{
		MustBeTrue(popts.graph_xmin >= 0.0 && popts.graph_xmax >= 0.0);
		double ymin = sqrt(popts.graph_ymin);
		double ymax = sqrt(popts.graph_ymax);
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.y_cross_hair = y1*y1;
	}
	else
	{
		double ymin = popts.graph_ymin;
		double ymax = popts.graph_ymax;
		double y1 = (py1-ph)*(ymax-ymin)/(-ph)+ymin;
		popts.y_cross_hair = y1;
	}

	// store label
	if ((popts.cursor_state == CrossHairWithLabelsState) &&
	    (popts.labelcount < NumberOfXHairLabels))
	{
		int il = popts.labelcount++;
		popts.energies[il] = popts.x_cross_hair;
		il += NumberOfNonXHairLabels;
		sprintf(labels[il].label, "%8lg", popts.x_cross_hair);
#if 0
		labels[il].x = px1;
#else
		labels[il].x = px1 -
			XTextWidth(dd.font, labels[il].label, 
				strlen(labels[il].label))/2;
		if (labels[il].x < 0) labels[il].x = px1;
#endif
		labels[il].y = py1;
		labels[il].width = 0;
		labels[il].height = 0;
		labels[il].direction = Direction0;
		labels[il].window = winfo[Graph].window;
	}

	// all done
	return;
}

// event handler functions for windows
int
x_plot_type(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore for now
		break;

	case ButtonPress:
		// advance plot type
		switch (popts.xplottype)
		{
		case LinearPlotState:
			if (CheckPlotType(X_Axis_Plot_Type, 
				LogarithmPlotState) == OK)
				popts.xplottype = LogarithmPlotState;
			else if (CheckPlotType(X_Axis_Plot_Type, 
				SquareRootPlotState) == OK)
				popts.xplottype = SquareRootPlotState;
			break;

		case LogarithmPlotState:
			if (CheckPlotType(X_Axis_Plot_Type, 
				SquareRootPlotState) == OK)
				popts.xplottype = SquareRootPlotState;
			break;

		case SquareRootPlotState:
		default:
			popts.xplottype = LinearPlotState;
			break;
		}

		// get new window data and plot
		ClearXHairData();
		GetWindowData();
		ClearAllWindows();
		XFlush(dd.display);
		PlotAllLabels();
		XFlush(dd.display);
		PlotGraph();
		XFlush(dd.display);
		break;
	}
	return(OK);
}

int
y_plot_type(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore for now
		break;

	case ButtonPress:
		// advance plot type
		switch (popts.yplottype)
		{
		case LinearPlotState:
			if (CheckPlotType(Y_Axis_Plot_Type, 
				LogarithmPlotState) == OK)
				popts.yplottype = LogarithmPlotState;
			else if (CheckPlotType(Y_Axis_Plot_Type, 
				SquareRootPlotState) == OK)
				popts.yplottype = SquareRootPlotState;
			break;

		case LogarithmPlotState:
			if (CheckPlotType(Y_Axis_Plot_Type, 
				SquareRootPlotState) == OK)
				popts.yplottype = SquareRootPlotState;
			break;

		case SquareRootPlotState:
		default:
			popts.yplottype = LinearPlotState;
			break;
		}

		// get new window data and plot
		ClearXHairData();
		GetWindowData();
		ClearAllWindows();
		XFlush(dd.display);
		PlotAllLabels();
		XFlush(dd.display);
		PlotGraph();
		XFlush(dd.display);
		break;
	}
	return(OK);
}

int
reset(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// remove high light button
		XSetForeground(dd.display, dd.gc, dd.background);
		XFillRectangle(dd.display, winfo[Reset].window, dd.gc,
				0, 0, winfo[Reset].width, winfo[Reset].height);
		XSetForeground(dd.display, dd.gc, dd.foreground);
		XFlush(dd.display);

		// reset to default and plot
		ResetData();
		ClearXHairData();
		GetWindowData();
		ClearAllWindows();
		XFlush(dd.display);
		PlotAllLabels();
		XFlush(dd.display);
		PlotGraph();
		XFlush(dd.display);
		break;

	case ButtonPress:
		// high light button
		XSetForeground(dd.display, dd.gc, dd.foreground);
		XFillRectangle(dd.display, winfo[Reset].window, dd.gc,
				0, 0, winfo[Reset].width, winfo[Reset].height);
		XFlush(dd.display);
		break;
	}
	return(OK);
}

int
cursor_mode(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		break;

	case ButtonPress:
		// advance cursor state
		popts.labelcount = 0;
		if (popts.cursor_state == CrossHairState)
			popts.cursor_state = CrossHairWithLabelsState;
		else if (popts.cursor_state == CrossHairWithLabelsState)
			popts.cursor_state = ZoomerState;
		else if (popts.cursor_state == ZoomerState)
			popts.cursor_state = CrossHairState;
		else
			popts.cursor_state = ZoomerState;

		// update labels and windows.
		ClearXHairData();
		GetWindowData();
		ClearAllWindows();
		XFlush(dd.display);
		PlotAllLabels();
		XFlush(dd.display);
		PlotGraph();
		XFlush(dd.display);
		break;
	}
	return(OK);
}

int
x_cross_hairs(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore
		break;

	case ButtonPress:
		// ignore
		break;
	}
	return(OK);
}

int
y_cross_hairs(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore
		break;

	case ButtonPress:
		// ignore
		break;
	}
	return(OK);
}

int
total_counts(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore
		break;

	case ButtonPress:
		// ignore
		break;
	}
	return(OK);
}

int
weighted_mean(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// ignore
		break;

	case ButtonPress:
		// ignore
		break;
	}
	return(OK);
}

int
quit_handler(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;

	case ButtonRelease:
		// remove high light button
		XSetForeground(dd.display, dd.gc, dd.background);
		XFillRectangle(dd.display, winfo[Quit].window, dd.gc,
				0, 0, winfo[Quit].width, winfo[Quit].height);
		XSetForeground(dd.display, dd.gc, dd.foreground);
		XFlush(dd.display);

		// set quit flag
		done = 1;
		break;

	case ButtonPress:
		// high light button
		XSetForeground(dd.display, dd.gc, dd.foreground);
		XFillRectangle(dd.display, winfo[Quit].window, dd.gc,
				0, 0, winfo[Quit].width, winfo[Quit].height);
		XFlush(dd.display);
		break;
	}
	return(OK);
}

int
graph_handler(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
		{
			PlotLabels(xev.xexpose.window);
			XFlush(dd.display);
			PlotGraph();
			XFlush(dd.display);
		}
		break;

	case ButtonPress:
		if (popts.cursor_state == ZoomerState)
		{
			// draw rubber band box
			if (xev.xbutton.button == Button1)
			{
				rbd.start_x = xev.xbutton.x;
				rbd.start_y = xev.xbutton.y;
				rbd.last_x = rbd.start_x;
				rbd.last_y = rbd.start_y;
				DrawBox(dd.display, winfo[Graph].window, 
					dd.xor_gc,
					rbd.start_x, rbd.start_y, 
					rbd.last_x, rbd.last_y);
				XFlush(dd.display);
			}
		}
		else if ((popts.cursor_state == CrossHairState) ||
			 (popts.cursor_state == CrossHairWithLabelsState))
		{
			// do nothing
		}
		break;

	case ButtonRelease:
		if (popts.cursor_state == ZoomerState)
		{
			// rescale the graph
			if (rbd.start_x != rbd.last_x && 
			    rbd.start_y != rbd.last_y)
			{
				DrawBox(dd.display, winfo[Graph].window, 
					dd.xor_gc,
					rbd.start_x, rbd.start_y, 
					rbd.last_x, rbd.last_y);
				XFlush(dd.display);
				UpdateLimits(rbd.start_x, rbd.start_y,
						rbd.last_x, rbd.last_y);
				GetWindowData();
				ClearAllWindows();
				XFlush(dd.display);
				PlotAllLabels();
				XFlush(dd.display);
				PlotGraph();
				XFlush(dd.display);
			}
			rbd.start_x = -1;
			rbd.start_y = -1;
			rbd.last_x = -1;
			rbd.last_y = -1;
		}
		else if ((popts.cursor_state == CrossHairState) ||
			 (popts.cursor_state == CrossHairWithLabelsState))
		{
			XDrawLine(dd.display, winfo[Graph].window,
				dd.xor_gc,
				xhd.last_x, 0, 
				xhd.last_x, winfo[Graph].height);
			XDrawLine(dd.display, winfo[Graph].window,
				dd.xor_gc,
				0, xhd.last_y, 
				winfo[Graph].width, xhd.last_y);
			UpdateLimits(xhd.last_x, xhd.last_y);
			xhd.last_x = -1;
			xhd.last_y = -1;
			GetWindowData();
			ClearAllWindows();
			XFlush(dd.display);
			PlotAllLabels();
			XFlush(dd.display);
			PlotGraph();
			XFlush(dd.display);
		}
		break;

	case MotionNotify:
		if (popts.cursor_state == ZoomerState)
		{
			// draw rubber band box
			if (xev.xbutton.button == Button1)
			{
				DrawBox(dd.display, winfo[Graph].window, 
					dd.xor_gc,
					rbd.start_x, rbd.start_y, 
					rbd.last_x, rbd.last_y);
				rbd.last_x = xev.xmotion.x;
				rbd.last_y = xev.xmotion.y;
				DrawBox(dd.display, winfo[Graph].window, 
					dd.xor_gc,
					rbd.start_x, rbd.start_y, 
					rbd.last_x, rbd.last_y);
				XFlush(dd.display);
			}
		}
		else if ((popts.cursor_state == CrossHairState) ||
			 (popts.cursor_state == CrossHairWithLabelsState))
		{
			if (xhd.last_x != -1 && xhd.last_y != -1)
			{
				XDrawLine(dd.display, winfo[Graph].window,
					dd.xor_gc,
					xhd.last_x, 0, 
					xhd.last_x, winfo[Graph].height);
				XDrawLine(dd.display, winfo[Graph].window,
					dd.xor_gc,
					0, xhd.last_y, 
					winfo[Graph].width, xhd.last_y);
			}
			xhd.last_x = xev.xmotion.x;
			xhd.last_y = xev.xmotion.y;
			XDrawLine(dd.display, winfo[Graph].window,
				dd.xor_gc,
				xhd.last_x, 0, 
				xhd.last_x, winfo[Graph].height);
			XDrawLine(dd.display, winfo[Graph].window,
				dd.xor_gc,
				0, xhd.last_y, 
				winfo[Graph].width, xhd.last_y);
		}
		break;
	}
	return(OK);
}

int
main_handler(XEvent &xev)
{
	switch (xev.xany.type)
	{
	case Expose:
		if (xev.xexpose.count == 0)
			PlotLabels(xev.xexpose.window);
		break;
	}
	return(OK);
}

// load up a font
XFontStruct *
LoadFont(Display *display, char *font_name, char *fallback_font_name)
{
	// load up a font
	XFontStruct *font_struct = XLoadQueryFont(display, font_name);
	if (font_struct == NULL)
	{
		font_struct = XLoadQueryFont(display, fallback_font_name);
		if (font_struct == NULL)
		{
			fprintf(stderr, "unable to load font.", 
				font_name);
			fprintf(stderr, "unable to load font.", 
				fallback_font_name);
			return(NULL);
		}
	}

	// all done
	return(font_struct);
}

// set window size and location hints
int
SetSizeHints(Display *display, Window window, 
		int x, int y, int width, int height)
{
	XSizeHints sizehints;

	// fill size hints
	sizehints.x = x;
	sizehints.y = y;
	sizehints.height = height;
	sizehints.width = width;
	sizehints.min_height = height;
	sizehints.min_width = width;
	sizehints.flags = USPosition | USSize | PMinSize;
	sizehints.base_width = width;
	sizehints.base_height = height;
	sizehints.flags |= PBaseSize;

	// set size hints 
	XSetWMNormalHints(display, window, &sizehints);

	// all done
	return(OK);
}

// set window name using text properties
int
SetWindowName(Display *display, Window window, char *name)
{
	XTextProperty text_property;

	// convert character string to text property
	(void) XStringListToTextProperty(&name, 1, &text_property);

	// set window name
	(void) XSetWMName(display, window, &text_property);

	// all done
	return(OK);
}

// set class hints
int
SetClassHints(Display *display, Window window, char *res_name, char *res_class)
{
	XClassHint class_hints;

	// fill in class hints
	class_hints.res_class = res_class;
	class_hints.res_name = res_name;

	// set values
	XSetClassHint(display, window, &class_hints);

	// all done
	return(OK);
}

//
// set window manager hints. the flag initial state indicates
// whether we want the window to be iconized or not. the 
// possible values are: WithDrawnState (0), NormalState(1), 
// and IconicState(3).
//
int
SetWMHints(Display *display, Window window, int initial_state)
{
	XWMHints wm_hints;

	// which flags are filled in
	wm_hints.flags = InputHint | StateHint;

	// start as icon or window
	wm_hints.initial_state = initial_state;

	// we want input
	wm_hints.input = True;

	// set values
	XSetWMHints(display, window, &wm_hints);

	// all done */
	return(OK);
}

// set up visual info
int
GetPseudoColorVisual(Display *display, int screen, Visual *&visual, int &depth)
{
	XVisualInfo *visual_array;
	XVisualInfo visual_info_template;
	int status = NOTOK;
	int number_visuals;

	// look for a pseudo-color visual
	if (DefaultVisual(display, screen)->c_class == PseudoColor)
	{
		// found a pseudo-color visual
		visual = DefaultVisual(display, screen);
		depth = DefaultDepth(display, screen);
		status = OK;
	}
	else
	{
		// looking for a pseudo-color visual
		visual_info_template.c_class = PseudoColor;
		visual_info_template.screen = screen;
		visual_array = XGetVisualInfo(display, 
			(VisualClassMask | VisualScreenMask),
			&visual_info_template, &number_visuals);
		if ((number_visuals > 0) && (visual_array != NULL))
		{
			visual = visual_array[0].visual;
			depth = visual_array[0].depth;
			XFree(visual_array);
			status = OK;
		}
		else
		{
			visual = CopyFromParent;
			status = NOTOK;
		}
	}

	// all done
	return(status);
}

// open a window
Window
CreateWindow(Display *display, Window parent, 
	int x, int y, int width, int height,
	unsigned long bordercolor, unsigned long backcolor,
	unsigned long event_mask, Visual *visual,
	unsigned int border_width, int depth)
{
	Window window;
	XSetWindowAttributes attributes;
	unsigned long attr_mask;

	// set up window attributes 
	attributes.event_mask = event_mask;
	attributes.border_pixel = bordercolor;
	attributes.background_pixel = backcolor;
	attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;

	// create window
	window = XCreateWindow(display, parent, x, y, width, height,
			border_width, depth, InputOutput,
			visual, attr_mask, &attributes);

	// all done 
	return(window);
}

// decode a key-press event
int
DecodeKeyPress(XKeyEvent *event, KeySym *keysym, char *string)
{
	int length;

	// initialize
	*keysym = 0;
	*string = 0;

	// verify the event
	if ((event->type != KeyPress) && (event->type == KeyRelease))
	{
		return(0);
	}

	// convert event to a string
	length =  XLookupString(event, string, 1, keysym, NULL);
	if (length > 0)
		string[1] = 0;
	else
		string[0] = 0;
	
	// all done
	return(strcmp(string, "q") == 0);
}

// set up color map for a screen
int
SetUpColorMap(Display *display, int screen, 
	Window window, Visual *visual, Colormap &colormap)
{
	int status = NOTOK;

	// look for a pseudo-color visual
	if (visual == DefaultVisual(display, screen))
	{
		colormap = DefaultColormap(display, screen);
		status = OK;
	}
	else
	{
		if (visual == CopyFromParent) 
			visual = DefaultVisual(display, screen);
		colormap = XCreateColormap(display, window, visual, AllocNone);
		if (colormap != None)
		{
			XSetWindowColormap(display, window, colormap);
			status = OK;
		}
		else
		{
			colormap = DefaultColormap(display, screen);
		}
	}

	// all done 
	return(status);
}

// set up color map for a screen
unsigned long
AllocateColorByName(Display *display, Colormap colormap, char *colorname, unsigned long default_color)
{
	XColor hardwarecolor, exactcolor;
	unsigned long color;
	int status;

	// look for given color
	status = XAllocNamedColor(display, colormap, colorname,
			&hardwarecolor, &exactcolor);
	if (status != 0)
	{
		color = hardwarecolor.pixel;
	}
	else
	{
		color = default_color;
	}

	/* all done */
	return(color);
}

// create a graphics context
GC
CreateGC(Display *display, Drawable drawable, XFontStruct *font, 
	unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor;
	xgcvalues.background = backcolor;

	// set values
	gc = XCreateGC(display, drawable, (GCForeground | GCBackground),
			&xgcvalues);

	// set up font in GC
	XSetFont(display, gc, font->fid);

	// all done
	return(gc);
}

// create a GC for xor-drawing
GC
CreateXorGC(Display *display, Drawable drawable, XFontStruct *font,
	unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor ^ backcolor;
	xgcvalues.background = 0;
	xgcvalues.function = GXxor;

	// set values
	gc = XCreateGC(display, drawable, 
			(GCFunction | GCForeground | GCBackground),
			&xgcvalues);

	// set up font in GC
	XSetFont(display, gc, font->fid);

	// all done
	return(gc);
}

// my X-error handler
extern "C" 
int X_Error_Handler(Display *d, XErrorEvent *e)
{
	char emsg[10*1024];
	memset((void *)emsg, 0, 10*1024);
	XGetErrorText(d, e->error_code, emsg, 10*1024);
	cerr << endl << "X-ERROR-EVENT DUMP:" << endl;
	cerr << "Type: " << e->type << endl;
	cerr << "Resource ID: " << e->resourceid << endl;
	cerr << "Serial: " << e->serial << endl;
	cerr << "Error Code: " << (unsigned long)e->error_code << endl;
	cerr << "Error Code Message: " << emsg << endl;
	cerr << "Request Code: " << (unsigned long)e->request_code << endl;
	cerr << "Minor Code: " << (unsigned long)e->minor_code << endl;
	exit(2);
	return(0);
}

// main entry point
main(int argc, char **argv)
{
	// error handling
	// XSetErrorHandler(X_Error_Handler);

	// get command line options
	int nextarg;
	if (CommandLineOptions(argc, argv, nextarg) != OK)
	{
		ERRORD("invalid command line option.", argv[nextarg], EINVAL);
		return(2);
	}

	// get data 
	if (ReadInData(argc, argv, nextarg) != OK)
	{
		ERROR("unable to get data.", errno);
		return(2);
	}

	// connect to server
	if (ConnectToServer(xopts.display, dd.display) != OK)
	{
		ERRORD("unable to open display to server.",
			XDisplayName(xopts.display), errno);
		return(2);
	}

	// get physical screen attributes
	if (GetDisplayData(dd.display, dd.screen, 
			dd.display_width, dd.display_height) != OK)
	{
		ERROR("unable to get display attributes.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// get font for writing
	dd.font = LoadFont(dd.display, xopts.font, (char *)"fixed");
	if (dd.font == NULL)
	{
		ERROR("unable to load font.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// get geometry data
	if (GetGeometryData() != OK)
	{
		ERROR("unable to get geometry data.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// determine window sizes and locations
	if (GetWindowData() != OK)
	{
		ERROR("unable to get window data.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// default black and white pixels
	dd.black = BlackPixel(dd.display, dd.screen);
	dd.white = WhitePixel(dd.display, dd.screen);

	// get pseudo-color display, if any
#if 0
	if (GetPseudoColorVisual(dd.display, dd.screen, 
			dd.visual, dd.depth) != OK)
	{
		ERROR("unable to get a color visual.", errno);
		//DisconnectFromServer(dd.display);
		//return(2);
	}
#else
	(void)GetPseudoColorVisual(dd.display, 
		dd.screen, dd.visual, dd.depth);
#endif

	// create a window
	unsigned int border_width = 2;
	dd.root_window = RootWindow(dd.display, dd.screen);
dd.visual=CopyFromParent;
dd.depth=CopyFromParent;
	winfo[Main].window = CreateWindow(
		dd.display, dd.root_window, 
		winfo[Main].x, winfo[Main].y, 
		winfo[Main].width, winfo[Main].height, 
		dd.black, dd.white, MainEventMask,
		dd.visual, border_width, dd.depth);

	// set size hints
	if (SetSizeHints(dd.display, winfo[Main].window, 
		winfo[Main].x, winfo[Main].y, 
		winfo[Main].width, winfo[Main].height) != OK)
	{
		ERROR("unable to set window size hints.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// set window name
	if (SetWindowName(dd.display, winfo[Main].window, xopts.title) != OK)
	{
		ERROR("unable to set window title.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// set class hints
	if (SetClassHints(dd.display, 
		winfo[Main].window, argv[0], argv[0]) != OK)
	{
		ERROR("unable to set class hints.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// set other window manager hints
	if (SetWMHints(dd.display, winfo[Main].window, xopts.iconic) != OK)
	{
		ERROR("unable to set WM hints.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// set up color map for window
	if (SetUpColorMap(dd.display, dd.screen, 
		winfo[Main].window, dd.visual, dd.colormap) != OK)
	{
		ERROR("unable to set up a color map.", errno);
		DisconnectFromServer(dd.display);
		return(2);
	}

	// allocate colors for background and foreground
	dd.background = 
		AllocateColorByName(dd.display, dd.colormap, 
				xopts.background, dd.white);
	dd.foreground = 
		AllocateColorByName(dd.display, dd.colormap, 
				xopts.foreground, dd.black);

	// set main window border color and background 
	XSetWindowBorder(dd.display, winfo[Main].window, dd.foreground);
	XSetWindowBackground(dd.display, winfo[Main].window, dd.background);

	// create GC for drawing
	dd.gc = CreateGC(dd.display, winfo[Main].window, dd.font, 
			dd.foreground, dd.background);
	dd.xor_gc = CreateXorGC(dd.display, winfo[Main].window, dd.font, 
			dd.foreground, dd.background);

	// create button windows
	unsigned int button_border_width = 2;
	for (int ib = 0; ib < NumberOfButtons; ib++)
	{
		winfo[ib].window = CreateWindow(
			dd.display, winfo[Main].window, 
			winfo[ib].x, winfo[ib].y, 
			winfo[ib].width, winfo[ib].height, 
			dd.foreground, dd.background, ButtonEventMask,
			CopyFromParent, button_border_width, CopyFromParent);
		if (winfo[ib].window == (Window)None)
		{
			ERRORD("failed to create button.", ib, errno);
			DisconnectFromServer(dd.display);
			return(2);
		}
	}

	// create graph window
	unsigned int graph_border_width = 2;
	winfo[Graph].window = CreateWindow(
		dd.display, winfo[Main].window, 
		winfo[Graph].x, winfo[Graph].y,
		winfo[Graph].width, winfo[Graph].height,
		dd.foreground, dd.background, GraphEventMask,
		CopyFromParent, graph_border_width, CopyFromParent);

	// map window
	XMapRaised(dd.display, winfo[Main].window);
	XMapSubwindows(dd.display, winfo[Main].window);
	XFlush(dd.display);

	// initialize rubber banding data
	rbd.start_x  = -1;
	rbd.start_y  = -1;
	rbd.last_x  = -1;
	rbd.last_y  = -1;
	xhd.start_x  = -1;
	xhd.start_y  = -1;
	xhd.last_x  = -1;
	xhd.last_y  = -1;

	// event loop
	XEvent event;
	KeySym keysym;
	int (*handler)(XEvent &) = NULL;
	char string[BUFSIZ];
	for (done = 0; !done; )
	{
		XNextEvent(dd.display, &event);
		switch (event.type)
		{
		case ConfigureNotify:
			// size of window changed
			winfo[Main].x = event.xconfigure.x;
			winfo[Main].y = event.xconfigure.y;
			winfo[Main].width = event.xconfigure.width;
			winfo[Main].height = event.xconfigure.height;

			// destroy old graph window
			XUnmapSubwindows(dd.display, winfo[Graph].window);
			XDestroyWindow(dd.display, winfo[Graph].window);
			XFlush(dd.display);

			// calculate new locations, etc.
			ClearXHairData();
			if (GetWindowData() != OK)
			{
				ERROR("unable to get window data.", errno);
				DisconnectFromServer(dd.display);
				return(2);
			}

			// create graph window
			winfo[Graph].window = CreateWindow(
				dd.display, winfo[Main].window, 
				winfo[Graph].x, winfo[Graph].y,
				winfo[Graph].width, winfo[Graph].height,
				dd.foreground, dd.background, GraphEventMask,
				CopyFromParent, graph_border_width, 
				CopyFromParent);

			// map window
			XMapRaised(dd.display, winfo[Graph].window);
			XMapSubwindows(dd.display, winfo[Graph].window);
			XFlush(dd.display);
			break;

		case KeyPress:
			done = DecodeKeyPress(&event.xkey, &keysym, string);
			break;

		case MappingNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;

		case Expose:
			handler = find_handler(event.xany.window);
			if (handler != NULL)
				handler(event);
			break;

		case ButtonPress:
			handler = find_handler(event.xany.window);
			if (handler != NULL)
				handler(event);
			break;

		case ButtonRelease:
			handler = find_handler(event.xany.window);
			if (handler != NULL)
				handler(event);
			break;

		case MotionNotify:
			handler = find_handler(event.xany.window);
			if (handler != NULL)
				handler(event);
			break;
		}
	}

	// close connection to server
	DisconnectFromServer(dd.display);

	// all done
	return(0);
}

