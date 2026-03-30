#ifndef MTREE_H
#define MTREE_H
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

void saveTree(mnode *root);
int compareIgnoreCase(char *a, char *b);
// CREATE NODE

mnode *newNode(char *msg){
    mnode *t = (mnode *)malloc(sizeof(mnode));

    strcpy(t->data, msg);
    t->count = 0;

    for (int i = 0; i < MAX; i++)
        t->child[i] = NULL;
    return t;
}
int printPath(mnode *root, char *target, char path[][50], int depth);
void deleteNode(mnode *root);

// BUILD TREE
void insertPath(mnode *root, char *line){
    char *token = strtok(line, "|");
    mnode *curr = root;

    while(token){
        int found = 0;

        // check if child exists
        for(int i=0;i<curr->count;i++){
            if(compareIgnoreCase(curr->child[i]->data, token)){
                curr = curr->child[i];
                found = 1;
                break;
            }
        }

        // if not found → create
        if(!found){
            mnode *newN = newNode(token);
            curr->child[curr->count++] = newN;
            curr = newN;
        }

        token = strtok(NULL, "|");
    }
}

mnode* loadTree(){
    FILE *fp = fopen("tree.txt","r");

    if(!fp){
    printf("No saved tree found. Using default.\n");

    mnode *root = newNode("Career");

    insertPath(root, "10th|12th|PCM|BTech");
    insertPath(root, "10th|12th|PCB|MBBS");
    insertPath(root, "10th|12th|Commerce|BCom");
    insertPath(root, "10th|12th|Arts|BA");

    return root;
}

    mnode *root = newNode("Career");
    char line[500];

    while(fgets(line,500,fp)){
        line[strcspn(line,"\n")] = 0;

        // skip root word
        char *start = strstr(line, "|");
        if(start) start++;  // move after "Career|"
        if(start != NULL)
            insertPath(root, start);
        
    }

    fclose(fp);
    return root;
}

// SEARCH
int compareIgnoreCase(char *a, char *b){
    while (*a && *b){
        if(tolower(*a) != tolower(*b)){
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

mnode *search(mnode *root, char *key){
    if(!root)return NULL;

    if(compareIgnoreCase(root->data,key)){
        return root;
    }

    for(int i=0; i<root->count; i++){
        mnode *res=search(root->child[i],key);
        if(res)return res;
    }
    return NULL;
}
mnode *findNode(mnode *root, char *name){
    if(root==NULL)return NULL;

    if(compareIgnoreCase(root->data, name)){
        return root;
    }

    for(int i=0;i<root->count;i++){
        mnode *found=findNode(root->child[i],name);
        if(found!= NULL)return found;
    }
    return NULL;
}
int deleteNodeHelper(mnode *root,char *key){
    if (!root)return 0;

    for(int i=0;i<root->count;i++){
        if (compareIgnoreCase(root->child[i]->data,key)){

            deletedInfo *info=(deletedInfo *)malloc(sizeof(deletedInfo));
            info->parent=root;
            info->node=root->child[i];
            info->index=i;

            push(&undoStack,info); // store for undo

            // shift children
            for(int j=i;j<root->count-1;j++)root->child[j]=root->child[j + 1];
            root->count--;
            return 1;
        }
        if(deleteNodeHelper(root->child[i], key)){
            return 1;
        }
    }
    return 0;
}

void deleteNode(mnode *root){
    char key[100];

    printf("Enter node to delete: ");
    getchar();
    fgets(key,100,stdin);
    key[strcspn(key,"\n")] = 0;

    if (deleteNodeHelper(root,key))
        printf("Node '%s' deleted successfully!\n",key);
    else
        printf("Node not found!\n");
}

// ADD NODE

void addNode(mnode *root, char *parentName, char *childName){
    mnode *parent =findNode(root, parentName);

    if(parent == NULL){
        printf("Parent '%s' not found!\n", parentName);
        return;
    }
    for(int i = 0; i < parent->count; i++){
    if(compareIgnoreCase(parent->child[i]->data, childName)){
        printf("Node already exists!\n");
        return;
    }
}
    // STRICT BOUND CHECK
    if(parent->count < 0 || parent->count >= MAX){
        printf("Invalid child count detected!\n");
        parent->count=0; // reset (safety recovery)
        return;
    }
    parent->child[parent->count]=newNode(childName);
    parent->count++;

    printf("Node '%s' added under '%s'\n",childName,parentName);
}
// DISPLAY CHILDREN

void showChildren(mnode *node){
    for(int i = 0; i < node->count; i++){
        if (node->child[i] != NULL && strlen(node->child[i]->data) > 0){
            printf("%d. %s\n",i+1,node->child[i]->data);
        }
        else{
            // stop immediately if garbage encountered
            break;
        }
    }
}

// NAVIGATION (WITH BACKTRACK)

void navigate(mnode *start, mnode *root){
    stack *st = NULL;
    mnode *curr = start;
    int ch;

    while (1){
        printf("\nCurrent: %s\n", curr->data);
        showChildren(curr);
        printf("0.Back  6.Undo Delete  7.Search  8.Delete(runtime)  9.Add(runtime)   10. delete(permanent)    11. add(permanent)-1.Print path\n");

        scanf("%d", &ch);
        if (ch == -1)
        {
            printf("\n Final Career Path:\n");
            char path[50][50];
            printPath(root, curr->data, path, 0);
            break;
        }

        // BACKTRACK
        else if(ch == 0){
            mnode *prev=(mnode *)pop(&st);
            if (prev)
                curr=prev;
            else
                printf("No previous node!\n");
        }

        else if(ch==6){
            deletedInfo *info = (deletedInfo*)pop(&undoStack);

            if(info==NULL){
                printf("Nothing to undo!\n");
            }
            else{
                mnode *parent = info->parent;

                // shift right to make space
                for(int j=parent->count; j>info->index;j--){
                    parent->child[j]=parent->child[j-1];
            }

            parent->child[info->index]=info->node;
            parent->count++;

            printf("Undo successful! Node restored: %s\n",info->node->data);
            free(info);
            }
        }

        else if (ch==7){
            char key[100];
            printf("Enter career to search: ");
            getchar(); // clear buffer
            fgets(key, 100, stdin);
            key[strcspn(key, "\n")] = 0;
            mnode *found = search(root, key);

            if (found){
                printf("Found: %s\n", found->data);
                printf("Path:\n");
                char path[50][50];
                printPath(root, found->data, path, 0);

                // optional: jump to that node
                push(&st, curr);   // save current position
                curr = found;      // move to searched node
            }
            else{
                printf("Career not found!\n");
            }
        }

        //  ADD NODE
        else if (ch==9){
            char parent[100];
            char child[100];
            printf("Enter parent node: ");
            getchar(); // clear buffer
            fgets(parent, 100, stdin);
            parent[strcspn(parent, "\n")]=0;

            printf("Enter new child: ");
            fgets(child, 100, stdin);
            child[strcspn(child,"\n")]=0;

            addNode(root, parent,child); // always full tree
        }

        // DELETE NODE
        else if (ch==8){
            deleteNode(root);
        }
        else if(ch == 11){
            char parent[100], child[100];

            printf("Enter parent node: ");
            getchar();
            fgets(parent,100,stdin);
            parent[strcspn(parent,"\n")] = 0;

            printf("Enter new child: ");
            fgets(child,100,stdin);
            child[strcspn(child,"\n")] = 0;

            addNode(root, parent, child);   // add in memory
            saveTree(root);                 // SAVE permanently

            printf("Node added permanently!\n");
        }
        else if(ch == 10){
            deleteNode(root);   // delete in memory
            saveTree(root);     // SAVE changes

            printf("Node deleted permanently!\n");
        }
        //  MOVE FORWARD
        else if (ch >= 1 && ch<=curr->count){
            push(&st, curr);
            curr=curr->child[ch - 1];
        }

        else{
            printf("Invalid choice!\n");
        }
    }
}
void savePaths(mnode *root, FILE *fp, char path[][100], int depth){
    if(!root) return;

    strcpy(path[depth], root->data);

    // if leaf node → save path
    if(root->count == 0){
        for(int i=0;i<=depth;i++){
            fprintf(fp, "%s", path[i]);
            if(i != depth) fprintf(fp, "|");
        }
        fprintf(fp, "\n");
        return;
    }

    for(int i=0;i<root->count;i++){
        savePaths(root->child[i], fp, path, depth+1);
    }
}
void saveTree(mnode *root){
    FILE *fp = fopen("tree.txt","w");
    char path[100][100];
    if(fp == NULL){
        printf("Error saving file!\n");
        return;
    }
    savePaths(root, fp, path, 0);

    fclose(fp);
    printf("Tree saved successfully!\n");
}
int printPath(mnode *root,char *target,char path[][50],int depth){
    if (root==NULL){
        return 0;
    }

    strcpy(path[depth],root->data);
    if(compareIgnoreCase(root->data, target)){
        for(int i=0; i<=depth; i++){
            printf("%s",path[i]);
            if (i!=depth){
                printf(" -> ");
            }
        }
        printf("\n");
        return 1;
    }

    for(int i=0;i<root->count;i++){
        if(printPath(root->child[i],target,path,depth+1)){
            return 1;
        }
    }
    return 0;
}
#endif