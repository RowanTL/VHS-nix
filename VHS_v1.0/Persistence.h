#pragma once

#include <phat/representations/bit_tree_pivot_column.h>
#include <phat/boundary_matrix.h>

class standard_reduction_with_inverse {
    public:
        void operator() ( phat::boundary_matrix< phat::bit_tree_pivot_column >& boundary_matrix, phat::boundary_matrix< phat::bit_tree_pivot_column >& inverse ) {

            const phat::index nr_columns = boundary_matrix.get_num_cols();
            std::vector< phat::index > lowest_one_lookup( nr_columns, -1 );
            
            for( phat::index cur_col = 0; cur_col < nr_columns; cur_col++ ) {
                phat::index lowest_one = boundary_matrix.get_max_index( cur_col );
                while( lowest_one != -1 && lowest_one_lookup[ lowest_one ] != -1 ) {
                    boundary_matrix.add_to( lowest_one_lookup[ lowest_one ], cur_col );
                    inverse.add_to( lowest_one_lookup[ lowest_one ], cur_col );
                    lowest_one = boundary_matrix.get_max_index( cur_col );
                }
                if( lowest_one != -1 ) {
                    lowest_one_lookup[ lowest_one ] = cur_col;
                }
                boundary_matrix.finalize( cur_col );
                inverse.finalize( cur_col );
            }
        }
};
