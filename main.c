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
#include <pthread.h>
#include "images.h"
#include "particles.h"
#include "ident.h"
#include "tracking.h"
#include "hashing.h"


pthread_mutex_t lock;

struct data {
    char name[90];
    int *num;
    char *directory;
    char *ext;
};

partlist *pimage(struct data *dat)
{				//This should get passed a file name and return a partlist
    image *img = NULL;
    partlist *plist;
    int *i;
    for (i = dat->num; (img == NULL); *(i)++) {
	sprintf(dat->name, "%s%d%s", dat->directory, *i, dat->ext);
	img = loadImage(dat->name);
	//printf("img %d\n",*i);
    }
    if (img == NULL)
	return NULL;
    threshold(img->red, img->red, 29, 149);
    //printf("%ld\n",(long int) img->red);
    img->red = gradient(img->red);
    //printf("%ld\n",(long int) img->red);
    plist = getParticles(img->red);
    freeImage(img);
    return plist;
}



int main(int argc, char **argv)
{

    if (argc < 4) {
	printf
	    ("Usage: <prgrm> <directory> <file extension> <# of images>\n");
	exit(1);
    }

    char *directory;
    directory = argv[1];

    char *ext;
    ext = argv[2];

    int num, i = 1;
    num = atoi(argv[3]);

    //initialize the data structure
    struct data dat;
    dat.directory = directory;
    dat.ext = ext;
    dat.num = &i;

    //create the particle lists
    partlist *listcurrent = 0, *listprev = 0, *tmp = 0;

    //process the first two images
    sprintf(dat.name, "%s%d%s", directory, i, ext);
    listprev = pimage(&dat);

    sprintf(dat.name, "%s%d%s", directory, ++i, ext);
    listcurrent = pimage(&dat);

    //process the rest of the image set
    //#pragma omp parallel for private(i)
    for (i = i + 1; i < num; i++) {
        #pragma omp parallel sections
	{
            #pragma omp section
	    {
		sprintf(dat.name, "%s%d%s", directory, i, ext);
		tmp = pimage(&dat);
	    }

            #pragma omp section
	    {
		link_parts(listprev, listcurrent);
		hashlist(listprev);
		freePartlist(listprev);
		listprev = listcurrent;
	    }
	}
	
	listcurrent = tmp;

    }
    pthread_mutex_destroy(&lock);
    freePartlist(listprev);
    freePartlist(listcurrent);
    return 0;

}
