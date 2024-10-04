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
char *searchFile(char *dirName, char *fileName);
//path related
void findPath(pathNode *root);
void addPath(pathNode **root, char *path);
void freePaths(pathNode *root);
pathNode *getPaths(char *fileName);
void printList(struct pathNode *head);
void searchPath(pathNode *root);

int main(int argc, char *argv[]) {
  char *result;
  result = strdup("Applications/packettracer");
  pathNode *path = getPaths(result);      // FUNKTIONIERT
  if (path != NULL) {
      searchPath(path);                       // fixen
  }
  
  
  // make it possible to search in "/"
  searchFile("/home", "LICENSE");
  return 0;
}

// actual main function
int checkFile(char *dirName, char *fileName) {
  pathNode *pathRef = getPaths(fileName);
  
  if (pathRef != NULL) {
    return 1;
  }else {
    searchFile(dirName, fileName);
    return 0;
  }  
}

char *searchFile(char *dirName, char *fileName) {
  DIR * dirp; 
  struct dirent *entp;
  struct stat attr;
  char path[1024], *result;
  dirp = opendir(dirName);
  entp = readdir(dirp);
  while((entp = readdir(dirp))) {
    if(strcmp(entp->d_name, ".") && strcmp(entp->d_name, "..")) {
      // generate path string -> make this absolute 
      strcpy(path, dirName);
      strcat(path, "/");
      strcat(path, entp->d_name);
      // exclude . git folder
      if(strcmp(entp->d_name, ".git")){ //REMOVE** FOR DEBUGGING searchPath
        if (strcmp(entp->d_name, fileName) == 0) {
          result = malloc(strlen(path) + 1);
          strcpy(result,path);
          printf("das Ergebnis: %s\n", result);
          closedir(dirp); 
          return result; 
        };
        // if entry is a directory, recurse it 
        stat(path, &attr);
        if(S_ISDIR(attr.st_mode)){
          // result needs to be returned in recursion, otherwise it wont be stacked 'upwards'
          result = searchFile(path, fileName);
          if (result != NULL)
            return result;
        }; //REMOVE** FOR DEBUGGING searchPath
      };
    };
  };
  closedir(dirp);
  return NULL;
}

// not working properly
void searchPath(pathNode *root) {
  char *currPath = strdup("/home"); // is passed later on
  char *foundPath = NULL;
  pathNode *curr = root; 

  while(curr != NULL) {
    free(foundPath);
    foundPath = searchFile(currPath, curr->dir);
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



