#include <stdio.h>
#include <math.h>

#define DATA 24
#define N 3 * (DATA - 1)

void view_array(double a[N]);
void view_matrix(double a[N][N]);
void spline(double *x, double *y, double *c);
void scaling(double a[N][N], double b[N]);
void pivot(double a[N][N], double b[N]);
void load_file(char *file, double *x, double *y);
void LU_y(double L[N][N], double *b, double *y);
void LU_x(double U[N][N], double *y, double *x);
void LU(double a[N][N], double b[N], double c[N]);

int main(int argc, char *argv[]){
    int i;
    double x[DATA], y[DATA], c[N];
    
    load_file(argv[1], x, y);
    
    for (i = 0; i < DATA; i++) {
      printf("%f %f\n", x[i],y[i]);
    }
    spline(x, y, c);
}

void view_array(double a[N]){
    int i;
    for (i = 1; i <= N; i++) {
      printf("%+2.8f ", a[i - 1]);
        if (i % N == 0) {
            printf("\n");
        }
    }
}

void view_matrix(double a[N][N]){
  int i, j;
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
	      printf("%+2.1f ", a[i][j]);
        }
        printf("\n");
    }
}

void LU_y(double L[N][N], double *b, double *y){
    int i, j;
    float sum;

    y[0] = b[0];

    for (i = 1; i < N; i++) {
        sum = 0;
        for (j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        y[i] = b[i] - sum;
    }
}
void LU_x(double U[N][N], double *y, double *x){
    int i, j;
    float sum;

    x[N - 1] = y[N - 1] / U[N - 1][N - 1];

    for (i = N - 2; i >= 0; i--) {
        sum = 0;
        for (j = N - 1; j > i; j--) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }
}

void LU(double a[N][N], double b[N], double c[N]){
    int i, j, k;
    double L[N][N] = {0}, U[N][N] = {0}, y[N] = {0}, sum;
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            if (i <= j) {
                if (i == j) {
                    L[i][i] = 1;
                }
                for (k = 0; k < i; k++) {
                    sum += L[i][k] * U[k][j];
                }
                U[i][j] = a[i][j] - sum;
            } else {
                for (k = 0; k < j; k++) {
                    sum += L[i][k] * U[k][j];
                }
                L[i][j] = (a[i][j] - sum) / U[j][j];
            }
        }
    }
    
    view_matrix(L);
    printf("\n");
    view_matrix(U);
    printf("\n");
    LU_y(L, b, y);
    LU_x(U, y, c);
    view_array(c);
    printf("\n");
}

void spline(double *x, double *y, double *c){
    int i, j, tmp;
    double a[N][N] = {0}, b[N] = {0};

    for (i = 0; i < DATA - 1; i++) {
        a[i][i * 3 + 0] = x[i + 1] - x[i];
        a[i][i * 3 + 1] = pow(x[i + 1] - x[i], 2);
        a[i][i * 3 + 2] = pow(x[i + 1] - x[i], 3);
        b[i] = y[i + 1] - y[i];
    }
    for (tmp = i, i = 0; i < DATA - 2; i++) {
        a[tmp + i][i * 3 + 0] = 1;
        a[tmp + i][i * 3 + 1] = 2 * (x[i + 1] - x[i]);
        a[tmp + i][i * 3 + 2] = 3 * pow(x[i + 1] - x[i], 2);
        a[tmp + i][(i + 1) * 3 + 0] = -1;
        b[tmp + i] = 0;
    }
    for (tmp += i, i = 0; i < DATA - 2; i++) {
        a[tmp + i][i * 3 + 1] = 1;
        a[tmp + i][i * 3 + 2] = 3 * (x[i + 1] - x[i]);
        a[tmp + i][(i + 1) * 3 + 1] = -1;
        b[tmp + i] = 0;
    }
    
    a[tmp + i][1] = 1;
    b[tmp + i] = 0;
    i++;
    a[tmp + i][N - 2] = 2;
    a[tmp + i][N - 1] = 6 * (x[DATA - 1] - x[DATA - 2]);
    b[tmp + i] = 0;
    
    view_matrix(a);
    scaling(a, b);
    printf("\n");
    view_matrix(a);
    pivot(a, b);
    printf("\n");
    view_matrix(a);
    printf("\n");
    LU(a, b, c);
}

void scaling(double a[N][N], double b[N]){
    int i, j;
    double max;
    
    for (i = 0; i < N; i++) {
        max = 0;
        for (j = 0; j < N; j++) {
            if (fabs(a[i][j]) > max) {
                max = fabs(a[i][j]);
            }
        }
        if (fabs(b[i]) > max) {
            max = fabs(b[i]);
        }
        for (j = 0; j < N; j++) {
            a[i][j] /= max;
        }
        b[i] /= max;
    }
}

void pivot(double a[N][N], double b[N]){
    int i, j, max;
    double tmp;
    
    for (i = 0; i < N; i++) {
        max = i;
        for (j = i; j < N; j++) {
            if (fabs(a[j][i]) > fabs(a[max][i])) {
                max = j;
            }
        }
        for (j = 0; j < N; j++) {
            tmp = a[i][j];
            a[i][j] = a[max][j];
            a[max][j] = tmp;
        }
        tmp = b[i];
        b[i] = b[max];
        b[max] = tmp;
    }
}

void load_file(char *file, double *x, double *y){
    int i;
    FILE *fp;
    
    fp = fopen("data.txt", "r");
    for (i = 0; i < DATA; i++) {
        fscanf(fp, "%lf", &x[i]);
        fscanf(fp, "%lf", &y[i]);
    }
    fclose(fp);
}
