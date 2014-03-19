%
%
%   Copyright (C) 2014, The University of Texas at Austin
%
%   This file is part of libflame and is available under the 3-Clause
%   BSD license, which can be found in the LICENSE file at the top-level
%   directory, or at http://opensource.org/licenses/BSD-3-Clause
%
%

function [ C_out ] = FLA_Syr2k_lt_blk_var10( A, B, C, nb_alg )

  [ AT, ...
    AB ] = FLA_Part_2x1( A, ...
                         0, 'FLA_BOTTOM' );

  [ BT, ...
    BB ] = FLA_Part_2x1( B, ...
                         0, 'FLA_BOTTOM' );

  while ( size( AB, 1 ) < size( A, 1 ) )

    b = min( size( AT, 1 ), nb_alg );

    [ A0, ...
      A1, ...
      A2 ] = FLA_Repart_2x1_to_3x1( AT, ...
                                    AB, ...
                                    b, 'FLA_TOP' );

    [ B0, ...
      B1, ...
      B2 ] = FLA_Repart_2x1_to_3x1( BT, ...
                                    BB, ...
                                    b, 'FLA_TOP' );

    %------------------------------------------------------------%

    C = C + A1' * B1 + B1' * A1;

    %------------------------------------------------------------%

    [ AT, ...
      AB ] = FLA_Cont_with_3x1_to_2x1( A0, ...
                                       A1, ...
                                       A2, ...
                                       'FLA_BOTTOM' );

    [ BT, ...
      BB ] = FLA_Cont_with_3x1_to_2x1( B0, ...
                                       B1, ...
                                       B2, ...
                                       'FLA_BOTTOM' );

  end

  C_out = C;


return

