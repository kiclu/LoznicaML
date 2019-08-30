#ifndef _MLL_MATRIX_HPP
#define _MLL_MATRIX_HPP

#include<vector>

namespace MLL{
    class Matrix{
    private:
        std::vector< std::vector<double> > m_matrix;

        void matrix_alloc(int, int);
        //void copy_data(double**);
        //double** get_data();
    public:
        Matrix(int, int);
        //Matrix(int, int, double**);

        //Matrix& operator=(const Matrix&);

        //Matrix& operator+();
        //Matrix& operator*();

        //Matrix& operator~();

        //int get_height();
        //int get_width();

        double get_at(int t_y, int t_x){ return m_matrix.at(t_y).at(t_x); }

        //void debug_in();
        //void debug_out();
    };
}

#endif//_MLL_MATRIX_HPP
