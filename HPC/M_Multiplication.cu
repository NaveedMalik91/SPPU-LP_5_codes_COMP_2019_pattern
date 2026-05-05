//Matrix Multiplication
#include<bits/stdc++.h>
#define N 3
#include<stdio.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

//kernal function 
__global__ void matrixMultiplication(int *a,int *b,int *c,int width){
    int row=blockIdx.y*blockDim.y+threadIdx.y;
    int col=blockIdx.x*blockDim.x+threadIdx.x;
    int sum=0;
    if(row<width && col<width){
        for(int k=0;k<width;k++){
            sum+=a[row*width+k]*b[k*width+col];
        }
        c[row*width+col]=sum;
    }
}

int main(){
    int size=N*N*sizeof(int);
    //memory assign to CPU(HOST)
    int *h_a=(int*)malloc(size);
    int *h_b=(int*)malloc(size);
    int *h_c=(int*)malloc(size);
    
    //memory assign to GPU(Device)
    int *d_a,*d_b,*d_c;
    cudaMalloc(&d_a,size);
    cudaMalloc(&d_b,size);
    cudaMalloc(&d_c,size);
    
    auto start_time = high_resolution_clock::now();
    //initialize
    for(int i=0;i<N*N;i++){
        h_a[i]=i;
        h_b[i]=N-i;
    }
    
    
    //copy values from cpu to gpu
    cudaMemcpy(d_a,h_a,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b,size,cudaMemcpyHostToDevice);
    
    //kernal function execution
    
    dim3 threadsPerBlock(16,16);
    dim3 blocksPerGrid((15+N)/16,(15+N)/16);
    
    matrixMultiplication<<<blocksPerGrid,threadsPerBlock>>>(d_a,d_b,d_c,N);
    
    
    
    //copy back from gpu to cpu
    cudaMemcpy(h_c,d_c,size,cudaMemcpyDeviceToHost);
    
    auto end_time = high_resolution_clock::now();
    duration<double>timeTaken=end_time-start_time;
    
    //print
    cout<<"Maxtrix A"<<endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<h_a[i*N+j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<endl;
    
    
    cout<<"Maxtrix B"<<endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<h_b[i*N+j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<endl;
    
    
    cout<<"Resultant Matrix C"<<endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<h_c[i*N+j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"Execution time by parallel Matrix Multiplication is: "<<timeTaken.count()<<" seconds"<<endl;
    cout<<endl;
    
}