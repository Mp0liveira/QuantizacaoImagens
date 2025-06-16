#include <iostream>
#include "includes/QuantizadorEscalar.h"

int main() {
    QuantizadorEscalar quantizador_escalar("lena_gray.png");
    quantizador_escalar.quantizarImagem();
    quantizador_escalar.quantizarDithering();

    return 0;
}