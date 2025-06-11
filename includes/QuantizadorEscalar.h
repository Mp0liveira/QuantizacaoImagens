#ifndef QUANTIZADORESCALAR_H
#define QUANTIZADORESCALAR_H
#include <string>

class QuantizadorEscalar {
private:
    std::string fileName;
public:
    void quantizarImagem();
    QuantizadorEscalar(const std::string& inputFile);


};




#endif //QUANTIZADORESCALAR_H
