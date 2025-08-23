#ifndef EXIFSEC_H
#define EXIFSEC_H

#include <ctype.h>
#include <endian.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define JPEG_SOI         { 0xFF, 0xD8, '\0' };
#define JPEG_EOI         { 0xFF, 0xD9, '\0' };
#define JPEG_APP1_START  { 0xFF, 0xE1, '\0' };
#define JPEG_EXIF_HEADER { 0x45, 0x78, 0x69, 0x66, 0x00, 0x00, '\0' };

typedef enum {
  EXIF_BYTE_ORDER_MOTOROLA,       // big endian
  EXIF_BYTE_ORDER_INTEL           // little endian
} t_exif_byte_order;

typedef enum {
  EXIF_FORMAT_BYTE      = 1,
  EXIF_FORMAT_ASCII     = 2,
  EXIF_FORMAT_SHORT     = 3,
  EXIF_FORMAT_LONG      = 4,
  EXIF_FORMAT_RATIONAL  = 5,
  EXIF_FORMAT_SBYTE     = 6,
  EXIF_FORMAT_UNDEFINED = 7,
  EXIF_FORMAT_SSHORT    = 8,
  EXIF_FORMAT_SLONG     = 9,
  EXIF_FORMAT_SRATIONAL = 10,
  EXIF_FORMAT_FLOAT     = 11,
  EXIF_FORMAT_DOUBLE    = 12
} t_exif_data_format;

typedef enum e_exif_formats {
  EXIF_FORMAT_SIZE_BYTE      = 1,
  EXIF_FORMAT_SIZE_ASCII     = 2,
  EXIF_FORMAT_SIZE_SHORT     = 3,
  EXIF_FORMAT_SIZE_LONG      = 4,
  EXIF_FORMAT_SIZE_RATIONAL  = 5,
  EXIF_FORMAT_SIZE_SBYTE     = 6,
  EXIF_FORMAT_SIZE_UNDEFINED = 7,
  EXIF_FORMAT_SIZE_SSHORT    = 8,
  EXIF_FORMAT_SIZE_SLONG     = 9,
  EXIF_FORMAT_SIZE_SRATIONAL = 10,
  EXIF_FORMAT_SIZE_FLOAT     = 11,
  EXIF_FORMAT_SIZE_DOUBLE    = 12
} t_exif_data_format;

typedef struct stat t_file_info;

typedef struct s_JFIF {
} t_JFIF;

typedef struct s_XMP {
} t_XMP;

typedef struct s_ifd_entry {
    unsigned int   tag;
    ssize_t        size;

    unsigned char *p;
} t_exif_entry;

typedef struct s_ifd {
    unsigned short count;
} t_exif_directory;

typedef struct s_EXIF {
    t_exif_byte_order alignment;

} t_EXIF;

typedef struct s_file {
    t_file_info    info;
    // const char *FileName;
    // const char *Directory;
    // ssize_t FileSize;
    // const char *FileModifyDate;
    // const char *FileAccessDate;
    // const char *FileInodeChangeDate;
    // const char *FilePermissions;
    // const char *FileType;
    // const char *FileTypeExtension;
    // const char *MIMEType;
    // const char *ExifByteOrder;
    // const char *ImageWidth;
    // const char *ImageHeight;
    // const char *EncodingProcess;
    // const char *BitsPerSample;
    // const char *ColorComponents;
    // const char *YCbCrSubSampling;
    unsigned char *content;
    t_EXIF        *exif;
    t_XMP         *xmp;
    t_JFIF        *jfif;
    void          *buf;
} t_file;

typedef struct s_app {
    t_file **files;
} t_app;

void uchar_to_hex( unsigned char c, char *out );
int  hex_to_int( const char *hex );

#endif       // !EXIFSEC_H
