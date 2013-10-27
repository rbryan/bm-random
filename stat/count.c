#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	FILE* f;
	long int *bins;
	bins = calloc(256,sizeof(long int));
	f = fopen(argv[1],"rb");
	char c;
	int size;
	fseek(f,0,SEEK_END);
	size = ftell(f);
	printf("size: %d\n",size);
	fseek(f,0,SEEK_SET);
	while(ftell(f) < size){
		fread(&c,sizeof(char),1,f);
		bins[(int)c]++;
		if(ftell(f)%1000==0) printf("%f\n",ftell(f)*1.0/size);
	}
	int i;
	long long int sum;
	for(i=0; i < 256; i++){
		printf("%d :: %ld\n",i,bins[i]);
		sum = sum+bins[i];
	}
	printf("Average: %f\n",sum*1.0/256);
	return 0;
}
