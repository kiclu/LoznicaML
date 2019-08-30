#ifndef _MLL_MATRIX_HPP
#define _MLL_MATRIX_HPP

#include<vector>

namespace MLL{
  class Matrix{
  private:
    std::vector< std::vector<double> > m_matrix;
  public:
    Matrix(int t_height, int t_width);
    Matrix(int t_height, int t_width, double** t_data);

    int get_height() const { }
    int get_width() const { }

    const Matrix& operator+();
    const Matrix& operator*();

    // Sigmoid squishification
    const Matrix& operator~();

    std::vector<double>& operator[](int t_index){}
  };
}

#endif//_MLL_MATRIX_HPP
