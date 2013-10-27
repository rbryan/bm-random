#include <stdio.h>
#include <stdlib.h>

void main(){
	char in;
	while(1==1){
		in = getc(stdin);
		if(in != EOF){
			putc(in,stdout);
		}
	}
}
