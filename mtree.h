#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include <ctype.h>

#define MAX 20

struct mnode{
    char data[100];
    int count;
    struct mnode *child[MAX];
};
typedef struct mnode mnode;

struct deletedInfo{
    mnode *parent;
    mnode *node;
    int index;
};
typedef struct deletedInfo deletedInfo;

stack *undoStack = NULL;

//forward declarations
void saveTree(mnode *root);
int  compareIgnoreCase(char *a,char *b);
int  printPath(mnode *root,char *target,char path[][50],int depth);
void deleteNode(mnode *root);

//Node
mnode *newNode(char *msg){
    mnode *t=(mnode*)malloc(sizeof(mnode));
    strcpy(t->data,msg);
    t->count=0;
    for(int i=0;i<MAX;i++){
        t->child[i] = NULL;
    }
        return t;
}

//Build tree
void insertPath(mnode *root,char *line){
    char *token=strtok(line,"|");
    mnode *curr=root;

    while(token){
        int found=0;
        for(int i=0;i<curr->count;i++){
            if(compareIgnoreCase(curr->child[i]->data,token)){
                curr=curr->child[i];
                found=1;
                break;
            }
        }
        if(!found){
            mnode *newN=newNode(token);
            curr->child[curr->count++]=newN;
            curr=newN;
        }
        token=strtok(NULL,"|");
    }
}

mnode* loadTree(){
    FILE *fp=fopen("tree.txt","r");
    mnode *root=newNode("Career");

    if(!fp){
        printf("No saved tree found. Using default.\n");
        insertPath(root,"10th|12th|PCM|BTech");
        insertPath(root,"10th|12th|PCB|MBBS");
        insertPath(root,"10th|12th|Commerce|BCom");
        insertPath(root,"10th|12th|Arts|BA");
        return root;
    }

    char line[500];
    while(fgets(line,500,fp)){
        line[strcspn(line,"\n")]=0;
        char *start=strstr(line,"|");
        if(start){
            insertPath(root,start+1);
        }
    }
    fclose(fp);
    return root;
}

//Utils
int compareIgnoreCase(char *a,char *b){
    while(*a && *b){
        if(tolower((unsigned char)*a)!=tolower((unsigned char)*b)){
            return 0;
        }
        a++; 
        b++;
    }
    return (*a=='\0' && *b=='\0');
}

mnode *search(mnode *root,char *key){
    if(!root){
        return NULL;
    }
    if(compareIgnoreCase(root->data,key)){
         return root;
    }
    for(int i=0;i<root->count;i++){
        mnode *res=search(root->child[i],key);
        if(res){
            return res;
        }
    }
    return NULL;
}

mnode *findNode(mnode *root,char *name){
    if(!root){
        return NULL;
    }
    if(compareIgnoreCase(root->data,name)){
        return root;
    }
    for(int i=0;i<root->count;i++){
        mnode *found=findNode(root->child[i],name);
        if(found){
            return found;
        }
    }
    return NULL;
}

//Delete
int deleteNodeHelper(mnode *root,char *key){
    if(!root){
        return 0;
    }
    for(int i=0;i<root->count;i++){
        if(compareIgnoreCase(root->child[i]->data,key)){
            deletedInfo *info=(deletedInfo*)malloc(sizeof(deletedInfo));
            info->parent=root;
            info->node=root->child[i];
            info->index=i;
            push(&undoStack,info);

            for(int j=i;j<root->count-1;j++)
                root->child[j]=root->child[j+1];
            root->count--;
            return 1;
        }
        if(deleteNodeHelper(root->child[i],key)){
            return 1;
        }
    }
    return 0;
}

void deleteNode(mnode *root){
    char key[100];
    printf("Enter node to delete: ");
    getchar();
    fgets(key, 100, stdin);
    key[strcspn(key,"\n")]=0;

    if(deleteNodeHelper(root,key))
        printf("Node '%s' deleted successfully!\n",key);
    else
        printf("Node not found!\n");
}

//Add
void addNode(mnode *root,char *parentName,char *childName){
    mnode *parent=findNode(root,parentName);
    if(!parent){ 
        printf("Parent '%s' not found!\n", parentName); 
        return; 
    }

    for(int i=0;i<parent->count;i++){
        if(compareIgnoreCase(parent->child[i]->data,childName)){
            printf("Node already exists!\n");
            return;
        }
    }
    if(parent->count>=MAX){
        printf("Max children reached!\n");
        return;
    }

    parent->child[parent->count]=newNode(childName);
    parent->count++;
    printf("Node '%s' added under '%s'\n", childName, parentName);
}

//Display
void showChildren(mnode *node){
    for(int i=0;i<node->count;i++){
        if(node->child[i] && strlen(node->child[i]->data)>0){
            printf("  %d. %s\n",i+1,node->child[i]->data);
        }
        else{
            break;
        }
    }
}

//SAVE / PRINT
void savePaths(mnode *root, FILE *fp, char path[][100], int depth){
    if(!root) return;
    strcpy(path[depth], root->data);
    if(root->count==0){
        for(int i=0;i<=depth;i++){
            fprintf(fp, "%s",path[i]);
            if(i!=depth){
                fprintf(fp, "|");
            }
        }
        fprintf(fp,"\n");
        return;
    }
    for(int i=0;i<root->count;i++){
        savePaths(root->child[i],fp,path,depth+1);
    }
}

void saveTree(mnode *root){
    FILE *fp=fopen("tree.txt","w");
    if(!fp){
        printf("Error saving file!\n");
        return;
    }
    char path[100][100];
    savePaths(root,fp,path,0);
    fclose(fp);
    printf("Tree saved successfully!\n");
}

int printPath(mnode *root,char *target,char path[][50],int depth){
    if(!root){
        return 0;
    }
    strcpy(path[depth],root->data);
    if(compareIgnoreCase(root->data,target)){
        for(int i=0;i<=depth;i++){
            printf("%s", path[i]);
            if(i != depth){
                printf(" -> ");
            }
        }
        printf("\n");
        return 1;
    }
    for(int i = 0; i < root->count; i++){
        if(printPath(root->child[i], target, path, depth + 1)){
            return 1;
        }
    }
    return 0;
}

//Navigate
void navigate(mnode *start,mnode *root,int loggedIn,trie *users){
    stack *st=NULL;
    mnode *curr=start;
    int ch;

    while(1){
        printf("\n============================\n");
        printf("Current: %s\n",curr->data);
        showChildren(curr);

        printf("\n[0] Back  [-1] Undo delete  [-2] Search  [-3] Delete(session)  [-4] Add(session)");
        if(loggedIn){
            printf("  [-5] Delete(permanent)  [-6] Add(permanent)");
            //printf("  [-7] Add Admin  [-8] Delete Admin");
        }
        if(loggedIn && compareIgnoreCase(curr->data, "10th"))
            printf("  [-9] Add Quiz question ");
        printf("  [-10] Print Path and exit  \n> ");

        scanf(" %d", &ch);
        //Print Path/Exit
        if(ch==-10){
            printf("\nFinal Career Path:\n");
            char path[50][50];
            printPath(root, curr->data, path, 0);
            break;
        }

        //Back
        else if(ch==0){
            mnode *prev=(mnode *)pop(&st);
            if(prev){
                curr=prev;
            }
            else{
                printf("Already at top!\n");
            }
        }

        //Undo
        else if(ch==-1){
            deletedInfo *info=(deletedInfo *)pop(&undoStack);
            if(!info){
                printf("Nothing to undo!\n");
            }
            else{
                mnode *parent=info->parent;
                for(int j=parent->count;j>info->index;j--){
                    parent->child[j]=parent->child[j-1];
                }
                parent->child[info->index]=info->node;
                parent->count++;
                printf("Undo successful! Restored: %s\n",info->node->data);
                free(info);
            }
        }

        //Search
        else if(ch==-2){
            char key[100];
            printf("Enter career to search: ");
            getchar();
            fgets(key,100,stdin);
            key[strcspn(key,"\n")]=0;

            mnode *found=search(root,key);
            if(found){
                printf("Found: %s\nPath:\n",found->data);
                char path[50][50];
                printPath(root,found->data,path,0);
                push(&st,curr);
                curr=found;
            } 
            else{
                printf("Career not found!\n");
            }
        }

        //Add(session)
        else if(ch==-4){
            char parent[100],child[100];
            printf("Enter parent node: ");
            getchar();
            fgets(parent,100,stdin); 
            parent[strcspn(parent,"\n")]=0;
            printf("Enter new child: ");
            fgets(child,100,stdin);
            child[strcspn(child,"\n")]=0;
            addNode(root,parent,child);
        }

        //Delete(session)
        else if(ch==-3){
            deleteNode(root);
        }

        //Add(permanent)
        else if(ch==-6){
            if(!loggedIn){
                printf("Login required!\n");
                continue;
            }
            char parent[100],child[100];
            printf("Enter parent node: ");
            getchar();
            fgets(parent,100,stdin);
            parent[strcspn(parent,"\n")]=0;
            printf("Enter new child: ");
            fgets(child,100,stdin);
            child[strcspn(child,"\n")]=0;
            addNode(root, parent, child);
            saveTree(root);
        }

        //Delete(permanent)
        else if(ch==-5){
            if(!loggedIn){
                printf("Login required!\n");
                continue; }
            deleteNode(root);
            saveTree(root);
            printf("Node deleted permanently!\n");
        }

        //Update Quiz
        else if(ch==-9){
            if(!loggedIn){
                printf("Login required!\n");
                continue;
            }
            updateQuiz();
        }

        //Navigate to child
        else{
            int idx=ch-1 ;
            if(idx<curr->count){
                push(&st,curr);
                curr=curr->child[idx];
            } 
            else{
                printf("Invalid selection!\n");
            }
        }

    }
}


mtree.h
