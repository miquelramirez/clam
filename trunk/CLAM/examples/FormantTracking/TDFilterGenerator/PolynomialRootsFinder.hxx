/* Copyright (C) 1981-1999 Ken Turkowski. <turk@computer.org>
 *
 * All rights reserved.
 *
 * Warranty Information
 *  Even though I have reviewed this software, I make no warranty
 *  or representation, either express or implied, with respect to this
 *  software, its quality, accuracy, merchantability, or fitness for a
 *  particular purpose.  As a result, this software is provided "as is,"
 *  and you, its user, are assuming the entire risk as to its quality
 *  and accuracy.
 *
 * This code may be used and freely distributed as long as it includes
 * this copyright notice and the above warranty information.
 */


#include "CLAM_Math.hxx"
#include "DataTypes.hxx"
#include <iostream>

/*
#ifdef DOUBLE_PRECISION
# define FLOAT double
# define PARAMFLOAT double_t
#else	// DOUBLE_PRECISION

# define FLOAT float
# define PARAMFLOAT float_t
#endif	// DOUBLE_PRECISION */

/*******************************************************************************
 * FindPolynomialRoots
 *
 * The Bairstow and Newton correction formulae are used for a simultaneous
 * linear and quadratic iterated synthetic division.  The coefficients of
 * a polynomial of degree n are given as a[i] (i=0,i,..., n) where a[0] is
 * the constant term.  The coefficients are scaled by dividing them by
 * their geometric mean.  The Bairstow or Newton iteration method will
 * nearly always converge to the number of figures carried, fig, either to
 * root values or to their reciprocals.  If the simultaneous Newton and
 * Bairstow iteration fails to converge on root values or their
 * reciprocals in maxiter iterations, the convergence requirement will be
 * successively reduced by one decimal figure.  This program anticipates
 * and protects against loss of significance in the quadratic synthetic
 * division.  (Refer to "On Programming the Numerical Solution of
 * Polynomial Equations," by K. W. Ellenberger, Commun. ACM 3 (Dec. 1960),
 * 644-647.)  The real and imaginary part of each root is stated as u[i]
 * and v[i], respectively, together with the corresponding constant,
 * conv[i], used in the convergence test.  This program has been used
 * successfully for over a year on the Bendix G15-D (Intercard System) and
 * has recently been coded for the IBM 709 (Fortran system).
 *
 * ACM algorithm #30 - Numerical Solution of the Polynomial Equation
 * K. W. Ellenberger
 * Missle Division, North American Aviation, Downey, California
 * Converted to C, modified, optimized, and structured by
 * Ken Turkowski
 * CADLINC, Inc., Palo Alto, California
 *******************************************************************************/

#define MAXN 24

using CLAM::TData;

void
FindPolynomialRoots(
	const TData		*a,			/* Coefficients */
	TData			*u,			/* Real component of each root */
	TData			*v,			/* Imaginary component of each root */
	TData			*conv,		/* Convergence constant associated with each root */
	register long	n,			/* Degree of polynomial (order-1) */
	long			maxiter,	/* Maximum number of iterations */
	long			fig			/* The number of decimal figures to be computed */
);
