#include "../includes/QuantizadorEscalar.h"

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

QuantizadorEscalar::QuantizadorEscalar(const std::string &inputFile) : fileName(inputFile) {}

void QuantizadorEscalar::quantizarImagem() {
    int width, height, original_channels, bitdepth;

    std::string input = "../Imagens/" + fileName;

    // Verificar as informações da imagem SEM carregá-la completamente
    if (!stbi_info(input.c_str(), &width, &height, &original_channels)) {
        std::cerr << "Erro: Nao foi possivel ler as informacoes do arquivo de imagem: " << fileName << std::endl;
        return;
    }

    // Validar se a imagem atende aos requisitos do codificador C1 (escala de cinza)
    if (original_channels != 1) {
        std::cerr << "Erro: O codificador C1 aceita apenas imagens em escala de cinza." << std::endl;
        std::cerr << "A imagem fornecida '" << fileName << "' tem " << original_channels << " canais." << std::endl;
        return;
    }

    std::cout << "Imagem de entrada valida (escala de cinza)." << std::endl;

    if (stbi_is_16_bit(input.c_str())) {
        bitdepth = 16;
    } else {
        // Assumimos que será 8 bits
        bitdepth = 8;
    }

    // Carregando a imagem com stb_image
    int channels_loaded;
    uint8_t *image = stbi_load(input.c_str(), &width, &height, &channels_loaded, 1);

    if (image == nullptr) {
        std::cerr << "Error loading image: " << fileName << std::endl;
        return;
    }

    // Passando as informações dos pixels para um vetor
    std::vector<uint8_t> pixels(image, image + width * height);

    stbi_image_free(image);

    // Vetor responsável por armazenar os pixels quantizados
    std::vector<uint8_t> newPixels(pixels.size());
    // Quantização
    for (size_t i = 0; i < pixels.size(); ++i) {
        if (pixels[i] < 128) {
            newPixels[i] = 0;   // Preto
        } else {
            newPixels[i] = 255; // Branco
        }
    }

    // Pega o nome base do arquivo de entrada
    std::string baseName = fileName.substr(fileName.find_last_of("/\\") + 1);

    // Remove a extensão antiga para evitar nomes como "quantizado_lena.bmp.png"
    size_t pos = baseName.find_last_of('.');
    if (pos != std::string::npos) {
        baseName = baseName.substr(0, pos);
    }

    std::string outputFile = "../ImagensQuantizadas/C1/quantizado_" + baseName + "_quantizado.bmp";

    // Salvando a nova imagem com stb_image
    stbi_write_bmp(outputFile.c_str(), width, height, 1, newPixels.data());

    std::cout << "Imagem quantizada com sucesso!" << std::endl;

    double mse_value = mse(pixels, newPixels);
    double psnr_value = psnr(mse_value, bitdepth);

    std::cout << "Valor MSE: " << mse_value << std::endl;
    std::cout << "Valor PSNR: " << psnr_value << " dB" << std::endl;
}

void QuantizadorEscalar::quantizarDithering() {
    int width, height, original_channels, bitdepth;

    std::string input = "../Imagens/" + fileName;

    // Verificar as informações da imagem SEM carregá-la completamente
    if (!stbi_info(input.c_str(), &width, &height, &original_channels)) {
        std::cerr << "Erro: Nao foi possivel ler as informacoes do arquivo de imagem: " << fileName << std::endl;
        return;
    }

    // Validar se a imagem atende aos requisitos do codificador C1 (escala de cinza)
    if (original_channels != 1) {
        std::cerr << "Erro: O codificador C2 aceita apenas imagens em escala de cinza." << std::endl;
        std::cerr << "A imagem fornecida '" << fileName << "' tem " << original_channels << " canais." << std::endl;
        return;
    }

    std::cout << "Imagem de entrada valida (escala de cinza)." << std::endl;

    if (stbi_is_16_bit(input.c_str())) {
        bitdepth = 16;
    } else {
        // Assumimos que será 8 bits
        bitdepth = 8;
    }

    // Carregando a imagem com stb_image
    int channels_loaded;
    uint8_t *image = stbi_load(input.c_str(), &width, &height, &channels_loaded, 1);

    if (image == nullptr) {
        std::cerr << "Error loading image: " << fileName << std::endl;
        return;
    }

    // Passando as informações dos pixels para um vetor (para ser compatível com a função do MSE)
    std::vector<uint8_t> pixels_mse(image, image + width * height);

    // Passando as informações dos pixels para um vetor de double (por conta da dispersão do erro)
    std::vector<double> pixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        pixels[i] = static_cast<double>(image[i]);
    }

    stbi_image_free(image);

    // Vetor responsável por armazenar os pixels quantizados
    std::vector<uint8_t> newPixels(pixels.size());
    // Quantização com dithering
    for (size_t i = 0; i < pixels.size(); ++i) {
        size_t limite = pixels.size();
        double erroQuantizacao = 0;
        if (pixels[i] < 128) {
            newPixels[i] = 0; // Preto
            erroQuantizacao = pixels[i]; // pixels[i] - 0
        } else {
            newPixels[i] = 255; // Branco
            erroQuantizacao = pixels[i] - 255;
        }

        double erroDithering = erroQuantizacao / 8.0;

        if (i + 1 < limite) {
            pixels[i + 1] += erroDithering;
        }
        if (i + 2 < limite) {
            pixels[i + 2] += erroDithering;
        }
        if (i + width - 1 < limite) {
            pixels[i + width - 1] += erroDithering;
        }
        if (i + width < limite) {
            pixels[i + width] += erroDithering;
        }
        if (i + width + 1 < limite) {
            pixels[i + width + 1] += erroDithering;
        }
        if (i + 2 * width < limite) {
            pixels[i + 2*width] += erroDithering;
        }
    }

    // Pega o nome base do arquivo de entrada
    std::string baseName = fileName.substr(fileName.find_last_of("/\\") + 1);

    // Remove a extensão antiga para evitar nomes como "quantizado_lena.bmp.png"
    size_t pos = baseName.find_last_of('.');
    if (pos != std::string::npos) {
        baseName = baseName.substr(0, pos);
    }

    std::string outputFile = "../ImagensQuantizadas/C2/quantizado_" + baseName + "_quantizado.bmp";

    // Salvando a nova imagem com stb_image
    stbi_write_bmp(outputFile.c_str(), width, height, 1, newPixels.data());

    std::cout << "Imagem quantizada com sucesso!" << std::endl;

    double mse_value = mse(pixels_mse, newPixels);
    double psnr_value = psnr(mse_value, bitdepth);

    std::cout << "Valor MSE: " << mse_value << std::endl;
    std::cout << "Valor PSNR: " << psnr_value << " dB" << std::endl;
}

double QuantizadorEscalar::mse(const std::vector<uint8_t>& original, const std::vector<uint8_t> &quantized) {

    double mse = 0;

    for (size_t i = 0; i < original.size(); ++i) {
        double diff = static_cast<double>(original[i]) - static_cast<double>(quantized[i]);
        mse += diff * diff;
    }
    mse = mse / original.size();

    return mse;
}

double QuantizadorEscalar::psnr(double mse, int bitdepth) {

    double psnr = 0;
    double temp = pow(2.0, bitdepth);
    temp = (temp - 1) * (temp - 1);
    psnr = 10 * log10(temp/mse);

    return psnr;
}
