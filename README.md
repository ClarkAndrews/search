# search
A C-script to search for files by name in the CLI

The format should be

``` 
>
> search [<folder/to/search/in>] <file/to/search/for> 
>
```

To elaborate further on this... it is my first small C-Project. 
I wanted to write something quite simple, but still challenging for me.

## requirements

1. search for files by their name                                                                           ️✅
2. output multiple paths if there are multiple files/folders with the same name ️                            ️✅
3. if the "file" searched for is a folder put a "/" at the end of the path string                           ️
4. search for paths e.g. "search/main.c" -> this is quite useless but good practice lol                     ✅
5. search for files in a specific directory if there are 2 args. if not search in root -> same as 4.
  -> first needs to be possible to search in root                                                           ✅

## problems occured 

- searchPath only prints 1 path and also doesn't allow searching for multiple path with the same folder-names 
- ALSO searchPath doesn't find the second folder if the first folder exists multiple times in some cases
---> maybe I a

## problems resolved
- when returning the path from recursion, multiple files with the same name are NOT found 
  -> to fix this I will add a 3rd parameter of type char * -> this one is then used to store the result and the function can go on as it should
- the 'attr' variable of type 'struct stat' caused segmentation err, can happen if permission on a file/folder is denied 
  -> I should start adding checks more frequently :|



