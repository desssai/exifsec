#include "../include/exifsec.h"

void dump_exif_data( t_exif *exif ) {
  for ( unsigned int j = 0; j < exif->count; j++ ) {
    if ( exif->ifd[j] ) {
      for ( unsigned int i = 0; i < exif->ifd[j]->count; i++ ) {
        t_exif_entry *entry;

        entry = exif->ifd[j]->entries[i];
        printf( "%d\t%04x\t%2d\t%10d\t%10d\t", i, entry->tag, entry->format, entry->count,
                entry->size );
        switch ( entry->format ) {
          case EXIF_FORMAT_BYTE:
            for ( unsigned int k = 0; k < entry->count; k++ ) {
              printf( "%02x", *( entry->p + k ) );
            }
            printf( "\n" );
            break;
          case EXIF_FORMAT_ASCII:
            for ( unsigned int k = 0; k < entry->count; k++ ) {
              printf( "%c", *( entry->p + k ) );
            }
            printf( "\n" );
            break;
          case EXIF_FORMAT_SHORT:
            printf( "%d\n", exif_get_short( entry->p, exif->alignment ) );
            break;
          case EXIF_FORMAT_LONG:
            printf( "%d\n", exif_get_long( entry->p, exif->alignment ) );
            break;
          case EXIF_FORMAT_RATIONAL:
            t_exif_rational rat = exif_get_rational( entry->p, exif->alignment );
            printf( "%d/%d\n", rat.num, rat.den );
            break;
          default:
            printf( "not a value\n" );
        }
      }
      printf( "\n" );
    }
  }
};

void freeExifDirectory( t_exif_directory **directory ) {
  if ( !directory || !( *directory ) )
    return;
  if ( ( *directory )->entries ) {
    for ( unsigned int i = 0; i < ( *directory )->count; i++ ) {
      if ( ( *directory )->entries[i] ) {
        free( ( *directory )->entries[i] );
      }
    }
    free( ( *directory )->entries );
  }
  // free( *directory );
  // *directory = NULL;
}

void freeExifData( t_exif **exif ) {
  if ( !exif || !( *exif ) )
    return;
  if ( ( *exif )->ifd ) {
    // for ( unsigned int i = 0; i < ( *exif )->count; i++ ) {
    // if ( ( *exif )->ifd[i] ) {
    // freeExifDirectory( &( ( *exif )->ifd[i] ) );
    // }
    // }
    // free( ( *exif )->ifd );
  }
  // free( *exif );
  // *exif = NULL;
}

t_exif_directory *parseExifDirectory( const t_exif *exif, unsigned char *p ) {
  t_exif_directory *directory;
  t_exif_entry     *entry;

  directory = malloc( sizeof( t_exif_directory ) );
  if ( !directory ) {
    return NULL;
  }

  directory->count = exif_get_short( p, exif->alignment );
  printf( "directory count is: %d\n", directory->count );
  directory->entries = ( t_exif_entry ** ) malloc( sizeof( t_exif_entry * ) * directory->count );
  if ( !directory->entries ) {
    printf( "here 1\n" );
    freeExifDirectory( &directory );
    return NULL;
  }

  for ( unsigned int i = 0; i < directory->count; i++ ) {
    entry = ( t_exif_entry * ) malloc( sizeof( t_exif_entry ) );
    if ( !entry ) {
      printf( "here\n" );
      freeExifDirectory( &directory );
      return NULL;
    }

    entry->tag    = exif_get_short( p + 2 + i * 12, exif->alignment );
    entry->format = exif_get_short( p + 4 + i * 12, exif->alignment );
    entry->count  = exif_get_long( p + 6 + i * 12, exif->alignment );
    entry->size   = entry->count * exif_format_sizes[entry->format];
    if ( entry->size > 4 ) {
      entry->p = exif->data + exif_get_long( p + 10 + i * 12, exif->alignment );
    } else {
      entry->p = p + 10 + i * 12;
    }
    directory->entries[i] = entry;
  }
  return directory;
}

void parseExif( t_file *file ) {
  t_exif *exif;

  exif = ( t_exif * ) malloc( sizeof( t_exif ) );
  if ( !exif ) {
    return;
  }

  exif->ifd = ( t_exif_directory ** ) malloc( sizeof( t_exif_directory * ) * 4 );
  if ( !exif->ifd ) {
    freeExifData( &exif );
    return;
  }
  for ( int i = 0; i < 4; i++ ) {
    exif->ifd[i] = NULL;
  }
  exif->count = 0;

  exif->data  = memmem( file->content, file->info.st_size, EXIF_APP1, sizeof( EXIF_APP1 ) );
  // Check that the pointer to the start of exif header exists and the size
  // is not less than 3 (one byte of the marker and two bytes of the SSSS size)
  // by the exif header specification
  if ( exif->data && file->info.st_size - ( exif->data - file->content ) > 2 ) {
    int result;

    exif->size = ( exif->data[2] << 8 ) | exif->data[3];

    // EXIF HEADER
    result     = memcmp( exif->data + 4, EXIF_HEADER, sizeof( EXIF_HEADER ) );
    if ( result ) {
      freeExifData( &exif );
      return;
    }
    exif->data = exif->data + 10;

    if ( memcmp( exif->data, TIFF_HEADER_INTEL, sizeof( TIFF_HEADER_INTEL ) ) ) {
      exif->alignment = EXIF_BYTE_ORDER_INTEL;
    } else if ( memcmp( exif->data, TIFF_HEADER_MOTOROLA, sizeof( TIFF_HEADER_MOTOROLA ) ) ) {
      exif->alignment = EXIF_BYTE_ORDER_MOTOROLA;
    } else {
      freeExifData( &exif );
      return;
    }

    unsigned int offset;
    offset = exif_get_short( exif->data + 4, exif->alignment );
    printf( "offset is: %d\n", offset );
    exif->ifd[0] = parseExifDirectory( exif, exif->data + offset );
    exif->count++;

    offset = exif_get_short( exif->data + offset + 2 + exif->ifd[0]->count * 12, exif->alignment );
    printf( "offset is: %d\n", offset );
    exif->ifd[1] = parseExifDirectory( exif, exif->data + offset );
    exif->count++;

    offset = exif_get_short( exif->ifd[0]->entries[10]->p, exif->alignment );
    printf( "offset is: %d\n", offset );
    exif->ifd[3] = parseExifDirectory( exif, exif->data + offset );
    exif->count++;

    offset = exif_get_short( exif->ifd[0]->entries[9]->p, exif->alignment );
    printf( "offset is: %d\n", offset );
    exif->ifd[2] = parseExifDirectory( exif, exif->data + offset );
    exif->count++;

    unsigned char *p = exif->data + offset + 2 + ( exif->ifd[1]->count ) * 12;

    for ( unsigned int i = 0; i < offset + 30 * 12; i++ ) {
      if ( !( i % 12 ) ) {
        printf( "\n" );
      }
      printf( "%02x ", *( exif->data + i ) );
    }
    printf( "\n" );
    printf( "\n" );

    for ( unsigned int i = 0; i < 16; i++ ) {
      printf( "%02x ", *( p + i ) );
    }
    printf( "\n" );

    // offset =
    //     exif_get_short( exif->data + offset + ( exif->ifd[1]->count - 1 ) * 12, exif->alignment
    //     );
    // printf( "offset is: %d\n", offset );
    dump_exif_data( exif );
  }
}

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

  parseExif( file );

  close( fd );
  return file;
};

int main( int argc, char **argv ) {
  if ( argc < 2 ) {
    return 1;
  }

  for ( int i = 1; i < argc; i++ ) {
    t_file *file = Readfile( argv[i] );
    freeExifData( &( file->exif ) );
    free( file->content );
    free( file );
  }

  return 0;
}
