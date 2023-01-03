#include <stdio.h>

/* 덧셈을 위한 커널 */
__global__ void add(int *a, int *b, int *sum) {
	*sum = *a + *b;
}

int main(int argc, char** argv)
{
	int a = 2, b = 4, sum;
	int *dev_a, *dev_b, *dev_sum;
	int size = sizeof( int );
	
	/* 변수 a, b, sum을 디바이스로 복사하기 위한 변수 */
	/* 정수형을 저장하기 위한 크기 */
	/* 변수 a, b, sum을 위한 디바이스의 공간 확보 */
	cudaMalloc( (void**)&dev_a, size );
	cudaMalloc( (void**)&dev_b, size );
	cudaMalloc( (void**)&dev_sum, size );
	
	/* 디바이스로 복사 */
	cudaMemcpy( dev_a, &a, size, cudaMemcpyHostToDevice );
	cudaMemcpy( dev_b, &b, size, cudaMemcpyHostToDevice );
	
	/* GPU에서 커널 함수 add() 실행 */
	add<<< 1, 1 >>>( dev_a, dev_b, dev_sum );
	
	/* GPU에서 계산한 결과값을 호스트의 sum 변수로 복사 */
	cudaMemcpy( &sum, dev_sum, size, cudaMemcpyDeviceToHost );

	/* 사용이 끝난 메모리 공간 해제 */
	cudaFree( dev_a );
	cudaFree( dev_b );
	cudaFree( dev_sum );

	/* 결과 출력 */
	printf("2 + 4 = %d from CUDA\n", sum);

	return 0;
}
