/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error REF_Trmm( FLA_Side side, FLA_Uplo uploA, FLA_Trans transA, FLA_Diag diagA, FLA_Obj alpha, FLA_Obj A, FLA_Obj B )
{

  FLA_Trmm_external( side, uploA, transA, diagA, alpha, A, B );

  return 0;
}
