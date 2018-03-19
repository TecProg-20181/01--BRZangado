#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

//declaração de funções

int pixel_igual(Pixel p1, Pixel p2);

void inverter_cores(unsigned short int pixel[512][512][3], unsigned int w, unsigned int h);

void blur(unsigned int h, unsigned short int pixel[512][512][3], int T, unsigned int w);

void espelhamento(int horizontal, unsigned short int pixel[512][512][3], unsigned int img_width, unsigned int img_height);

void imprimirImagem(Image img);

void filtroSepia(unsigned short int pixel[512][512][3], unsigned int img_width, unsigned int img_height);

Image escala_de_cinza(Image img);

Image rotacionar90direita(Image img, int quantas_vezes);

Image cortar_imagem(Image img, int x, int y, int new_width, int new_height);

Image initImage(Image img);



int main() {


    Image img;
    img = initImage(img);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                
				filtroSepia(img.pixel, img.width, img.height);
                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                img = rotacionar90direita(img, quantas_vezes);
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);
                espelhamento(horizontal, img.pixel, img.width, img.height);
                break;
            }
            case 6: { // Inversao de Cores
            	inverter_cores(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                
                int x, y;
                int new_width, new_height;
                
                scanf("%d %d", &x, &y);
                scanf("%d %d", &new_width, &new_height);

                img = cortar_imagem(img, x, y, new_width, new_height);
                break;
            }
        }

    }


	imprimirImagem(img);

    return 0;
}

//  --------- Implementado funções previamente declaradas

void filtroSepia(unsigned short int img_pixel[512][512][3], unsigned int img_width, unsigned int img_height){


	for (int i = 0; i < img_height; ++i) {
       for (int j = 0; j < img_width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img_pixel[i][j][0];
            pixel[1] = img_pixel[i][j][1];
            pixel[2] = img_pixel[i][j][2];

          	int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            img_pixel[i][j][0] = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
          	img_pixel[i][j][1] = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img_pixel[i][j][2] = menor_r;
         }
     }	


}

void imprimirImagem(Image img){

	printf("P3\n");
      printf("%u %u\n255\n", img.width, img.height);
      for (int i = 0; i < img.height; ++i) {
          for (int j = 0; j < img.width; ++j) {
              printf("%hu %hu %hu ", img.pixel[i][j][0],
                                     img.pixel[i][j][1],
                                     img.pixel[i][j][2]);

          }
          printf("\n");
      }

}

void espelhamento(int horizontal, unsigned short int pixel[512][512][3], unsigned int img_width, unsigned int img_height) {
  
    int width = img_width;
    int height = img_height;

    if (horizontal == 1)
        width /= 2;
    else
        height /= 2;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            
            int x = i;
            int y = j;

            if (horizontal == 1)
                y = img_width - 1 - j;
            else
                x = img_height - 1 - i;

            Pixel aux1;
            aux1.r = pixel[i][j][0];
            aux1.g = pixel[i][j][1];
            aux1.b = pixel[i][j][2];

            pixel[i][j][0] = pixel[x][y][0];
            pixel[i][j][1] = pixel[x][y][1];
            pixel[i][j][2] = pixel[x][y][2];

            pixel[x][y][0] = aux1.r;
            pixel[x][y][1] = aux1.g;
            pixel[x][y][2] = aux1.b;
        }
    }
}

Image cortar_imagem(Image img, int x, int y, int new_width, int new_height) {
    Image cortada;

    cortada.width = new_width;
    cortada.height = new_height;

    for(int i = 0; i < new_height; ++i) {
        for(int j = 0; j < new_width; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cortada;
}

Image initImage(Image img){

        // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    return img;


}



void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image rotacionar90direita(Image img, int quantas_vezes) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;
    
    for (int x = 0; x < quantas_vezes; ++x) {
    
    	for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
		    for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
		        rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
		        rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
		        rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
		    }
    	}
    
    
    }

    return rotacionada;
}


int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r && p1.g == p2.g && p1.b == p2.b)
        return 1;
    return 0;
}

Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
        
            int media = img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2];
            
            media /= 3;
            
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(unsigned int h, unsigned short int pixel[512][512][3], int T, unsigned int w) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
            int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}
