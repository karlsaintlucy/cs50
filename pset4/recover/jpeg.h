#include <stdint.h>

#define BLOCK_MAX_SIZE 512

typedef uint8_t  BYTE;

typedef struct
{
  BYTE JPEGData[BLOCK_MAX_SIZE];
} BLOCK;