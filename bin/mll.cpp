#include<MLL/mll.hpp>

void MLL::Matrix::matrix_alloc(int t_height, int t_width){
    m_matrix.resize(t_height);
    for(int i=0;i<t_height;i++){
        m_matrix.at(i) = std::vector<double>(t_width, 0);
    }
}

MLL::Matrix::Matrix(int t_height, int t_width){
    matrix_alloc(t_height, t_width);
}

/*
MLL::Matrix::Matrix(int t_height, int t_width, double** t_data){
    matrix_alloc(t_height, t_width);
    //copy_data(t_data);
}
*/

/*
MLL::Matrix& MLL::Matrix::operator=(){


    return *this;
}
*/

/*
MLL::Matrix& MLL::Matrix::operator+(){

}
*/
