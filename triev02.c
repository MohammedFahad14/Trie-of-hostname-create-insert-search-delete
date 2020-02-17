#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHAR_SIZE 40
struct trie
{
 struct trie *alpha[CHAR_SIZE];
 int wordflag;
};

typedef struct trie trie;

trie * root;

trie * createnode()
{
  trie * node=(trie*) malloc(sizeof(trie));
  int i=0;
  for(;i<CHAR_SIZE;i++)
  node->alpha[i]=NULL;
  node->wordflag=0;
  return node;
}

int scale(char x)
{
	if (x >= 'a' && x <= 'z')
		return (x - 87);
  else if (x >= 'A' && x <= 'Z')
		return (x - 55);
	else if (x >= '0' && x <= '9')
		return (x - '0');
	else if (x == '.')
		return 36;
	else if (x == '-')
		return 37;
  else if (x == '_')
		return 38;
  else if (x == '+')
		return 39;
	else
	{
		//printf("unexpected character found x = %d\n", x);
		return -1;
	}

}

int validword(char *word)
{
  int ind=0;
  while(*word)
  {
    ind = scale(*word);
    if(ind == -1)
      return -1;
    else
      word++;

  }
  return 0;
}

int search_insert_delete(trie *node, char *word, char mode)
{
  if(!node)
 {
   if(mode=='i')
  {
   printf("\nCreating root node...");
   node=createnode();
   root=node;
  }
  else if(mode=='s' || mode=='d')
  {
    printf("\nRoot node does not exist.");
    return 1;
  }
 }
   printf("word in search %s\n",word);

 while(*word)
 {
   int index = scale(*word);

   if(!node->alpha[index])
   {
     if(mode=='i')
     {
       node->alpha[index]=createnode();
     }
     else if(mode=='s' || mode=='d')
    {
      printf("\nNo node found for character '%c'.",*word);
      return 1;
    }
   }
   node=node->alpha[index];
   word++;
 }
 if(!node->wordflag)
 {
   if(mode=='i')
  {
    node->wordflag=1;
    return 0;
  }
  else if(mode=='s' || mode=='d')
  {
    printf("\nWordflag not set."); return 1; 
  }
  }
  else if(mode=='d')
  {
    printf("\nResetting wordflag...");
    node->wordflag=0;
    return 0;
  }
  else return 0;
}

char *trim_string(char *str)
{
	size_t len = 0;
	char *frontp = str;
	char *endp = NULL;

	if (str == NULL)
	{
		return NULL;
	}
	if (str[0] == '\0')
	{
		return str;
	}

	len = strlen(str);
	endp = str + len;

	while (isspace((unsigned char)*frontp))
	{
		++frontp;
	}
	if (endp != frontp)
	{
		while (isspace((unsigned char)*(--endp)) && endp != frontp)
		{
		}
	}

	if (str + len - 1 != endp)
		*(endp + 1) = '\0';
	else if (frontp != str && endp == frontp)
		*str = '\0';

	endp = str;
	if (frontp != str)
	{
		while (*frontp)
		{
			*endp++ = *frontp++;
		}
		*endp = '\0';
	}

	return str;
}


void insert()
{
		int read1,read2,result,vw;
		size_t len1 = 1;
		size_t len2 = 1;
    char *word,*grpn;
    char buffer[256];
	  int st,si;
    FILE *gp,*fp;

    gp = fopen("group_file.txt", "r");

    char* group_name = malloc(len1*sizeof(group_name));
  
  while ((read1= getline(&group_name, &len1, gp)) != -1)
	{
				printf("group name = %s\n",group_name);

                grpn = strndup(group_name+2, 3);

            		if (grpn[0] >= 'A' && grpn[0] <= 'Z')
                  {
                    grpn[0] = 'a' + (grpn[0] - 'A');
                  }

    fp = fopen(trim_string(group_name), "r");
      if(!fp)
      {
        printf("file is null");
      }

  char* line = malloc(len2*sizeof(line));
  while ((read2 = getline(&line, &len2, fp)) != -1)
	{
   

	  st = strlen(line);
	  word = strndup(line+1, st-3);
    
    vw = validword(word);
    if (vw == -1)
    {
     printf("not a valid word\n");
     continue;
     }

    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"%s%s",grpn,word);
        
    result=search_insert_delete(root,buffer,'i');
    memset(buffer,0,sizeof(buffer));
    if(result) { printf("\nInsertion unsuccessful!"); }
  }
    free(line);
  }
    free(group_name);
}

main()
{
 int ch,vw=0;
  insert();
 do
 {
  char choice[10],cchoice[10];
  int result,si=0,rem=0;
  char *word,*wordseg;

  printf("\n\n1. Enter into trie \n2. Search from trie \n3. Delete from trie \n4. Exit \n\nEnter your choice: ");
  scanf("%d",&ch);

  switch(ch)
  {

    case 2: printf("\nEnter the word to be searched: ");

                  memset(cchoice,0,sizeof(cchoice));
                  memset(choice,0,sizeof(choice));

                  scanf("%s",choice);
                  vw = validword(choice);
                  if (vw == -1)
                  {
                    printf("not a valid word\n");
                    break;
                  }
                  strcpy(cchoice,choice);
                  si = strlen(choice);
                  rem = 4;
                  wordseg = strndup(choice+3,si);
                  
                  word = strndup(choice, 3);

                  while(rem<si) 
                  {
                    result=search_insert_delete(root,choice,'s');
                    
                  if(result==0) { 
                    printf("\nSearch successful!\nThe word is present in the trie.");
                    break;
                  }
                  else
                  { printf("\nSearch unsuccessful!\nThe word is not present in the trie!");

                  wordseg = strndup(cchoice+rem,si);
                  memset(choice,0,sizeof(choice));
                  sprintf(choice,"%s%s",word,wordseg);
                      
                  rem++;
                  }
                  }
                  break;
  
    case 3 : printf("\nEnter the word to be deleted: ");
                   scanf("%s",choice);
                  
                   vw = validword(choice);

                  if (vw == -1)
                  {
                    printf("not a valid word\n");
                    break;
                  }
                   result=search_insert_delete(root,choice,'d');
                   if(result==0) printf("\nDeletion successful!");
                   else printf("\nSearch unsuccessful!\nThe word is not present in the trie!");
                   break;
    case 4 :  break;
    default: break;
  }
 }
 while(ch!=4);
}