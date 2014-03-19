%
%
%   Copyright (C) 2014, The University of Texas at Austin
%
%   This file is part of libflame and is available under the 3-Clause
%   BSD license, which can be found in the LICENSE file at the top-level
%   directory, or at http://opensource.org/licenses/BSD-3-Clause
%
%
function [ X ] = Symm_ll_unb_var9( alpha, A, B, C )
%
% Invariant: [ CL, CR ] = [ hatCL + alpha + A * BL, hatCR ]
%
  [ BL, BR ] = FLA_Part_1x2( B, 0, 'FLA_LEFT' );

  [ CL, CR ] = FLA_Part_1x2( C, 0, 'FLA_LEFT' );

  while( size( CL, 2 ) ~= size( C, 2 ) )
    [ C0, c1, C2 ] = FLA_Repart_1x2_to_1x3( CL, CR,
					     1, 'FLA_RIGHT' );

    [ B0, b1, B2 ] = FLA_Repart_1x2_to_1x3( BL, BR,
 				            1, 'FLA_RIGHT' );
%* ********************************************************************** *%
    c1 = alpha * ( tril( A ) + tril( A, -1 )' ) * b1 + c1;
%* ********************************************************************** *%
    [ CL, CR ] = FLA_Cont_with_1x3_to_1x2( C0, c1, C2, 'FLA_LEFT' );

    [ BL, BR ] = FLA_Cont_with_1x3_to_1x2( B0, b1, B2, 'FLA_LEFT' );
  end

  X = CL;
  return;
