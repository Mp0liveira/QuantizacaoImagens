#ifndef QUANTIZADORESCALAR_H
#define QUANTIZADORESCALAR_H
#include <cstdint>
#include <string>
#include <vector>

class QuantizadorEscalar {
private:
    std::string fileName;
    double mse(const std::vector<uint8_t>& original, const std::vector<uint8_t> &quantized);
    double psnr(double mse, int bitdepth);
public:
    void quantizarImagem();
    void quantizarDithering();
    QuantizadorEscalar(const std::string& inputFile);

};

#endif //QUANTIZADORESCALAR_H