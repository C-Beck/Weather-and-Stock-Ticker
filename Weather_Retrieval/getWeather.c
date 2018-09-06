#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int getIdents(char *idents[])
{
    int numIdents = 0;
    FILE *fp;
    fp=fopen("./icao_idents","r");
    if(fp != NULL)
    {
	    char in[6];
	    while(fgets(in,6,fp)!=NULL)
	    {
		    numIdents++;
	    }
	    //printf("%d\n",numIdents);
	    //printf("%x\n",idents);
	    rewind(fp);
	    int i = 0;
	    while(fgets(in, 6, fp)!=NULL)
	    {
		    in[strcspn(in, "\n")] = 0;
		    //printf("%s\n",in);
		    idents[i]=malloc(5);
		    strncpy(idents[i],in,5);
		    i++;
	    }
	    for(int j=0; j<numIdents; j++)
	    {
		    printf("%s\n",idents[j]);
	    }
	    int ret = fclose(fp);
    }
    else{
	    fprintf(stderr,"File either does not exist or cannot be read\n");
    }
    return(numIdents);
}

int main()
{
    char *idents[64];
    int numIdents = getIdents(idents);
    
}
