#ifndef INC_INTERVAL
#define INC_INTERVAL

/*
 *******************************************************************************
 *
 *
 *                       Copyright (c) 2002-2014
 *                       Future Team Aps 
 *                       Denmark
 *
 *                       All Rights Reserved
 *
 *   This source file is subject to the terms and conditions of the
 *   Future Team Software License Agreement which restricts the manner
 *   in which it may be used.
 *   Mail: hve@hvks.com
 *
 *******************************************************************************
*/

/*
 *******************************************************************************
 *
 *
 * Module name     :   intervalprecision.h
 * Module ID Nbr   :   
 * Description     :   Interval Precision arithmetic template class
 *                     Works with the int_precision and float_precision classes
 * --------------------------------------------------------------------------
 * Change Record   :   
 *
 * Version	Author/Date		Description of changes
 * -------  -----------		----------------------
 * 01.01	HVE/020209		Initial release
 * 01.02    HVE/030421		Optimized the * operator to reduce the 8 multiplications to 2.
 * 01.03	HVE/JUN-26-2014	Added is_empty(), contains_zero() method to the class
 * 01.04	HVE/JUN-27-2014	Corrected several errors in in cin >> template function
 * 01.05	HVE/JUN-28-2014	Added is_class() method for getting the interval classification
 *							and width() method for the interval width
 *
 * End of Change Record
 * --------------------------------------------------------------------------
*/


/* define version string */
static char _VinterP_[] = "@(#)intervalprecision.h 01.05 -- Copyright (C) Future Team Aps";

#include <float.h>

// HARDWARE_SUPPORT controlled if IEEE754 floating point control can be used for interval arithmetic.
// if not used interval arithmetic will be handle in software
//#define HARDWARE_SUPPORT

/// The four different interval classification
/// # ZERO			a=0 && b=0
/// # POSITIVE		a>=0 && b>0
/// # NEGATIVE		a<0 && b<=0
/// # MIXED			a<0 && b>0
enum int_class { NO_CLASS, ZERO, POSITIVE, NEGATIVE, MIXED };

// 
// Interval class
// Realisticly the class Type can be float, double, int_precision, & float_precision
// Since float and double are done unsing the Intel cpu (H/W) and using "specilization" 
// the int_precision and float_precision is done using the arbitrary precision packages
// Since their is no way to specific portable ROUND_DOWN and ROUND_UP mode the template class 
// heavily use specilization. For any other type besides float, double, int_precision and float_precision
// the operations is not defined
//
template<class _IT> class interval {
   _IT low, high;
   public:
      typedef _IT value_type;

      // constructor. zero, one or two arguments 
      interval( const _IT& l, const _IT& h) { if( l < h ) { low =l; high = h; } else { low = h; high = l; } }
      interval( const _IT& d = _IT(0) ) { low=d; high=d; }

      // constructor for any other type to _IT
      template<class X> interval( const interval<X>& a ) : low(_IT(a.lower())), high( _IT(a.upper()))  { if( a.lower() < a.upper() ) { low =_IT(a.lower()); high = _IT(a.upper()); } else { low = _IT(a.upper()); high = _IT(a.lower()); } }
      
      // Coordinate functions
      _IT upper() const				{ return high; }
      _IT lower() const				{ return low; }
      _IT upper( const _IT& u )		{ return ( high = u ); }
      _IT lower( const _IT& l )		{ return ( low = l ); }

      _IT center() const			{ return ( high + low ) / _IT(2); }
      _IT radius() const			{ _IT r; r =( high - low ) / _IT(2); if( r < _IT(0) ) r = -r; return r; }
	  _IT width() const				{ _IT r; r = high - low; if (r < _IT(0)) r = -r; return r; }
	  bool contains_zero() const	{ return low <= _IT(0) && _IT(0) <= high; }
	  bool is_empty() const			{ return high < low; }
	  enum int_class is_class() const{ 
									if (low == _IT(0) && high == _IT(0)) return ZERO;
									if (low >= _IT(0) && high > _IT(0)) return POSITIVE;
									if (low < _IT(0) && high <= _IT(0)) return NEGATIVE;
									if (low < _IT(0) && high > _IT(0)) return MIXED;
									return NO_CLASS;
									}
      operator double() const		{ return (double)( ( high + low ) / _IT(2) ); }	// Conversion to Double 
      operator int() const			{ return (int)( ( high + low ) / _IT(2) ); }	// Conversion to int 

      _IT *ref_lower()				{ return &low; }
      _IT *ref_upper()				{ return &high; }

      // Essential operators
      interval<_IT>& operator= ( const interval<_IT>& );
      interval<_IT>& operator+=( const interval<_IT>& );
      interval<_IT>& operator-=( const interval<_IT>& );
      interval<_IT>& operator*=( const interval<_IT>& );
      interval<_IT>& operator/=( const interval<_IT>& );
	  interval<_IT>& operator&=( const interval<_IT>& );
	  interval<_IT>& operator|=( const interval<_IT>& );
	  interval<_IT>& operator^=( const interval<_IT>& );
   };


// Unary and Binary arithmetic
template<class _IT> interval<_IT> operator+( const interval<_IT>&, const interval<_IT>& );
template<class _IT> interval<_IT> operator+( const interval<_IT>& );  // Unary 
template<class _IT> interval<_IT> operator-( const interval<_IT>&, const interval<_IT>& );
template<class _IT> interval<_IT> operator-( const interval<_IT>& );                 // Unary
template<class _IT> interval<_IT> operator*( const interval<_IT>&, const interval<_IT>& );
template<class _IT> interval<_IT> operator/( const interval<_IT>&, const interval<_IT>& );

template<class _IT> interval<_IT> operator&( const interval<_IT>&, const interval<_IT>& );
template<class _IT> interval<_IT> operator|( const interval<_IT>&, const interval<_IT>& );
template<class _IT> interval<_IT> operator^( const interval<_IT>&, const interval<_IT>& );

// Boolean Comparision Operators
template<class _IT> bool operator==(const interval<_IT>&, const interval<_IT>&);
template<class _IT> bool operator!=(const interval<_IT>&, const interval<_IT>&);

// Other functions
template<class _IT> interval<_IT> abs(const interval<_IT>&);

inline interval<float> sqrt(const interval<float>&);
inline interval<double> sqrt(const interval<double>&);
inline interval<float_precision> sqrt(const interval<float_precision>&);
inline interval<float> log( const interval<float>& );
inline interval<double> log(const interval<double>&);
inline interval<float_precision> log(const interval<float_precision>&);
inline interval<float> log10(const interval<float>&);
inline interval<double> log10(const interval<double>&);
inline interval<float_precision> log10(const interval<float_precision>&);
inline interval<float> pow(const interval<float>&, const float );
inline interval<double> pow( const interval<double>&, const double );
inline interval<float_precision> exp( const interval<float_precision>& );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Floating point control for the IEEE754 hardware. Only fo non managed application
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HARDWARE_SUPPORT
inline void fpnear()
{
	unsigned int currentControl;
	_controlfp_s( &currentControl, _RC_NEAR, _MCW_RC);
}

inline void fpdown()
{
	unsigned int currentControl;
	_controlfp_s( &currentControl, _RC_DOWN, _MCW_RC);
}

inline void fpup()
{
	unsigned int currentControl;
	_controlfp_s( &currentControl, _RC_UP, _MCW_RC);
}
#else
inline void fpnear()	{}
inline void fpdown()	{}
inline void fpup()		{}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   End Floating point control for the IEEE754 hardware
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Output Operator <<
//
template<class _Ty> inline std::ostream& operator<<( std::ostream& strm, interval<_Ty>& a ) { return strm << "[" << a.lower() << "," << a.upper() << "]"; }

// Input operator >>
//
template<class _Ty> inline std::istream& operator>>( std::istream& strm, interval<_Ty>& c ) 
   {
   _Ty l, u; char ch;
   if( strm >> ch && ch != '[')
      strm.putback(ch), strm >> l, u = l;
	else
      if( strm >> l >> ch && ch != ',')
	      if( ch == ']')
	         u = l;
	      else 
            strm.putback( ch ); // strm.setstate(std::ios::failbit);
	   else
         if( strm >> u >> ch && ch != ']')
	         strm.putback( ch ); //, strm.setstate(ios_base::failbit);
	
   if(!strm.fail())
	   c = interval<_Ty>( l, u );

   return strm;
   }
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Essential Operators =,+=,-=,*=,/=
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////


// Assignment operator. Works for all class types
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator=( const interval<_IT>& a )
   {
   low = a.lower();
   high = a.upper();
   return *this;
   }


// += operator. Works all other classes. 
// Please note that this is for all integer classes. interval<int>, interval<long>, interval<int_precision>
// were there os no loss of precision
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator+=( const interval<_IT>& a )
   {
   low += a.lower();
   high += a.upper();
   return *this;
   }

// Specilization for float_precision and +=
//
inline interval<float_precision>& interval<float_precision>::operator+=( const interval<float_precision>& a )
   {
   low.mode( ROUND_DOWN );
   low += a.lower();
   high.mode( ROUND_UP );
   high += a.upper();
   
   return *this;
   }


// Specilization for float and +=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<float>& interval<float>::operator+=( const interval<float>& a )
	{
#ifdef HARDWARE_SUPPORT
	fpdown();
	low += a.lower();
	fpup();
	high += a.upper();
	fpnear();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs += lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}


// Specilization for double and +=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<double>& interval<double>::operator+=( const interval<double>& a )
	{
#ifdef HARDWARE_SUPPORT
	fpdown();
	low += a.lower();
	fpup();
	high += a.upper();
	fpnear();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs += lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}


// -= operator. Works all other classes. 
// Please note that this is for all integer classes. interval<int>, interval<long>, interval<int_precision>
// were there is no loss of precision
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator-=( const interval<_IT>& a )
   {
   low -= a.high;
   high -= a.low;
   return *this;
   }

// Specilization for float_precision and -=
//
inline interval<float_precision>& interval<float_precision>::operator-=( const interval<float_precision>& a )
   {
   low.mode( ROUND_DOWN );
   low -= a.lower();
   high.mode( ROUND_UP );
   high -= a.upper();
   
   return *this;
   }

// Specilization for float and -=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<float>& interval<float>::operator-=( const interval<float>& a )
	{
#ifdef HARDWARE_SUPPORT
	fpdown();
	low -= a.high;
	fpup();
	high -= a.low;
	fpnear();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs -= lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}

// Specilization for double and -=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<double>& interval<double>::operator-=( const interval<double>& a )
	{
#ifdef HARDWARE_SUPPORT
	fpdown();
	low -= a.high;
	fpup();
	high -= a.low;
	fpnear();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs -= lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}

// Works all other classes. 
// Please note that this is for all interger classes. interval<int>, interval<long>, interval<int_precision>
// were there is no loss of precision
// Instead of doing the mindless low = MIN(low*a.high, low*a.low,high*a.low,high*a.high) and
// high = MAX(low*a.high, low*a.low,high*a.low,high*a.high) requiring a total of 8 multiplication
//
//   low, high, a.low, a.high    result
//    +     +     +     +        +  +  [ low*a.low, high*a.high ]
//    +     +     -     +        -  +  [ high*a.low, high*a.high ]
//    +     +     -     -        -  -  [ high*a.low, low*a.high ]
//    -     +     +     +        -  +  [ low*a.high, high*a.high ]  
//    -     +     -     +        -  +  [ MIN(low*a.high,high*a.low), MAX(low*a.low,high*a.high) ]
//    -     +     -     -        -  -  [ high*a.low, low*a.low ]
//    -     -     +     +        -  -  [ low*a.high, high,a.low ]
//    -     -     -     +        -  -  [ low*a.high, low*a.low ]
//    -     -     -     -        +  +  [ high*a.high, low * a.low ]
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator*=( const interval<_IT>& a )
   {
   _IT l, h, t;

   if( low >= _IT(0) ) // 
      { // both low and high >= 0
      if( a.lower() >= _IT(0) )
         { // a.low >=0, a.high >= 0
         l = low * a.lower();
         h = high * a.upper();
         }
      else
         if( a.upper() >= _IT(0) )
            {  //  a.low < 0, a.high >= 0
            l = high * a.lower();
            h = high * a.upper();
            }
         else
            { // a.low and a.high < 0 
            l = high * a.lower();
            h = low * a.upper();
            }
      }
   else
      if( high >= _IT(0) )
         {  // low < 0, high >= 0
         if( a.lower() >= _IT(0) )
            { // a.low >=0, a.high >= 0
            l = low * a.upper();
            h = high * a.upper();
            }
         else
            if( a.upper() >= _IT(0) )
               {  //  a.low < 0, a.high >= 0
               l = low * a.upper(); if ( l > ( t = high * a.lower() ) ) l = t;
               h = high * a.upper(); if ( h < ( t = low * a.lower() ) ) h = t;
               }
            else
               { // a.low and a.high < 0 
               l = high * a.lower();
               h = low * a.lower();
               }
         }
      else
         { // low and high are < 0 
         if( a.lower() >= _IT(0) )
            { // a.low >=0, a.high >= 0
            l = low * a.upper();
            h = high * a.lower();
            }
         else
            if( a.upper() >= _IT(0) )
               {  //  a.low < 0, a.high >= 0
               l = low * a.upper(); 
               h = low * a.lower();
               }
            else
               { // a.low and a.high < 0 
               l = high * a.upper();
               h = low * a.lower();
               }
        }

   low = l;
   high = h;

   return *this;
   }


// Specilization for float_precision and *= operator
//
inline interval<float_precision>& interval<float_precision>::operator*=( const interval<float_precision>& a )
   {
   float_precision l, h, t;

   l.precision( low.precision() );
   h.precision( low.precision() );   
   t.precision( low.precision() );

   l.mode( ROUND_DOWN );
   h.mode( ROUND_UP );

   if( low.sign() > 0 ) // 
      { // both low and high >= 0
      if( a.lower().sign() > 0 )
         { // a.low >=0, a.high >= 0
         l = low;  l *= a.lower();
         h = high; h *= a.upper();
         }
      else
         if( a.upper().sign() > 0 )
            {  //  a.low < 0, a.high >= 0
            l = high;  l *= a.lower();
            h = high; h *= a.upper();
            }
         else
            { // a.low and a.high < 0 
            l = high; l *= a.lower();
            h = low;  h *= a.upper();
            }
      }
   else
      if( high.sign() > 0 )
         {  // low < 0, high >= 0
         if( a.lower().sign() > 0 )
            { // a.low >=0, a.high >= 0
            l = low;  l *= a.upper();
            h = high; h *= a.upper();
            }
         else
            if( a.upper().sign() > 0 )
               {  //  a.low < 0, a.high >= 0
               t.mode( ROUND_DOWN );
               l = low;  l *= a.upper(); if( l > ( t = high, t *= a.lower() ) ) l = t;
               t.mode( ROUND_UP );
               h = high; h *= a.upper(); if( h < ( t = low, t *= a.lower() ) ) h = t;
               }
            else
               { // a.low and a.high < 0 
               l = high; l *= a.lower();
               h = low;  h *= a.lower();
               }
         }
      else
         { // low and high are < 0 
         if( a.lower().sign() > 0 )
            { // a.low >=0, a.high >= 0
            l = low;  l *= a.upper();
            h = high; h *= a.lower();
            }
         else
            if( a.upper().sign() > 0 )
               {  //  a.low < 0, a.high >= 0
               l = low; l *= a.upper(); 
               h = low; h *= a.lower();
               }
            else
               { // a.low and a.high < 0 
               l = high; l *= a.upper();
               h = low; h *= a.lower();
               }
        }

   low = l;
   high = h;

   return *this;
   }

// Specilization for float and *=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<float>& interval<float>::operator*=( const interval<float>& a )
   {
#ifdef HARDWARE_SUPPORT
	_IT l, h, t;

	if( low >= _IT(0) ) // 
	{ // both low and high >= 0
		if( a.lower() >= _IT(0) )
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.lower();
			fpup();
			h = high * a.upper();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = high * a.lower();
			fpup();
			h = high * a.upper();
		}
		else
		{ // a.low and a.high < 0
			fpdown();
			l = high * a.lower();
			fpup();
			h = low * a.upper();
		}
	}
	else
	if (high >= _IT(0))
	{  // low < 0, high >= 0
		if (a.lower() >= _IT(0))
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = high * a.upper();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = low * a.upper(); if (l > (t = high * a.lower())) l = t;
			fpup();
			h = high * a.upper(); if (h < (t = low * a.lower())) h = t;
		}
		else
		{ // a.low and a.high < 0 
			fpdown();
			l = high * a.lower();
			fpup();
			h = low * a.lower();
		}
	}
	else
	{ // low and high are < 0 
		if (a.lower() >= _IT(0))
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = high * a.lower();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = low * a.lower();
		}
		else
		{ // a.low and a.high < 0 
			fpdown();
			l = high * a.upper();
			fpup();
			h = low * a.lower();
		}
	}

	low = l;
	high = h;
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs *= lhs;

	low=rhs.lower();
	high=rhs.upper();
#endif
   return *this;
   }

// Specilization for double and *=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<double>& interval<double>::operator*=( const interval<double>& a )
	{
#ifdef HARDWARE_SUPPORT
	_IT l, h, t;

	if (low >= _IT(0)) // 
	{ // both low and high >= 0
		if (a.lower() >= _IT(0))
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.lower();
			fpup();
			h = high * a.upper();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = high * a.lower();
			fpup();
			h = high * a.upper();
		}
		else
		{ // a.low and a.high < 0
			fpdown();
			l = high * a.lower();
			fpup();
			h = low * a.upper();
		}
	}
	else
	if (high >= _IT(0))
	{  // low < 0, high >= 0
		if (a.lower() >= _IT(0))
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = high * a.upper();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = low * a.upper(); if (l > (t = high * a.lower())) l = t;
			fpup();
			h = high * a.upper(); if (h < (t = low * a.lower())) h = t;
		}
		else
		{ // a.low and a.high < 0 
			fpdown();
			l = high * a.lower();
			fpup();
			h = low * a.lower();
		}
	}
	else
	{ // low and high are < 0 
		if (a.lower() >= _IT(0))
		{ // a.low >=0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = high * a.lower();
		}
		else
		if (a.upper() >= _IT(0))
		{  //  a.low < 0, a.high >= 0
			fpdown();
			l = low * a.upper();
			fpup();
			h = low * a.lower();
		}
		else
		{ // a.low and a.high < 0 
			fpdown();
			l = high * a.upper();
			fpup();
			h = low * a.lower();
		}
	}

	low = l;
	high = h;
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs *= lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}

// Works for all other classes
// Please note that this is for all interger classes. interval<int>, interval<long>
// were there is no loss of precision
// Actually there is specialization for both <int> and <int_precision> further down.
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator/=( const interval<_IT>& b )
   {
   interval<_IT> a, c;

   c.low = (_IT)1 / b.upper();
   c.high = (_IT)1 / b.lower();

   a = interval( low, high );
   c *= a;

   low = c.lower();
   high = c.upper();

   return *this; 
   }

// Specilization for float_precision and /=
//
inline interval<float_precision>& interval<float_precision>::operator/=( const interval<float_precision>& b )
   {
   float_precision l = b.upper(), h = b.lower();
   interval<float_precision> c(b);

   l.mode( ROUND_DOWN );
   l = _float_precision_inverse( l );

   h.mode( ROUND_UP );
   h = _float_precision_inverse( h );

   c = interval<float_precision>( l , h );
   *this *= c;

   return *this;
   }

// Specilization for float and /=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<float>& interval<float>::operator/=( const interval<float>& a )
   {
#ifdef HARDWARE_SUPPORT
	interval<float> b, c;

	fpdown();
	c.low = (float)1 / a.upper();
	fpup();
	c.high = (float)1 / a.lower();
	fpnear();

	b = interval(low, high);
	c *= b;

	low = c.lower();
	high = c.upper();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs /= lhs;

	low=rhs.lower();
	high=rhs.upper();
#endif
   return *this;
   }

// Specilization for double and /=
// That can work with both managed an unmanged application.
// HARDWARE_SUPPORT indicate if we use the build in ROUND_DOWN or ROUND_UP Mode when performing the operation
// For now Hardware support we covert to float_precision to do the interval arithmetic in software
//
inline interval<double>& interval<double>::operator/=( const interval<double>& a )
	{
#ifdef HARDWARE_SUPPORT
	interval<double> b, c;

	fpdown();
	c.low = (double)1 / a.upper();
	fpup();
	c.high = (double)1 / a.lower();
	fpnear();

	b = interval(low, high);
	c *= b;

	low = c.lower();
	high = c.upper();
#else
	interval<float_precision> rhs(*this), lhs(a);

	rhs /= lhs;
	low=rhs.lower();
	high=rhs.upper();
#endif
	return *this;
	}

// Specilization for int_precision and /=
//
inline interval<int_precision>& interval<int_precision>::operator/=( const interval<int_precision>& b )
   {
   float_precision l = b.upper(), h = b.lower();
   interval<float_precision> c(b), a(*this);

   l.mode( ROUND_DOWN );
   l = _float_precision_inverse( l );

   h.mode( ROUND_UP );
   h = _float_precision_inverse( h );

   c = interval<float_precision>( l , h );
   a *= c;

   low = (int_precision)floor( a.lower() );
   high = (int_precision)ceil( a.upper() );

   return *this;
   }


// Specialization for int and /=
//
inline interval<int>& interval<int>::operator/=( const interval<int>& b )
   {
   double tlow, thigh;
   interval<int> a;
   interval<double> c;

   tlow = 1 / (double)b.upper();
   thigh = 1 / (double)b.lower();

   a = interval( low, high );
   c = interval<double>( tlow, thigh );
   c *= a;

   low = (int)floor( c.lower() );
   high = (int)ceil( c.upper() );

   return *this; 
   }

// Works on all classes. 
// Return the intersection
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator&=(const interval<_IT>& a)
	{
	if (a.lower() > low ) 
		low = a.lower();
	if (a.upper() < high)
		high = a.upper();
	if (low > high)  // Empty set
		{
		low = 0; high = 0;
		}

	return *this;
	}

// Works on all classes. 
// Return the union
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator|=(const interval<_IT>& a)
	{
	if (low > a.upper() || high < a.lower())
		{
		if (a.upper() - a.lower() > high - low)
			{ // return the largest set
			low = a.lower();
			high = a.upper();
			}
		}
	else
		{ // non empty intersection
		if (a.lower() < low)
			low = a.lower();
		if (a.upper() > high)
			high = a.upper();
		}
	}

	
// Works on all classes. 
// Return the set minus
//
template<class _IT> inline interval<_IT>& interval<_IT>::operator^=(const interval<_IT>& a)
	{
	if ( a.lower() < high && a.upper() > low ) // intersection is not empty
		{
		if (a.upper() <= low)
			low = a.upper();
		else
			if (a.high() >= high)
				high = a.lower();
		}

	return *this;
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END Essential Operators
///
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Binary and Unary Operators +,-,*,/
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Binary + operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator+( const interval<_IT>& a, const interval<_IT>& b )
   {
   interval<_IT> c(a);

   c += b; 
   return c;
   }


// Unary + operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator+( const interval<_IT>& a )
   {
   return a;
   }

// Binary - operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator-( const interval<_IT>& a, const interval<_IT>& b )
   {
   interval<_IT> c(a);

   c -= b;
   return c;
   }


// Unary - operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator-( const interval<_IT>& a )
   {
   interval<_IT> c(a);

   c = interval();
   c -= a;
   return c;
   }

// Binary * operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator*( const interval<_IT>& a, const interval<_IT>& b )
   {
   interval<_IT> c(a);

   c *= b;
   return c;
   }

// Binary / operator
// Works for all classes
//
template<class _IT> inline interval<_IT> operator/( const interval<_IT>& a, const interval<_IT>& b )
   {
   interval<_IT> c(a);

   //if( c.lower() == b.lower() && c.upper() == b.upper() )
   if ( c == b && b.is_class() != ZERO )
	  c = interval<_IT>(1,1);
   else
      c /= b;
   
   return c;
   }

// Binary & operator
// Return intersection
// Works for all classes
//
template<class _IT> inline interval<_IT> operator&( const interval<_IT>& a, const interval<_IT>& b )
	{
	interval<_IT> c(a);

	c &= b;
	return c;
	}

// Binary | operator.
// Return union
// Works for all classes
//
template<class _IT> inline interval<_IT> operator|(const interval<_IT>& a, const interval<_IT>& b)
	{
	interval<_IT> c(a);

	c |= b;
	return c;
	}

// Binary ^ operator
// Return set minus
// Works for all classes
//
template<class _IT> inline interval<_IT> operator^(const interval<_IT>& a, const interval<_IT>& b)
	{
	interval<_IT> c(a);

	c ^= b;
	return c;
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END Binary and Unary Operators
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// Boolean Interval for == and !=
///
//////////////////////////////////////////////////////////////////////////////////////

template<class _IT> inline bool operator==(const interval<_IT>& a, const interval<_IT>& b)
	{
	return a.lower() == b.lower() && a.upper() == b.upper();
	}

template<class _IT> inline bool operator!=(const interval<_IT>& a, const interval<_IT>& b)
	{
	return a.lower() != b.lower() || a.upper() != b.upper();
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END Boolean operators
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// Interval abs()
///
//////////////////////////////////////////////////////////////////////////////////////

template<class _IT> inline interval<_IT> abs( const interval<_IT>& a )
	{
	if (a.lower() >= _IT(0) )
		return a;
	else
	if (a.upper() <= _IT(0))
		return -a;

	return interval<_IT>(_IT(0), ( a.upper() > -a.lower() ? a.upper() - -a.lower() ) );
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END interval functions
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// Interval sqrt(), log10(), log(), exp() and pow()
///
//////////////////////////////////////////////////////////////////////////////////////

#ifdef HARDWARE_SUPPORT
static double sqrtlower(double l)
	{
	double lower = l;

	fpdown();
	_asm {
		fld qword ptr[ebp - 8h];      Load lower into floating point stack
			fsqrt;                       Calculate sqrt
			fstp qword ptr[ebp - 8h];     Store result in lower
		}
	fpnear();
	return lower;
	}

static double sqrtupper(double u)
	{
	double upper = u;

	fpup();
	_asm {
		fld qword ptr[ebp - 8h];      Load upper into floating point stack
			fsqrt;                       Calculate sqrt
			fstp qword ptr[ebp - 8h];     Store result in upper
	}
	fpnear();
	return upper;
	}
#endif

// Specilization for sqrt(float_precision)
// 
inline interval<float_precision> sqrt( const interval<float_precision>& x )
   {
   float_precision l, u;
   
   l.assign( x.lower() );  // Assign value, precision and mode
   l.mode( ROUND_DOWN );
   l = sqrt( l );

   u.assign( x.upper() );  // Assign value, precision and mode
   u.mode( ROUND_UP );
   u = sqrt( u );

   return interval<float_precision>( l, u );
   }

// sqrt for float using managed code.
//
inline interval<float> sqrt( const interval<float>& x )
   {
   float lower, upper;
   interval<float_precision> fx(x);

   fx=sqrt(fx);
   lower=fx.lower();
   upper=fx.upper();

   return interval<float>( lower, upper );
   }

// sqrt for double using managed code.
//
inline interval<double> sqrt( const interval<double>& x )
   { 
   double lower, upper;
#ifdef HARDWARE_SUPPORT
	lower = sqrtlower(x.lower());
	upper = sqrtupper(x.upper());
#else
	interval<float_precision> fx(x);
   
   fx=sqrt(fx);
   lower=fx.lower();
   upper=fx.upper();
#endif
   return interval<double>( lower, upper );
   }


// Specilization for log float_precision
// 
inline interval<float_precision> log( const interval<float_precision>& x )
   {
   float_precision l, u;
   
   l.assign( x.lower() );  // Assign value, precision and mode
   l.mode( ROUND_DOWN );
   l = log( l );

   u.assign( x.upper() );  // Assign value, precision and mode
   u.mode( ROUND_UP );
   u = log( u );

   return interval<float_precision>( l, u );
   }

// log for float using manged code.
//
inline interval<float> log( const interval<float>& x )
	{
	float lower, upper;
	interval<float_precision> fx(x);

   fx=log(fx);
   lower=fx.lower();
   upper=fx.upper();

   return interval<float>( lower, upper );
   }

// log for double using managed code.
//
inline interval<double> log( const interval<double>& x )
	{
	double lower, upper;
#ifdef HARDWARE_SUPPORT
	lower = x.lower();
	upper = x.upper();

	fpdown();
	_asm {
		fld qword ptr[ebp - 8h];      Load lower into floating point stack
			fldln2;                      Load loge2
			fxch st(1);                  Exchange stack top
			fyl2x;                       Calculate y * ln2 x
			fstp qword ptr[ebp - 8h];     Store result in lower
	}

	fpup();
	_asm {
		fld qword ptr[ebp - 10h];     Load lower into floating point stack
			fldln2;                      Load loge2
			fxch st(1);                  Exchange stack top
			fyl2x;                       Calculate y * ln2 x
			fstp qword ptr[ebp - 10h];    Store result in upper
	}

	fpnear();
#else
	interval<float_precision> fx(x);

   fx=log(fx);
   lower=fx.lower();
   upper=fx.upper();
#endif
   return interval<double>( lower, upper );
   }



// Specilization for log float_precision
// 
inline interval<float_precision> log10( const interval<float_precision>& x )
   {
   float_precision l, u;
   
   l.assign( x.lower() );  // Assign value, precision and mode
   l.mode( ROUND_DOWN );
   l = log10( l );

   u.assign( x.upper() );  // Assign value, precision and mode
   u.mode( ROUND_UP );
   u = log10( u );

   return interval<float_precision>( l, u );
   }

// log10 for float using manged code.
//
inline interval<float> log10( const interval<float>& x )
   {
	interval<float_precision> fx(x);
   float lower, upper;
   
   fx=log10(fx);
   lower=fx.lower();
   upper=fx.upper();
   return interval<float>( lower, upper );
   }

// log10 for double using managed code.
//
inline interval<double> log10( const interval<double>& x )
   {   double lower, upper;
#ifdef HARDWARE_SUPPORT
	lower = x.lower();
	upper = x.upper();

	fpdown();
	_asm {
		fld qword ptr[ebp - 8h];      Load lower into floating point stack
			fldlg2;                      Load log10(2)
			fxch st(1);                  Exchange stack top
			fyl2x;                       Calculate y * ln2 x
			fstp qword ptr[ebp - 8h];     Store result in lower
	}

	fpup();
	_asm {
		fld qword ptr[ebp - 10h];     Load lower into floating point stack
			fldlg2;                      Load log10(2)
			fxch st(1);                  Exchange stack top
			fyl2x;                       Calculate y * ln2 x
			fstp qword ptr[ebp - 10h];    Store result in upper
	}

	fpnear();
#else
	interval<float_precision> fx(x);
 
   fx=log10(fx);
   lower=fx.lower();
   upper=fx.upper();
#endif
   return interval<double>( lower, upper );
   }

   
// Specilization for exp float_precision
// 
inline interval<float_precision> exp( const interval<float_precision>& x )
   {
   float_precision l, u;
   
   l.assign( x.lower() );  // Assign value, precision and mode
   l.mode( ROUND_DOWN );
   l = exp( l );

   u.assign( x.upper() );  // Assign value, precision and mode
   u.mode( ROUND_UP );
   u = exp( u );

   return interval<float_precision>( l, u );
   }

#ifdef NOT_USED
// exp for float using manged code.
//
inline interval<float> exp( const interval<float>& x )
   {
	interval<float_precision> fx(x);
   float lower, upper;
   
   fx=exp(fx);
   lower=fx.lower();
   upper=fx.upper();
   return interval<float>( lower, upper );
   }

// exp for double using managed code.
//
inline interval<double> exp( const interval<double>& x )
   {
	interval<float_precision> fx(x);
   double lower, upper;
   
   fx=exp(fx);
   lower=fx.lower();
   upper=fx.upper();
   return interval<double>( lower, upper );
   }
#endif

// MSC exp() does not alllow rounding control
// So we have to do it manually
// Use a taylor series until their is no more change in the result
// exp(x) == 1 + x + x^2/2!+x^3/3!+....
//
inline interval<float> exp(const interval<float>& x)
	{
	int  i;
	interval<float> c, res, p0, old;
	float dd;

	p0 = x;  dd = 1.0;
	old = interval<float>(dd);
	res = old + p0;
	for (i = 2; i < 30 && (res.lower() != old.lower() && res.upper() != old.upper()); i++)
		{
		old = res;
		p0 *= (x / interval<float>((float)i));
		res += p0;
		}

	return res;
	}

// MSC exp() does not alllow rounding control
// So we have to do it manually
// Use a taylor series until their is no more change in the result
// exp(x) == 1 + x + x^2/2!+x^3/3!+....
//
inline interval<double> exp(const interval<double>& x)
	{
	int  i;
	interval<double> c, res, p0, old;
	double dd;
	//   char buf[128];
	//   char buf2[128];

	p0 = x;  dd = 1.0;
	old = interval<double>(dd);
	res = old + p0;
	for (i = 2; i < 30 && (res.lower() != old.lower() && res.upper() != old.upper()); i++)
		{
		old = res;
		p0 *= (x / interval<double>((double)i));
		res += p0;
		//dd = res.radius();
		//sprintf( buf, "%+.18g %+.18g", res.lower(), res.upper() );
		}

	//sprintf( buf2, "%+.18g", pow( 2.0, 3.0 ) );
	//dd = res.radius();
	return res;
	}

// Specilization for pow float_precision
// 
inline interval<float_precision> pow( const interval<float_precision>& x, const float_precision& y )
   {
   interval<float_precision> c(x);

   c = log( x );
   c *= interval<float_precision>( y );
   c = exp( c );

   return c;
   }

#ifdef NOT_USED
// pow for float using manged code.
//
inline interval<float> pow( const interval<float>& x, const float y  )
   {
	interval<float_precision> fx(x);
	float_precision yy(y);
   float lower, upper;
   
   fx=pow(fx,yy);
   lower=fx.lower();
   upper=fx.upper();
   return interval<float>( lower, upper );
   }

// pow for double using managed code.
//
inline interval<double> pow( const interval<double>& x, const double y )
   {
	interval<float_precision> fx(x);
	float_precision yy(y);
   double lower, upper;
   
   fx=pow(fx,yy);
   lower=fx.lower();
   upper=fx.upper();
   return interval<double>( lower, upper );
   }
#endif

// MSC pow() does not alllow rounding control
// So we have to do it manually
// x^y == exp( y * ln( x ) ) );
// 
inline interval<float> pow(const interval<float>& x, const float y)
	{
	interval<float> c;

	c = log(x);
	c *= interval<float>(y);
	c = exp(c);

	return c;
	}

// MSC pow() does not alllow rounding control
// So we have to do it manually
// x^y == exp( y * ln( x ) ) );
// 
inline interval<double> pow(const interval<double>& x, const double y)
	{
	interval<double> c;

	c = log(x);
	c *= interval<double>(y);
	c = exp(c);

	return c;
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END Interval sqrt(), log10(), log(), exp(), pow()
///
//////////////////////////////////////////////////////////////////////////////////////


#endif
