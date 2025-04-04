#ifndef ZIO_H
#define ZIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <time.h>

/* MSVC 用の定義 */
#ifdef _MSC_VER
    #include <BaseTsd.h>
    typedef SSIZE_T ssize_t;
#endif

/*
 * Header size of the ebzip compression file.
 */
#define ZIO_SIZE_EBZIP_HEADER		22

/*
 * Margin size for ebzip compression buffer.
 * (Since compressed data is larger than original in the worst case,
 * we must add margin to a compression buffer.)
 */
#define ZIO_SIZE_EBZIP_MARGIN		1024

/*
 * Maximum ebzio compression level.
 */
#define ZIO_MAX_EBZIP_LEVEL		5

/*
 * Huffman node types.
 */
#define ZIO_HUFFMAN_NODE_INTERMEDIATE	0
#define ZIO_HUFFMAN_NODE_EOF		1
#define ZIO_HUFFMAN_NODE_LEAF8		2
#define ZIO_HUFFMAN_NODE_LEAF16		3
#define ZIO_HUFFMAN_NODE_LEAF32		4

/*
 * Compression type codes.
 */
#define ZIO_PLAIN			0
#define ZIO_EBZIP1			1
#define ZIO_EPWING			2
#define ZIO_EPWING6			3
#define ZIO_SEBXA			4
#define ZIO_INVALID        		-1
#define ZIO_REOPEN			-2

/*
 * Compression type.
 */
typedef int Zio_Code;

/*
 * A node of static Huffman tree.
 */
typedef struct Zio_Huffman_Node_Struct Zio_Huffman_Node;

struct Zio_Huffman_Node_Struct {
    /*
     * node type (ITNERMEDIATE, LEAF8, LEAF16, LEAF32 or EOF).
     */
    int type;

    /*
     * Value of a leaf node.
     */
    unsigned int value;

    /*
     * Frequency of a node.
     */
    int frequency;

    /*
     * Left child.
     */
    Zio_Huffman_Node *left;

    /*
     * Right child.
     */
    Zio_Huffman_Node *right;
};

/*
 * Compression information of a book.
 */
typedef struct Zio_Struct Zio;

struct Zio_Struct {
    /*
     * ID.
     */
    int id;

    /*
     * Zio type. (PLAIN, EBZIP, EPWING, EPWING6 or SEBXA)
     */
    Zio_Code code;

    /*
     * File descriptor.
     */
    int file;

    /*
     * Current location.
     */
    off_t location;

    /*
     * Size of an uncompressed file.
     */
    off_t file_size;

    /*
     * Slice size of an EBZIP compressed file.
     */
    size_t slice_size;

    /*
     * Compression level. (EBZIP compression only)
     */
    int zip_level;

    /*
     * Length of an index. (EBZIP compression only)
     */
    int index_width;

    /*
     * Adler-32 check sum of an uncompressed file. (EBZIP compression only)
     */
    unsigned int crc;

    /*
     * mtime of an uncompressed file. (EBZIP compression only)
     */
    time_t mtime;

    /*
     * Location of an index table. (EPWING and S-EBXA compression only)
     */
    off_t index_location;

    /*
     * Length of an index table. (EPWING and S-EBXA compression only)
     */
    size_t index_length;

    /*
     * Location of a frequency table. (EPWING compression only)
     */
    off_t frequencies_location;

    /*
     * Length of a frequency table. (EPWING compression only)
     */
    size_t frequencies_length;

    /*
     * Huffman tree nodes. (EPWING compression only)
     */
    Zio_Huffman_Node *huffman_nodes;

    /*
     * Root node of a Huffman tree. (EPWING compression only)
     */
    Zio_Huffman_Node *huffman_root;

    /*
     * Region of compressed pages. (S-EBXA compression only)
     */
    off_t zio_start_location;
    off_t zio_end_location;

    /*
     * Add this value to offset written in index. (S-EBXA compression only)
     */
    off_t index_base;

    /*
     * ebnet mode flag.
     */
    int is_ebnet;
};

/*
 * Function declarations.
 */
/* zio.c */
int zio_initialize_library(void);
void zio_finalize_library(void);
void zio_initialize(Zio *zio);
void zio_finalize(Zio *zio);
int zio_set_sebxa_mode(Zio *zio, off_t index_location, off_t index_base,
    off_t zio_start_location, off_t zio_end_location);
int zio_open(Zio *zio, const char *file_name, Zio_Code zio_code);
void zio_close(Zio *zio);
int zio_file(Zio *zio);
Zio_Code zio_mode(Zio *zio);
off_t zio_lseek(Zio *zio, off_t offset, int whence);
ssize_t zio_read(Zio *zio, char *buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* not ZIO_H */
