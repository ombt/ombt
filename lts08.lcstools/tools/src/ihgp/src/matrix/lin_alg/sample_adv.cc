//	       Sample code showing off a few advanced features
//	  and comparing them (time-wise) with traditional ones
//
// Simple example: downsampling a matrix, that is, creating a matrix
// that is 4 times (twice in each dimension) smaller than the original
// matrix, by picking every other sample of the latter.
//
// $Id: sample_adv.cc,v 3.1 1995/02/07 14:40:37 oleg Exp oleg $

#include "LinAlg.h"
#include <std.h>
#include <iostream.h>

				// Downsample matrix - new style
class downsample_matrix : public LazyMatrix
{
  const Matrix& orig_matrix;
  void fill_in(Matrix& m) const;
public:
  downsample_matrix(const Matrix& orig_matrix);
};

				// Just figure out the dimensions of the
				// downsampled (lazy) matrix
downsample_matrix::downsample_matrix(const Matrix& _orig_matrix)
	: LazyMatrix(_orig_matrix.q_row_lwb(),
		     (_orig_matrix.q_nrows()+1)/2 + _orig_matrix.q_row_lwb()-1,
		     _orig_matrix.q_col_lwb(),
		     (_orig_matrix.q_ncols()+1)/2 +_orig_matrix.q_col_lwb()-1),
		     orig_matrix(_orig_matrix)
{
}

				// "construct" the new matrix (when the lazy
				// matrix is being "rolled out")
void downsample_matrix::fill_in(Matrix& m) const
{
  class do_downsample : public ElementAction
  {
    const Matrix& orig_matrix;
    const int row_lwb, col_lwb;
    void operation(REAL& element)
	{ element = orig_matrix((i-row_lwb)*2+row_lwb,(j-col_lwb)*2+col_lwb); }
    public: do_downsample(const Matrix& _orig_matrix)
      : orig_matrix(_orig_matrix),
	row_lwb(orig_matrix.q_row_lwb()),
	col_lwb(orig_matrix.q_col_lwb()) {}
  };
  m.apply(do_downsample(orig_matrix));
}

				// Downsample in the traditional style
static Matrix traditional_downsampling(const Matrix& orig_matrix)
{
  Matrix smaller_m(orig_matrix.q_row_lwb(),
		   (orig_matrix.q_nrows()+1)/2 + orig_matrix.q_row_lwb()-1,
		   orig_matrix.q_col_lwb(),
		   (orig_matrix.q_ncols()+1)/2 + orig_matrix.q_col_lwb()-1);

  for(register int i=0; i<smaller_m.q_nrows(); i++)
    for(register int j=0; j<smaller_m.q_ncols(); j++)
      smaller_m(i+smaller_m.q_row_lwb(),j+smaller_m.q_col_lwb()) =
	orig_matrix(2*i+smaller_m.q_row_lwb(),2*j+smaller_m.q_col_lwb());
  return smaller_m;
}

main(void)
{
  cout << "\nDonsample matrices using traditional and non-traditional methods"
    << endl;

  {
    cout << "\nMake sure that both methods give the same results" << endl;
    Matrix orig_m = haar_matrix(9,201);	// which is a pretty big matrix
    Matrix small1 = traditional_downsampling(orig_m);
    Matrix small2 = downsample_matrix(orig_m);
    assert( small1 == small2 );
  }

  {
    cout << "\nClock the traditional downsampling" << endl;
    start_timer();
    for(int order=1; order<=10; order++)
    {
      Matrix orig_m = haar_matrix(order);	// may be pretty big, btw
      for(int count=0; count < (1<<(12-order)); count++)
      {
	Matrix small = traditional_downsampling(orig_m);
	small(1,1) = 1;				// just to use the matrix
      }
    }
    cout << "\tIt took " << return_elapsed_time(0) 
         << " sec to complete the test" << endl;
  }

  {
    cout << "\nClock the 'new style' downsampling (with lazy matrices)"<< endl;
    start_timer();
    for(int order=1; order<=10; order++)
    {
      Matrix orig_m = haar_matrix(order);	// may be pretty big, btw
      for(int count=0; count < (1<<(12-order)); count++)
      {
	Matrix small = downsample_matrix(orig_m);
	small(1,1) = 1;				// just to use the matrix
      }
    }
    cout << "\tIt took " << return_elapsed_time(0) 
         << " sec to complete the test" << endl;
  }
}	
