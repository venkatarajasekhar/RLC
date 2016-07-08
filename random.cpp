/*	mylib.cpp		   		1-25-2010			GNU GCC g++
	Copyright (C)1997-2000, 2006-2010 Steven Whitney.
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

#including this file pulls in the source code for the library functions.
I don't use a precompiled library. The code changes often.

------
NOTES:
--01-25-2010
  This file was adapted for g++ use from a previous version for Microsoft Visual C++, 
  which in turn was adapted from a Borland C++ version, which is why it contains
  vestigal code and comments relating to those. 
  I do not plan any attempt to keep the current g++ and VC++ versions of mylib.cpp
  coordinated with each other, nor to try to make one version of mylib.cpp fit both. 
  The two mylib.cpp versions are used on separate hard drives under separate operating 
  systems, and there is no central location from which it can be accessed by
  both compilers. 
--Most is untested in g++. The random number methods in particular could require adjustments.


*/
#include "random.h"

using namespace std;

//----------------------------------------------------------------------------
/*
void SystemStringToBasicString(String^ src, string& dest) 
{
using namespace Runtime::InteropServices;
const char* p = (const char*)((Marshal::StringToHGlobalAnsi(src)).ToPointer());
dest = p;
Marshal::FreeHGlobal(IntPtr((void*)p));
}
*/
//----------------------------------------------------------------------------
/*	Creates a backup copy of a file.
	Given the name of a file you want to backup before writing to, it deletes filename.bak (the
	current backup file), renames the current filename.ext to filename.bak (backs it up), and
	returns a reference to the original filename.ext string.
	It should work properly with full path names.
	Use: ofstream outfile(backupfile(filename.ext).c_str());
*/
const string& backupfile(const string& filename)
{
if(filename.length())
{
	string bakfile(filename);

	if(bakfile.find(".") != string::npos)                       // delete extension
		bakfile.erase(bakfile.rfind("."));
	bakfile += ".bak";                             	// add ".bak"

	unlink(bakfile.c_str());						// delete .bak file, if any
	rename(filename.c_str(),bakfile.c_str());		// rename current to .bak
}
return(filename);		// return reference to original filename
}
//----------------------------------------------------------------------------
//counts number of occurrences of a char in a string. Pulled from code.cpp.
uint countchr(const string& s, char ch)	// string to search, char to look for
{
int count = 0;
int N = s.length();
for(int i = 0 ; i < N ; i++)
	if(s[i] == ch)
		count++;
return(count);
}
//----------------------------------------------------------------------------
/*	Returns true if char array is zero-length or all whitespace, false otherwise.
	Could add additional version for a const string&
*/
bool isallspace(char *s)
{
char *p = s;
while(*p)
	if(!isspace(*(p++)))	// first non-space char causes return
		return(false);
return(true);
}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
/*	functions for appending strings to text files:
	logs an error string to an error log file, and keeps count.
*/
//----------------------------------------------------------------------------
// append a string to a text file.
uint logtofile(const string& s, const string& filename)
{
if(s.length() && filename.length())
{
	ofstream os(filename.c_str(),ios::app);
	os << s << endl;
	return(1);
}
return(0);
}							//logtofile
//----------------------------------------------------------------------------
// write error (if provided) to disk.  returns cumulative error count.
// you can call with empty string just to get the count.
uint logerror(string error, string filename, bool beep)
{
static uint errorcount = 0;			// # of errors
if(error.length())					// if an error occurred,
{
	ostringstream os;
	os << time(NULL) << ":  " << error;
	logtofile(os.str(),filename);
	//if(beep)						// note: only beeps if we're writing an error
	//{
	//	#if defined(_Windows)
	//		MessageBeep(MB_ICONEXCLAMATION);	// a global WinSDK function
	//	#else
	//		cout << "\a";
	//		// 	SerialCom.Send(os.str());
	//	#endif
	//}

	errorcount++;
}
return(errorcount);
}						//logerror
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// duplicates of this may exist local to some project. also keep a watch for places where I
// now round incorrectly or not at all, and could use this instead.
// usage: int x = round(speed * sine(30) * x);
/* 1-25-2010 Fn might be no longer needed.
int round(double d)
{
if(d > 0)    		// if positive, make more positive before rounding
	d += .5;
else
	if(d < 0)    	// if negative, make more negative before rounding
		d -= .5;
return((int)d);			// truncates decimal portion
}
*/
//----------------------------------------------------------------------------
/*	1-17-2010 
	returns a uniformly distributed random integer in the range 0 (inclusive) to i-1,
	1-25-2010 REVISED FOR GCC, UNTESTED. The use of modulo was the cause of a recurring cycle bug in Borland C++,
	so I am skeptical of the method.
*/
unsigned random(unsigned i)
{
if(i < 2)
	return(0);	
else
	return (unsigned)(rand() % i);
}
//----------------------------------------------------------------------------
/*	returns a uniformly distributed random double in the range 0 (inclusive) to 1.0 (limit).
	builds as text because that's the only way I could think of that seems foolproof.
	There are pitfalls to generating random numbers.
*/
double frand()
{
int len = 17;	// "0." plus 15 significant digits is max possible for a double.
char buf[18];	// must be len + 1

buf[0] = '0'; buf[1] = '.';
for(int i = 2 ; i < len ; i++)
{
	buf[i] = (char)(0x30 + (int)(rand() % 10) );
}
buf[len] = 0;
return(atof(buf));
}   							//frand
//----------------------------------------------------------------------------
/*	Returns a long random number from 0 to specified upper limit (top) - 1, inclusive.
	For a possibly negative random number, use this, and then if(random(2)) { make it negative }
	this is very fast, and seems to work, based on running the results through stats.exe.
	
	In Win32 only, Borland has a function _lrand(), which returns a random long
	value (probably 0 to LONG_MAX), but you can't scale it to a different interval
	by the usual method because the result of the required multiplication can
	overflow LONG_MAX and even ULONG_MAX, and that's the largest integral value
	the computer can handle.  Borland's stdlib.h method of using % operator doesn't
	work because its random(N) has a repeat cycle of N if N is a power of 2, probably
	because of coincidence(?) with cycles inherent in the random # generation.
*/
long lrandom(long top)
{
if(top < 2L)			// 0, 1 always produce 0, and negative is illegal
	return(0L);
	
// In GCC, lrandom is same as random().	
return((long)(rand() % top));
}           	            	//lrandom
//----------------------------------------------------------------------------
// returns a normally distributed random DOUBLE from the distribution with the given mean and standard deviation
// sin() can be used in place of cos(). 
double NormalRandomDouble(double mean, double stddev)
{
return(sqrt(-2.0 * log(frand())) * cos(2.0 * M_PI * frand()) * stddev + mean);
}
//----------------------------------------------------------------------------
// returns a normally distributed random INTEGER from the distribution with the given mean and standard deviation
int NormalRandom(double mean, double stddev)
{
return round(NormalRandomDouble(mean, stddev));
}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
// (MYSTRING.CPP) STRING FUNCTIONS
//----------------------------------------------------------------------------
// a replacement for Borland string::contains())
// returns true if the string contains the given substring, else false.
bool stringcontains(string& s, string& tofind)
{
return(s.find(tofind) != string::npos);
}    						// stringcontains
//----------------------------------------------------------------------------
// a replacement for Borland string::contains())
// returns true if the string contains the given substring, else false.
bool stringcontains(string& s, const char* tofind)
{
return(s.find(tofind) != string::npos);
}    						// stringcontains
//----------------------------------------------------------------------------
// a replacement for Borland string::contains())
// returns true if the string contains the given char, else false.
bool stringcontains(string& s, char tofind)
{
for(uint i = 0 ; i < s.length() ; i++)
	if(s[i] == tofind)
		return true;
return false;		
}    						// stringcontains
//----------------------------------------------------------------------------
// change a string to upper case. (a replacement for Borland string::to_upper())
string& toupper(string& s)
{
uint i;
for(i = 0 ; i < s.length() ; i++)	
	s[i] = (char)toupper(s[i]);
return(s);
}    						// toupper
//----------------------------------------------------------------------------
// change a string to lower case, (a replacement for Borland string::to_lower())
string& tolower(string& s)
{
uint i;
for(i = 0 ; i < s.length() ; i++)	
	s[i] = (char)tolower(s[i]);
return(s);
}    						// tolower
//----------------------------------------------------------------------------
// prepend a string into another string
string& prepend(string& s, string& prefix)
{
s.insert(0,prefix);
return(s);
}    						// prepend
//----------------------------------------------------------------------------
// prepend a char* into a string
string& prepend(string& s, const char* prefix)
{
string t(prefix);
prepend(s,t);
return(s);
}    						// prepend
//----------------------------------------------------------------------------
// change a string to "title case", all lower case except first letter of each word
string& titlecase(string& s)
{
uint i;
for(i = 0 ; i < s.length() ; i++)	// start with all lower case, was s.to_lower()
	s[i] = (char)tolower(s[i]);

bool spacebefore = true;               		// whether the previous char was space
for(i = 0 ; i < s.length() ; i++)
{
	if(spacebefore)
		s[i] = (char)toupper(s[i]);
	spacebefore = !isalnum(s[i]);
}
return(s);
}    						// titlecase
//----------------------------------------------------------------------------
// for one-step output of a string in quotes: cout << qstring(s). source string is unchanged.
string qstring(const string& s)
{
string t(s);
t += ("\"");
t.insert(0,"\"");
return(t);
}							// qstring
//----------------------------------------------------------------------------
// remove the quotes from around a quoted string.  source string is unchanged.
string unqstring(const string& s)
{
string t(s);
if((t.length() >= 2) && (t[0] == '\"') && (t[t.length() - 1] == '\"'))
{
	t.erase(0,1);
	t.erase(t.length() - 1,1);
}
return(t);
}							// unqstring
//----------------------------------------------------------------------------
// for one-step output of a string in brackets: cout << bstring(s). source string is unchanged.
string bstring(const string& s)
{
string t(s);
t += ("]");
t.insert(0,"[");
return(t);
}							// bstring
//----------------------------------------------------------------------------
// appends the entire contents of a file to a string.
// replacement for Borland string::read_file(istream&) with some differences.
// it does not test for NULL string terminator within the file.
string& stringfromfile(string& s, ifstream& is)
{
char ch;
while(is)
{
	ch = is.get();
	if(is)				// supposed to prevent appending the EOF when it occurs
		if(s.length() < s.max_size())	// prevent overflow
			s += (char)ch;
}
return(s);
}    						// toupper
//----------------------------------------------------------------------------
/*	fromstring													1-1-99
	A strings-only override of my template version. Necessary because if this situation
	fromstring(string,string) were in template version, the compiler, when instantiating the template,
	would have to GENERATE the code for the line (below) "var = value" for ALL instantiations,
	including those where that line is inapplicable because the types are not the same.
	But if the types aren't the same, the code itself is illegal, a compile-time error.
	(and there's no RTTI at compile time, so you can't conditionally exclude the code)
	(am I sure there's not?  Even if there is, probably not for templates.)
	copies ALL of a multi-word string, which the >> procedure in template version would not.
*/
string& fromstring(string& var, const string& value)
{
	var = value;   		// just copy the whole string
	return(var);
}                      		// fromstring
//----------------------------------------------------------------------------
/*	measures how similar two strings, usually tokens (single words), are.
	This is an experimental concept that was interesting to work on.
	I think this is used in wtalk.cpp; some other programs have similarity() or
	SimilarityTo() functions that explore a more generalized concept of measuring
	"similarity" of two objects.
	returns a value between 0 (no similarity at all) and 1.0 (identical)
	Measure is the same regardless of which string is first or second.
	Some similar and likely better measures have been devised:
	See "Levenshtein distance", php functions levenshtein())and similar_text()
*/
double similarity(const string& first, const string& second)
{
string shorter = first, longer = second;	// copy to local for use

if(!shorter.length() && !longer.length())	// 2 empties are equal.
	return(1.0);
if(!shorter.length() || !longer.length())	// now require both to have length
	return(0.0);
if(shorter.length() > longer.length())		// comparisons easier if we know which is which
	swap(shorter,longer);

// calculates return value as a percentage of longer, so a short word that "contains" shorter
// is a better match than a longer word that also does.  if they're equal, score = 1.0
double denominator = longer.length();		// save now; processing may destroy the string

//----------
// Version 3.  NEWER version OF version 1!  More restrictive.
// find what chars they share at the exact same locs.  no displacements allowed for.
// you don't want "UNfastened" to match "fastened", which version 1 would do.
// e.g. run and ran match 2 chars in order, = 2/3 = .67 match.
uint count = 0;						// count of matching chars
for(uint i = 0 ; i < shorter.length() ; i++)
	if(shorter[i] == longer[i])
		count++;
return(count / denominator);
//----------

#if 0
//----------
// Version 2.  Still not convinced I've found a satisfactory one.
// newest version.  It destroys the strings as it finds matches.
// it tends to overrate the degree of match.
uint totmatches = 0;						// total matching chars found
while(shorter.length() && longer.length())
{
	// in each string, delete all chars that don't occur in the other.
	// those chars can't possibly match.  Do it each loop pass because a char can
	// become newly unmatched when one string has more occurrences of it than the other.
	size_t u;
	while((u = shorter.find_first_not_of(longer)) != string::npos) shorter.erase(u,1);
	while((u = longer.find_first_not_of(shorter)) != string::npos) longer.erase(u,1);

	// if either was emptied, further matches are impossible
	if(!shorter.length() || !longer.length())
		break;
	if(shorter.length() > longer.length())		// test again
		swap(shorter,longer);

	uint matches = 0;			// new char matches found during this loop pass
	// find, tally, and delete any sequential leading matching chars (prefixes)
	while(shorter.length() && (shorter[0] == longer[0]))
	{
		matches++;
		shorter.erase(0,1);
		longer.erase(0,1);
	}
	// start from the other end and work backwards.
	// This will find matching suffixes in strings of different length.
	int i = shorter.length() - 1;
	int j = longer.length() - 1;
	for( ; shorter.length() && (shorter[i] == longer[j]) ; i--, j--)
	{
		matches++;
		shorter.erase(i,1);
		longer.erase(j,1);
	}
	if(matches)     				// if new matches were found
		totmatches += matches;
	else       						// if no new matches, MUST prevent endless loop
		break;						// in case strings still have contents.
}									//while(shorter.length() && longer.length())
// experimental final test (from below), untested even on paper.
// strings may have substantial internal matches that still haven't been caught.
// (such as when they're displaced by one loc).  I don't do this inside loop
// because doing it multiple times looks like it can degenerate into "matching
// chars regardless of order", which must be avoided.

// test yet again
size_t u;
while((u = shorter.find_first_not_of(longer)) != string::npos) shorter.erase(u,1);
while((u = longer.find_first_not_of(shorter)) != string::npos) longer.erase(u,1);
if(shorter.length() > longer.length())
	swap(shorter,longer);

uint highcount = 0;						// longest sequence of matching chars
uint remaining = shorter.length();      // chars left to search in shorter
for(uint i = 0 ; (i < shorter.length()) && (highcount < remaining) ; i++, remaining--)
{
	uint j = longer.find(shorter[i]);
	if(j != string::npos)           			// if first char found, search subsequent chars
	{
		uint count = 0;
		for(uint k = i ; (k < shorter.length()) && (j < longer.length()) ; k++, j++)
			if(shorter[k] == longer[j])
				count++;
		highcount = max(highcount,count);
	}
}
totmatches += highcount;
return(totmatches / denominator);

//----------
// version 1.  seems to work better than Version 2.
// find what chars they share in the same order.
// Start a new search at EACH position in shorter, and use the highest count found.
// the first nonmatching char does not abort the search.  internal nonmatches are allowed.
// #error DOES THIS ACCURATELY DESCRIBE WHAT'S BELOW? NO.
// e.g. run and ran match 2 chars in order, = 2/3 = .67 match.
// You could also factor in a measure of the displacements of the shared chars
// (i.e. they match, but are not at exactly the same locs in the strings), using a method
// similar to that for the Q-set deck, but it doesn't seem that useful or necessary.
uint highcount = 0;						// longest (maybe broken) sequence of matching chars
uint remaining = shorter.length();      // chars left to search in shorter
// for speed, if there's not enough string left to search to improve highcount, quit looking.
for(uint i = 0 ; (i < shorter.length()) && (highcount < remaining) ; i++, remaining--)
{
	uint j = longer.find(shorter[i]);
	if(j != string::npos)           			// if first char found, search subsequent chars
	{
		// a test here similar to "remaining", above, is more trouble than it's worth.
		uint count = 0;
		for(uint k = i ; (k < shorter.length()) && (j < longer.length()) ; k++, j++)
			if(shorter[k] == longer[j])
				count++;
		highcount = max(highcount,count);
	}
}
return(highcount / denominator);

//----------
// in oldest version, this was the final step.
// This section is useless, *especially* for strings.  Kept only for the method.
// find what percent of the chars in shorter are found in longer, regardless of order.
// this test destroys longer
highcount = 0;
for(i = 0 ; i < shorter.length() ; i++)
{
	uint j = longer.find(shorter[i]);
	if(j != string::npos)						// char was found
	{
		highcount++;             		// increment count
		longer.erase(j,1);             // remove the char so it doesn't get found again
	}
}
double anyorder = (double)highcount / (double)shorter.length() * .25;
#endif // 0

}							//similarity
//----------------------------------------------------------------------------
// returns string with each LF changed to CR. Source string is unchanged.
string LFtoCR(const string& s)
{
string t;
for(uint i = 0 ; i < s.length() ; i++)
	if(s[i] == '\n')
		t += '\r';
	else
		t += s[i];
return t;
}                   		//LFtoCR
//----------------------------------------------------------------------------
// returns string with each CR changed to LF. Source string is unchanged.
// useful for making text from a TEdit suitable for writing to disk in text mode,
// (will prevent writing double-CRs).
// #error (where, if anywhere, do I use this?  It would change CRLF to LFLF.)
string CRtoLF(const string& s)
{
string t;
for(uint i = 0 ; i < s.length() ; i++)
	if(s[i] == '\r')
		t += '\n';
	else
		t += s[i];
return t;
}                   		//CRtoLF
//----------------------------------------------------------------------------
// returns string with each LF changed to CRLF.  source string is unchanged.
// useful for output to modem or to a TEdit. (See mylib.cpp)
string LFtoCRLF(const string& s)
{
string t;
for(uint i = 0 ; (i < s.length()) && (t.length() < (t.max_size() - 2)) ; i++)
{
	if(s[i] == '\n')
		t += '\r';		// insert CR before LF
	t += s[i];
}
return t;
}                   		//LFtoCRLF
//----------------------------------------------------------------------------
// returns string with each CRLF changed to LF.  source string is unchanged.
string CRLFtoLF(const string& s)
{
string t(s);
size_t u;
while((u = t.find("\r\n")) != string::npos)		// find each CRLF
	t.erase(u,1);								// only remove each CR
return t;
}                   		//CRLFtoLF
//----------------------------------------------------------------------------
/*	STRing AND		12-2-96
	An "AND" operator for strings.
	Searches a given string (such as "ABCDE") for a pattern given by another
	string (such as "?B??E", where the question marks are "don't care").
	The above example would return true, because the target string does
	contain a B in the 2nd position, and E in the 5th.
	Returns true if pattern matched, false if not.

	Used in CLASSIFY.CPP.  Another planned use for this is in LIFE.C,
	for matching strings that encode the neighboring colors around a pixel.
	See paper file A-LIFE in file cabinet.

	Briefly tested, seems to work ok.
	string to search, pattern to search for, the dont-care char in pattern.
*/
bool strand(const char *searchin, const char *pattern, char dontcare)
{
if(strlen(pattern) > strlen(searchin))	// can't find pattern in a shorter string
	return(false);

int slen = strlen(pattern);
for(int i = 0 ; i < slen ; i++)
	if(pattern[i] != dontcare)
		if(pattern[i] != searchin[i])
			return(false);
return(true);
}
//----------------------------------------------------------------------------
// version for strings
bool strand(const string& searchin, const string& pattern, char dontcare)
{
if(pattern.length() > searchin.length()) // can't find pattern in a shorter string
	return(false);
int slen = pattern.length();
for(int i = 0 ; i < slen ; i++)
	if(pattern[i] != dontcare)
		if(pattern[i] != searchin[i])
			return(false);
return(true);
}
//----------------------------------------------------------------------------
/*	strips leading and trailing whitespace from a string.
	modify as needed to strip leading, trailing, both.
	returns a reference to the stripped source string.
*/
string& stripstring(string& s)
{
while(s.length())						// strip leading 
	if(isspace(s[0]) || iscntrl(s[0]))
		s.erase(0,1);
	else
		break;

while(s.length())						// strip trailing 
	if(isspace(s[s.length() - 1]) || iscntrl(s[s.length() - 1]))
		s.erase(s.length() - 1);
	else
		break;

return(s);
}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
/*	Token extraction functions and sentence extraction functions.
	A token is a punctuation mark or an alphanumeric string, which can contain
	hyphens or apostrophes.  This is DIFFERENT from what string::read_token() extracts.
*/
//----------------------------------------------------------------------------
/*	get token from a string
	this has a minor unfixable bug: if the source string ends with a "-" (such as might
	happen when reading lines from a file), it means the token continues on the next line,
	but this function must quit looking because the source string is used up.
	
	returns the index of the location in the provided string that is 1 char beyond
	all skipped chars and the gotten token (might be beyond end of string, too).
	or 0 if no token was created.  If it returns 0, it also means there is nothing
	left to get.  You can use: while(gettoken()){...};
	
	sentence to scan, start, token holder
*/
uint gettoken(const string& s, unsigned startpos, string& buf)
{
buf.clear();		// must empty each call: it's a reference, and we're using +=

if(startpos >= s.length())				// startpos invalid index
	return(0);                        	// means string is used up

size_t i = startpos;                  // pointer into s
while(iscntrl(s[i]) || (s[i] == ' '))   // skip over control-chars (incl. nulls) & spaces
	if(++i >= s.length())               // string ended
		return(0);                   	// before any valid token-starting char found

										// got to here means there is a token to be got
if(ispunct(s[i]))                   	// punctuation
{                    // identical multiples, like --, allowed.  Also gets "" as 1 token
	string t = "?!";					// allows !?  ?!?!?!  ???! etc. as a single token.
	char ch = s[i];
	while((s[i] == ch) || ((t.find(s[i]) != string::npos) && (t.find(ch) != string::npos)) )
	{
		buf += s[i];
		if(++i >= s.length())  			// hit end of string
			break;
	}
}
else             // alphanumeric, and non-leading hyphens, apostrophes all legal
	while(isalnum(s[i]) || (s[i] == '-') || (s[i] == '\''))
	{
		buf += s[i];
		if(++i >= s.length())  			// hit end of string
			break;
	}
return(i);              				// return new search-start loc.
}                    	// end gettoken
//----------------------------------------------------------------------------
/*	get token from a stream
	returns number of chars in the retrieved token.  if 0, there is also nothing more to get.
	You can use: while(gettoken()){...};
*/	
uint gettoken(istream& is, string& buf)	// stream to read from, token holder
{
buf.clear();			// must empty each call: it's a reference, and we're using +=

char ch = ' ';
while(iscntrl(ch) || (ch == ' '))   	// skip over control-chars (incl. nulls) & spaces
	if(!is.get(ch))						// stream used up
		return(0);	                    // before valid token-starting char found
										// got to here means there is a token to be got
if(ispunct(ch))                   		// punctuation
{                                       // identical multiples, like --, allowed)
	string t = "?!";					// allows !?  ?!?!?!  ???! etc. as a single token.
	char p;
	buf += ch;
	// peek() can only return a value that will fit into a char.
	// you should decide schar vs. uchar, but probably makes no difference. 
	while(is.good() && (((p = (char)(is.peek())) == ch) || 
		((t.find(p) != string::npos) && (t.find(ch)  != string::npos))) )
	{
		is.get(ch);
		buf += ch;
	}
}
else             // alphanumeric, and non-leading hyphens, apostrophes all legal
{
	buf += ch;
	int test = is.peek();
	while(is.good() && (isalnum(test) || (test == '-') || (test == '\'')))
	{
		is.get(ch);
		buf += ch;
		if(ch == '-')					// if the char is a hyphen, extract and discard
			is >> ws;					// trailing or line-ending whitespace, if any
		test = is.peek();
	}
}
return(buf.length());
}                    		// gettoken
//----------------------------------------------------------------------------
// returns true if token is a type that terminates a sentence
bool isterminator(const string& s)
{
if( (s == ".") || (s == ";") ||
// 	(s == ":") || 					// unsure about ":". for now, it's not a term.
	(s.find("...") != string::npos) || (s.find("?") != string::npos) || (s.find("!") != string::npos) )
		return(true);
return(false);
}                     		//isterminator
//----------------------------------------------------------------------------
/*	get a sentence from a string
	returns the index of the location in the provided string that is 1 char beyond
	all skipped chars and the gotten sentence (might be beyond end of string, too).
	or 0 if no sentence was created.  0 also means there is nothing left to get.
	You can use: while(getsentence()){...};
	text block to scan, starting position, sentence holder
*/
uint getsentence(const string& source, uint startpos, string& sentence)
{
sentence.clear();  				// always empty it at start
if(startpos >= source.length())		// source used up?
	return(0);
string token;
while((startpos = gettoken(source,startpos,token)) != 0)
{
	sentence += token + " ";
	if(isterminator(token))    		// end of sentence?
		break;
}
if(sentence.length() && !isterminator(token)) 	// if it has contents, and no terminator,
	sentence += ".";							// give it one.
stripstring(sentence);				// new fn, see above.

// if gettoken failed, we might first have gotten a sentence before it did.
// so THIS function shouldn't report failure until the next call.
if(!startpos && sentence.length())
	startpos = source.length();		// set startpos so next call fails

return(startpos);
}                    		//getsentence
//----------------------------------------------------------------------------
/*	extract a sentence from a stream.
	returns number of chars in the retrieved sentence.  if 0, there is also nothing more to get.
	You can use: while(getsentence()){...};
*/	
uint getsentence(istream& is, string& sentence)
{
sentence.clear();  				// always empty it at start
string token;
while(gettoken(is,token))
{
	sentence += token + " ";
	if(isterminator(token))    		// end of sentence?
		break;
}
if(sentence.length() && !isterminator(token)) 	// if it has contents, and no terminator,
	sentence += ".";							// give it one.
stripstring(sentence);
return(sentence.length());
}                    		//getsentence
//----------------------------------------------------------------------------
// Translate Control Chars fns
//----------------------------------------------------------------------------
// replaces control chars in a string with their corresponding ASCII mnemonics.
// CRLF turns into the STRING "<CR><LF>", etc.
// returns a string containing the translation; source string s is unchanged.
string TranslateControls(const string& s)
{
static const char* ControlText[32] =
{
	"<NUL>","<SOH>","<STX>","<ETX>","<EOT>","<ENQ>","<ACK>","<BEL>","<BS>","<HT>",
	"<LF>","<VT>","<FF>","<CR>","<SO>","<SI>","<DLE>","<DCl>","<DC2>","<DC3>",
	"<DC4>","<NAK>","<SYN>","<ETB>","<CAN>","<EM>","<SUB>","<ESC>","<FS>","<GS>",
	"<RS>","<US>"
};
string t;
int ch;
for(size_t i = 0 ; i < s.length() ; i++)
{
	ch = s[i] & 0xFF;			// mapdown into ASCII range
	if(ch < 32)
		t += ControlText[ch];	// control code
	else
		if(ch == 127)        	// special, DEL is out of normal sequence
			t += "<DEL>";
		else
			t += s[i];			// normal char (retains original parity bit, if set)
}
return(t);
}                 		//TranslateControls
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
// TRIG MATH FUNCTIONS
//----------------------------------------------------------------------------
double cosine(double degrees) { return(cos(degrees * M_PI_180)); }	// radians = degrees * pi/180
double sine(double degrees) { return(sin(degrees * M_PI_180)); }	// radians = degrees * pi/180
//----------------------------------------------------------------------------
// whole-degrees sine/cosine tables
// constructor
Trigtable::Trigtable()
{
for(int i = 0 ; i < 360 ; i++)
{
	cosines[i] = cosine(i);
	sines[i] = sine(i);
}
// set some exact entries to avoid rounding errors.
cosines[90] = cosines[270] = sines[0] = sines[180] = 0;
cosines[0] = sines[90] = 1;
cosines[180] = sines[270] = -1;
sines[30] = sines[150] = cosines[60] = cosines[300] = .5;
sines[330] = sines[210] = cosines[120] = cosines[240] = -.5;
}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
// Some console utility fns.
//----------------------------------------------------------------------------
/*	says press any key to continue, and waits for a key.  erases its message
	to leave a cleaner screen, but moves to new line anyway so that
	if echoed to printer with ^P, subsequent text won't overwrite.

	The default os is cout, but can be, for example, a constream.
	12-2-99 returns the char you pressed so it can be used.
*/
int presskey(ostream& os)
{
os << "Press ENTER to continue...";
int ch = getchar();
os << "\r                            " << endl;
return(ch);
}
//----------------------------------------------------------------------------
/*	prints a specified error message and aborts program execution
	"plain" version.  see graborts() for version that calls closegraph().

	Advantage of this over BC4 abort() is that this will cause destructors
	to be called for all objects, while abort() calls NO destructors, not
	even for globals (PG:147). Any open fstreams are closed by their destructors.
	On the other hand, calling destructors in a crashing program may not be good. Oh well.
*/
void aborts(const string& s)
{
cerr << endl << s << endl;
presskey();        			// prevent msg scrolling off before being seen
exit(1);
}

//----------------------------------------------------------------------------
/*	Asks user a yes/no question, using either cout or a provided constream.
	returns true if the answer is yes, false if no.

	prompt is a string, not a string&, so it can have a default value (see my.h)
	(Figure out and document somewhere why I decided a reference parm couldn't be given
	a default value.)
*/
bool yesno(string prompt, ostream& os)
{
char ch;
string discard;
os << endl;
do
{
	os << prompt << " (y/n)? ";
	cin >> ch;
	ch = tolower(ch);
	getline(cin,discard);	// must discard anything extra up to the \n or it causes next getline() to fail
}
while((ch != 'y') && (ch != 'n'));
return(ch == 'y');
}
//----------------------------------------------------------------------------
// Untested.
//int FontHeightInPixels(int pointsize)
//{
//	TScreenDC dc;
//	return((long)(-pointsize) * (long)(dc.GetDeviceCaps(LOGPIXELSY)) / 72L);
//}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
/*	FINANCIAL CALCULATIONS
	References for some of the methods
	--Wikipedia
	--How To Make Money In Stock Options, by Norman Saint-Peter.
*/
//----------------------------------------------------------------------------------------------
/*	COMPOUND INTEREST
	present value, future value, imputed compound interest rate per period.
	NumPeriods and InterestPerPeriod are relative to each other, regardless of the units used for either.
*/	
//----------------------------------------------------------------------------------------------
double PresentValue(double NumPeriods, double InterestPerPeriod, double FutureValue)
{
return FutureValue / pow(1 + InterestPerPeriod, NumPeriods); 
}
double FutureValue(double NumPeriods, double InterestPerPeriod, double PresentValue)
{
return PresentValue * pow(1 + InterestPerPeriod, NumPeriods); 
}
double ImputedInterest(double PresentValue, double FutureValue, double NumPeriods)
{
return pow(FutureValue / PresentValue, 1 / NumPeriods) - 1; 
}
double NumPeriods(double PresentValue, double FutureValue, double InterestPerPeriod)
{
return (log(FutureValue) - log(PresentValue)) / log(1 + InterestPerPeriod); 
}
//----------------------------------------------------------------------------------------------
/*	252 is generally used as the number of trading days in a year, 21 in a month.
	Those are what the values returned by this function round to, except for February.
	When calculating time to option expiration, you usually know the number of 
	calendar days, not trading days, so this can be used to convert the number.
	365.25 days in a year, 5/7 are weekdays, - 9 U.S. trading holidays
	The multiplier is 0.68964505720152537400997359929598
*/
double TradingDaysIn(double CalendarDays)
{
return CalendarDays * (((365.25 * 5/7) - 9) / 365.25);
}
// How many calendar days are required for the given number of trading days to occur.
// The multiplier is 1.4500212675457252233092301148447
double CalendarDaysFor(double TradingDays)
{
return TradingDays * (365.25 / ((365.25 * 5/7) - 9));
}
//----------------------------------------------------------------------------------------------
/*	Given a current stock price, its volatility (as a decimal, not percentage), and a time period, these functions compute the 
	boundaries that the stock could be expected to stay within during a time period (with 68% probability).
	There is a 16% probability it will be above UpperLimit, and 16% probability it will be below LowerLimit.
	These are really just the 1 standard deviation points that define the price envelope at the end of the time period.
	Using these as indicators of "potential" upside and downside price targets is highly questionable 
	because the choice to use 1 standard deviation is arbitrary.
	TimePeriod is a fraction (or multiple) of the Volatility period: if volatility is annualized, 
	TimePeriod is the fraction of 1 year.
*/	 
double UpperLimitPrice84(double CurrentPrice, double VolatilityPerPeriod, double TimePeriod)
{
return CurrentPrice * exp(sqrt(TimePeriod) * VolatilityPerPeriod); 
}
double LowerLimitPrice84(double CurrentPrice, double VolatilityPerPeriod, double TimePeriod)
{
return CurrentPrice * exp(sqrt(TimePeriod) * (-VolatilityPerPeriod)); 
}
//----------------------------------------------------------------------------------------------
/*	Given the current stock price, a price objective, and an annualized volatility (as a decimal), 
	calculates the number of CALENDAR days it should take for the stock to reach the objective.
	This equation is derived from the "potential upside and downside stock prices" equations.
	What this derivation does is start with the envelope and calculate at what 
	point in time the 1 standard deviation point first reaches the target price. 
	The probability that the price will actually have reached or exceeded the objective price is only 16%.
	Thus, this calculation has a lot more uncertainty in it than its name implies. 
*/	
double CalendarDaysToReachPrice16(double CurrentPrice, double TargetPrice, double Volatility)
{
double diff = log(TargetPrice) - log(CurrentPrice);
double yearfrac = (diff * diff) / (Volatility * Volatility);
// We now have the estimated time in fraction of a year.
// .5 trading year = .5 calendar years (it's just a different number of days),
// so we can simply multiply by the number of days in the type of year we want.
return yearfrac * 365.25;
}
//----------------------------------------------------------------------------------------------
/*	Given current price, a target price, annualized volatility (as a decimal), 
	years (or fraction) from now till target date,
	calculates the probability of stock price being below the target. 
	TimePeriod is a fraction (or multiple) of the Volatility period: 
	if volatility is annualized, TimePeriod is the fraction of 1 year.
*/
double ProbabilityStockPriceBelow(double CurrentPrice, double TargetPrice, double VolatilityPerPeriod, double TimePeriod)
{
return StandardNormalPx(log(TargetPrice / CurrentPrice) / (VolatilityPerPeriod * sqrt(TimePeriod)));
}
//----------------------------------------------------------------------------------------------
// Probability of stock price being above the target price. 
double ProbabilityStockPriceAbove(double CurrentPrice, double TargetPrice, double VolatilityPerPeriod, double TimePeriod)
{
return StandardNormalQx(log(TargetPrice / CurrentPrice) / (VolatilityPerPeriod * sqrt(TimePeriod)));
}
//----------------------------------------------------------------------------------------------
// Functions for calculating value of a stock option using the Black-Scholes model.
double BlackScholesDen1(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear) 
{
return (log(Current / Strike) + ((TBillRate + ((Volatility * Volatility) / 2)) * FractionalYear)) / (Volatility * sqrt(FractionalYear)); 
}
//----------------------------------------------------------------------------------------------
double BlackScholesDen2(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear)
{ 
return (log(Current / Strike) + ((TBillRate - ((Volatility * Volatility) / 2)) * FractionalYear)) / (Volatility * sqrt(FractionalYear)); 
}
//----------------------------------------------------------------------------------------------
double BlackScholesCallHedgeRatio(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear)
{
return StandardNormalPx(BlackScholesDen1(Current, Strike, TBillRate, Volatility, FractionalYear));
}
//----------------------------------------------------------------------------------------------
double BlackScholesPutHedgeRatio(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear)
{
return BlackScholesCallHedgeRatio(Current, Strike, TBillRate, Volatility, FractionalYear) - 1;
}
//----------------------------------------------------------------------------------------------
double BlackScholesCallValue(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear)
{
double a = (Current * BlackScholesCallHedgeRatio(Current, Strike, TBillRate, Volatility, FractionalYear));
double b = (StandardNormalPx(BlackScholesDen2(Current, Strike, TBillRate, Volatility, FractionalYear)));
double d = (Strike * exp(TBillRate * (-FractionalYear)));
return a - (b * d);
}
//----------------------------------------------------------------------------------------------
double BlackScholesPutValue(double Current, double Strike, double TBillRate, double Volatility, double FractionalYear)
{
double a = (Current * (StandardNormalPx(-BlackScholesDen1(Current, Strike, TBillRate, Volatility, FractionalYear))));
double b = (StandardNormalPx(-BlackScholesDen2(Current, Strike, TBillRate, Volatility, FractionalYear)));
double d = (Strike * exp(-TBillRate * FractionalYear)); 
return (b * d) - a;
}
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
//	FUNCTIONS RELATED TO PROBABILITY CALCULATIONS
//----------------------------------------------------------------------------------------------
/*	Calculates a point Z(x), the Probability Density Function, on any normal curve. 
	This is the height of the point ON the normal curve.
	For values on the Standard Normal Curve, call with Mean = 0, StdDev = 1.
*/
double NormalDensityZx(double x, double Mean, double StdDev)
{
double a = x - Mean;
return exp(-(a * a) / (2 * StdDev * StdDev)) / (sqrt(2 * M_PI) * StdDev); 
}
//----------------------------------------------------------------------------------------------
// Calculates Q(x), the right tail area under the Standard Normal Curve. 
double StandardNormalQx(double x)
{
if(x == 0)	// no approximation necessary for 0
	return 0.50;
	
double t1,t2,t3,t4,t5,qx;
bool negative = false;
if(x < 0)
{
	x = -x;
	negative = true;
}
t1 = 1 / (1 + (0.2316419 * x)); 
t2 = t1 * t1; 
t3 = t2 * t1; 
t4 = t3 * t1; 
t5 = t4 * t1;
qx = NormalDensityZx(x,0,1) * ((0.319381530 * t1) + (-0.356563782 * t2) + 
							(1.781477937 * t3) + (-1.821255978 * t4) + (1.330274429 * t5));
if(negative == true)
	qx = 1 - qx;
return qx;
}
//----------------------------------------------------------------------------------------------
// Calculates P(x), the left tail area under the Standard Normal Curve, which is 1 - Q(x). 
double StandardNormalPx(double x)
{
return 1 - StandardNormalQx(x);
}
//----------------------------------------------------------------------------------------------
// Calculates A(x), the area under the Standard Normal Curve between +x and -x. 
double StandardNormalAx(double x)
{
return 1 - (2 * StandardNormalQx(fabs(x)));
}
//----------------------------------------------------------------------------------------------
// Area under the Standard Normal Curve between z-scores x and y. 
double StandardNormalAxy(double x, double y)
{
if(x > y)		// x is supposed to be the lower value
	swapvalues(x,y);
return StandardNormalPx(y) - StandardNormalPx(x);
}
//----------------------------------------------------------------------------------------------
// END PROBABILITY CALCULATIONS
//////////////////////////////////////////////////////////////////////////////
/*	MODIFIED GNU SCIENTIFIC LIBRARY FUNCTIONS
	The functions in this section are copied and greatly modified from the 
	GNU Scientific Library version 1.12, specifically from exponential.c, levy.c. 
	Copyright (C) 1996, 1997, 1998, 1999, 2000, 2007 James Theiler, Brian Gough
	Copyright (C) 2009 Steven Whitney. The following modifications made:
	2-26-2009:
	Converted to Visual C++, with C++ variable declaration conventions;
	substituted alternative methods for some calls to external GSL functions;
	used different function names, condensed, reorganized, and reformatted.
*/	
//----------------------------------------------------------------------------
// RANDOM DOUBLE FROM EXPONENTIAL DISTRIBUTION
// The exponential distribution has the form p(x) dx = exp(-x/mu) dx/mu, for x = 0 to +infinity 
double GSLRandomExponential(double mu)
{
double u;
do			// emulate gsl_rng_uniform_pos()
{
	u = frand();		// returns [0-1)
}
while(u == 0);			// discard 0
return -mu * log(u);
}
//----------------------------------------------------------------------------
/*	RANDOM DOUBLE FROM LEVY ALPHA-STABLE DISTRIBUTION
	The stable Levy probability distributions have the form
	p(x) dx = (1/(2 pi)) \int dt exp(- it x - |c t|^alpha)
	with 0 < alpha <= 2. 

	For alpha = 1, we get the Cauchy distribution
	For alpha = 2, we get the Gaussian distribution with sigma = sqrt(2) c.

	From Chapter 5 of Bratley, Fox and Schrage "A Guide to Simulation". The original reference given there is,
	J.M. Chambers, C.L. Mallows and B. W. Stuck. "A method for simulating stable random variates". 
	Journal of the American Statistical Association, JASA 71 340-344 (1976).

	See also: 
	http://en.wikipedia.org/wiki/Stable_distributions 
	http://en.wikipedia.org/wiki/L%C3%A9vy_distribution
	Apparent properties of the values returned: 
	mean (mu) = 0; 
	c = scale (dispersion) parameter, analogous to standard deviation in a normal distribution.
	alpha = kurtosis. High alpha -> broad peak, platykurtotic. Low alpha -> pointy with fat tails, leptokurtotic.
	This function does not provide for a beta (skewness) value.
*/
double GSLRandomLevy(double c, double alpha)
{
// enforce limits by returning a value that will be obviously wrong (all zeroes)
if((alpha <= 0) || (alpha > 2))	
	return 0;
	
double u;
do			// emulate gsl_rng_uniform_pos()
{
	u = frand();			// returns [0.0-1.0)
}
while(u == 0);				// discard 0
u = M_PI * (u - 0.5);

if(alpha == 1)				// Cauchy case 
	return c * tan(u);

double v;
do			// emulate gsl_ran_exponential()
{
	v = GSLRandomExponential(1.0);
}
while(v == 0);

if(alpha == 2)				// Gaussian case 
	return c * (2 * sin(u) * sqrt(v));

// general case
double t = sin(alpha * u) / pow(cos(u), 1 / alpha);
double s = pow(cos((1 - alpha) * u) / v, (1 - alpha) / alpha);
return c * t * s;
}
//----------------------------------------------------------------------------
// END OF MODIFIED GNU SCIENTIFIC LIBRARY FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
