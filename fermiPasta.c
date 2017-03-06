#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N = 64
#define beta = 1.0
#define Pi = 3.14
#define delta_t 0.005
#define T = 5.0*pow(N,2.2)
#define instantaneas 1000 // Numero de instantaneas que se tomaran

void segunda_derivada(double *x_tt, double *x, int n, double beta); // Devuelve la segunda derivada

int main(){
  int i,iter, Nt;

  Nt = (int) T/delta_t;
  double *x, *v, *x0, *v0, *vmedio, *a; // Posiciones, velocidad, pos ini, vel ini, vel en el medio, aceleracion
  x0 = malloc(N*sizeof(double)); // Crea posicion inicial
  v0 = malloc(N*sizeof(double)); // Crea velocidad inicial
  a = malloc(N*sizeof(double)); // Crea velocidad inicial
  xf = malloc(N*sizeof(double)); // Crea velocidad inicial

  for(i=0;i<N;i++){ // Inicializa valores iniciales
    x[i] = x0[i] = sin(Pi*i/((double)(N - 1)));
    v[i] = v0[i] = 0;
  }

  for(iter=0;iter<Nt;iter++){
    segunda_derivada(a,x,N,beta);
    for(i=0;i<N;i++){
      vmedio[i] = v[i] +0.5*delta_t*a[i];
    }
    vmedio[0] = vmedio[N-1] = 0;
    for(i=0;i<N;i++){
      x[i] = x[i] + delta_t*vmedio[i];
    }
    segunda_derivada(a,x,N,beta);
    for(i=0;i<N;i++){
      v[i] = vmedio[i] + 0.5*delta_t*a[i];
    }
  }
  return 0;
}

void segunda_derivada(double *x_tt, double *x, int n, double beta){
  int i;
  x_tt[0] = 0;
  x_tt[n-1] = 0;

  for(i=0;i<n-1;i++){
    x_tt[i] = (x[i+1] - 2*x[i] + x[i-1]) + beta*(pow((x[i+1] - x[i]),3) - pow((x[i] - x[i-1]),3));
  }
}
