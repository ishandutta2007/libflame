%
%
%   Copyright (C) 2014, The University of Texas at Austin
%
%   This file is part of libflame and is available under the 3-Clause
%   BSD license, which can be found in the LICENSE file at the top-level
%   directory, or at http://opensource.org/licenses/BSD-3-Clause
%
%
function [ X ] = Symm_ll_unb_var7( alpha, A, B, C )
%
% Invariant: [ CT,...
%              CB, ] = [ hatCT+alpha*ABL'*BB,...
%                        hatCB+alpha*ABL*BT+alpha*ABR*BB ]
%
  [ ATL, ATR,...
    ABL, ABR     ] = FLA_Part_2x2( A,...
                                   0, 0, 'FLA_BR' );
  [ BT,...
    BB ] = FLA_Part_2x1( B,...
                         0, 'FLA_BOTTOM' );
  [ CT,...
    CB ] = FLA_Part_2x1( C,...
                         0, 'FLA_BOTTOM' );

  while( size( CB, 1 ) ~= size( C, 1 ) )
    [ A00,  a01,     A02,...
      a10t, alpha11, a12t,...
      A20,  a21,     A22      ] = FLA_Repart_2x2_to_3x3( ATL, ATR,...
                                                         ABL, ABR,...
                                                         1, 1, 'FLA_TL' );
    [ B0,...
      b1t,...
      B2 ] = FLA_Repart_2x1_to_3x1( BT,...
                                    BB, 1, 'FLA_TOP' );
    [ C0,...
      c1t,...
      C2 ] = FLA_Repart_2x1_to_3x1( CT,...
                                    CB, 1, 'FLA_TOP' );
%* ********************************************************************** *%
    c1t = alpha * alpha11 * b1t + c1t;
    c1t = alpha * a10t * B0  + c1t;
    C0  = alpha * a10t' * b1t  + C0;
%* ********************************************************************** *%
    [ ATL, ATR,...
      ABL, ABR     ] = FLA_Cont_with_3x3_to_2x2( A00,  a01,     A02,...
                                                 a10t, alpha11, a12t,...
                                                 A20,  a21,     A22,...
                                                 'FLA_BR' );
    [ BT,...
      BB ] = FLA_Cont_with_3x1_to_2x1( B0,...
                                       b1t,...
                                       B2, 'FLA_BOTTOM' );
    [ CT,...
      CB ] = FLA_Cont_with_3x1_to_2x1( C0,...
                                       c1t,...
                                       C2, 'FLA_BOTTOM' );

  end

  X = CB;
  return;
