#include <stdio.h>
#include <math.h>
#define COUNT 24
#define N (3 * (COUNT - 1))

void view_matrix(float a[N][N], int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%0.3f\t", a[i][j]);
        }
        printf("\n");
    }
}
void view_array(float *b, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        printf("%f\n", b[i]);
    }
    printf("\n");
}
void LU_y(float L[N][N], float *b, float *y)
{
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
void LU_x(float U[N][N], float *y, float *x)
{
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
void scaling(float a[N][N], float b[N])
{
    int i, j;
    float max;
    
    for (i = 0; i < N; i++) {
        max = a[i][0];
        for (j = 0; j < N; j++) {
            if (fabs(a[i][j]) > fabs(max)) {
                max = a[i][j];
            }
        }
        if (fabs(b[i]) > fabs(max)) {
            max = b[i];
        }
        for (j = 0; j < N; j++) {
            a[i][j] /= max;
        }
        b[i] /= max;
    }
}
void LU(float a[N][N], float *b, float *x)
{
    int i, j, k, max;
    float sum, L[N][N], U[N][N], y[N], tmp;
    
    scaling(a, b);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            /* 上三角と下三角にわける */
            if (i > j) {
                for (k = 0; k < j; k++) {
                    sum += L[i][k] * U[k][j];
                }
                L[i][j] = 1 / U[j][j] * (a[i][j] - sum);
                U[i][j] = 0;
            } else {
                
                for (k = 0; k < i; k++) {
                    sum += L[i][k] * U[k][j];
                }
                /* 対角要素かどうか */
                if (i == j) {
                    L[i][j] = 1;
                    max = i;
                    for (k = i; k < N; k++) {
                        if (fabs(a[k][j]) > fabs(a[max][j])) {
                            max = k;
                        }
                    }
                    for (k = 0; k < N; k++) {
                        tmp = a[i][k];
                        a[i][k] = a[max][k];
                        a[max][k] = tmp;
                    }
                    tmp = b[i];
                    b[i] = b[max];
                    b[max] = tmp;
                } else {
                    L[i][j] = 0;
                }
                U[i][j] = a[i][j] - sum;
            }
        }
    }
    LU_y(L, b, y);
    LU_x(U, y, x);
}

void spline(float *x, float *y, float *c)
{
    int i, j, k, tmp;
    float a[N][N] = {0}, b[N];

    for (i = 0; i <= COUNT - 1; i++) {
        b[i] = y[i];
    }
    for (i = 0; i < COUNT - 1; i++) {
        for (k = 0; k < 3; k++) {
            a[i][i * 3 + k] = pow(x[i + 1] - x[i], k + 1);
        }
        b[i] = y[i + 1] - y[i];
    }
    tmp = i;
    for (; i < 2 * COUNT - 3; i++) {
        for (k = 0; k < 3; k++) {
            a[i][(i - tmp) * 3 + k] = (k + 1) * pow(x[i - tmp + 1] - x[i - tmp], k);
        }
        a[i][(i - tmp) * 3] = -1;
        b[i] = 0;
    }
    tmp = i;
    for (; i < 3 * COUNT - 5; i++) {
        a[i][(i - tmp) * 3 + 1] = 1;
        a[i][(i - tmp) * 3 + 2] = 3 * (x[i - tmp + 1] - x[i - tmp]);
        a[i][(i - tmp + 1) * 3 + 1] = -1;
        b[i] = 0;
    }

    a[i][2] = 1;
    b[i] = 0;
    a[++i][3 * (COUNT - 2) + 1] = 2;
    a[i][3 * (COUNT - 2) + 2] = 6 * (x[COUNT - 1] - x[COUNT - 2]);
    b[i] = 0;
    LU(a, b, c);
}

int main(void){
    FILE *fp, *gp;
    int i, j, m, n;
    float x[COUNT], y[COUNT], c[N];

    gp = popen("gnuplot -persist","w");
    
    fp = fopen("data.txt","r");
    
    for(m = 0; m <COUNT; m++){
        fscanf(fp, "%f", &x[m]);
        fscanf(fp, "%f", &y[m]);
        fprintf(gp, "set label %d point at %f,%f\n", m + 1, x[m], y[m]);
    }
    
    m = COUNT;
    
    fprintf(gp, "unset key\n");
    fprintf(gp, "set xlabel 'x-axis'\n");
    fprintf(gp, "set ylabel 'y-axis'\n");
    fprintf(gp, "set xrange[-15:25]\n");
    fprintf(gp, "set yrange[-2.5:2]\n");
    fprintf(gp, "set xtics 2\n");
    fprintf(gp, "set ytics 0.5\n");
    
    for(i=0; i<COUNT-1; i++){
        fprintf(gp, i==0 ? "plot " : "replot");
        fprintf(gp, "(x<=%f || %f<=x) ? NaN : ", x[i], x[i+1]);
        fprintf(gp, "%f",y[i]);
        fprintf(gp, "+(%f)*(x-(%f))",c[i*3+0],x[i]);
        fprintf(gp, "+(%f)*(x-(%f))**2",c[i*3+1],x[i]);
        fprintf(gp, "+(%f)*(x-(%f))**3\n",c[i*3+2],x[i]);
    }
    
    fprintf(gp,"set terminal png\n");
    fprintf(gp,"set output 'spline.png'\n");
    fprintf(gp,"replot\n");
    
    spline(x, y, c);
    
}
/*     gp = popen("gnuplot -persist", "w");

    fp = fopen("data.txt", "r");
    for (m = 0; m < COUNT; m++) {
        fscanf(fp, "%f", &x[m]);
        fscanf(fp, "%f", &y[m]);
        fprintf(gp, "set label %d point at %f,%f\n", m + 1, x[m], y[m]);
    }
    m = COUNT;
    printf("データx\n");
    view_array(x, COUNT);
    printf("データy\n");
    view_array(y, COUNT);

    spline(x, y, c);
    printf("解\n");

//    printf("P\ta\t\tb\t\tc\t\td\n");
   for (i = 0; i < COUNT; i++) {
        printf("v%d=%+f\n", i, x[i]);
        printf("w%d=%+f\n", i, y[i]);
        printf("b%d=%+f\n", i, c[i * 3 + 0]);
        printf("c%d=%+f\n", i, c[i * 3 + 1]);
        printf("d%d=%+f\n", i, c[i * 3 + 2]);
        printf("f(X%d)=w%d+(b%d)*(x%d-(v%d))+(c%d)*(x%d-(v%d))**2+(d%d)*(x%d-(v%d))**3\n",i,i,i,i,i,i,i,i,i,i,i);
        printf("replot f(X%d) title w lines\n",i);
        
    }

    fprintf(gp, "plot (%f)", c[0]);
    for (i = 1; i < n; i++) {
        fprintf(gp, "(%f*x**%d)", c[i], i);
        if (i < n - 1) {
            fprintf(gp, "+");
        } else {
            fprintf(gp, "\n");
        }
    }
    pclose(gp);
 }
*/

