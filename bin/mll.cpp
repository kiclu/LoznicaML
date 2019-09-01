#include<MLL/mll.hpp>

void MLL::Matrix::matrix_alloc(int t_height, int t_width){
    m_matrix.resize(t_height);
    for(int i=0;i<t_height;i++){
        m_matrix.at(i) = std::vector<double>(t_width, 0);
    }
}

void MLL::Matrix::copy_data(double* t_data){
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            get_at(i, j) = t_data[i * get_width() + j];
        }
    }
}

double* MLL::Matrix::get_data(){
    double* tmp = (double*)malloc(get_height() * get_width() * sizeof(double));

    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            tmp[i * get_width() + j] = get_at(i, j);
        }
    }

    return tmp;
}

double MLL::sigm(double x){
    return 1 / (1 + exp(-x));
}

MLL::Matrix::Matrix(int t_height, int t_width){
    matrix_alloc(t_height, t_width);
}

MLL::Matrix::Matrix(int t_height, int t_width, double* t_data){
    matrix_alloc(t_height, t_width);
    copy_data(t_data);
}

MLL::Matrix::Matrix(MLL::Matrix& t_matrix){
    matrix_alloc(t_matrix.get_height(), t_matrix.get_width());
    copy_data(t_matrix.get_data());
}


MLL::Matrix& MLL::Matrix::operator=(MLL::Matrix& t_matrix){
    matrix_alloc(t_matrix.get_height(), t_matrix.get_width());
    copy_data(t_matrix.get_data());

    return *this;
}

MLL::Matrix& MLL::operator+(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if(t_matrix1.get_height() != t_matrix2.get_height() ||
       t_matrix1.get_width() != t_matrix2.get_width()) {
        std::cout << "Unmatched sizes" << std::endl;
        exit(100);
    }
    static MLL::Matrix result(t_matrix1.get_height(), t_matrix1.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result.get_at(i, j) = t_matrix1.get_at(i, j) + t_matrix2.get_at(i, j);
        }
    }

    return result;
}

MLL::Matrix& MLL::operator*(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if(t_matrix1.get_width() != t_matrix2.get_height()) {
        std::cout << "Unmatched sizes" << std::endl;
        exit(100);
    }
    static MLL::Matrix result(t_matrix1.get_height(), t_matrix2.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            for(int k=0;k<t_matrix1.get_width();k++){
                result.get_at(i, j) += t_matrix1.get_at(i, k) * t_matrix2.get_at(k, j);
            }
        }
    }

    return result;
}

MLL::Matrix& MLL::operator~(MLL::Matrix& t_matrix){
    static auto result(t_matrix);

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result.get_at(i, j) = sigm(t_matrix.get_at(i, j));
        }
    }

    return result;
}

std::vector<double>& MLL::Matrix::operator[](int t_index){
    return m_matrix.at(t_index);
}

void MLL::Matrix::debug_out(){
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            std::cout << get_at(i, j) << " ";
        }
        std::cout << std::endl;
    }
}
