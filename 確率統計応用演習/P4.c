#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define DATA_MAX 1024

typedef struct csv_template {
    double x1;
    double x2;
    double x3;
} Csv;



int main()
{
   int i, data_count;
   int N =93;
   double ab1=0,ab2=0,ab3=0,ab4=0,absh;
   double sum=0;
   double a,b,E;
   
 
   //ファイルの読み込み
    FILE *fp;
    char file_name[] = "all.dat";
    Csv csv[DATA_MAX];

    if ((fp = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return 0;
    }

    i = 0;
    while (fscanf(fp,"%lf\t%lf\t%lf", &csv[i].x1, &csv[i].x2, &csv[i].x3) != EOF){
        i++;
    }
    data_count =i;

    fclose(fp);
    
    //csv[i].x1・・・石の数(yi)
    //csv[i].x2・・・手の幅(xi)
    for(i=0; i<data_count; i++){
        ab1 += (csv[i].x2)*(csv[i].x1);
        ab2 += csv[i].x2;
        ab3 += csv[i].x1;  
        ab4 += pow(csv[i].x2,2);
    }
    absh =pow(ab2,2);
    
    //aの計算
    a = (N * ab1 - ab2 * ab3) / (N * ab4 - absh);
    printf("a=%lf\n",a);
    
    //bの計算
    b = (ab4 * ab3 - ab1 * ab2) / (N * ab4 - absh);
    printf("b=%lf\n",b);
    
   
}
