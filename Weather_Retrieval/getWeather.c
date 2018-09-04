#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main()
{
    FILE *fp;
    fp=fopen("./icao_idents","r");
    if(fp != NULL)
    {
	    char in[100];
	    while(fgets(in, 100, fp)!=NULL)
	    {
		    in[strcspn(in, "\n")] = 0;
		    printf("%s\n",in);
	    }
	    int ret = fclose(fp);
    }
    else{
	    fprintf(stderr,"File either does not exist or cannot be read\n");
    }

}
