/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#define N_PARAM_COMBOS    2

#define FLA_ALG_REFERENCE 0
#define FLA_ALG_FRONT     1

char* pc_str[N_PARAM_COMBOS] = { "l", "u" };

void time_Ttmm(
                 int param_combo, int type, int n_repeats, int m, FLA_Uplo uplo,
                 FLA_Obj A, FLA_Obj x, FLA_Obj b, FLA_Obj norm,
                 double *dtime, double *diff, double *gflops );


int main(int argc, char *argv[])
{
  int 
    datatype,
    m_input,
    m,
    p_first, p_last, p_inc,
    p,
    n_repeats,
    param_combo,
    i,
    n_param_combos = N_PARAM_COMBOS;

  FLA_Uplo uplo;
  
  char *colors = "brkgmcbrkg";
  char *ticks  = "o+*xso+*xs";
  char m_dim_desc[14];
  char m_dim_tag[10];

  double max_gflops=6.0;

  double
    dtime,
    gflops,
    diff;

  FLA_Obj
    A, x, b, norm;
  

  FLA_Init();


  fprintf( stdout, "%c number of repeats:", '%' );
  scanf( "%d", &n_repeats );
  fprintf( stdout, "%c %d\n", '%', n_repeats );

  fprintf( stdout, "%c enter problem size first, last, inc:", '%' );
  scanf( "%d%d%d", &p_first, &p_last, &p_inc );
  fprintf( stdout, "%c %d %d %d\n", '%', p_first, p_last, p_inc );

  fprintf( stdout, "%c enter m (-1 means bind to problem size): ", '%' );
  scanf( "%d", &m_input );
  fprintf( stdout, "%c %d\n", '%', m_input );


  fprintf( stdout, "\nclear all;\n\n" );


  if     ( m_input >  0 ) {
    sprintf( m_dim_desc, "m = %d", m_input );
    sprintf( m_dim_tag,  "m%dc", m_input);
  }
  else if( m_input <  -1 ) {
    sprintf( m_dim_desc, "m = p/%d", -m_input );
    sprintf( m_dim_tag,  "m%dp", -m_input );
  }
  else if( m_input == -1 ) {
    sprintf( m_dim_desc, "m = p" );
    sprintf( m_dim_tag,  "m%dp", 1 );
  }

  //datatype = FLA_FLOAT;
  //datatype = FLA_DOUBLE;
  //datatype = FLA_COMPLEX;
  datatype = FLA_DOUBLE_COMPLEX;

  for ( p = p_first, i = 1; p <= p_last; p += p_inc, i += 1 )
  {
    m = m_input;

    if( m < 0 ) m = p / abs(m_input);

    for ( param_combo = 0; param_combo < n_param_combos; param_combo++ ){
      
      FLA_Obj_create( datatype, m, m, 0, 0, &A );
      FLA_Obj_create( datatype, m, 1, 0, 0, &x );
      FLA_Obj_create( datatype, m, 1, 0, 0, &b );

/*
      FLA_Obj_create( datatype, m, m, m, 1, &A );
      FLA_Obj_create( datatype, m, 1, 1, 1, &x );
      FLA_Obj_create( datatype, m, 1, 1, 1, &b );
*/

      if ( FLA_Obj_is_single_precision( A ) )
        FLA_Obj_create( FLA_FLOAT, 1, 1, 0, 0, &norm );
      else
        FLA_Obj_create( FLA_DOUBLE, 1, 1, 0, 0, &norm );

      FLA_Param_map_netlib_to_flame_uplo( &pc_str[param_combo][0], &uplo );

      FLA_Random_spd_matrix( uplo, A );
      FLA_Random_matrix( b );

      fprintf( stdout, "data_ttmm_%s( %d, 1:5 ) = [ %d  ", pc_str[param_combo], i, p );
      fflush( stdout );

/*
      time_Ttmm( param_combo, FLA_ALG_REFERENCE, n_repeats, m, uplo,
                 A, x, b, norm, &dtime, &diff, &gflops );

      fprintf( stdout, "%6.3lf %6.2le ", gflops, diff );
      fflush( stdout );
*/

      time_Ttmm( param_combo, FLA_ALG_FRONT, n_repeats, m, uplo,
                 A, x, b, norm, &dtime, &diff, &gflops );

      fprintf( stdout, "%6.3lf %6.2le ", gflops, diff );
      fflush( stdout );

      fprintf( stdout, " ]; \n" );
      fflush( stdout );

      FLA_Obj_free( &A );
      FLA_Obj_free( &x );
      FLA_Obj_free( &b );
      FLA_Obj_free( &norm );
    }

    fprintf( stdout, "\n" );
  }

/*
  fprintf( stdout, "figure;\n" );

  fprintf( stdout, "hold on;\n" );

  for ( i = 0; i < n_param_combos; i++ ) {
    fprintf( stdout, "plot( data_ttmm_%s( :,1 ), data_ttmm_%s( :, 2 ), '%c:%c' ); \n",
            pc_str[i], pc_str[i], colors[ i ], ticks[ i ] );
    fprintf( stdout, "plot( data_ttmm_%s( :,1 ), data_ttmm_%s( :, 4 ), '%c-.%c' ); \n",
            pc_str[i], pc_str[i], colors[ i ], ticks[ i ] );
  }

  fprintf( stdout, "legend( ... \n" );

  for ( i = 0; i < n_param_combos; i++ )
    fprintf( stdout, "'ref\\_ttmm\\_%s', 'fla\\_ttmm\\_%s', ... \n", pc_str[i], pc_str[i] );

  fprintf( stdout, "'Location', 'SouthWest' ); \n" );

  fprintf( stdout, "xlabel( 'problem size p' );\n" );
  fprintf( stdout, "ylabel( 'GFLOPS/sec.' );\n" );
  fprintf( stdout, "axis( [ 0 %d 0 %.2f ] ); \n", p_last, max_gflops );
  fprintf( stdout, "title( 'FLAME ttmm front-end performance (%s)' );\n", m_dim_desc );
  fprintf( stdout, "print -depsc ttmm_front_%s.eps\n", m_dim_tag );
  fprintf( stdout, "hold off;\n");
  fflush( stdout );
*/

  FLA_Finalize( );

  return 0;
}

