#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"

#define N  64 // Numero de particulas
#define beta  1.0 // Constante de la ec. dif.
#define Pi  3.14159265359
#define delta_t 0.005 // Intervalo de tiempo para cada paso
#define T  5.0*pow((double)N,2.2) // Tiempo total de integracion
#define instantaneas 1000 // Numero de instantaneas que se tomaran

void segunda_derivada(double *x_tt, double *x); // Devuelve la segunda derivada
void imprimir_energia(double *x, double *v, double t); // Imprime la energia

int main(int argc, char **argv){
  int n = atoi(argv[1]); // Numero de threads a usar
  omp_set_num_threads(n);

  int i,iter, Nt, im;

  Nt = (int) T/(1000*delta_t); // Numero de pasos en t entre cada instantanea
  
  double *x, *v, *x0, *v0, *vmedio, *a; // Posiciones, velocidades, pos ini, vel ini, vel en el medio, aceleracion
  x = malloc(N*sizeof(double)); // Crea posiciones
  v = malloc(N*sizeof(double)); // Crea velocidades
  x0 = malloc(N*sizeof(double)); // Crea posicion inicial
  v0 = malloc(N*sizeof(double)); // Crea velocidad inicial
  vmedio = malloc(N*sizeof(double)); // Crea velocidades tras medio paso
  a = malloc(N*sizeof(double)); // Crea aceleracion

#pragma omp parallel for shared(x,v,x0,v0)
  for(i=0;i<N;i++){ // Inicializa valores iniciales
    x[i] = x0[i] = sin(Pi*i/(N - 1));
    v[i] = v0[i] = 0;
  }
  x[0] = x[N-1] = 0; // Cond. de front.

  imprimir_energia(x,v,0); // Imprime la primera instantanea

  for(im=1;im<1000;im++){
    for(iter=0;iter<Nt;iter++){
      segunda_derivada(a,x);

#pragma omp parallel for shared(v,vmedio,a)
      for(i=0;i<N;i++){
	vmedio[i] = v[i] + 0.5*delta_t*a[i];
      }

      vmedio[0] = vmedio[N-1] = 0;

#pragma omp parallel for shared(x,vmedio)
      for(i=0;i<N;i++){
	x[i] += delta_t*vmedio[i];
      }

      x[0] = x[N-1] = 0;
      segunda_derivada(a,x);

#pragma omp parallel for shared(v,vmedio,a)
      for(i=0;i<N;i++){
	v[i] = vmedio[i] + 0.5*delta_t*a[i];
      }

      v[0] = v[N-1] = 0;
    }
    imprimir_energia(x,v,T*im/1000);
  }
  return 0;
}

void segunda_derivada(double *x_tt, double *x){
  int i;
  x_tt[0] = x_tt[N-1] = 0;

#pragma parallel for shared(x_tt,x)
  for(i=1;i<N-1;i++){
    x_tt[i] = (x[i+1] - 2*x[i] + x[i-1]) + beta*(pow((x[i+1] - x[i]),3) - pow((x[i] - x[i-1]),3));
  }

}

void imprimir_energia(double *x, double *v, double t){
  int i,j;
  double A[3], Apunto[3], k[3], w_2[3], E[3], suma;

  double tiempo = omp_get_wtime();

  for(j=0;j<3;j++){
    A[j] = 0;
    Apunto[j] = 0;
    k[j] = j + 1;
    w_2[j] = 4*pow(sin(k[j]*Pi/(2*N + 2)),2);

    suma = 0;
#pragma omp parallel for reduction( + : suma)
    for(i=0;i<N;i++){
      suma = suma + x[i]*sin(i*k[j]*Pi/(N+1));
    }
    A[j] = suma;

    suma = 0;
#pragma omp parallel for reduction ( + : suma)
    for(i=0;i<N;i++){
      suma = suma + v[i]*sin(i*k[j]*Pi/(N+1));
    }
    Apunto[j] = suma;

    A[j] = A[j]*pow((2.0/(N+1)),0.5);
    Apunto[j] = Apunto[j]*pow((2.0/(N+1)),0.5);
    E[j] = (pow(Apunto[j],2) + w_2[j]*pow(A[j],2))/2;
  }
  printf("%f %f %f %f\n",t,E[0],E[1],E[2]);
}
