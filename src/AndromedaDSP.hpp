/*
AndromedaDSP class declarations and definitions of DSP Project
Copyright (C) 2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <cmath>
#include <algorithm>

#define REAL 0
#define IMAG 1

namespace AndromedaDSP {
    template<class T>
    class AndromedaDSP {
    public:
        static inline T *hanningMultipliers(size_t dataSize);
        static inline T *hanningMultipliersMatlab(int N, short itype = 0);
        static inline T *hammingMultipliers(int windowLength);
        static inline T *blackmanMultipliers(int windowLength);
        static inline T calculateMagnitude(T real, T imaginary);
        static inline T calculateDecibel(T real, T imaginary);
        static inline T magnitudeToDecibel(T magnitude);
        static inline void magnitudeToDecibel(T* magnitudeValues, T* decibelValues, const size_t count);
        static inline T decibelToMagnitude(T decibel);
        static inline T calculateVolumeDbLevel(T* leftBuffer, T* rightBuffer,size_t count);
        //static inline T powerToDecibel(T powerValue);
        //static inline T decibelToPower(T decibelValue);
        static inline T linearToDecibel(T v1, T v2);
        static inline T logarithm(T);
    };
}
// Inline Method Definitions

template <class T> inline T AndromedaDSP::AndromedaDSP<T>::magnitudeToDecibel(T magnitude) {
    return T(20)*logarithm(magnitude);
}

template <class T> inline T AndromedaDSP::AndromedaDSP<T>::decibelToMagnitude(T decibel) {
    return pow<T>(10, decibel/T(20));
}

template <> inline double AndromedaDSP::AndromedaDSP<double>::logarithm(double value) {
    return log10(value);
}

template <> inline float AndromedaDSP::AndromedaDSP<float>::logarithm(float value) {
    return log10f(value);
}

template <> inline long double AndromedaDSP::AndromedaDSP<long double>::logarithm(long double value) {
    return log10l(value);
}

template <class T> inline T AndromedaDSP::AndromedaDSP<T>::logarithm(T value) {
    return (T) log10(value);
}

template <class T> inline T AndromedaDSP::AndromedaDSP<T>::calculateDecibel(T real, T imaginary) {
    return T(20) * logarithm(calculateMagnitude(real, imaginary));
}

template <class T> inline T AndromedaDSP::AndromedaDSP<T>::calculateMagnitude(T real, T imaginary) {
    //return std::sqrt(real*real + imaginary*imaginary); //<-- This calculation can cause owerflow
    // In order to prevent overflow, we use the algorithm below
    T small, big;
    if(real<imaginary) {
        small = real;
        big = imaginary;
    }
    else {
        small = imaginary;
        big = real;
    }
    if(big == 0)
        return abs(small);
    else if(small == 0)
        return abs(big);
    T a = small/big;
    return abs(big) * sqrt(1+a*a);
}

template <class T> inline T * AndromedaDSP::AndromedaDSP<T>::hanningMultipliers(size_t dataSize){
    T *multipliers;

    multipliers = new T[dataSize];
    for (unsigned int i = 0; i < dataSize; i++) {
        T multiplier = T(0.5) * (T(1) - cos(T(2)*M_PI*i/(dataSize-1)));
        multipliers[i] = multiplier;
    }
    return multipliers;
}

// Create a hamming window of windowLength samples in buffer
template<class T> inline T* AndromedaDSP::AndromedaDSP<T>::hammingMultipliers(int windowLength) {
    T *timeDomain = new T[windowLength];
    for(int i = 0; i < windowLength; i++) {
        timeDomain[i] = T(0.53836) - ( T(0.46164) * cos( T(2) * M_PI * T(i)  / ( T(windowLength) - T(1) ) ) );
        //timeDomain[i] = 0.54 - (0.46 * cos( 2 * M_PI * (i / ((windowLength - 1) * 1.0))));
    }
    return timeDomain;
}

template<class T> inline T* AndromedaDSP::AndromedaDSP<T>::blackmanMultipliers(int windowLength) {
    T *timeDomain = new T[windowLength];
    for(int i = 0; i < windowLength; i++) {
        timeDomain[i] = T(0.42) - T(0.5) * std::cos(T(2) * M_PI * i / (T(windowLength) - T(1))) + T(0.08) * std::cos(T(4.0) * M_PI * T(i) / (T(windowLength) - T(1)));
    }
    return timeDomain;
}

/*  function w = hanning(varargin)
%   HANNING   Hanning window.
%   HANNING(N) returns the N-point symmetric Hanning window in a column
%   vector.  Note that the first and last zero-weighted window samples
%   are not included.
%
%   HANNING(N,'symmetric') returns the same result as HANNING(N).
%
%   HANNING(N,'periodic') returns the N-point periodic Hanning window,
%   and includes the first zero-weighted window sample.
%
%   NOTE: Use the HANN function to get a Hanning window which has the
%          first and last zero-weighted samples.ep
    itype = 1 --> periodic
    itype = 0 --> symmetric
    default itype=0 (symmetric)

    Copyright 1988-2004 The MathWorks, Inc.
%   $Revision: 1.11.4.3 $  $Date: 2007/12/14 15:05:04 $
*/
template <class T> inline T * AndromedaDSP::AndromedaDSP<T>::hanningMultipliersMatlab(int N, short itype){
    int half, i, idx, n;
    T *w;

    w = new T[N];
    std::fill(w, w+N, T(0));

    if(itype==1)    //periodic function
        n = N-1;
    else
        n = N;

    if(n%2==0)
    {
        half = n/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-1;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }
    else
    {
        half = (n+1)/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-2;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }

    if(itype==1)    //periodic function
    {
        for(i=N-1; i>=1; i--)
            w[i] = w[i-1];
        w[0] = 0.0;
    }
    return w;
}

template <typename T> inline void AndromedaDSP::AndromedaDSP<T>::magnitudeToDecibel(T* magnitudeValues, T* decibelValues, size_t count) {
    for(size_t i = 0; i < count; i++) {
        decibelValues[i] = magnitudeToDecibel(magnitudeValues[i]);
    }
    return;
}

template <typename T> inline T AndromedaDSP::AndromedaDSP<T>::calculateVolumeDbLevel(T* leftBuffer, T* rightBuffer,size_t count) {
    T sum = 0;
    T volume = 0;

    for(size_t i = 0; i < count; i++) {
        sum += pow((T(leftBuffer[i]) + T(rightBuffer[i]))/T(2), 2);
    }

    volume = T(20) * logarithm(std::sqrt(sum / T(count)));

    return volume;
}

/*
Using the natural log, ln, log base e:
linear-to-db(x) = ln(x) / (ln(10) / 20)
db-to-linear(x) = e^(x * (ln(10) / 20))

Using the common log, log, log base 10:
linear-to-db(x) = log(x) * 20
db-to-linear(x) = 10^(x / 20)
*/

template<class T> inline T AndromedaDSP::AndromedaDSP<T>::linearToDecibel(T v1, T v2) {
    return T(20) * logarithm(v1/v2);
}
