#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trie {
    struct trie *child[26];
    int isEnd;
    char password[50];
};
typedef struct trie trie;

//create node 
trie* newTrieNode(){
    trie *t = (trie*)malloc(sizeof(trie));
    t->isEnd = 0;
    strcpy(t->password, "");
    for(int i = 0; i < 26; i++) t->child[i] = NULL;
    return t;
}

//insert (memory only)
void insertUser(trie *root, char *username, char *password){
    trie *curr = root;
    for(int i = 0; username[i]; i++){
        int idx = username[i] - 'a';
        if(curr->child[idx] == NULL)
            curr->child[idx] = newTrieNode();
        curr = curr->child[idx];
    }
    curr->isEnd = 1;
    strcpy(curr->password, password);
}

//login 
int loginUser(trie *root, char *username, char *password){
    trie *curr = root;
    for(int i = 0; username[i]; i++){
        int idx = username[i] - 'a';
        if(curr->child[idx] == NULL) return 0;
        curr = curr->child[idx];
    }
    return (curr->isEnd && strcmp(curr->password, password) == 0);
}

//userExists 
int userExists(trie *root, char *username){
    trie *curr = root;
    for(int i = 0; username[i]; i++){
        int idx = username[i] - 'a';
        if(curr->child[idx] == NULL) return 0;
        curr = curr->child[idx];
    }
    return curr->isEnd;
}

//delete (memory only) 
int deleteUserHelper(trie *root, char *username, int depth){
    if(!root) return 0;
    if(username[depth] == '\0'){
        if(root->isEnd){ root->isEnd = 0; return 1; }
        return 0;
    }
    int idx = username[depth] - 'a';
    return deleteUserHelper(root->child[idx], username, depth + 1);
}

//SAVE all users to file 
void saveUsersHelper(trie *curr, char *buf, int depth, FILE *fp){
    if(!curr) return;
    if(curr->isEnd){
        buf[depth] = '\0';
        fprintf(fp, "%s %s\n", buf, curr->password);
    }
    for(int i = 0; i < 26; i++){
        if(curr->child[i]){
            buf[depth] = 'a' + i;
            saveUsersHelper(curr->child[i], buf, depth + 1, fp);
        }
    }
}

void saveUsers(trie *root){
    FILE *fp = fopen("users.txt", "w");
    if(!fp){ printf("Error saving users!\n"); return; }
    char buf[50];
    saveUsersHelper(root, buf, 0, fp);
    fclose(fp);
}

//LOAD users from file
void loadUsers(trie *root){
    FILE *fp = fopen("users.txt", "r");
    if(!fp) return;        
    char uname[50], pass[50];
    while(fscanf(fp, "%49s %49s", uname, pass) == 2)
        insertUser(root, uname, pass);
    fclose(fp);
}

//public delete (memory + file)
void deleteUser(trie *root, char *username){
    if(deleteUserHelper(root, username, 0)){
        printf("Admin '%s' deleted successfully!\n", username);
        saveUsers(root);
    } else {
        printf("Admin not found!\n");
    }
}

