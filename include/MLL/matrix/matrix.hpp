#ifndef _MLL_MATRIX_HPP
#define _MLL_MATRIX_HPP

#include<vector>
#include<stdlib.h>
#include<iostream>
#include<math.h>
#include<time.h>


namespace MLL{
    class Matrix{
    private:
        std::vector< std::vector<double> > m_matrix;



        double& get_at(int t_y, int t_x) { return m_matrix.at(t_y).at(t_x); }
    public:
        Matrix();
        Matrix(int, int);
        Matrix(int, int, double*);

        Matrix& operator=(Matrix&);

        friend Matrix& operator+(Matrix&, Matrix&);
        friend Matrix& operator*(Matrix&, Matrix&);
        friend Matrix& operator*(Matrix&, double);

        // Sigmoid squishification
        friend Matrix& operator~(Matrix&);
        friend Matrix& operator!(Matrix&);

        std::vector<double>& operator[](int t_index) { return m_matrix.at(t_index); }

        void randomize();
        void randomize(int);
        void matrix_alloc(int, int);
        void fill(int, int, double);

        int get_height() const { return m_matrix.size(); }
        int get_width() const { return m_matrix.at(0).size(); }

        void debug_out();

        void copy_data(double*);
        double* get_data();
    };

    double sigm(double);
    double sigm_deriv(double);
    Matrix& operator+(Matrix&, Matrix&);
    Matrix& operator*(Matrix&, Matrix&);
    Matrix& operator~(Matrix&);
}


#endif//_MLL_MATRIX_HPP
