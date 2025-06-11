#include "../includes/QuantizadorEscalar.h"

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>
#include "../libs/stb_image.h"
#include "../libs/stb_image_write.h"

QuantizadorEscalar::QuantizadorEscalar(const std::string &inputFile) : fileName(inputFile) {}

void QuantizadorEscalar::quantizarImagem() {
    int width, height, channels;

    // Carregando a imagem com stb_image
    unsigned char *image = stbi_load(fileName.c_str(), &width, &height, &channels, 1);

    if (image == nullptr) {
        std::cerr << "Error loading image: " << fileName << std::endl;
        return;
    }

    // Vetor responsável por armazenar os pixels quantizados
    std::vector<uint8_t> newImage(width * height);
    // Quantização
    for (int i = 0; i < width * height; ++i) {
        if (image[i] < 128) {
            newImage[i] = 0;   // Preto
        } else {
            newImage[i] = 255; // Branco
        }
    }

    std::string outputFile = "ImagensQuantizadas/quantizado_" + fileName;

    // Salvando a nova imagem com stb_image
    stbi_write_bmp(outputFile.c_str(), width, height, 1, newImage.data());

    std::cout << "Imagem quantizada com sucesso!" << std::endl;

    stbi_image_free(image);
}
