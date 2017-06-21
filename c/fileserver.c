/*
     This file is part of libmicrohttpd
     Copyright (C) 2007 Christian Grothoff (and other contributing authors)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 * @file fileserver_example.c
 * @brief minimal example for how to use libmicrohttpd to serve files
 * @author Christian Grothoff
 */
/**
 * Modified by Mathieu CAROFF and Alix BOULOC
 * Adapted to the cojsTetris game project
 */

#include "../include/all.h"

#define PAGE404 "<!DOCTYPE html><html><head><title>File not found</title></head><body>File not found</body></html>"
#define DEMO_PAGE "<!DOCTYPE html><html><head><title>File not found</title></head><body>File not found. The query string is `%s`.</body></html>" 
#define DOCUMENT_ROOT "www"
#define DEFAULT_PATH "/jsTetris.html"
#ifndef S_ISREG
#define S_ISREG(x) (S_IFREG == (x & S_IFREG))
#endif /* S_ISREG */
#define DATA_BUFFER_SIZE 8000
#define nbJoueurs 2
char data[DATA_BUFFER_SIZE] ;
char* id ; 
sem_t sem_data ; 
sem_t sem_id ; 
int tabID[nbJoueurs] ;
clock_t temps1 ; 
clock_t temps2 ; 

int testURL( const char *url) {
	int spurl = -1; 
	if (strncmp(url, "/!/", 3)) {
		spurl = 0 ; 
			if (url == strstr(url , "/!/get")) {
				spurl = 1 ; 
			}
			if (url == strstr(url , "/!/post/1/")){
				spurl = 1 ;
				sem_wait(&sem_data) ; 
				strcpy(data, url) ; 
				temps1 = clock() ; 
				sem_post (&sem_data) ; 
			}
			if (url ==strstr(url, "/!/post/2/")) {
				spurl = 1 ;
				sem_wait(&sem_data) ; 
				strcpy(data, url) ; 
				temps2 = clock() ; 
				sem_post (&sem_data) ; 
			}
			if (url == strstr(url, "/!/newid")) {
				spurl = 2 ; 
				sem_wait(&sem_id) ; 
				if (((clock()-temps1)/CLOCKS_PER_SEC) > 90) { // Check of the activity of the two players
					tabID[0] = 1 ;
					}
				if (((clock()-temps2)/CLOCKS_PER_SEC) > 90) {
					tabID[1] = 1 ;
					}
				if (tabID[0] !=0) { 
					tabID[0] = 0 ; 
					id = "1" ; 
				}
				else  {
					if (tabID[1] !=0) {
						tabID[1] = 0 ; 
						id = "2" ; 
						}
					else spurl=0 ; 
				}
				sem_post(&sem_id) ;
			}
	
	return spurl ; 
	}
}

static int
ahc_echo (void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data,
	  size_t *upload_data_size, void **ptr)
{
  static int aptr;
  struct MHD_Response *response;
  int ret;
  int fd;
  struct stat buf;
	int spurl ; 
	spurl = testURL(url) ; 
	if (spurl == 1) {
      enum MHD_ResponseMemoryMode mode = MHD_RESPMEM_PERSISTENT;
			sem_wait(&sem_data) ; 
      response = MHD_create_response_from_buffer (strlen(data),
						  (void *) data,
						  mode);
			sem_post(&sem_data) ; 
	      if (NULL == response)
		{
		  return MHD_NO;
		}
	      ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	      MHD_destroy_response (response);
	}
	if (spurl == 2) {
      enum MHD_ResponseMemoryMode mode = MHD_RESPMEM_PERSISTENT;
      response = MHD_create_response_from_buffer (strlen(id),
						  (void *) id,
						  mode);
	      if (NULL == response)
		{
		  return MHD_NO;
		}
	      ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	      MHD_destroy_response (response);
	}
	if (spurl == 0 ) {
			int length = strlen("") ;
			char* page = "" ; 
      enum MHD_ResponseMemoryMode mode = MHD_RESPMEM_PERSISTENT;
      response = MHD_create_response_from_buffer (length, (void *) page,mode);
	    if (NULL == response)
			{
		  return MHD_NO;
			}
	    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	    MHD_destroy_response (response);
	}		

  if ( (0 != strcmp (method, MHD_HTTP_METHOD_GET)) &&
       (0 != strcmp (method, MHD_HTTP_METHOD_HEAD)) )
    return MHD_NO;              /* unexpected method */
  if (&aptr != *ptr)
    {
      /* do never respond on first call */
      *ptr = &aptr;
      return MHD_YES;
    }
  *ptr = NULL;                  /* reset when done */
  /* WARNING: direct usage of url as filename is for example only!
   * NEVER pass received data directly as parameter to file manipulation
   * functions. Always check validity of data before using.
   */
if (spurl ==-1 ) {
	  if (NULL != strstr(url, "../")) /* Very simplified check! */
	    fd = -1; /* Do not allow usage of parent directories. */
	  else
	    {
	      if (strcmp(url, "/") == 0)
		url = DEFAULT_PATH;
	      char *path = calloc(1 + strlen(DOCUMENT_ROOT) + strlen(url), sizeof(char));
	      strcat(path, DOCUMENT_ROOT);
	      strcat(path + strlen(path), url);
	      fd = open(path, O_RDONLY);
	    }
	  if (-1 != fd)
	    {
	      if ( (0 != fstat (fd, &buf)) ||
		   (! S_ISREG (buf.st_mode)) )
		{
		  /* not a regular file, refuse to serve */
		  if (0 != close (fd))
		    abort ();
		  fd = -1;
		}
	    }
	  if (-1 == fd)
	    {
	      //const char *queryString = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, NULL);
	      //int length = snprintf(NULL, 0, DEMO_PAGE, queryString);
	      //char *page = calloc(length, sizeof(char));
	      //if (page == NULL)
	      //  return MHD_NO;
	      //snprintf(page, length, DEMO_PAGE, queryString);
	      int length = strlen(PAGE404);
	      char *page = PAGE404;
	      enum MHD_ResponseMemoryMode mode = MHD_RESPMEM_PERSISTENT; // MHD_RESPMEM_MUST_FREE; // 
	      response = MHD_create_response_from_buffer (length,
							  (void *) page,
							  mode);
	      ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, response);
	      MHD_destroy_response (response);
	    }
	  else
	    {
	      response = MHD_create_response_from_fd64 (buf.st_size, fd);

	      if (NULL == response)
		{
		  if (0 != close (fd))
		    abort ();
		  return MHD_NO;
		}
	      ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	      MHD_destroy_response (response);
	    }
	}
  return ret;
}


int
main (int argc, char *const *argv)
{ 

  struct MHD_Daemon *d;

  if (argc != 2)
    {
      printf ("%s PORT\n", argv[0]);
      return 1;
    }
  sem_init (&sem_data, 0, 1) ;
  sem_init (&sem_id, 0, 1) ;                                                                                                
  tabID[0] = 1;
	tabID[1] = 1 ;  
   
  d = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, // | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
                        atoi (argv[1]),
                        NULL, NULL, &ahc_echo, PAGE404, MHD_OPTION_END);
  if (d == NULL)
    return 1;
  (void) getc (stdin);
  MHD_stop_daemon (d);
  return 0;
}
