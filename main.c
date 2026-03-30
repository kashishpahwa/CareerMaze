/*
#include<stdio.h>
#include<string.h>
#include "quiz.h"
#include "mtree.h"

int main(){
    int choice;
    printf("Career Maze\n");
    printf("If u are a 10th student attempt a quiz to check which field will suit u best");
    printf("\n");
    printf("1. 10th Student (Quiz)\n");
    printf("2. Explore Careers\n");
    scanf("%d",&choice);
    mnode *root;
    root=loadTree();
    if(choice==1){
        Question q[50];
        int n;
        loadQuiz(q,&n);
        runQuiz(q,n);
       
       
        //printTree(bt, 0);
        printf("\nNow explore career paths from 10th:\n");
        mnode *start;
        start=search(root,"10th");
        navigate(start,root);
        /*
        int result = runQuiz(q,n);

        if(result == 0)
            navigate(search(root,"PCM"), root);
        else if(result == 1)
            navigate(search(root,"PCB"), root);
        else if(result == 2)
            navigate(search(root,"Commerce"), root);
        else
            navigate(search(root,"Arts"), root);
            }
        
        //else{
        int ch;
        printf("\nSelect Starting Point:\n");
        printf("1. 12th\n2. BTech\n3. MBBS\n");
        scanf("%d",&ch);
        if(ch == 1)
            navigate(search(root,"12th"),root);
        else if(ch == 2)
            navigate(search(root,"BTech"),root);
        else if(ch == 3)
            navigate(search(root,"MBBS"),root);
    }

    return 0;
}
*/
#include<stdio.h>
#include<string.h>
#include "quiz.h"
#include "mtree.h"

int main(){
    int choice;
    printf("Career Maze\n");
    printf("If u are a 10th student attempt a quiz to check which field will suit u best");
    printf("\n");
    printf("1. 10th Student (Quiz)\n");
    printf("2. Explore Careers\n");
    scanf("%d",&choice);
    mnode *root;
    root=loadTree();
    if(choice==1){
        Question q[50];
        int n;
        loadQuiz(q,&n);
        runQuiz(q,n);

        printf("\nNow explore career paths from 10th:\n");
        mnode *start;
        start=search(root,"10th");
        navigate(start,root);
    }
    else{
        int ch;
        printf("\nSelect Starting Point:\n");
        printf("1. 12th\n2. BTech\n3. MBBS\n");
        scanf("%d",&ch);
        if(ch == 1)
            navigate(search(root,"12th"),root);
        else if(ch == 2)
            navigate(search(root,"BTech"),root);
        else if(ch == 3)
            navigate(search(root,"MBBS"),root);
    }

    return 0;
}