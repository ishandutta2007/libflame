%
%
%   Copyright (C) 2014, The University of Texas at Austin
%
%   This file is part of libflame and is available under the 3-Clause
%   BSD license, which can be found in the LICENSE file at the top-level
%   directory, or at http://opensource.org/licenses/BSD-3-Clause
%
%
function [ X ] = Symm_ll_blk_var6( alpha, A, B, C, nb )
%
% Invariant: [ CT,...
%              CB, ] = [ hatCT,...
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
     b = min( size( CT, 1 ), nb );

    [ A00, A01, A02,...
      A10, A11, A12,...
      A20, A21, A22      ] = FLA_Repart_2x2_to_3x3( ATL, ATR,...
                                                         ABL, ABR,...
                                                         b, b, 'FLA_TL' );
    [ B0,...
      B1,...
      B2 ] = FLA_Repart_2x1_to_3x1( BT,...
                                    BB, b, 'FLA_TOP' );
    [ C0,...
      C1,...
      C2 ] = FLA_Repart_2x1_to_3x1( CT,...
                                    CB, b, 'FLA_TOP' );
%* ********************************************************************** *%
    C1 = alpha * A10 * B0  + C1;
    C1 = Symm_ll_unb_var6( alpha, A11, B1, C1 );
    C1 = alpha * A21' * B2  + C1;
%* ********************************************************************** *%
    [ ATL, ATR,...
      ABL, ABR     ] = FLA_Cont_with_3x3_to_2x2( A00, A01, A02,...
                                                 A10, A11, A12,...
                                                 A20, A21, A22,...
                                                 'FLA_BR' );
    [ BT,...
      BB ] = FLA_Cont_with_3x1_to_2x1( B0,...
                                       B1,...
                                       B2, 'FLA_BOTTOM' );
    [ CT,...
      CB ] = FLA_Cont_with_3x1_to_2x1( C0,...
                                       C1,...
                                       C2, 'FLA_BOTTOM' );

  end

  X = CB;
  return;
