#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include "fractal/fractalfuncs.h"

#define MPI_PARALLEL
#ifdef MPI_PARALLEL
#include <mpi.h>
#endif


// #ifdef _OPENMP
// 	#include <omp.h>
// #else
// 	#define omp_get_num_threads() 0
// 	#define omp_get_thread_num() 0
// #endif



void Generate(struct IMG * img, int cluster_size, int rank){
    int scrsizex,scrsizey;
    scrsizex=img->cols;
    scrsizey=img->rows;
    int nloops= 0;
    int loop_start =(int) ( rank - 1 ) * ceil(scrsizey / (cluster_size - 1));
    int loop_end =(int) ( rank ) * ceil(scrsizey / (cluster_size - 1));
    //printf("loop_start is %d and loop_end is %d, with rank: %d and cluster_size: %d\n", loop_start, loop_end, rank, cluster_size);
	
    for(int j = loop_start; j<loop_end; j++) //Start vertical loop
    {
		for(int i = 0; i<scrsizex ; i++)						  			   //Start horizontal loop
		{				
			julia(img,i,j);							   
			i++;
			nloops++;
		} //End horizontal loop
    } //End vertical loop
}
    
int main(int argc, char ** argv){
    clock_t t1,t2;
    int resx,resy;
    struct IMG * img;
    int nepocs=0;
    float alpha=0;
    int rank = 0, size;
	
//alteração
    #ifdef MPI_PARALLEL
        MPI_Status *status;

        MPI_Init( &argc , &argv );
        MPI_Comm_rank(MPI_COMM_WORLD, &rank );
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        status=(MPI_Status*) malloc(sizeof(MPI_Status));
    #endif

    if (argc==1){
        resx=640;
        resy=480;
    } else if (argc==3){
        resx=atoi(argv[1]);
        resy=atoi(argv[2]);
    } else {
        printf("Erro no número de argumentos\n");
        printf("Se não usar argumentos a imagem de saida terá dimensões 640x480\n");
        printf("Senão devera especificar o numero de colunas seguido do numero de linhas\n");
        printf("Adicionalmente poderá especificar o numero de epocas de difusao e o factor de difusao,\\ caso contrario serao considerados como 0.");
        printf("\nExemplo: %s 320 240 \n",argv[0]);
        printf("\nExemplo: %s 320 240 100 0.5\n",argv[0]);
        exit(1);
    }  

    int img_size = resx*resy*sizeof(PIXEL);

    if( rank !=0 ) {
        img=(struct IMG *)malloc(sizeof(struct IMG));
        // divisao pelos varios procs, sem contar com proc 0
        img->pixels=(PIXEL *)malloc(img_size);
        img->cols=resx;
        img->rows=resy;

	    t1=clock();
	    Generate(img, size, rank);
        t2=clock();

        double time = (((double)(t2-t1))/CLOCKS_PER_SEC);

        MPI_Send((void*)&time, sizeof(double), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Send((void*)img->pixels, img_size, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
        // printf("rank %d is sending this: \nr:%f g:%f b:%f, \nr:%f g:%f b:%f\n\n", rank, 
        //     img->pixels[img->cols/4].r, img->pixels[img->cols/4].g, img->pixels[img->cols/4].b,
        //     img->pixels[img->cols/2].r, img->pixels[img->cols/2].g, img->pixels[img->cols/2].b);
    } else { 
        double time_mess;
        double clk;
        struct IMG * img_mess;

        img_mess = (struct IMG *)malloc(sizeof(struct IMG));
        img_mess->pixels=(PIXEL *)malloc(img_size);
        img_mess->cols=resx;
        img_mess->rows=resy;
        
        img=(struct IMG *)malloc(sizeof(struct IMG));
        img->pixels=(PIXEL *)malloc(resx*resy*sizeof(PIXEL));
        img->cols=resx;
        img->rows=resy;
        int chunk_size = (resx * resy) / ( size -1 );

        for( int source = 1 ; source < size ; source++){
            MPI_Recv((void*)&time_mess , sizeof(double), MPI_CHAR, source , 0 , MPI_COMM_WORLD, status);
            printf("%6.3f\n", time_mess );
            MPI_Recv( img_mess->pixels,  img_size, MPI_CHAR, source , source , MPI_COMM_WORLD, status);
            // printf("second message received from %d \nr:%f g:%f b:%f, \nr:%f g:%f b:%f\n\n", source,
            //     img_mess->pixels[img_mess->cols/4].r, img_mess->pixels[img_mess->cols/4].g, img_mess->pixels[img_mess->cols/4].b,
            //     img_mess->pixels[img_mess->cols/2].r, img_mess->pixels[img_mess->cols/2].g, img_mess->pixels[img_mess->cols/2].b);
            clk += (double)time_mess;
            
            printf("loop_start is %d and loop_end is %d\n", (source-1) * chunk_size, (source) * chunk_size);
            for( int i = (source-1) * chunk_size; i <  source * chunk_size; i++){
                img->pixels[i] = img_mess->pixels[i];
            }
        }

		printf("Julia Fractal gerado em %6.3f secs.\n",clk);

        /*
	    t1=clock();
	    Generate(img, 4, 1 );
        t2=clock();
        printf("rank %d is sending this: \nr:%f g:%f b:%f, \nr:%f g:%f b:%f\n\n", rank, 
            img->pixels[img->cols/4].r, img->pixels[img->cols/4].g, img->pixels[img->cols/4].b,
            img->pixels[img->cols/2].r, img->pixels[img->cols/2].g, img->pixels[100/2].b);
		//	mandel(img,resx,resy);
		*/
        saveimg(img,"build/julia.pgm");
	}
    
	MPI_Finalize();
}