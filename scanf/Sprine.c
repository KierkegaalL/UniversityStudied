#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  N 10

double A[N*3-3][N*3-3];
double B[N*3-3];
double x[N], y[N];
double c[N*3-3];

void Sprine();
void lu();
void scaling();
void pivot();


int main(void){
    FILE *fp, *gp;
    int n, m, i, j;
    
    
    fp = fopen("data.txt","r");
    for(m = 0; m <N; m++)
    {
        fscanf(fp, "%lf", &x[m]);
        fscanf(fp, "%lf", &y[m]);
    }
    
    m = N;
    
    gp = popen("gnuplot -persist","w");
    
    for(i=0; i<N; i++)
    {
        fprintf(gp, "set label %d point pt 6 ps 2 at %f, %f\n",i+1,x[i],y[i]);
    }
    
    fprintf(gp, "unset key\n");
    fprintf(gp, "set xlabel 'x軸'\n");
    fprintf(gp, "set ylabel 'y軸'\n");
    fprintf(gp, "set xrange[-16:22]\n");
    fprintf(gp, "set yrange[-2.5:3]\n");
    fprintf(gp, "set xtics 2\n");
    fprintf(gp, "set ytics 0.5\n");
    
    for(i=0; i<N-1; i++)
    {
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
    
    
    /*for(n=0; n<N; n++)
     {
     printf("%f\n",x[n]);
     }
     
     for(n=0; n<N; n++)
     {
     printf("%f\n",y[n]);
     }*/
    
    
    Sprine();
    scaling();
    pivot();
    lu();
    
    for(i=0; i<=(N*3-3); i++)
    {
        printf("%f\n",c[i]);
    }
    
    return 0;
}




void Sprine(){
    int i, j, tmp;
    int d;
    
    
    for(i=0; i<N-1; i++)
    {
        A[i][i*3] = x[i+1]-x[i];
        A[i][(i*3)+1] = pow(x[i+1]-x[i],2);
        A[i][(i*3)+2] = pow(x[i+1]-x[i],3);
        B[i] = y[i+1]-y[i];
    }
    
    for(tmp=i; i<N*2-3; i++)
    {
        d = i-tmp;
        A[i][d*3] = 1;
        A[i][(d*3)+1] = (x[i+1]-x[i])*2;
        A[i][(d*3)+2] = (x[i+1]-x[i])*3;
        A[i][(d+1)*3] = -1;
        B[i] = 0;
    }
    
    for(tmp=i; i<N*3-5; i++)
    {
        d = i-tmp;
        A[i][(d*3)+1] = 1;
        A[i][(d*3)+2] = (x[i+1]-x[i])*3;
        A[i][((d+1)*3)+1] = -1;
        B[i] = 0;
    }
    
    A[i][1] = 1;
    B[i] = 0;
    i++;
    
    
    A[i][(N-2)*3+1] = 2;
    A[i][(N-2)*3+2] = (x[N-1]-x[N-2])*6;
    B[i] = 0;
    
    
}




void lu(){
    int i,j,k;
    float sum;
    double L[N*3-3][N*3-3] = {0};
    double U[N*3-3][N*3-3] = {0};
    double x[N*3-3] = {0};
    double y[N*3-3] = {0};
    double b[N*3-3] = {0};
    
    /*上三角行列、下三角行列*/
    
    for(i=0; i<=(N*3-3); i++)
    {
        
        for(j=0; j<(N*3-3); j++)
        {
            sum = 0;
            
            if(i <= j)
            {
                if(i == j)
                {
                    L[i][i] = 1;
                }
                for(k = 0; k<j; k++)
                {
                    sum += L[i][k] * U[k][j];
                }
                
                U[i][j] = A[i][j] - sum;
                
            }
            else
            {
                for(k = 0; k<j; k++)
                {
                    sum += L[i][k] * U[k][j];
                }
                
                L[i][j] = (A[i][j] - sum) /U[j][j];
            }
        }
    }
    
    /* 前進代入 */
    
    y[0] = b[0];
    
    for(i=1; i<(N*3-3); i++)
    {
        sum = 0;
        
        for(j=0; j<i; j++)
        {
            sum += L[i][j] * y[j];
        }
        y[i] = b[i] -sum;
    }
    
    
    /* 後退代入 */
    
    c[(N*3-3)-1] = y[(N*3-3)-1] / U[(N*3-3)-1][(N*3-3)-1];
    
    for(i=(N*3-3)-2; i>=0; i--)
    {
        sum = 0;
        
        for(j=(N*3-3)-1; j>i; j--)
        {
            sum += U[i][j] * c[j];
        }
        
        c[i] = (y[i] - sum) / U[i][i];
    }
}



void scaling(){
    
    int i,j;
    double max;
    
    for(i=0; i<=(N*3-3); i++)
    {
        max = 0;
        
        for(j=0; j<=(N*3-3); j++)
        {
            if(max<=fabs(A[i][j]))
            {
                max = fabs(A[i][j]);
            }
            
        }
        if(max<=fabs(B[i]))
        {
            max = fabs(B[i]);
        }
        for(j=0; j<=(N*3-3); j++)
        {
            A[i][j] = (A[i][j]) / max;
        }
        B[i] = (B[i]) / max;
    }
    
    
}



void pivot(){
    
    int i,j;
    int  max;
    double tmp;
    
    for(i=0; i<=(N*3-3); i++)
    {
        max =0;
        
        for(j=i; j<(N*3-3); j++)
        {
            if(fabs(A[max][i])<fabs(A[j][i]))
            {
                max = j;
            }
            
        }
        
        for(j=0; j<(N*3-3); j++)
        {
            tmp = A[i][j];
            A[i][j] = A[max][j];
            A[max][j] = tmp;
        }
        tmp = B[i];
        B[i] = B[max];
        B[max] = tmp;
    }
    
}
