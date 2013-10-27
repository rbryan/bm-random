#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void pop(char * data,int *len, int index){
	int i;
	
	for(i=index; i < *len; i++){
		data[i] = data[i+1];
	}
	(*len)--;
	
}

char * cleanhash(char *data, int len){
	int i;
	for(i=0; i < len; i++){
		if(data[i] == EOF){ //|| data[i] == '\0')
			pop(data,&len,i);
		}
	}
	return data;
}

char * SHA512_hash(const char *data, int len){

	SHA512_CTX ctx;
	char *md;
	if((md=malloc(SHA512_DIGEST_LENGTH))==NULL){
		fprintf(stderr,"Out of memory error...\n");
		exit(1);
	}
	SHA512_Init(&ctx);
	SHA512_Update(&ctx,data,len);
	SHA512_Final((unsigned char *)md,&ctx);
	//md[SHA512_DIGEST_LENGTH] = '\0';
	//md = cleanhash(md,SHA512_DIGEST_LENGTH+1);
	return md;
}
