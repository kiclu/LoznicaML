#include<MLL/mll.hpp>

#define SQR(x) ((x) * (x))

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

MLL::Matrix::Matrix(int t_height, int t_width){
    matrix_alloc(t_height, t_width);
}

MLL::Matrix::Matrix(int t_height, int t_width, double t_fill){
    matrix_alloc(t_height, t_width);
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = t_fill;
        }
    }
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

void MLL::Matrix::debug_out(){
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            std::cout << get_at(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

void MLL::Network::randomize(){
    for(int i=0;i<m_weights.size();i++){
        m_weights[i].randomize(m_layers[i]);
        m_biases[i] = Matrix(m_layers[i], 1, 0);
    }
}

void MLL::Network::write_to_file(const char* t_filename){

}

void MLL::Network::read_from_file(const char* t_filename){

}

void MLL::Network::clear_gradient(){
    for(int i=0;i<m_layers.size()-1;i++){
        m_weight_gradient[i] = Matrix(m_weights[i].get_height(), m_weights[i].get_width(), 0);
        m_bias_gradient[i] = Matrix(m_biases[i].get_height(), m_biases[i].get_width(), 0);
    }
}

MLL::Network::Network(std::vector<int> t_layers){
    m_layers = t_layers;

    m_activations.resize(m_layers.size());

    for(int i=0;i<m_layers.size();i++){
        m_activations[i] = Matrix(m_layers[i], 1);
    }

    m_weights.resize(m_layers.size() - 1);
    m_biases.resize(m_layers.size() - 1);

    for(int i=0;i<m_layers.size()-1;i++){
        m_weights[i] = Matrix(m_layers[i+1], m_layers[i]);
        m_biases[i] = Matrix(m_layers[i], 1);
    }

    randomize();
    clear_gradient();
}

MLL::Network::Network(const char* t_filename){
    //read_from_file(t_filename);
}

void MLL::Network::set_input(std::vector<double> t_input, int t_label){
    m_activations[0] = Matrix(m_layers[1], m_layers[0], t_input.data());

    m_label = t_label;
}

void MLL::Network::calculate(){
    for(int i=1;i<m_layers.size();i++){
        m_activations[i] = ~(m_weights[i-1] * m_activations[i-1] + m_biases[i-1]);
    }
}

std::vector<double> MLL::Network::get_result(){
    static std::vector<double> result;
    result.resize(m_layers.back());

    for(int i=0;i<result.size();i++){
        result[i] = m_activations.back()[i][0];
    }

    return result;
}

double MLL::Network::get_cost(){
    auto result_vector = get_result();
    std::vector<double> correct_vector(m_layers.back(), 0.f);
    correct_vector[m_label] = 1.f;

    double cost = 0.f;
    for(int i=0;i<m_layers.back();i++)
        cost += SQR(result_vector[i] - correct_vector[i]);

    return cost;
}

void MLL::Network::backprop(){

}

void MLL::Network::debug_out(){
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Layers: ";
    for(int i=0;i<m_layers.size();i++){
        std::cout << m_layers[i] << " ";
    }
    std::cout << std::endl;

    for(int i=0;i<m_layers.size();i++){
        m_activations[i].debug_out();
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;

    for(int i=0;i<m_layers.size()-1;i++){
        m_weights[i].debug_out();
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;

    for(int i=0;i<m_layers.size()-1;i++){
        m_biases[i].debug_out();
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;


    std::vector<double> result = get_result();

    for(int i=0;i<result.size();i++){
        std::cout << result[i] << " ";
    }
    std::cout << get_cost();
    std::cout << "-----------------------------------------------" << std::endl;
}
