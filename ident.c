/*
 * ident.c
 * 
 * Copyright 2013 Russell Bryan <russell@godel>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "particles.h"
#include "images.h"
#include "config.h"
#include <math.h>
#include <openssl/sha.h>

#include <string.h>

#include "hashing.h"



extern void find_radius(particle *part);
extern void find_center(particle *part);
extern particle *floodfill(matrix *mat, int id, int x, int y);
extern void floodfill_work(matrix *mat, particle *part, int x, int y, int *id);

struct thread{
	pthread_t id;
	int state;
};
typedef struct thread thread;

//This function just makes it more convenient to assign an id to each
//new particle. It's really only used in getParticles().

int getid(int *id_counter){
	(*id_counter)++;
	return (*id_counter);
}

partlist *getParticles(matrix *mat){
	int i,j;
	int id_counter=0;
	int width,height;
	
	partlist *plist;
	if((plist = calloc(1,sizeof(partlist)))==NULL){
		fprintf(stderr, "ident.c: getParticles(): Out of memory error.\n");
		exit(1);
	}

	width=mat->width;
	height=mat->height;
	
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){

			if(mat->vals[i][j] == 0){
				continue;
			}

			particle *part;
			
			part = floodfill(mat,getid(&id_counter),i,j);
			find_center(part);
			find_radius(part);
#ifndef USE_RADIUS_THRESH
			if(part->size > MAX_PART_SIZE){			//This if statement removes any particles that are too large or too small.
				freeParticle(part);
				continue;
			}else if(part->size < MIN_PART_SIZE){
				freeParticle(part);
				continue;
			}
#endif
			
			
			
			
#ifdef	USE_RADIUS_THRESH
			
			if(part->radius > RAD_MAX){
				freeParticle(part);
				continue;
			}else if(part->radius < RAD_MIN){
				freeParticle(part);
				continue;
			}
#endif

			
			
			else if(part->x > mat->width-6){
				freeParticle(part);
				continue;
			}else if(part->x < 5){
				freeParticle(part);
				continue;
			}else if(part->y > mat->height-6){
				freeParticle(part);
				continue;
			}else if(part->y < 5){
				freeParticle(part);
				continue;
			}else{
				pushParticle(plist,part);
			}		
			
			
		}
	}
	return plist;
}


void drawline(matrix *mat, int val, int x1, int y1, int x2, int y2){
	int i,j,m;
	int xlarge, xsmall;
	if(x2 > x1){
		xlarge = x2;
		xsmall = x1;
	}else{
		xlarge = x1;
		xsmall = x2;
	}
	for(i=xsmall; i < xlarge; i++){
		m = ((y2-y1)/(x2-x1));
		j = (int)(m*i+(y1-m*x1));
		mat->vals[i][j] = val;
		
	}
		
	
	
}
//This function is really only for testing and viewing purposes.
//It serves no algorithmic purpose.

void drawparts(matrix *mat, partlist *parts, uint8_t val){
	int k,pixel;
	particle *current=0;
	pt *cur_pixel=0;
	//FILE *f;
	//f=fopen("output.txt","ab");
	current = parts->spart;
	for(k=0; k<parts->size; k++, current = current->nextParticle){
		
	
		//printf("ident.c: drawparts(): Particle %d has its center at: (%d,%d).\n", current->id, current->x, current->y);
		mat->vals[current->x][current->y] = 255;

		cur_pixel = current->spt;
		for(pixel = 0; pixel < current->size; pixel++, cur_pixel = cur_pixel->nextPt){
			mat->vals[cur_pixel->x][cur_pixel->y] = val;
			
		}
		//printf("%d %d %d %d\n",current->x, current->y, current->nextFrame->x, current->nextFrame->y);
		
		//printf("%s",SHA512_hash(message,20));
		drawline(mat,200,current->x, current->y, current->nextFrame->x, current->nextFrame->y);
		cur_pixel = current->nextFrame->spt;
		for(pixel = 0; pixel < current->nextFrame->size; pixel++, cur_pixel = cur_pixel->nextPt){
			mat->vals[cur_pixel->x][cur_pixel->y] = 50;
		}
		
		
	}
	//fclose(f);
}

//This function is really only a container to hold the actual recursive
//floodfill function (floodfill_work()). This one just conveniently handles input and 
//output from the recursive function.


particle *floodfill(matrix *mat, int id, int x, int y){

	particle *newparticle;
	int *current_id;
	if((current_id = calloc(1,sizeof(int)))==NULL){
		fprintf(stderr, "Out of memory error\n");
		exit(1);
	}
	*current_id = 1;
	newparticle = newParticle(id);
	pt *newpt;
	newpt = newPt(*current_id,x,y);
	newparticle->spt = newpt;
	newparticle->ept = newpt;
	newparticle->size++;
	floodfill_work(mat,newparticle,x,y,current_id);
	free(current_id);
	return newparticle;
}

void floodfill_work(matrix *mat, particle *part, int x, int y, int *id){
	
	
	if(mat->vals[x][y]==0){
		return;
	}
	
	pt *newpt;
	(*id)++;
	newpt = newPt(*id,x,y);
	pushPt(newpt, part);
	part->size++;
	
	mat->vals[x][y] = 0;
	if(!(x==0) && !(y==0)){
		floodfill_work( mat, part, x-1, y-1, id);
	}
	if(!(x==mat->width-1) && !(y==mat->height-1)){
		floodfill_work( mat, part, x+1, y+1, id);
	}
	if(!(x==0) && !(y==mat->height-1)){
		floodfill_work( mat, part, x-1, y+1, id);
	}
	if(!(x==(mat->width-1)) && !(y==0)){
		floodfill_work( mat, part, x+1, y-1, id);
	}
	if(!(x==0)){
		floodfill_work( mat, part,x-1, y, id);
	}
	if(!(x==mat->width-1)){
		floodfill_work( mat, part,x+1,y, id);
	}
	if(!(y==0)){
		floodfill_work( mat, part,x,y-1, id);
	}
	if(!(y==mat->height-1)){
		floodfill_work( mat, part,x, y+1, id);
	}
	return;

}

void find_center(particle *part){
	int x,y,i;
	int xmin,xmax,ymin,ymax;
	pt *current;
	current = part->spt;
	
	xmax=current->x;
	xmin=current->x;
	ymin=current->y;
	ymax=current->y;
	
	for(i=0; i<part->size; i++, current = current->nextPt){
		if(current->x > xmax){
			xmax = current->x;
		}else if(current->x < xmin){
			xmin = current->x;
		}
		if(current->y > ymax){
			ymax = current->y;
		}else if(current->y < ymin){
			ymin = current->y;
		}
	}
	x = (xmax-xmin)/2 + xmin;
	y = (ymax-ymin)/2 + ymin;
	
	part->x = x;
	part->y = y;
	
}

double dist(int x1,int y1,int x2,int y2){
	return sqrt((pow((x2-x1),2)+pow((y2-y1),2)));
}

//This fucntion requires that the center of the particle have been found.

void find_radius(particle *part){
	int i;
	double sum;
	pt *current;
	
	sum = 0;
	current = part->spt;
	
	for(i=0; i < part->size; i++, current = current->nextPt){
		sum = sum + dist(part->x, part->y, current->x, current->y);
	}
	part->radius = (sum/part->size);
	
}








