#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef PRECISION real;

const int NX = 128;
const int NY = 128;
const int MAX_ITERATIONS = 1<<16;

const real DX = 1.0/(NX-1);
const real DY = 1.0/(NY-1);

const real D = 2.0*(DX*DX + DY*DY);
const real D_x = DY*DY/D;
const real D_y = DX*DX/D;
const real B = -(DX*DX*DY*DY)/D;

void run_jacobi(real p[NX][NY], const real b[NX][NY]) {
  for(int iter = 0; iter<MAX_ITERATIONS; ++iter) {
    for(int i=1; i<NX-1; ++i) {
      for(int j=1; j<NY-1; ++j) {
        p[i][j] = D_x*(p[i+1][j] + p[i-1][j]) + D_y*(p[i][j+1] + p[i][j-1]) + B*b[i][j];
      }
    }
  }
}

int main(int argc, char* argv[]) {
  const int nx = NX;
  const int ny = NY;
  const int max_iterations = MAX_ITERATIONS;

  real p[NX][NY];
  real b[NX][NY];
  real p_soln[NX][NY];

  for(int i=0; i<nx; ++i) {
    for(int j=0; j<ny; ++j) {
      real x = i*DX;
      real y = j*DY;

      b[i][j] = sin(M_PI*x)*sin(M_PI*y);
      p_soln[i][j] = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p[i][j] = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<nx-1; ++i) {
    for(int j=1; j<ny-1; ++j) {
      av_error += fabs(p[i][j] - p_soln[i][j]);
    }
  }
  av_error /= (nx*ny);

  printf("exe name, nx, ny, MAX_ITERATIONS, Runtime, Average Error\n");
  printf("%s, %d, %d, %d, %d, %e\n", argv[0], nx, ny, max_iterations, msec, av_error);

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
