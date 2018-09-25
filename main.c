#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "150.bmp"

#define THRESHOLD 0.2f
#define ALTURA_MIN 0
#define LARGURA_MIN 0
#define N_PIXELS_MIN 3
/*============================================================================*/

/*============================================================================*/

int main()
{
    char *files[13] = {"60", "82", "114", "150", "205"};

    char fileName[25];

    for (int idx = 0;idx <5; idx++){

        sprintf(fileName, "./%s.bmp", files[idx]);
        Imagem *img = abreImagem(fileName, 3);
        if (!img)
        {
            printf("Erro abrindo a imagem.\n");
            exit(1);
        }

        Imagem *img_bin = criaImagem(img->largura, img->altura, 3);
        Imagem *img_ero = criaImagem(img->largura, img->altura, 3);

        Imagem *kernel = criaKernelCircular(3);
        Coordenada centro_kernel = criaCoordenada(1, 1);

        binarizaAdapt(img, img_bin, 301, THRESHOLD, NULL);
        //binariza(img, img_bin, thresholdOtsu (img));

        erode(img_bin, kernel, centro_kernel, img_ero);

        sprintf(fileName, "./resultados/%s-binariza.bmp", files[idx]);
        salvaImagem(img_bin, fileName);

        sprintf(fileName, "./resultados/%s-ero.bmp", files[idx]);
        salvaImagem(img_ero, fileName);

        ComponenteConexo *componentes;
        int numComponentes = rotulaFloodFill(img_ero, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);

        for (int i = 0; i < numComponentes; i++)
        {
            desenhaRetangulo(componentes[i].roi, criaCor(0, 255, 0), img_ero);
        }

        sprintf(fileName, "./resultados/%s-final.bmp", files[idx]);
        salvaImagem(img_ero, fileName);

        printf("Num de componentes: %d\n", numComponentes);

        destroiImagem(img);
        destroiImagem(kernel);
        destroiImagem(img_ero);
        destroiImagem(img_bin);
    }
    return (0);
}

/*============================================================================*/
