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

void MLL::Network::read_from_file(const char*){

}

MLL::Network::Network(std::vector<int> t_layers){
    m_layers = t_layers;

    //randomize();
}

MLL::Network::Network(const char* t_filename){
    //read_from_file(t_filename);
}

void MLL::Network::set_input(std::vector<double> t_input, int t_label){
    //m_activations[0] = t_input;         //FIX
    m_label = t_label;
}

void MLL::Network::calculate(){
    for(int i=1;i<m_layers.size();i++){
        m_activations[i] = ~(m_weights[i-1] * m_activations[i-1] + m_biases[i-1]);
    }
}

std::vector<double> MLL::Network::get_result(){
    std::vector<double> result;
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

Dataset::Dataset(const char* t_imgfile, const char* t_lblfile, int t_length=IMAGE_NUM,int t_pos=0){
    int a;
    unsigned char tmp;
    std::ifstream fin;

    data.resize(t_length);

    fin.open(t_imgfile, std::ios::binary | std::ios::in);
    fin.seekg(0, std::ios::beg);

    for(int i=0;i<4;i++){
        fin.read((char*)&a,sizeof(a));
    }

    fin.seekg(t_pos*IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(char), std::ios::cur);
    for(int i=0;i<data.size();i++){
        for(int j=0;j<IMAGE_WIDTH*IMAGE_HEIGHT;j++){
            fin.read((char*)&tmp, sizeof(char));
            data[i].pixel[j] = tmp; //div by 255
        }
    }
    fin.close();

    fin.open(t_lblfile, std::ios::binary | std::ios::in);
    fin.seekg(0, std::ios::beg);

    for(int i=0;i<2;i++){
        fin.read((char*)&a, sizeof(a));
    }

    fin.seekg(t_pos*sizeof(char), std::ios::cur);
    for(int i=0;i<data.size();i++){
        fin.read((char*)&tmp, sizeof(char));
        data[i].label=(tmp);
    }
    fin.close();
}
