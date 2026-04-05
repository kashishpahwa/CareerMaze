#include <stdio.h>
#include "trie.h"
#include <string.h>
#include "quiz.h"
#include "mtree.h"

int main(){
    trie *users = newTrieNode();
    insertUser(users, "admin", "1234");//default admins
    insertUser(users, "user",  "1111");
    loadUsers(users);
    int loggedIn=0;
    int choice;

    //login
    printf("   Career Maze\n");
    printf("1. ADMIN Login\n2.Explore the career maze\n> ");//default name- admin password-1234
    scanf("%d", &choice);

    if(choice==1){
        char uname[50],pass[50];
        printf("Enter username: ");
        scanf("%49s", uname);
        printf("Enter password: ");
        scanf("%49s", pass);
        if(loginUser(users, uname, pass)){
            printf("Login successful!\n");
            loggedIn = 1;
        }else{
            printf("Invalid credentials. Continuing as guest.\n");
        }
    }
    while(choice!=-1){
        //main code
        printf("\nIf you are a 10th student, take a quiz to find your best field!\n");
        printf("1. 10th Student (Quiz)\n");
        printf("2. Explore Careers\n> ");
        if(loggedIn==1){
            printf("  [-7] Add Admin  [-8] Delete Admin     [-11]EXIT\n");
        }
        scanf("%d", &choice);

        mnode *root = loadTree();
        if(choice==1){
            Question q[MAX_Q];
            int n;
            loadQuiz(q, &n);
            if(n==0){
                printf("No quiz questions found!\n");
            } else {
                runQuiz(q, n);
            }

            printf("\nNow explore career paths from 10th:\n");
            mnode *start = search(root, "10th");
            if(!start){ 
                printf("10th node not found in tree!\n"); 
                return 1; 
            }
            navigate(start, root, loggedIn, users);
        }
        else if(choice==2){
            int ch;
            printf("\nSelect Starting Point:\n");
            printf("1. 12th\n2. BTech\n3. MBBS\n> ");
            scanf("%d", &ch);
            if(ch==1){
                navigate(search(root, "12th"), root, loggedIn, users);
            }else if(ch==2){
                navigate(search(root, "BTech"), root, loggedIn, users);
            }else if(ch==3){
                navigate(search(root, "MBBS"), root, loggedIn, users);
            }else
                printf("Invalid choice!\n");
        }
        else if(choice==-7){
            if(!loggedIn){ 
                printf("Login required!\n");  
            }

            char uname[50], pass[50];
            printf("Enter new admin username: ");
            getchar();
            fgets(uname, 50, stdin); uname[strcspn(uname, "\n")]=0;

            if(userExists(users, uname)){
                printf("Admin already exists!\n");
            }
            printf("Enter password: ");
            fgets(pass, 50, stdin); pass[strcspn(pass, "\n")] = 0;

            insertUser(users, uname, pass);
            saveUsers(users);          
            printf("New admin '%s' added and saved!\n", uname);
        }

            //delete admin
        else if(choice==-8){
            if(!loggedIn){ 
                printf("Login required!\n");  
            }

            char uname[50];
            printf("Enter admin username to delete: ");
            getchar();
            fgets(uname, 50, stdin); uname[strcspn(uname, "\n")] = 0;
            deleteUser(users, uname); 
        }
        else if(choice==-11){
            printf("EXIT");
            break;
        }
        else{
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
