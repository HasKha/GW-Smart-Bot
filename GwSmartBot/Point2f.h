#pragma once

#include <assert.h>
#include <cmath>

/***************************************************************************
 Point2f.h
 Comment:  This file contains a 2f point definition.
 ***************************************************************************/

//-----------------------------------------------------------------------------
/**
 * This is the main class for all 2f points.
 * The type of the two coordinates is float.
 */
class Point2f {
    
    //-----------------------------------------------------------------------------
public:
    
    /**
     * Standard constructor. Point will be set to 0.
     */
    Point2f()
    : _x( 0 ), _y( 0 ) {};
    
    /**
     * Constructor with given value that will be set to all coordinates.
     * @param v - the value
     */
    Point2f( const float v )
    : _x( v ), _y( v ) {};
    
    /**
     * Constructor with given values for the 2 coordinates.
     * @param x - first coordinate of this point
     * @param y - second coordinate of this point
     */
    Point2f( const float x, const float y )
    : _x( x ), _y( y ) {};
    
    
    /**
     * Returns the first coordinate of this point.
     * @return the \b first coordinate
     */
    float& x() { return _x; };
    
    /**
     * Returns the first coordinate of this point (constant).
     * @return the \b first coordinate
     */
    float x() const { return _x; };
    
    /**
     * Returns the second coordinate of this point.
     * @return the \b second coordinate
     */
    float& y() { return _y; };
    
    /**
     * Returns the second coordinate of this point (constant).
     * @return the \b second coordinate
     */
    float y() const { return _y; };
    
    /**
     * Operator that returns the coordinate at the given index.
     * @param i - index of the coordinate
     * @return the \b coordinate at index \em i
     */
    float& operator [] ( const int i ) {
        assert(i < 2);
		assert(i >= 0);
        if ( i == 0 )
            return _x;
        return _y;
    };
    
    /**
     * Operator that returns the coordinate at the given index (constant).
     * @param i - index of the coordinate
     * @return the \b coordinate at index \em i
     */
    float operator [] ( const int i ) const {
		assert(i < 2);
		assert(i >= 0);
        if ( i == 0 )
            return _x;
        return _y;
    };
    
    /**
     * Equality operator based on the coordinates of the points.
     * @param p - point to compare with
     * @return \b true if this point is equal to p, else \b false
     */
    bool operator == ( const Point2f& p ) const {
        if ( _x == p.x() && _y == p.y() )
            return true;
        return false;
    };
    
    /**
     * Operator that returns the inverted point.
     * @return the <b> inverted point </b>
     */
    const Point2f operator - () const {
        return Point2f( -_x, -_y );
    };
    
    /**
     * Adding Operator.
     * @param p - the addend
     * @return the \b sum of the points
     */
    const Point2f operator + ( const Point2f& p ) const {
        return Point2f( _x + p.x(), _y + p.y() );
    };
    
    /**
     * Add a point to this point.
     * @param p - the addend
     * @return this point
     */
    Point2f& operator += ( const Point2f& p ) {
        _x += p.x(); _y += p.y();
        return *this;
    };
    
    /**
     * Minus operator.
     * @param p - the subtrahend
     * @return the \b difference point
     */
    const Point2f operator - ( const Point2f& p ) const {
        return Point2f( _x - p.x(), _y - p.y() );
    };
    
    /**
     * Substract a point from this point.
     * @param p - the subtrahend
     * @return this point
     */
    Point2f& operator -= ( const Point2f& p ) {
        _x -= p.x(); _y -= p.y();
        return *this;
    };
    
    /**
     * Division operator for a single value.
     * All coordinates will be divided by the given value.
     * @param w - the divisor
     * @return the <b> new point </b>
     */
    const Point2f operator / ( const float w ) const {
        return Point2f( _x / w, _y / w );
    };
    
    /**
     * Division operator for a single value.
     * All coordinates will be divided by the given value.
     * @param w - the divisor
     * @return the <b> new point </b>
     */
    friend const Point2f operator / ( const float w, const Point2f& p ) {
        return p / w;
    };
    
    /**
     * Divide all coordinates of this point by the given value.
     * @param w - the divisor
     * @return this point
     */
    Point2f& operator /= ( const float w ) {
        _x /= w; _y /= w;
        return *this;
    };
    
    /**
     * Multiply operator for a single value.
     * All coordinates will be multiplied with the given value.
     * @param w - the multiplier
     * @return the <b> new point </b>
     */
    const Point2f operator * ( const float w ) const {
        return Point2f( _x * w, _y * w );
    };
    
    /**
     * Multiply operator for a single value.
     * All coordinates will be multiplied with the given value.
     * @param w - the multiplier
     * @return the <b> new point </b>
     */
    friend const Point2f operator * ( const float w, const Point2f& p ) {
        return p * w;
    };
    
    /**
     * Multiply all coordinates of this point with the given value.
     * @param w - the multiplier
     * @return this point
     */
    Point2f& operator *= ( const float w ) {
        _x *= w; _y *= w;
        return *this;
    };
    
    /**
     * Dot product operator.
     * @param p - another point
     * @return the <b> dot product </b> of the two points
     */
    float operator * ( const Point2f& p ) const {
        return ( _x * p.x() + _y * p.y() );
    };
    
    /**
     * Returns the norm1 of this vector.
     * @return the \b norm1
     */
    float norm1() const {
        return _x + _y;
    };
    
    /**
     * Returns the norm of this vector.
     * @return the \b norm
     */
    float norm() const {
        return std::sqrt( _x * _x + _y * _y );
    };
    
    /**
     * returns the squared norm of this vector
     * @return the <b> squared norm </b>
     */
    float squaredNorm() const {
        return _x * _x + _y * _y;
    };
    
    /**
     * Normalize this point and return a new point with the calculated coordinates.
     * @return the <b> normalized point </b>
     */
    const Point2f normalized() const {
        return ( *this / norm() );
    };
    
    /** 
     * Normalize this point. 
     * @return this point
     */
    Point2f& normalize() {
        const float n = norm();
        _x /= n; _y /= n;
        return *this;
    };
    
    
    //-----------------------------------------------------------------------------
private:
    
    /** The first coordinate. */
    float _x;
    /** The second coordinate. */
    float _y;
    
};
