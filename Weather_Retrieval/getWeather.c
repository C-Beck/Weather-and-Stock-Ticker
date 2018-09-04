#include <stdio.h>

int main()
{
    FILE *fp;
    fp=fopen("./icao_idents","r");
    int ret = fclose(fp);
}
