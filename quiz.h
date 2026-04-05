#include <stdio.h>
#include <string.h>

#define MAX_Q 50

struct que {
    char q[300];
    char opt[6][200];
};
typedef struct que Question;


void loadQuiz(Question q[], int *n){
    FILE *fp = fopen("quiz_10th.txt", "r");
    if(!fp){
        printf("Quiz file not found!\n");
        *n = 0;
        return;
    }
    *n = 0;
    char line[300];

    while(*n < MAX_Q){
        /* ---- read question line ---- */
        int got = 0;
        while(fgets(line, sizeof(line), fp)){
            line[strcspn(line, "\n")] = 0;
            if(strlen(line) > 2){ /* skip blank / separator lines */
                strncpy(q[*n].q, line, 299);
                q[*n].q[299] = '\0';
                got = 1;
                break;
            }
        }
        if(!got) break;   /* no more questions */

        /* ---- read 6 option lines ---- */
        int opts = 0;
        while(opts < 6 && fgets(line, sizeof(line), fp)){
            line[strcspn(line, "\n")] = 0;
            if(strlen(line) > 1){
                strncpy(q[*n].opt[opts], line, 199);
                q[*n].opt[opts][199] = '\0';
                opts++;
            }
        }
        if(opts < 6) break;  
        (*n)++;
    }
    fclose(fp);
}


int runQuiz(Question q[], int n){
    int score[4] = {0};   /* 0=PCM  1=PCB  2=COM  3=ARTS */
    int ch;

    for(int i = 0; i < n; i++){
        printf("\nQ%d. %s\n", i+1, q[i].q);
        for(int j = 0; j < 6; j++)
            printf("  %d. %s\n", j+1, q[i].opt[j]);

        printf("Your choice (1-6): ");
        scanf("%d", &ch);

        if(ch == 1 || ch == 5)      score[0]++;
        else if(ch == 3)            score[1]++;
        else if(ch == 2 || ch == 6) score[2]++;
        else if(ch==4)     score[3]++;
        else printf("invalid choice question skipped");
    }

    printf("\n--- Field Priority ---\n");
    printf("PCM: %d | PCB: %d | Commerce: %d | Arts: %d\n",
           score[0], score[1], score[2], score[3]);

    int max = 0;
    for(int i = 1; i < 4; i++)
        if(score[i] > score[max]) max = i;

    const char *labels[] = {"PCM (Science Maths)", "PCB (Science Biology)",
                            "Commerce", "Arts"};
    printf("Recommended stream: %s\n", labels[max]);
    return max;
}


void updateQuiz(){
    FILE *fp = fopen("quiz_10th.txt", "a");
    if(!fp){ printf("Cannot open quiz file!\n"); return; }

    /* blank line separates questions in the file */
    fprintf(fp, "\n");

    char buf[300];
    getchar();   /* flush leftover '\n' from scanf */

    printf("Enter question:\n");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    fprintf(fp, "%s\n", buf);

    for(int i = 0; i < 6; i++){
        printf("Option %d: ", i+1);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;
        fprintf(fp, "%s\n", buf);
    }

    fclose(fp);
    printf("Question added successfully!\n");
}

