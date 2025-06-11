#include <iostream>

#include "includes/QuantizadorEscalar.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

int main() {
    QuantizadorEscalar quantizador_escalar("Imagens/lena_grey.bmp");
    quantizador_escalar.quantizarImagem();

    std::cout << "STB configurado com sucesso!" << std::endl;
    return 0;
}