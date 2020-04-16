#include "hip/hip_runtime.h"
#include "stdio.h"
#include <iostream>
#include <hip/hip_runtime.h>
#include <hip/hip_runtime.h>

// Defining number of elements in Array
#define N 64
#define NO_OF_BLOCKS 4
#define NO_OF_THREADS 256



#define HIPCHECK(cmd)                                                          \
do {                                                                           \
  hipError_t error = (cmd);                                                    \
  if (error != hipSuccess)                                                     \
  {                                                                            \
    std::cerr << "Encountered HIP error (" << error << ") at line "            \
              << __LINE__ << " in file " << __FILE__ << "\n";                  \
    exit(-1);                                                                  \
  }                                                                            \
} while (0)

#define MAX_GPU 8



// Defining Kernel function for vector addition
__global__ void VectorAdd(int *d_a, int *d_b, int *d_c)
{
  // Getting block index of current kernel
  int tid = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;
  if (tid < N)
    d_c[tid] = d_a[tid] + d_b[tid];
}


int main(void)
{
  // Defining host arrays
  int h_a[N], h_b[N], h_c[N];
  // Defining device pointers
  int *d_a[N], *d_b[N], *d_c[N];
  // allocate the memory
  
  hipStream_t stream[MAX_GPU];

  int nGpu = 1;
  HIPCHECK(hipGetDeviceCount(&nGpu));
  for (int i = 0; i < nGpu; i ++) {
    HIPCHECK(hipSetDevice(i));
    hipDeviceProp_t prop;
    HIPCHECK(hipGetDeviceProperties(&prop, i));
    printf("#   device %d [0x%02x] %s\n",
                    i, prop.pciBusID, prop.name);
    //create stream
    HIPCHECK(hipStreamCreate(&stream[i]));

    hipMalloc((void**)&d_a[i], N * sizeof(int));
    hipMalloc((void**)&d_b[i], N * sizeof(int));
    hipMalloc((void**)&d_c[i], N * sizeof(int));
    // Initializing Arrays
    for (int i = 0; i < N; i++) {
      h_a[i] = 2*i;
      h_b[i] = i ;
    }

    // Copy input arrays from host to device memory
    hipMemcpyAsync(d_a[i], h_a, N * sizeof(int), hipMemcpyHostToDevice, stream[i]);
    hipMemcpyAsync(d_b[i], h_b, N * sizeof(int), hipMemcpyHostToDevice, stream[i]);
  }
  
  for (int i = 0; i < nGpu; i ++) {
  HIPCHECK(hipSetDevice(i));

  // Calling VectorAdd kernels with NO_OF_BLOCKS and NO_OF_THREADS per block, passing
  // device pointers as parameters
  hipLaunchKernelGGL(VectorAdd, dim3(NO_OF_BLOCKS), dim3(NO_OF_THREADS), 0, stream[i], d_a[i], d_b[i], d_c[i]);
  }
  
  for (int i = 0; i < nGpu; i ++) {
    HIPCHECK(hipSetDevice(i));
    // Copy result back to host memory from device memory
    hipMemcpyAsync(h_c, d_c[i], N * sizeof(int), hipMemcpyDeviceToHost, stream[i]);
    HIPCHECK(hipStreamSynchronize(stream[i]));
    //printf("Vector addition on GPU \n");
    // Printing result on console
    for (int i = 0; i < N; i++) {
      /*printf("Operation result of %d element is %d + %d = %d\n",
         i, h_a[i], h_b[i],h_c[i]);*/
      if(h_a[i]+h_b[i] !=h_c[i]) {
        HIPCHECK(hipErrorUnknown); 
      }
    }
    // Free up memory
    HIPCHECK(hipStreamDestroy(stream[i]));
    hipFree(d_a[i]);
    hipFree(d_b[i]);
    hipFree(d_c[i]);
  }
  return 0;
}
