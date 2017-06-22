#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <microhttpd.h>
#include <fcntl.h>

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/** 
 * This function is made to test if the first characters of the const char *a are the one of const char *b
 * It returns 1 if the chain b is the beggining of chain a. Else, it returns 0. 
 */
int starts_with(const char *a, const char *b) ; 

/**
 * This function is made to analyse the url received. 
 * It returns -1 if the url is incorrect. 
 * It returns 0 if the url is incorrect but made to be received by the server
 * It returns 1 if the url is a request to modifie (in that case data is directly modified in that function) or
 * to acquire the data.
 * It returns 2 if the url is request to acquire an ip 	
 */
int testURL( const char *url) ; 

/**
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param url the requested url
 * @param method the HTTP method used (#MHD_HTTP_METHOD_GET,
 *        #MHD_HTTP_METHOD_PUT, etc.)
 * @param version the HTTP version string (i.e.
 *        #MHD_HTTP_VERSION_1_1)
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of #MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        @a upload_data)
 * @param[in,out] upload_data_size set initially to the size of the
 *        @a upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;

 * @return #MHD_YES if the connection was handled successfully,
 *         #MHD_NO if the socket must be closed due to a serious
 *         error while handling the request
 *        ret if the response has been created. 
*/
static int
ahc_echo (void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data,
	  size_t *upload_data_size, void **ptr) ;

/**
 * The main must receive 2 arguments. The second one must be the connection point of the server. 
 * @return 1 if the thread wasn't supported
 * @return 0 if success
 */

