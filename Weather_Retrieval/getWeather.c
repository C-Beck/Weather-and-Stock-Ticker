#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/ssl.h>

struct ident{
	int old_time;
	int new_time;
	char code[5];
};

struct MemoryStruct{
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(ptr == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}
	
//	printf("%s\n",contents);

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	 
	return realsize;
}

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

struct MemoryStruct getXML(struct ident idents[], int numIdents, struct MemoryStruct chunk)
{
	char *head = "https://aviationweather.gov/adds/dataserver_current/httpparam?dataSource=metars&requestType=retrieve&format=xml&stationString=";
	char *tail = "&hoursBeforeNow=1";
	char *url = malloc(strlen(head));
	strcpy(url,head);
	for(int i=0; i< numIdents;i++)
	{
		url = (char *)realloc(url,strlen(url)+5);
		strcat(url,idents[i].code);
		if(i<numIdents-1)
		{
			strcat(url,",");
		}
	}
	url=(char *)realloc(url,strlen(url)+strlen(tail));
	strcat(url,tail);

	printf("%s\n",url);

	SSL_library_init();
	curl_global_init(CURL_GLOBAL_NOTHING);
	CURL *curl = curl_easy_init();
	CURLcode res;
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_HTTPGET,1);
//	curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&chunk);
	res = curl_easy_perform(curl);
	if(res!=0)
	{
		printf("%d\nERROR1\n%s\n",res,curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	free(url);
    	return chunk;
}

char * parseXML(char *var, char *out)
{
	char *sub = strstr(var,"<raw_text>");
	int len = 0;
//	printf("%d\n",strlen(out));
	while(sub!=NULL)
	{
		sub=strstr(sub,">")+1;
//		printf("%s\n",sub);
		len=strstr(sub,"<")-sub;
//		printf("%d\n",len);
		out = (char *)realloc(out,strlen(out)+len+1);
		strncat(out,sub,len);
		strcat(out,"\n");
//		printf("%d\n",strlen(out));
		sub=strstr(sub,"<raw_text>");
	}
	out = (char *)realloc(out,strlen(out)+1);
	strcat(out,"\x00");
//	printf("%s\n",out);
	return(out);
}

int main()
{
	struct ident idents[64];
	int numIdents = getIdents(idents);
	char *output = malloc(1);
	strcpy(output,"");
//	printf("%d\n",strlen(output));
	struct MemoryStruct chunk;
	chunk.memory=malloc(1);
	chunk.size=0;
	chunk=getXML(idents,numIdents,chunk);
	if(chunk.size!=0)
	{
//		printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
//		printf("%s\n",chunk.memory);
		output = parseXML(chunk.memory,output);
		printf("%s\n",output);
	}
	free(output);
	return 0;
}
