/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_UDdate_UT_update_rhs( FLA_Obj T, FLA_Obj bR,
                                    FLA_Obj C, FLA_Obj bC,
                                    FLA_Obj D, FLA_Obj bD )
{
	FLA_Obj W;
	FLA_Obj bC_copy;
	FLA_Obj bD_copy;

	// Check parameters.
	if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
		FLA_UDdate_UT_update_rhs_check( T, bR, C, bC, D, bD );

	// Create workspace according to the algorithmic blocksize (length of T)
	// and the number of right-hand sides.
	FLA_Apply_QUD_UT_create_workspace( T, bR, &W );
	
	// Make temporary copies of the bC and bD right-hand side objects so we
	// don't destory their original contents.
	FLA_Obj_create_copy_of( FLA_NO_TRANSPOSE, bC, &bC_copy );
	FLA_Obj_create_copy_of( FLA_NO_TRANSPOSE, bD, &bD_copy );

	// Apply the updowndating Q' to the right-hand sides.
	FLA_Apply_QUD_UT( FLA_LEFT, FLA_CONJ_TRANSPOSE, FLA_FORWARD, FLA_COLUMNWISE,
	                  T, W,
	                     bR,
	                  C, bC_copy,
	                  D, bD_copy );

	// Free the temporary objects.
	FLA_Obj_free( &bC_copy );
	FLA_Obj_free( &bD_copy );

	// Free the workspace object.
	FLA_Obj_free( &W );

	return FLA_SUCCESS;
}

