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
   double Sh,Sf,Shf,Sfh;
   double sumH =0,varH =0,avgH =0;
   double sumF =0,varF =0,avgF =0;
   double varHF=0;
   //double S;
   
   
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
    
    /* for ( i = 0; i < data_count; i++ ) {
        printf("%.2f, %.2f, %.2f\n", csv[i].x1, csv[i].x2, csv[i].x3);
	}
    printf("データの数: %d\n", data_count);*/

    
     //csv[i].x3・・・足の幅(Sf)
     //csv[i].x2・・・手の幅(Sh)
    
    
    //手の幅の分散(1,1)
    for(i=0; i<data_count; i++){
            sumH += csv[i].x2;
        }   
        avgH = sumH/data_count;
    //printf("μ = %.2f\n",avgH);

    
    for (i = 0; i < data_count; i++){
            varH += (csv[i].x2 - avgH) * (csv[i].x2 - avgH);
        }
        //Sh = sqrt(varH/data_count);
        Sh = varH/data_count;
    //printf("Sh  = %.2f\n",Sh);

    
    
     //足の幅の分散(2,2)
    for(i=0; i<data_count; i++){
            sumF += csv[i].x3;
        }   
        avgF = sumF/data_count;


    for (i = 0; i < data_count; i++){
            varF += (csv[i].x3 - avgF) * (csv[i].x3 - avgF);
        }
        Sf = varF/data_count;
    //printf("Sf  = %.2f\n",Sf);
    
    

    //手と足の共分散(1,2)(2,1)
    for(i=0; i<data_count; i++){
            varHF += (csv[i].x2-avgH) * (csv[i].x3-avgF);
        }   
        Shf = varHF/data_count;
    //printf("Shf = %.2f\n",Shf);
    
    
    printf("S = (%.2f %.2f\n     %.2f %.2f)\n",Sh,Shf,Shf,Sf);
    
}
