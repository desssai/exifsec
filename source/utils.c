#include "../include/exifsec.h"

// Convert an unsigned char to hex string (2 chars + null terminator)
void uchar_to_hex( unsigned char c, char *out ) {
  const char hex_digits[] = "0123456789ABCDEF";
  out[0]                  = hex_digits[( c >> 4 ) & 0xF];       // high nibble
  out[1]                  = hex_digits[c & 0xF];                // low nibble
  out[2]                  = '\0';
}

// Convert hex string (e.g. "1A", "ff") to int
int hex_to_int( const char *hex ) {
  int result = 0;
  while ( *hex ) {
    char c = toupper( ( unsigned char ) *hex++ );
    int  value;
    if ( c >= '0' && c <= '9' )
      value = c - '0';
    else if ( c >= 'A' && c <= 'F' )
      value = 10 + ( c - 'A' );
    else
      return -1;       // invalid hex char
    result = ( result << 4 ) | value;
  }
  return result;
}

// ----- BYTE-SWAPPING HELPERS -----
uint16_t swap16( uint16_t v ) {
  return ( v >> 8 ) | ( v << 8 );
}

uint32_t swap32( uint32_t v ) {
  return ( ( v >> 24 ) & 0xff ) | ( ( v >> 8 ) & 0xff00 ) | ( ( v << 8 ) & 0xff0000 ) |
         ( ( v << 24 ) & 0xff000000 );
}

uint64_t swap64( uint64_t v ) {
  return ( ( uint64_t ) swap32( ( uint32_t ) ( v & 0xffffffffULL ) ) << 32 ) |
         ( uint64_t ) swap32( ( uint32_t ) ( v >> 32 ) );
}

// ----- LOAD FUNCTIONS -----

uint8_t exif_get_byte( const unsigned char *buf ) {
  return buf[0];
}

int8_t exif_get_sbyte( const unsigned char *buf ) {
  return ( int8_t ) buf[0];
}

uint16_t exif_get_short( const unsigned char *buf, t_exif_byte_order order ) {
  uint16_t v;
  memcpy( &v, buf, sizeof( v ) );
  if ( order == EXIF_BYTE_ORDER_INTEL )
    v = swap16( v );
  return v;
}

int16_t exif_get_sshort( const unsigned char *buf, t_exif_byte_order order ) {
  return ( int16_t ) exif_get_short( buf, order );
}

uint32_t exif_get_long( const unsigned char *buf, t_exif_byte_order order ) {
  uint32_t v;
  memcpy( &v, buf, sizeof( v ) );
  if ( order == EXIF_BYTE_ORDER_INTEL )
    v = swap32( v );
  return v;
}

int32_t exif_get_slong( const unsigned char *buf, t_exif_byte_order order ) {
  return ( int32_t ) exif_get_long( buf, order );
}

t_exif_rational exif_get_rational( const unsigned char *buf, t_exif_byte_order order ) {
  t_exif_rational r;
  r.num = exif_get_long( buf, order );
  r.den = exif_get_long( buf + 4, order );
  return r;
}

t_exif_srational exif_get_srational( const unsigned char *buf, t_exif_byte_order order ) {
  t_exif_srational r;
  r.num = exif_get_slong( buf, order );
  r.den = exif_get_slong( buf + 4, order );
  return r;
}

float exif_get_float( const unsigned char *buf, t_exif_byte_order order ) {
  uint32_t v = exif_get_long( buf, order );
  float    f;
  memcpy( &f, &v, sizeof( f ) );
  return f;
}

double exif_get_double( const unsigned char *buf, t_exif_byte_order order ) {
  uint64_t v;
  memcpy( &v, buf, sizeof( v ) );
  if ( order == EXIF_BYTE_ORDER_INTEL )
    v = swap64( v );
  double d;
  memcpy( &d, &v, sizeof( d ) );
  return d;
}
