#ifndef EXIFSEC_H
#define EXIFSEC_H

#include <ctype.h>
#include <endian.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const unsigned char EXIF_APP1[]            = { 0xFF, 0xE1 };
static const unsigned char EXIF_HEADER[]          = { 0x45, 0x78, 0x69, 0x66, 0x00, 0x00 };
static const unsigned char TIFF_HEADER_INTEL[]    = { 0x49, 0x49, 0x2A, 0x00 };
static const unsigned char TIFF_HEADER_MOTOROLA[] = { 0x4D, 0x4D, 0x00, 0x2A };

typedef enum {
  EXIF_BYTE_ORDER_MOTOROLA,       // big endian
  EXIF_BYTE_ORDER_INTEL           // little endian
} t_exif_byte_order;

typedef enum e_exif_format {
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
} t_exif_format;

typedef struct {
    uint32_t num;
    uint32_t den;
} t_exif_rational;

typedef struct {
    int32_t num;
    int32_t den;
} t_exif_srational;

static const unsigned short exif_format_sizes[] = { 0, 1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8 };

typedef struct stat         t_file_info;

typedef struct s_ifd_entry {
    unsigned short tag;
    unsigned int   count;
    t_exif_format  format;
    unsigned int   size;

    unsigned char *p;
} t_exif_entry;

typedef struct s_ifd {
    unsigned int   count;
    t_exif_entry **entries;
} t_exif_directory;

typedef struct s_EXIF {
    unsigned char     *data;
    unsigned int       size;
    t_exif_byte_order  alignment;
    unsigned int       count;
    t_exif_directory **ifd;

} t_exif;

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
    t_exif        *exif;
} t_file;

typedef struct s_app {
    t_file **files;
} t_app;

void             uchar_to_hex( unsigned char c, char *out );
int              hex_to_int( const char *hex );

uint8_t          exif_get_byte( const unsigned char *buf );
int8_t           exif_get_sbyte( const unsigned char *buf );
uint16_t         exif_get_short( const unsigned char *buf, t_exif_byte_order order );
int16_t          exif_get_sshort( const unsigned char *buf, t_exif_byte_order order );
uint32_t         exif_get_long( const unsigned char *buf, t_exif_byte_order order );
int32_t          exif_get_slong( const unsigned char *buf, t_exif_byte_order order );
t_exif_rational  exif_get_rational( const unsigned char *buf, t_exif_byte_order order );
t_exif_srational exif_get_srational( const unsigned char *buf, t_exif_byte_order order );
float            exif_get_float( const unsigned char *buf, t_exif_byte_order order );
double           exif_get_double( const unsigned char *buf, t_exif_byte_order order );

#endif       // !EXIFSEC_H
