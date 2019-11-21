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
   double cor1=0,cor2=0,cor3=0;
   double corS,corH;
   double sumS=0;
   double sumH=0;
   double avgS =0;
   double avgH =0; 
   double r;
   
   //ファイルの読み込み
    FILE *fp;
    char file_name[] = "all.dat";
    Csv csv[DATA_MAX];

    if ((fp = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return 0;
    }

    i = 0;
    while (fscanf(fp,"%lf,%lf,%lf", &csv[i].x1, &csv[i].x2, &csv[i].x3) != EOF){
        i++;
    }
    data_count =i;

    fclose(fp);
    
    
    //石の数の平均
    for(i=0; i<data_count; i++){
        sumS += csv[i].x1;
    }   
    avgS = sumS/data_count;
    
    
    //手の幅の平均
     for(i=0; i<data_count; i++){
        sumH += csv[i].x2;
    }
    avgH = sumH/data_count;
    
    
    //rの計算
    for(i=0; i<data_count; i++){
       cor1 += (csv[i].x1-avgS)*(csv[i].x2-avgH);
       cor2 += pow((csv[i].x1-avgS),2);
       cor3 += pow((csv[i].x2-avgH),2);
       
    }
    corS = sqrt(cor2);
    corH = sqrt(cor3);
    
    r = cor1 / (corS * corH);
    printf("r=%lf\n",r);
}
