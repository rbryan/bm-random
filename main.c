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

struct data
{
  char name[90];
  int *num;
  char *directory;
  char *ext;
};

void *
pimage (void *dat)
{				//This should get passed a file name and return a partlist
  image *img = NULL;
  struct data *data;
  partlist *plist;
  int *i;
  data = (struct data *) dat;
  pthread_mutex_lock (&lock);
  for (i = data->num; (img == NULL); *(i)++)
    {
      sprintf (data->name, "%s%d%s", data->directory, *i, data->ext);
      img = loadImage (data->name);
      //printf("img %d\n",*i);
    }
  pthread_mutex_unlock (&lock);
  if (img == NULL)
    return NULL;
  threshold (img->red, img->red, 29, 149);
  img->red = gradient (img->red);
  plist = getParticles (img->red);
  freeImage (img);
  return (void *) plist;
}

void
spawnt (struct data *dat, pthread_t * thread)
{
  pthread_create (thread, NULL, &pimage, (void *) dat);
}


int
main (int argc, char **argv)
{
  pthread_t threads[2];
  pthread_mutex_init (&lock, NULL);

  if (argc < 4)
    {
      printf ("Usage: <prgrm> <directory> <file extension> <# of images>\n");
      exit (1);
    }
  char *directory;
  directory = argv[1];
  char *ext;
  ext = argv[2];
  int num, i = 1;
  num = atoi (argv[3]);
  struct data dat;
  dat.directory = directory;
  dat.ext = ext;
  dat.num = &i;

  partlist *listcurrent = 0, *listprev = 0;
  sprintf (dat.name, "%s%d%s", directory, i, ext);
  spawnt (&dat, &threads[0]);
  sprintf (dat.name, "%s%d%s", directory, ++i, ext);
  spawnt (&dat, &threads[1]);
  pthread_join (threads[0], (void **) &listprev);
  pthread_join (threads[1], (void **) &listcurrent);

  for (i = i + 1; i < num; i++)
    {
      //fprintf(stderr,"Image #%d\n",i);
      sprintf (dat.name, "%s%d%s", directory, i, ext);
      spawnt (&dat, &threads[0]);
      link_parts (listprev, listcurrent);
      hashlist (listprev);
      freePartlist (listprev);
      listprev = listcurrent;
      pthread_join (threads[0], (void **) &listcurrent);

    }
  pthread_mutex_destroy (&lock);
  freePartlist (listprev);
  freePartlist (listcurrent);
  return 0;

}
