#include "../include/exifsec.h"

// Convert an unsigned char to hex string (2 chars + null terminator)
void uchar_to_hex( unsigned char c, char *out ) {
  static const char hex_digits[] = "0123456789ABCDEF";
  out[0]                         = hex_digits[( c >> 4 ) & 0xF];       // high nibble
  out[1]                         = hex_digits[c & 0xF];                // low nibble
  out[2]                         = '\0';
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
