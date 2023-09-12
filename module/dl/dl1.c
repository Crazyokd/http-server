#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include "dl.h"

void invoke_method( char *lib, char *method, float argument )
{
  void *dl_handle;
  float (*func)(float);
  char *error;

  /* Open the shared object */
  dl_handle = dlopen( lib, RTLD_LAZY );
  if (!dl_handle) {
    printf( "!!! %s\n", dlerror() );
    return;
  }

  /* Resolve the symbol (method) from the object */
  func = (float (*)(float))dlsym( dl_handle, method );
  error = dlerror();
  if (error != NULL) {
    printf( "!!! %s\n", error );
    return;
  }

  /* Call the resolved method and print the result */
  printf("  %f\n", (*func)(argument) );

  /* Close the object */
  dlclose( dl_handle );

  return;
}