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

1. search for files by their name
2. output multiple paths if there are multiple files/folders with the same name
3. if the "file" searched for is a folder put a "/" at the end of the path string
4. search for paths e.g. "search/main.c" -> this is quite useless but good practice lol
5. search for files in a specific directory if there are args. if not search in root -> same as 4.