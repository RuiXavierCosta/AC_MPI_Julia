#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include "fractal/fractalfuncs.h"

// #ifdef _OPENMP
// 	#include <omp.h>
// #else
// 	#define omp_get_num_threads() 0
// 	#define omp_get_thread_num() 0
// #endif

void Generate(struct IMG * img){
    int j=0;
    int scrsizex,scrsizey;
    scrsizex=img->cols;
    scrsizey=img->rows;
    int nloops= 0;
	
	//#pragma omp parallel reduction(+: j) num_threads(4)
	{
		//#pragma omp sections
		{
			//#pragma omp section
			{
				do //Start vertical loop
				{
					int i=0;
					do							  			   //Start horizontal loop
					{			
						julia(img,i,j);
						i++;
						nloops++;
					}
					while ( (i<scrsizex)  );	  //End horizontal loop
				j++;
				}
				while ( (j<1*(scrsizey/4)) );        //End vertical loop
				//printf("loops so far: %d\n, nloops);
			}
			//#pragma omp section
			{
				j = 1*(scrsizey/4);
				do //Start vertical loop
				{
					int i=0;
					do							  			   //Start horizontal loop
					{			
						julia(img,i,j);
						i++;
						nloops++;
					}
					while ( (i<scrsizex)  );	  //End horizontal loop
				j++;
				}
				while ( (j< 2* (scrsizey/4))  );        //End vertical loop
				//printf("loops so far: %d\n, nloops);
			}
			//#pragma omp section
			{
				j = 2*(scrsizey/4);
				do //Start vertical loop
				{
					int i=0;
					do							  			   //Start horizontal loop
					{			
						julia(img,i,j);
						i++;
						nloops++;
					}
					while ( (i<scrsizex)  );	  //End horizontal loop
				j++;
				}
				while ( (j< 3* (scrsizey/4))  );        //End vertical loop
				//printf("loops so far: %d\n, nloops);
			}
			//#pragma omp section
			{
				j = 3*(scrsizey/4);
				do //Start vertical loop
				{
					int i=0;
					do							  			   //Start horizontal loop
					{			
						julia(img,i,j);
						i++;
						nloops++;
					}
					while ( (i<scrsizex)  );	  //End horizontal loop
				j++;
				}
				while ( (j< 4* (scrsizey/4))  );        //End vertical loop
				//printf("loops so far: %d\n, nloops);
			}
		}
		// printf("The total number of iterations is %d\n",
		// nloops);
	}	
}

PIXEL calculateDiffusion(int i, int j, struct IMG * I_k, float alpha){
    PIXEL diffused_pixel;
    
    diffused_pixel.r = (1 - alpha) * I_k->pixels[j * I_k->cols + i].r +  //(1 − α)Ik(i, j) +
        alpha * (1/8) * (                       // α*(1/8)[
            I_k->pixels[(j-1) * I_k->cols + (i-1)].r +     // Ik(i-1, j-1)] +
            I_k->pixels[j * I_k->cols + i-1].r +           // Ik(i − 1, j) + 
            I_k->pixels[(j + 1) * I_k->cols + i-1].r +     // Ik(i − 1, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i].r +       // Ik(i, j − 1) +
            I_k->pixels[(j + 1) * I_k->cols + i].r +       // Ik(i, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i+1].r +     // Ik(i + 1, j − 1) +
            I_k->pixels[j * I_k->cols + i+1].r +           // Ik(i + 1, j) +
            I_k->pixels[(j + 1) * I_k->cols + i + 1].r     // Ik(i + 1, j + 1)] ,
        );      

    diffused_pixel.g = (1 - alpha) * I_k->pixels[j * I_k->cols + i].g +  //(1 − α)Ik(i, j) +
        alpha * (1/8) * (                       // α*(1/8)[
            I_k->pixels[(j-1) * I_k->cols + (i-1)].g +     // Ik(i-1, j-1)] +
            I_k->pixels[j * I_k->cols + i-1].g +           // Ik(i − 1, j) + 
            I_k->pixels[(j + 1) * I_k->cols + i-1].g +     // Ik(i − 1, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i].g +       // Ik(i, j − 1) +
            I_k->pixels[(j + 1) * I_k->cols + i].g +       // Ik(i, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i+1].g +     // Ik(i + 1, j − 1) +
            I_k->pixels[j * I_k->cols + i+1].g +           // Ik(i + 1, j) +
            I_k->pixels[(j + 1) * I_k->cols + i + 1].g     // Ik(i + 1, j + 1)] ,
        );  

    diffused_pixel.b = (1 - alpha) * I_k->pixels[j * I_k->cols + i].b +  //(1 − α)Ik(i, j) +
        alpha * (1/8) * (                       // α*(1/8)[
            I_k->pixels[(j-1) * I_k->cols + (i-1)].b +     // Ik(i-1, j-1)] +
            I_k->pixels[j * I_k->cols + i-1].b +           // Ik(i − 1, j) + 
            I_k->pixels[(j + 1) * I_k->cols + i-1].b +     // Ik(i − 1, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i].b +       // Ik(i, j − 1) +
            I_k->pixels[(j + 1) * I_k->cols + i].b +       // Ik(i, j + 1) +
            I_k->pixels[(j - 1) * I_k->cols + i+1].b +     // Ik(i + 1, j − 1) +
            I_k->pixels[j * I_k->cols + i+1].b +           // Ik(i + 1, j) +
            I_k->pixels[(j + 1) * I_k->cols + i + 1].b     // Ik(i + 1, j + 1)] ,
        );  

    return diffused_pixel;
}

void difuse(struct IMG * imgin, int nepocs, float alpha){
    struct IMG * temp,*imgnew;
    int k, i, j;
    char filename[200];
	clock_t t1,t2;
	
    imgnew=(struct IMG *) malloc(sizeof(struct IMG));
    imgnew->rows=imgin->rows;
    imgnew->cols=imgin->cols;
    imgnew->pixels=(PIXEL *)malloc(imgnew->cols*imgnew->rows*sizeof(PIXEL));
	t1 = clock();
    for (k=1;k<=nepocs;k++){
	// apply diffusion for each color channel, NEVER mixing them...
        for(j=1; j < imgin->cols -1; j++)
        {
            //#pragma omp parallel
            {
                //#pragma omp for
                for(i=1; i < imgin->rows -1; i++)
                {
					imgnew->pixels[j * imgnew->cols + i] = calculateDiffusion(i, j, imgin, alpha);
                }
            }
        }
        
        sprintf(filename,"build/difuse_images/output%04d.jpg",k);
        saveimg(imgnew,filename);
        temp=imgin;
        imgin=imgnew;
        imgnew=temp;
	}
	
    t2=clock();
    printf("Imagens geradas em %6.3f secs.\n",(((double)(t2-t1))/CLOCKS_PER_SEC));
}
    
int main(int argc, char ** argv){
    clock_t t1,t2;
    int resx,resy;
    struct IMG * img;
    int nepocs=0;
    float alpha=0;

	
    if (argc==1){
	resx=640;
	resy=480;
    } else if ((argc==3)||(argc==5)){
	resx=atoi(argv[1]);
	resy=atoi(argv[2]);
	if(argc==5){
	    nepocs=atoi(argv[3]);
	    alpha=atof(argv[4]);
	    if (alpha<0.0 || alpha>1.0){
		printf("Alpha tem de estar entre 0 e 1\n");
		exit(1);
	    }
	}
    } else {
	printf("Erro no número de argumentos\n");
	printf("Se não usar argumentos a imagem de saida terá dimensões 640x480\n");
	printf("Senão devera especificar o numero de colunas seguido do numero de linhas\n");
	printf("Adicionalmente poderá especificar o numero de epocas de difusao e o factor de difusao,\\ caso contrario serao considerados como 0.");
	printf("\nExemplo: %s 320 240 \n",argv[0]);
	printf("\nExemplo: %s 320 240 100 0.5\n",argv[0]);
	exit(1);
    }
    img=(struct IMG *)malloc(sizeof(struct IMG));
    
    img->pixels=(PIXEL *)malloc(resx*resy*sizeof(PIXEL));
    img->cols=resx;
    img->rows=resy;
    
	
    t1=clock();
    Generate(img);
    t2=clock();
    printf("Julia Fractal gerado em %6.3f secs.\n",(((double)(t2-t1))/CLOCKS_PER_SEC));
    //	mandel(img,resx,resy);
    saveimg(img,"build/julia.pgm");
    
    if(nepocs>0)
	difuse(img,nepocs,alpha);
}
										  
