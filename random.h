/*	my.h				1-25-2010			GNU GCC g++
	Copyright (C)1994-2003, 2006-10 Steven Whitney.
	Initially published by http://25yearsofprogramming.com.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	Version 3 as published by the Free Software Foundation.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------
Notes:

--01-25-2010
  This file was adapted for g++ use from a previous version for Microsoft Visual C++, 
  which in turn was adapted from a Borland C++ version, which is why it contains
  vestigal code and comments relating to those. 
  I do not plan any attempt to keep the current g++ and VC++ versions of my.h
  coordinated with each other, nor to try to make one version of my.h fit both. 
  The two versions are used on separate hard drives under separate operating 
  systems, and there is no central location from which a single version can be 
  accessed by both compilers and maintained by me. 
  

*/
#ifndef __MY_H
#define __MY_H

#include <cstdio>
#include <cstring>   			// so that my library functions don't each need this line
#include <sstream>
#include <fstream>				// the rest are so commonly used, might as well include them.
#include <istream>
#include <ostream>				
#include <iostream>				
#include <iomanip>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cctype>
#include <typeinfo>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

// turn off warnings about insecure strcpy() because the official replacement strcpy_s() doesn't seem to work properly.
//#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////////////
typedef unsigned int 	uint;
typedef unsigned short 	ushort;
typedef unsigned long 	ulong;
typedef unsigned char 	uchar;
typedef signed char 	schar;

//////////////////////////////////////////////////////////////////////////////
// some macro definitions
//----------------------------------------------------------------------------
#define ERR		(-1)
#define ON		(1)
#define OFF		(0)
#define YES		(1)
#define NO		(0)

//for compatibility with old Borland and OWL code, they make some trivial text replacements unnecessary, but it's lazy.
#define TRUE	(true)
#define FALSE	(false)
#define GetItemsInContainer()	size()
typedef	bool	BOOL;		

#define M_PI_180			0.0174532925199432951    	// pi / 180
#define M_180_PI			57.2957795130823231			// 180 / pi
#define ROOT12OF2			1.05946309435929526			//12th root of 2, for music apps.

//these are leftover from Windows 3.1
//#define MAXUINT		(0xFFFF)	// max unsigned, 65535 (should be (uint)(-1) but check its uses first)
//#define MAXULONG	(ulong)(-1)		// largest possible ulong

//////////////////////////////////////////////////////////////////////////////
/*	PROTOTYPES FOR MY GLOBAL LIBRARY FUNCTIONS.
	if a function has default parameters, specify them HERE because if you
	call the function without the parameter, the compiler only has the
	declaration here to refer to.  It can't look ahead to the library module to
	discover the default value there.  Since it doesn't know there is a default
	value, the missing parameter will generate an error.
	Be sure to include my.h in all library function .cpp files so any
	different or redeclared default values are caught.
*/
//----------------------------------------------------------------------------
void 			aborts(const string&);                	// abnormal termination, with message
const string& 	backupfile(const string& filename);  	// create a file, rename its old version to .bak
double			BlackScholesCallHedgeRatio(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
double			BlackScholesCallValue(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
double			BlackScholesDen1(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
double			BlackScholesDen2(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
double			BlackScholesPutHedgeRatio(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
double			BlackScholesPutValue(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear);
string 			bstring(const string& s);				// output a string in brackets
double			CalendarDaysFor(double TradingDays);
double			CalendarDaysToReachPrice16(double CurrentPrice, double TargetPrice, double Volatility);
int 			ci();                                	// console input
double 			cosine(double degrees);               	// cosine, with argument in degrees
uint 			countchr(const string& searchin, char lookforandcount); // count occurrences of a char in a string
string 			CRLFtoLF(const string& s); 				// change each CRLF to LF
string 			CRtoLF(const string& s);   				// change each CR to LF
int 			FontHeightInPixels(int pointsize);		// Windows utility fn
double 			frand();								// random 0 to 1.0 (limit)
string& 		fromstring(string& var, const string& value); //overrides my template version, for for strings only
double			FutureValue(double NumPeriods, double InterestPerPeriod, double PresentValue);
uint	 		getpixel(double dx, double dy);       	// get color value of a pixel with x,y as doubles
uint 			gettoken(const string& source, size_t startpos, string& token); // extract token from text
uint	 		gettoken(istream& is, string& token);	// extract token from stream
uint			getsentence(const string& source, size_t startpos, string& sentence); // extract a sentence from text
uint			getsentence(istream& is, string& sentence);	// extract a sentence from stream
void 			gputs(int x, int y, int color, char *text);	// prints string to screen in DOS BGI graphics
void 			graborts(const string&);	// aborts() from within DOS BGI graphics app
double			ImputedInterest(double PresentValue, double FutureValue, double NumPeriods);
int 			initgraf();               	// set up BGI graphics
bool 			isallspace(char *s);		// if string contains all whitespace
bool 			isterminator(const string& token);	// if it's a sentence-terminating token
string 			LFtoCR(const string& s);   			// change each LF to CR
string 			LFtoCRLF(const string& s); 			// insert a CR before each LF
uint 			logerror(string error = "", string filename = "error.log", bool beep = false); // log program error to a file
uint 			logtofile(const string& s, const string& filename);	// send any string to a file
double			LowerLimitPrice84(double CurrentPrice, double VolatilityPerPeriod, double TimePeriod);
long 			lrandom(long top);                            		// (long) random from 0 to top-1
double			NormalDensityZx(double x, double Mean, double StdDev);
int 			NormalRandom(double mean, double stddev);		// normally distributed random integer
double			NormalRandomDouble(double mean, double stddev);	//  normally distributed random double
double			NumPeriods(double PresentValue, double FutureValue, double InterestPerPeriod);
int 			picload(const char *filename);   			// load a .PIC file
int 			picsave(const char *filename);				// save a .PIC file
double			PresentValue(double NumPeriods, double InterestPerPeriod, double FutureValue);
int 			presskey(ostream& os = cout);  		// says "Press any key to continue..."
double			ProbabilityStockPriceAbove(double CurrentPrice, double TargetPrice, double VolatilityPerPeriod, double TimePeriod);
double			ProbabilityStockPriceBelow(double CurrentPrice, double TargetPrice, double VolatilityPerPeriod, double TimePeriod);
string 			qstring(const string& s);  			// put a string in quotes
unsigned		random(unsigned i);					// random integer from 0 to i-1 (inclusive)
// This generates a duplication error in g++. 
// Watch for whether the built-in fn rounds properly to nearest int. Or if a "mode" (e.g. bankers) is required.
//int 			round(double d);             		// round a double properly
uint	 		sgetpixel(int x, int y);      		// ignores out of bounds requests
double 			similarity(const string& first, const string& second);	// measures similarity of two strings
double 			sine(double degrees);           	// sine, with argument in degrees
double			StandardNormalAx(double x);
double			StandardNormalAxy(double x, double y);
double			StandardNormalPx(double x);
double			StandardNormalQx(double x);
bool 			strand(char *str, char *tofind, char dontcare = '?'); // performs an AND on two strings
bool 			strand(const string& searchin, const string& pattern, char dontcare = '?');
bool			stringcontains(string& s, string& tofind);		// true if s contains tofind
bool			stringcontains(string& s, const char* tofind); 
bool			stringcontains(string& s, char tofind); 
string&			stringfromfile(string& s, ifstream& is); // append entire file contents to string s
//void			SystemStringToBasicString(String^ src, string& dest);//convert String to string
string& 		titlecase(string& s);           	// renders string in Title Case
string&			tolower(string& s);					// renders string in lower Case
string&			toupper(string& s);					// renders string in upper Case
double			TradingDaysIn(double CalendarDays);
string 			TranslateControls(const string& s);	// replaces control chars in a string with ASCII mnemonics, e.g. <NUL>.
string 			unqstring(const string& s);			// remove quotes from around a string
double			UpperLimitPrice84(double CurrentPrice, double VolatilityPerPeriod, double TimePeriod);
bool 			yesno(string prompt = "Continue", ostream& os = cout); // asks user a yes/no question

//----------------------------------------------------------------------------
// PROTOTYPES FOR FUNCTIONS FROM OTHER SOURCES
//----------------------------------------------------------------------------
// FUNCTIONS ADAPTED FROM GNU SCIENTIFIC LIBRARY

double GSLRandomExponential(double mu);			// random double from expotential distribution
double GSLRandomLevy(double c, double alpha);	// random double from Levy alpha-stable distribution


//////////////////////////////////////////////////////////////////////////////
//WHOLE-DEGREES SINE/COSINE LOOKUP TABLES, FOR SPEED
struct Trigtable
{
	Trigtable();	//constructor
	
	double cosines[360];
	double sines[360];
};

//////////////////////////////////////////////////////////////////////////////
// TOSHIBA PRINTER FUNCTIONS
//----------------------------------------------------------------------------
#define	TOSHIBADRAFT		"\033*0"
#define TOSHIBAELITE		"\033*1"
#define TOSHIBACOURIER		"\033*2"
#define	TOSHIBACONDENSE		"\033["
#define TOSHIBABOLDON		"\033K2"
#define TOSHIBABOLDOFF		"\033M"

//////////////////////////////////////////////////////////////////////////////
/*	CONSOLE APPLICATIONS ONLY
	#error I have not yet found the test that distinguishes a console app
	from a Windows Forms app. The distinction might no longer be necessary.
*/	
//----------------------------------------------------------------------------
//#if !defined(__OWL_OWLCORE_H)

	/*	formerly userinpu.h		5-20-07
		Variable input routines:  prompts user, accepts input.
		If user enters CR, current value for the variable is unchanged.
		If user enters a value, variable is changed.
		Luckily, this behavior is inherent in >> as used with cin (PG:213).
		Don't call it input() because of possible conflicts, too general a name.
		There are 2 versions because "only trivial type conversions are performed
		with compiler-generated template functions" (PG:162).
		Normally, you can pass a char array to a function requiring a string&, and
		the compiler will do the conversion.  In this case it won't, so you must
		have the const char* version.  The 2nd version is only useful where you
		have a pre-constructed string that you want to pass, or you can cast the
		prompt, as in:  userinput(string("Enter something"),something);
		which works but is cumbersome.
	*/
	//----------------------------------------------------------------------------
	template<class T> void userinput(const char* prompt, T& defalt)
	{
		char temp[128] = { 0 };
		cerr << prompt << " <CR for " << defalt << ">: ";
		cin.getline(temp,sizeof(temp));		// get new value, or a CR (empty)
		if(strlen(temp))
			istringstream(temp) >> defalt;
	}
	//----------------------------------------------------------------------------
	template<class T> void userinput(const string& prompt, T& defalt)
	{
		userinput(prompt.c_str(),defalt);
	}


//#endif	//!defined(__OWL_OWLCORE_H)

//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
/*	signum function, 	 	12-3-96
	In this version, the returned value has the same type as the argument.
	For some uses (such as switch()), you should cast the returned value to int.
*/
template<class T> T signum(const T& a)
{
	return((a == (T)0) ? (T)0 : (a < (T)0) ? (T)(-1) : (T)1);
}
//----------------------------------------------------------------------------
/*	swap two variables.  class T must have a default constructor and an = operator.
	this was previously called swap(), which conflicts with multimap::swap.
	But this might still be redundant because VC++ apparently has a template swap() of its own.
*/	
template<class T> void swapvalues(T& a, T& b)
{
	T temp;
	temp = a;
	a = b;
	b = temp;
}
//////////////////////////////////////////////////////////////////////////////
/*	5-19-07
	If a value falls outside the allowable range, bring it into the range. (no wrap)
	Min and max are the lowest and highest values allowable, INCLUSIVE.
*/
template<class T> T range(T val, T min, T max)
{
	if(min > max)					// prevent mistakes
		swapvalues(min,max);
	if(val < min)
		return min;
	if(val > max)
		return max;
	return val;
}
//----------------------------------------------------------------------------
/*	6-30-99
	wraparound.  If a value falls outside the allowable range,
	wrap it back into the range.  Useful for screen wrap.
	Only works properly for screen wrap for integral types (int, long).
	See adapt.cpp for a specialized version for doubles.
	Min and max are the lowest and highest values allowable, INCLUSIVE.
*/
template<class T> T wrap(T val, T min, T max)
{
	if(min > max)					// prevent runaway disaster
		swap(min,max);
	if(min == max)					// not much you can do about this
		return(min);				// just return a legal value

	T adj = max - min + (T)1;
	while(val < min) val += adj;
	while(val > max) val -= adj;

	return(val);
}
//////////////////////////////////////////////////////////////////////////////
/*	number to string	12-25-96
	Create a string containing the text representation of a number.
	Eliminates need for itoa() and similar functions and makes it easier to build
	compound strings from both text and numeric pieces.
	Example: string s = string("Number = ") + tostring(10) + " " + tostring(i);
	Note it must return a string object, not a string&.
	Template makes it applicable to any type numeric parameter, and
	oddly, even any non-numeric object with an ostream << operator.
	That's why the os is dynamic, just in case.

	#error the MS VC++ compiler can't convert a (type) to a (type&)!!,
	so this doesn't work anymore. However there is a .NET equivalent, ToString().
*/
//----------------------------------------------------------------------------
template<class T> string tostring(const T& t)
{
	ostringstream os;
	os << t;			// << ends not required for an ostringstream
	string s(os.str());
	return(s);
}
//////////////////////////////////////////////////////////////////////////////
/*	fromstring		4-18-00
	complementary function to tostring().
	Allows you to assign to a variable the value encoded in a string.
	Similar to Basica I = VAL(A$), and similar to atoi(), atod(), etc.,
	or istream >> variable, but for STRINGS.
	Works for any type for which read-from operator >> is defined, since it uses it.
	But mystring.cpp has a special overriding version for strings.
*/
//----------------------------------------------------------------------------
template<class T> T& fromstring(T& var, const string& value)
{
	// refuse to change ANY references or pointers, including char*
	// refuse to change strings.  they have a separate version in mystring.cpp.
	// (I haven't tested this after adding the string exclusion here.)
	string n = typeid(var).name();
	if(stringcontains(n,"*") || stringcontains(n,"&") || stringcontains(n,"string"))
		return(var);

	char* s = new char[value.length() + 1];
	strcpy(s,value.c_str());
	istringstream(s) >> var;
	delete[] s;
	return(var);
}                      		// fromstring
//////////////////////////////////////////////////////////////////////////////
/*	allocate and delete 2-dimensional arrays of any type object.  5-17-97
	the result is (height) arrays, each (width) wide.
	----------------------------------------------------------------------------
	needed because new int[a][b] is illegal.  b must be a compile time constant.
	if new fails, it will simply terminate the application, as xalloc always does.
	adapted from PG:121.  See also commented version in booksamp.cpp.
	The unused T& argument is necessary so it knows what type a T is.
	You can pass it any example of a "T" object.
	----------------------------------------------------------------------------
	it is dimensioned backwards as [y][x] because x should vary fastest when traversing
	in scan-line fashion.  Seems like it wouldn't matter if it were [x][y],
	but don't change, because it is already in use.
	----------------------------------------------------------------------------
	When using with char arrays, don't try to load or write entire scan lines
	at once, because each of the horizontal lines is not null-terminated.
*/
//----------------------------------------------------------------------------
template<class T> T** newarray2dim(const T&, int height, int width)
{
	T** a = new T*[height];

	for(int i = 0; i < height ; i++)
		a[i] = new T[width];

	return(a);
}
//----------------------------------------------------------------------------
template<class T> void deletearray2dim(T** a, int height)
{
	for(int i = 0; i < height ; i++)
		delete[] a[i];

	delete[] a;
	a = 0;		// probably always want this, since it's an invalid pointer now, anyway
}
//////////////////////////////////////////////////////////////////////////////
/*	csts()  console status, for compatibility with DeSmet C.
	It recognizes a function key as a keystroke, which kbhit() does not.
	returns a (strangely encoded) non-zero value if keystroke waiting, zero if none.
	If a keystroke is waiting, repeated csts() calls continue to return the SAME value
	even if more keys are pressed.  So if csts is nonzero, you should always
	follow it with ci() to get whatever char is there.
	Move to its own fn? if a key was pressed, get and return it, to flush.
*/
//----------------------------------------------------------------------------
#define csts()		bioskey(_NKEYBRD_READY)

#ifdef WIN32
inline double round( double d ) {
    return floor( d + 0.5 );
}
#endif

#endif				// __MY_H DEFINED