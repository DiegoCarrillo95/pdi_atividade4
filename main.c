#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"
#include <math.h>

/*============================================================================*/
#define INPUT_IMAGE "150.bmp"

#define THRESHOLD 0.2f
#define ALTURA_MIN 1
#define LARGURA_MIN 1
#define N_PIXELS_MIN 1
/*============================================================================*/

/*============================================================================*/

int main()
{
    char *files[13] = {"60", "82", "114", "150", "205"};

    char fileName[25];

    for (int idx = 1;idx <5; idx++){

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

        int pixels = 0;
        float variancia = 0;
        float media = 0;
        int desvioPadrao = 0;
        int numArroz = 0;
        int i;

        for (i = 0; i < numComponentes; i++){
            pixels += componentes[i].n_pixels;
        }

        media = pixels/numComponentes;

        for (i = 0; i < numComponentes; i++){
          int sub = (int) componentes[i].n_pixels - media;
          variancia = variancia + sub*sub;
        }

        variancia = variancia / (numComponentes - 1);

        desvioPadrao = sqrt(variancia);

        numArroz = 0;

        for (i = 0; i < numComponentes; i++){
          if(componentes[i].n_pixels <= media){
            numArroz++;

          }else{
            float n =componentes[i].n_pixels / (float) media;
            numArroz += (int) (n + 0.5f);
            //printf("rice correction: %f\n", n + 0.5f);    
          }

        }
        
        printf("Num de componentes: %d\n", numArroz);

        destroiImagem(img);
        destroiImagem(kernel);
        destroiImagem(img_ero);
        destroiImagem(img_bin);
    }
    return (0);
}

/*============================================================================*/