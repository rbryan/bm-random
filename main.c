/*
 * main.c
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
#include <stdlib.h>
#include "images.h"
#include "particles.h"
#include "ident.h"
#include "tracking.h"

#define IMG_SET 3

int main(int argc, char *argv[])
{	
	int start, finish;
	start = 1;
	finish = 599;
	image *img;
	char fn[30],outname[30];
	partlist *listcurrent=0, *listprev=0;
	int i;
	
	sprintf(fn,"data/set_%d/%d.jpg",IMG_SET,start);
	img = loadImage(fn);
	threshold(img->red, img->red, 29, 149);
	img->red = gradient(img->red);
	listprev = calloc(1,sizeof(partlist));
	listprev = getParticles(img->red);
	freeImage(img);
	
	
	for(i=start+1; i<finish; i++){
		//fprintf(stderr,"Image #%d\n",i);
		sprintf(fn,"data/set_%d/%d.jpg",IMG_SET,i);
		img = loadImage(fn);
		
		if(img==NULL){
			continue;
		}
		
		threshold(img->red, img->red, 29, 149);

		img->red = gradient(img->red);

		listcurrent = getParticles(img->red);

		link_parts(listprev,listcurrent);
		//printf("main.c: main(): Number of particles: %d\n",(int)listcurrent->size);
		drawparts(img->red, listprev, 255);
		fill_matrix(img->green, 0);
		fill_matrix(img->blue, 0);
		sprintf(outname,"output/%d.bmp",i);
		saveImage(img,outname);
		
		freePartlist(listprev);
		listprev = listcurrent;
		
		freeImage(img);

	}
	freePartlist(listcurrent);
    return 0;

}
