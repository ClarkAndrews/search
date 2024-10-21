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


int searchFile(char *dirName, char *fileName, char *result, int recursionCount, int isAim);
void addPath(pathNode **root, char *path);
void freePaths(pathNode *root);
pathNode *getPaths(char *fileName);
void searchPath(char *dirName, pathNode *root);

int main(int argc, char *argv[]) {
  char result[1024];
  char *pathToFind;
  pathToFind= strdup("Applications/packettracer/kg");
  if (argc < 2 || argc > 3 ) {
    printf("format: search [<path/to/search/in>] <file/to/search/for>\n");
    return 1;
  }
  pathToFind = strdup(argv[argc-1]);
  pathNode *root = getPaths(pathToFind);
  if (argc == 2 && root){
    searchPath("/", root);
    freePaths(root);
  } else if(argc == 3 && root) {
    searchPath(argv[1], root);
  } else if(argc == 2 && !root) {
    searchFile("/", argv[argc-1], result, 0, 1);
  } else  if(argc == 3 && !root) {
    searchFile(argv[1], argv[2], result, 0, 1);
  }
  return 0;
}

int searchFile(char *dirName, char *fileName, char *result, int recursionCount, int isAim) {
  DIR * dirp; 
  struct dirent *entp;
  struct stat attr;
  char path[1024];
  int found = 0;

  dirp = opendir(dirName);
  if (dirp == NULL && recursionCount == 0) {
    printf("Folder %s couldn't be opened\n",dirName);
    exit(1);
  } else if(dirp == NULL) {
    return 0;
  }
  while((entp = readdir(dirp))) {
    if(strcmp(entp->d_name, ".") && strcmp(entp->d_name, "..")) {
      // generate path string -> make this absolute 
      strcpy(path, dirName);
      // don't add "/" if path is root-dir
      if (strcmp(dirName, "/")) { 
        strcat(path, "/");
      }
      strcat(path, entp->d_name);
      // if file is the one searched for -> copy string to result AND end recursion there 
      if (strcmp(entp->d_name, fileName) == 0) {
        strcpy(result, path);
        if(isAim) {
          printf("%s\n", result);
        }
        closedir(dirp);
        found = 1;  
        return found;
      };
  
      // lstat NEEDS to be checked -> if e.g. permission is denied attr.st_mode won't have a value
      if(lstat(path, &attr) != -1 && S_ISDIR(attr.st_mode)){
        // if file is directory recurse it
        // found only needs to be reassigned if there was no file found yet
        if(!found) {
          found = searchFile(path, fileName, result, recursionCount + 1, isAim);
        }else{
          searchFile(path, fileName, result, recursionCount + 1, isAim);
        }
      }
    }
  }
  if (recursionCount == 0 && !found) {
    printf("'%s' wasn't found in '%s'\n", fileName, dirName);
  }

  closedir(dirp);
  return found;
}

// problem with multiple paths
// e.g. if build/Debug exists several times or specifically the 'build' folder exists several times result returns only ONE path for ONE of the build-folders
// something to be corrected after refactoring the code
void searchPath(char *dirName, pathNode *root) {
  char result[1024];
  char *currPath = strdup(dirName);
  pathNode *curr = root;  
  int isAim = 0;

  while(curr != NULL) {    
    if(curr->next == NULL) {
      isAim = 1; 
    }
  
    searchFile(currPath, curr->dir, result, 0, isAim);
    free(currPath);
    currPath = strdup(result);
    curr = curr->next;
  }
  free(currPath);
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
    free(current->dir);
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
