#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "particles.h"
#include <stdint.h>

union partdata{
	struct particle part;
	char data[20];	
};


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
static void hashpart(particle *p){
	char message[20];
	char md[SHA512_DIGEST_LENGTH];
	memcpy(message, ((union partdata *)p)->data,20);
	memcpy(md,SHA512_hash(message,20),64);
	int counter;
	for(counter = 0; counter < SHA512_DIGEST_LENGTH; counter=counter+4){
		printf("%u\n",*((uint32_t *) &md[counter]));
	}

}
void hashlist(partlist *parts){
	particle *current;
	int i;
	current=parts->spart;
	for(i=0; i<parts->size; i++, current = current->nextParticle){
		hashpart(current);
	}

}
