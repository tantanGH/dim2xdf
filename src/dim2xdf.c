#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>

#define VERSION "0.1.0"

#define MAX_PATH_LEN (256)
#define DIM_HEADER_BYTES (256)
#define DIM_TRACK_BYTES (8192)

inline static void* _dos_malloc(size_t size) {
  uint32_t addr = MALLOC(size);
  return (addr >= 0x81000000) ? NULL : (void*)addr;
}

inline static void _dos_mfree(void* ptr) {
  if (ptr == NULL) return;
  MFREE((uint32_t)ptr);
}

static void show_help_message() {
  printf("usage: dim2xdf [options] <dim-file> [xdf-file]\n");
  printf("options:\n");
  printf("     -h ... show version and help message\n");
}

//
//  main
//
int32_t main(int32_t argc, uint8_t* argv[]) {

  // default return code
  int32_t rc = -1;

  // source file name
  uint8_t* dim_name = NULL;

  // source buffer pointer
  uint8_t* dim_data = NULL;

  // source file pointer
  FILE* fp = NULL;

  // output file name
  static uint8_t xdf_name[ MAX_PATH_LEN + 1 ];
  xdf_name[0] = '\0';

  // output file pointer
  FILE* fo = NULL;

  // credit
  printf("DIM2XDF.X - DIM image to XDF image converter for X680x0 " VERSION " by tantan\n");

  // check command line
  if (argc < 2) {
    show_help_message();
    goto exit;
  }

  // parse command lines
  for (int16_t i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) >= 2) {
      if (argv[i][1] == 'h') {
        show_help_message();
        goto exit;
      } else {
        printf("error: unknown option (%s).\n",argv[i]);
        goto exit;
      }
    } else {
      if (dim_name == NULL) {
        if (strlen(argv[i]) < 5) {
          printf("error: not a dim file.\n");
          goto exit;
        }
        dim_name = argv[i];
      } else if (xdf_name[0] == '\0') {
        strcpy(xdf_name, argv[i]);
      } else {
        printf("error: too many files are specified.\n");
        goto exit;
      }
    }
  }

  if (dim_name == NULL) {
    show_help_message();
    goto exit;
  }

  if (xdf_name[0] == '\0') {
    strcpy(xdf_name, dim_name);
    uint8_t* xdf_ext = xdf_name + strlen(xdf_name) - 4;
    uint8_t* dim_ext = dim_name + strlen(dim_name) - 4;
    if (strcmp(dim_ext, ".dim") == 0) {
      strcpy(xdf_ext, ".xdf");
    } else {
      strcpy(xdf_ext, ".XDF");
    }
  }

  printf("source file: %s\n", dim_name);
  printf("output file: %s\n", xdf_name);
  
  // output file overwrite check
  struct FILBUF filbuf;
  if (FILES(&filbuf, xdf_name, 0x20) >= 0) {
    printf("warning: output file (%s) already exists. overwrite? (y/n)", xdf_name);
    uint8_t c;
    do {
      c = INKEY();
      if (c == 'n' || c == 'N') {
        printf("\ncanceled.\n");
        goto exit;
      }
    } while (c != 'y' && c != 'Y');
    printf("\n");
  }

  // read DIM header
  static uint8_t dim_header[ DIM_HEADER_BYTES ];
  fp = fopen(dim_name, "rb");
  if (fp == NULL) {
    printf("error: failed to open the source file (%s).\n", dim_name);
    goto exit;
  }

  size_t len = fread(dim_header, sizeof(uint8_t), DIM_HEADER_BYTES, fp);
  if (len != DIM_HEADER_BYTES) {
    printf("error: failed to read DIM header.\n");
    goto exit;
  }

  if (dim_header[0] != 0 || memcmp(dim_header + 0xab, "DIFC", 4) != 0) {
    printf("error: not DIM format.\n");
    goto exit;
  }
  
  // open output file
  fo = fopen(xdf_name, "wb");
  if (fo == NULL) {
    printf("error: failed to open the output file (%s).\n", xdf_name);
    goto exit;
  }

  // allocate memory buffer
  dim_data = _dos_malloc(DIM_TRACK_BYTES);
  if (dim_data == NULL) {
    printf("error: memory allocation error (out of memory?).\n");
    goto exit;
  }
  
  // conversion
  for (int16_t i = 0; i < 154; i++) {
    int16_t n = dim_header[ i + 1 ];
    if (n > 0) {
      fread(dim_data, sizeof(uint8_t), DIM_TRACK_BYTES, fp);
    } else {
      memset(dim_data, 0, DIM_TRACK_BYTES);
    }
    size_t write_len = 0;
    do {
      size_t w = fwrite(dim_data + write_len, sizeof(uint8_t), DIM_TRACK_BYTES - write_len, fo);
      if (w == 0) break;
      write_len += w;
    } while (write_len < DIM_TRACK_BYTES);
    if (write_len < DIM_TRACK_BYTES) {
      printf("error: file write error (disk full?).\n");
      goto exit;
    }
  }

  printf("Done.\n");

  rc = 0;

exit:

  if (fo != NULL) {
    fclose(fo);
    fo = NULL;
  }

  if (fp != NULL) {
    fclose(fp);
    fp = NULL;
  }

  if (dim_data != NULL) {
    _dos_mfree(dim_data);
    dim_data = NULL;
  }

  return rc;
}