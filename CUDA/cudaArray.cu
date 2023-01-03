#include <stdio.h>

#define NUM_BLOCKS 4
#define NUN_THREAD 2

__global__ void arrayFunc( )
{
    if ((threadIdx.x | threadIdx.y | threadIdx.z
       | blockIdx.x | blockIdx.y | blockIdx.z) == 0) {
          printf("gridDim(%d, %d, %d), blockDim(%d, %d, %d)\n",
                  gridDim.x, gridDim.y, gridDim.z,
                  blockDim.x, blockDim.y, blockDim.z);
    }

    printf("threadIdx(%d, %d, %d) blockIdx(%d, %d, %d)\n",
            threadIdx.x, threadIdx.y, threadIdx.z,
            blockIdx.x, blockIdx.y, blockIdx.z);
}

int main(int argc, char **argv)
{
    arrayFunc<<<NUM_BLOCKS, NUN_THREAD>>>( ); 	/* 커널 호출 */

    /* 커널에서 사용한 printf( ) 함수의 결과를 화면으로 출력(flush) */
    cudaDeviceSynchronize( );

    printf("Hello CUDA! from Host\n"); 		/* Hello CUDA! 출력 */
    return 0;
}
