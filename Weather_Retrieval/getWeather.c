#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct ident{
	int old_time;
	int new_time;
	char code[5];
};

int getIdents(struct ident idents[])
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
		    //idents[i]=malloc(5);
		    strncpy(idents[i].code,in,5);
		    i++;
	    }
	    for(int j=0; j<numIdents; j++)
	    {
		    printf("%s\n",idents[j].code);
	    }
	    int ret = fclose(fp);
    }
    else{
	    fprintf(stderr,"File either does not exist or cannot be read\n");
    }
    return(numIdents);
}

char *getXML(struct ident idents[], char *rawXML)
{
	curl_global_init(CURL_GLOBAL_NOTHING);
	CURL *curl = curl_easy_init();
	if(curl)
	{
		CURLcode res;
		curl_easy_setopt(curl,CURLOPT_URL,"https://aviationweather.gov/adds/dataserver_current/httpparam?dataSource=metars&requestType=retrieve&format=xml&stationString=KRDU,KGSO,KINT&hoursBeforeNow=1");
		res = curl_easy_perform(curl);
		if(res!=0)
		{
			printf("ERROR\n");
		}
		curl_easy_cleanup(curl);
	}
	else{printf("ERROR\n");}
	curl_global_cleanup();
    	return rawXML;
}

int main()
{
    struct ident idents[64];
    int numIdents = getIdents(idents);
    char *rawXML;
    getXML(idents,rawXML);
    return 0;
}
