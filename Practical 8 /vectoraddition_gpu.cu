#include <stdio.h>
#include <cuda.h>

#define N 8

__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int A[N], B[N], C[N];
    int *d_A, *d_B, *d_C;
    int size = N * sizeof(int);
    cudaError_t err;

    // User input
    printf("Enter %d elements for vector A:\n", N);
    for(int i=0;i<N;i++) scanf("%d",&A[i]);

    printf("Enter %d elements for vector B:\n", N);
    for(int i=0;i<N;i++) scanf("%d",&B[i]);

    // Allocate GPU memory
    err = cudaMalloc((void**)&d_A, size); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}
    err = cudaMalloc((void**)&d_B, size); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}
    err = cudaMalloc((void**)&d_C, size); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}

    // Copy data to GPU
    err = cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}
    err = cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}

    // Launch kernel
    vectorAdd<<<1, N>>>(d_A, d_B, d_C, N);
    cudaDeviceSynchronize(); // Wait for kernel to finish

    // Copy result back
    err = cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost); if(err != cudaSuccess) {printf("%s\n",cudaGetErrorString(err)); return -1;}

    // Print result
    printf("Result vector C:\n");
    for(int i=0;i<N;i++) printf("%d ", C[i]);
    printf("\n");

    // Free GPU memory
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);

    return 0;
}
#include <stdio.h>
#include <cuda.h>

#define N 8

// CUDA kernel function
__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int A[N], B[N], C[N];
    int *d_A, *d_B, *d_C;
    int size = N * sizeof(int);

    // Take input from user
    printf("Enter %d elements for vector A:\n", N);
    for(int i = 0; i < N; i++)
        scanf("%d", &A[i]);

    printf("Enter %d elements for vector B:\n", N);
    for(int i = 0; i < N; i++)
        scanf("%d", &B[i]);

    // Allocate memory on GPU
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy data from host to device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Launch kernel: 1 block, N threads
    vectorAdd<<<1, N>>>(d_A, d_B, d_C, N);

    // Wait for kernel to finish
    cudaDeviceSynchronize();

    // Copy result back to host
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Print result
    printf("Result vector C:\n");
    for(int i = 0; i < N; i++)
        printf("%d ", C[i]);
    printf("\n");

    // Free GPU memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}

