#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

typedef struct pathNode {
  char *dir;
  struct pathNode *next;
} pathNode;


int checkFile(char *dirName, char *fileName);
void searchFile(char *dirName, char *fileName, char *result);
//path related
void findPath(pathNode *root);
void addPath(pathNode **root, char *path);
void freePaths(pathNode *root);
pathNode *getPaths(char *fileName);
void printList(struct pathNode *head);
void searchPath(pathNode *root);

int main(int argc, char *argv[]) {
  //the result variable is passed to safe the result in it
  char result[1024];
  char *pathToFind;
  pathToFind= strdup("Applications/packettracer");
  // make it possible to search in "/"
  searchFile("/home", "LICENSE", result);
//  printf("das Ergebnis: %s\n", result);
  return 0;
}

void searchFile(char *dirName, char *fileName, char *result) {
  DIR * dirp; 
  struct dirent *entp;
  struct stat attr;
  char path[1024];
  
  dirp = opendir(dirName);
  while((entp = readdir(dirp))) {
    if(strcmp(entp->d_name, ".") && strcmp(entp->d_name, "..")) {
      // generate path string -> make this absolute 
      strcpy(path, dirName);
      strcat(path, "/");
      strcat(path, entp->d_name);
      // if file is the one searched for -> copy string to result AND end recursion there 
      if (strcmp(entp->d_name, fileName) == 0) {
        strcpy(result, path);
        printf("%s\n", result);
        closedir(dirp);    
        return;
      };
  
      // stat NEEDS to be checked -> if e.g. permission is denied attr.st_mode won't have a value
      if(stat(path, &attr) != -1 && S_ISDIR(attr.st_mode)){
        // if file is directory recurse it    
        searchFile(path, fileName, result);
      }
    }
  }
  closedir(dirp);
}

// not working properly
void searchPath(pathNode *root) {
  char *currPath = strdup("/home"); // is passed later on
  char *foundPath = NULL;
  pathNode *curr = root; 

  while(curr != NULL) {
    free(foundPath);
    ///foundPath = searchFile(currPath, curr->dir);
    // kein string returned? => was passiert wenn kein passender Path gefunden wurde? 
    if(foundPath == NULL) {
      printf("STOOOp");
      break;
    } else{
      free(currPath);
      currPath = strdup(foundPath);
    }
    free(currPath);  
    currPath = strdup(foundPath);
    curr = curr->next;
  }
  free(currPath);
  free(foundPath);
}

// If fileName is path, dir's are put in LL 
void addPath(pathNode **root, char *path){
  pathNode* newPath = (pathNode*)malloc(sizeof(pathNode));
  newPath->dir = strdup(path);
  newPath->next = NULL;
  // if LL is empty add Path at the beginning
  if (*root == NULL) {
    *root = newPath;
  }
  // if path not empty get to last element and set
  else{
    pathNode* lastPath = *root;
    while (lastPath->next != NULL) {
      lastPath = lastPath->next;
    }
    lastPath->next = newPath;
  }
}

// free LL Medmory -> ready
void freePaths(pathNode *root) {
  pathNode *current = root, *nextNode;  
  while(current != NULL) {
    nextNode = current->next;
    free(current);
    current = nextNode;
  }
}

// get LL of Path Dir's -> ready  
pathNode *getPaths(char *fileName) {
  char *token; 
  pathNode *root = NULL;  
  // returns NULL if no '/' in it or if path is root('/') 
  if( strchr(fileName, '/') == NULL || strlen(fileName) == 1) {
    //printf("Searched file isnt a path\n"); 
    return NULL;
  }
  token = strtok(fileName,"/");
  while(token != NULL) {
    addPath(&root, token);
    token = strtok(NULL,"/");
  }

  return root;
}

// print the LL
void printList(struct pathNode *head)
{
    struct pathNode *temp = head;
    while(temp != NULL){
      printf("%s->\n", temp->dir);
      temp = temp->next;
    }
}



