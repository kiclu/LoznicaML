#include<MLL/mll.hpp>

void MLL::Matrix::matrix_alloc(int t_height, int t_width){
    m_matrix.resize(t_height);
    for(int i=0;i<t_height;i++){
        m_matrix.at(i).resize(t_width);
    }
}

MLL::Matrix::Matrix(){

}

MLL::Matrix::Matrix(int t_height, int t_width){
    matrix_alloc(t_height, t_width);
}

MLL::Matrix::Matrix(int t_height, int t_width, double* t_data){
    matrix_alloc(t_height, t_width);
    copy_data(t_data);
}

MLL::Matrix& MLL::Matrix::operator=(MLL::Matrix& t_matrix){
    matrix_alloc(t_matrix.get_height(), t_matrix.get_width());
    copy_data(t_matrix.get_data());
}

MLL::Matrix& MLL::operator+(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if( t_matrix1.get_height() != t_matrix2.get_height() ||
        t_matrix1.get_width() != t_matrix2.get_width()){
        std::cout << "Unmatched sizes (+)" << std::endl;
        t_matrix1.debug_out();
        t_matrix2.debug_out();
        exit(100);
    }

    static MLL::Matrix result(t_matrix1.get_height(), t_matrix1.get_width());

    for(int i=0;i<t_matrix1.get_height();i++){
        for(int j=0;j<t_matrix1.get_width();j++){
            result[i][j] = t_matrix1[i][j] + t_matrix2[i][j];
        }
    }

    return result;
}

MLL::Matrix& MLL::operator*(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if(t_matrix1.get_width() != t_matrix2.get_height()){
        std::cout << "Unmatched sizes (*)" << std::endl;
        t_matrix1.debug_out();
        t_matrix2.debug_out();
        exit(100);
    }

    static MLL::Matrix result(t_matrix1.get_height(), t_matrix2.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result[i][j] = 0;
            for(int k=0;k<t_matrix1.get_width();k++){
                result[i][j] += t_matrix1[i][k] * t_matrix2[k][j];
            }
        }
    }

    return result;
}

MLL::Matrix& MLL::operator~(MLL::Matrix& t_matrix){
    static MLL::Matrix result(t_matrix.get_height(), t_matrix.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result[i][j] = sigm(t_matrix[i][j]);
        }
    }

    return result;
}

void MLL::Matrix::randomize(int t_layer_size){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 1.0);

    double coef = sqrt(1 / (double)t_layer_size);
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = distribution(generator) * coef;
        }
    }
}

void MLL::Matrix::fill(int t_height, int t_width, double t_fill){
    matrix_alloc(t_height, t_width);
    for(int i=0;i<t_height;i++){
        for(int j=0;j<t_width;i++){
            m_matrix[i][j] = t_fill;
        }
    }
}

void MLL::Matrix::debug_out(){
    std::cout << "-------------------------------" << std::endl << "Matrix debug:";
    std::cout << std::endl;
    std::cout << "[" << get_height() << " x " << get_width() << "]" << std::endl;
    std::cout << std::endl;
    int c = getchar();
    if(c == 's'){
        for(int i=0;i<get_height();i++){
            for(int j=0;j<get_width();j++){
                std::cout << m_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << "-------------------------------" << std::endl;
}

void MLL::Matrix::copy_data(double* t_data){
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = t_data[i*get_width() + j];
        }
    }
}

double* MLL::Matrix::get_data(){
    double* data = new double[get_height() * get_width()];

    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            data[i*get_width() + j] = m_matrix[i][j];
        }
    }

    return data;
}

double MLL::sigm(double x){ return 1 / (1 + exp(-x)); }
double MLL::sigm_deriv(double x){ return sigm(x) * (1 - sigm(x)); }

void MLL::Network::randomize(){

}

void MLL::Network::write_to_file(const char* t_filename){

}

void MLL::Network::read_from_file(const char* t_filename){

}

void MLL::Network::clear_gradient(){

}

MLL::Network::Network(std::vector<int> t_layers){
    m_layers = t_layers;

    m_activations.resize(t_layers.size());
    for(int i=0;i<m_layers.size();i++){
        m_activations[i].fill(m_layers[i], 1, 0);
    }
}

MLL::Network::Network(const char* t_filename){

}

void MLL::Network::set_input_layer(std::vector<double>, int){

}

void MLL::Network::calculate(){

}

std::vector<double> MLL::Network::get_result(){
    return std::vector<double>(0);
}

double MLL::Network::get_cost(){
    return 0;
}

void MLL::Network::backprop(){

}

void MLL::Network::debug_out(){
    std::cout << "-------------------------------" << std::endl << "Network debug:";
    std::cout << std::endl << std::endl;

    std::cout << "Layer count: " << m_layers.size() << std::endl << "[ ";
    for(int i=0;i<m_layers.size();i++){
        std::cout << m_layers[i] << " ";
    }
    std::cout << "]" << std::endl << std::endl;

    std::cout << "Activation layers: " << m_activations.size() << std::endl;
    for(int i=0;i<m_activations.size();i++){
        //m_activations[i].debug_out();
    }

    std::cout << std::endl << std::endl << "-------------------------------" << std::endl;
}
