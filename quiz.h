#ifndef QUIZ_H
#define QUIZ_H
#include<stdio.h>
#include<string.h>
#define MAX_Q 50
struct que {
    char q[300];
    char opt[6][200];
};
typedef struct que Question;

void loadQuiz(Question q[], int *n){
    FILE *fp=fopen("quiz_10th.txt","r");
    *n=0;
    while(fgets(q[*n].q,300,fp)){
        for(int i=0;i<6;i++)
            fgets(q[*n].opt[i],200,fp);
        (*n)++;
    }
    fclose(fp);
}

int runQuiz(Question q[], int n){
    int score[4]={0}; // PCM PCB COM ARTS
    int ch;
    for(int i=0;i<n;i++){
        printf("\n%s",q[i].q);
        for(int j=0;j<6;j++){
            printf("%d. %s",j+1,q[i].opt[j]);
        }

        scanf("%d",&ch);

        if(ch==1||ch==5){ 
            score[0]++;
            
        }
        else if(ch==3) {
            score[1]++;
            
        }
        else if(ch==2 || ch==6){
            score[2]++;
            
        }
        else 
        {
            score[3]++;
            
        }
    }

    printf("\nField Priority:\nPCM:%d PCB:%d COM:%d ARTS:%d\n",
           score[0],score[1],score[2],score[3]);

    int max=0;
    for(int i=1;i<4;i++)
        if(score[i]>score[max]){ 
            max=i;
            
        }

    return max;
}

#endif