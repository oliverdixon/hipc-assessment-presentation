extern __shared__ __align__(sizeof(T)) unsigned char shared_memory[];
T* shared_data = reinterpret_cast<T*>(shared_memory);

const unsigned int thread_idx = threadIdx.x;
const unsigned int input_idx = blockIdx.x * blockDim.x * 2 + thread_idx;

// Fold the first two elements into a sum and copy to shared memory.
if (input_idx + blockDim.x < input_size)
    shared_data[thread_idx] = input[input_idx] + input[input_idx + blockDim.x];
else if (input_idx < input_size)
    shared_data[thread_idx] = input[input_idx];
else
    shared_data[thread_idx] = 0;
__syncthreads();

// Do the reduction in shared memory.
for (unsigned int stride = blockDim.x >> 1; stride > 0; stride >>= 1) {
    if (thread_idx < stride)
        shared_data[thread_idx] += shared_data[thread_idx + stride];
    __syncthreads();
}

// Write the result for this block to global memory.
if (thread_idx == 0)
    output[blockIdx.x] = shared_data[0];
