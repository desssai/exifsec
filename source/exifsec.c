#include "../include/exifsec.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

t_file *Readfile( const char *filepath ) {
  int     fd;
  t_file *file;

  fd = open( filepath, O_RDONLY );
  if ( fd < 0 ) {
    return NULL;
  }

  file = malloc( sizeof( t_file ) );
  if ( file == NULL ) {
    close( fd );
    return NULL;
  }

  if ( fstat( fd, &file->info ) ) {
    free( file );
    close( fd );
    return NULL;
  }

  file->content = malloc( file->info.st_size );
  if ( file->content == NULL ) {
    free( file );
    close( fd );
    return NULL;
  }

  memset( file->content, 0, file->info.st_size );
  int n = read( fd, file->content, file->info.st_size );
  if ( n != file->info.st_size ) {
    free( file->content );
    free( file );
    close( fd );
    return NULL;
  }

  // unsigned char soi_marker[]  = { 0xFF, 0xD8 };
  unsigned char app1_marker[] = { 0xFF, 0xE1 };
  // unsigned char eoi_marker[]  = { 0xFF, 0xD9 };
  void         *ptr;

  ptr = memmem( file->content, file->info.st_size, app1_marker, sizeof( app1_marker ) );

  if ( ptr != NULL ) {
    ssize_t size = ( ( ( unsigned char * ) ( ptr ) )[2] << 8 ) | ( ( unsigned char * ) ptr )[3];

    for ( int i = 0; i < size; i++ ) {
      printf( "%02x ", *( ( unsigned char * ) ( ptr + i ) ) );
      if ( ( i + 1 ) % 16 == 0 ) {
        printf( "\n" );
      }
    }

    printf( "\n" );
    printf( "\n" );
    for ( int i = 0; i < 128; i++ ) {
      printf( "%c  ", *( ( unsigned char * ) ( ptr + i ) ) );
      if ( ( i + 1 ) % 16 == 0 ) {
        printf( "\n" );
      }
    }
  }

  // for ( ssize_t i = 0; i < file->info.st_size; i++ ) {
  //   printf( "%02X ", file->content[i] );
  //   if ( ( i + 1 ) % 16 == 0 ) {
  //     printf( "\n" );
  //   }
  // }

  close( fd );
  return file;
};

int main( int argc, char **argv ) {
  if ( argc < 2 ) {
    return 1;
  }

  for ( int i = 1; i < argc; i++ ) {
    t_file *file = Readfile( argv[i] );
    free( file->content );
    free( file );
  }

  return 0;
}
