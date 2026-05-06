//Vector addition 
//CUDA-> compute unified device architecture -> developed by NVIDIA -> prallel computing platform
//Grid-> Blocks -> Multiple Threads
#include<iostream>
#include<stdio.h>
#include<cuda.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

//kernal function 

__global__ void vectorAdd(int *a,int *b,int *c,int N){
    //global id for thread
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    
    if(i<N){
        c[i]=a[i]+b[i];
    }
}


int main(){
    //vector contains 10 elements
    int N=10;
    //size of entire vector
    auto size = N*sizeof(int);
    
    //create memory for CPU (HOST)
    
    int *h_a=(int*)malloc(size);
    int *h_b=(int*)malloc(size);
    int *h_c=(int*)malloc(size);
    
    
    //create memory for GPU (Device)
    
    int *d_a,*d_b,*d_c;
    cudaMalloc(&d_a,size);
    cudaMalloc(&d_b,size);
    cudaMalloc(&d_c,size);
    
    //initialize vectors with elements 
    
    auto start_time= high_resolution_clock::now();
    
    for(int i=0;i<N;i++){
        h_a[i]=i;
        h_b[i]=N-i;
    }
    
    
    //copy values from cpu to GPU
    
    cudaMemcpy(d_a,h_a,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b,size,cudaMemcpyHostToDevice);
    
    //kernal function execution
    int block_size=256;
    int grid_size=(block_size+N-1)/block_size;
    
    vectorAdd<<<grid_size,block_size>>>(d_a,d_b,d_c,N);
    
    
    //copy back from gpu to cpu
    
    cudaMemcpy(h_c,d_c,size,cudaMemcpyDeviceToHost);
    
    auto end_time = high_resolution_clock::now();
    duration<double> timeTaken=end_time-start_time;
    
    cout<<"Execution time for parallel Vector Addition: "<<timeTaken.count()<<" seconds"<<endl;
    
    
    auto start_time1=high_resolution_clock::now();
    for(int i=0;i<N;i++){
        h_c[i]=h_a[i]+h_b[i];
    }
    auto end_time1=high_resolution_clock::now();
    duration<double>timeTaken2= end_time1-start_time1;
    
    cout<<"Execution time for Sequential vector addition: "<<timeTaken2.count()<<" seconds"<<endl;
    //print 
    
    
    
    cout<<"Vector A"<<endl;
    for(int i=0;i<N;i++){
        cout<<h_a[i]<<" ";
    }
    cout<<endl;
    
    cout<<"Vector B"<<endl;
    for(int i=0;i<N;i++){
        cout<<h_b[i]<<" ";
    }
    cout<<endl;
    
    cout<<"Vector C"<<endl;
    for(int i=0;i<N;i++){
        cout<<h_c[i]<<" ";
    }
    cout<<endl;   
}

//command: !nvcc vectorAdd.cu -o vectorAdd ->>>> !./vectorAdd
