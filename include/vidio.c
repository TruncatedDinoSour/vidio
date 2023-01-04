#ifndef _VIDIO_C
#define _VIDIO_C
/*
 * C89-compatible vidio parsing and rendering library
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long vidio_dim_t;

typedef struct {
    unsigned fps, width, height;
} VidioHeader;

typedef struct {
    unsigned char red, green, blue;
} VidioPixel;

/*
 * internal vidio lib function, dropin replacement for fdprintf
 * as its not a POSIX function
 */
static void
vidio_fdprintf(const int fd, const size_t buf_size, const char *fmt, ...) {
    char *buffer;
    int write_count;
    va_list va;

    if (!(buffer = malloc(buf_size)))
        return;

    va_start(va, fmt);
    write_count = vsprintf(buffer, fmt, va);
    va_end(va);

    if (write_count > 0)
        write(fd, buffer, write_count);

    free(buffer);
}

/*
 * returns header information of a vidio (*.vi) file opened on the
 * `fd` file descriptor, returns NULL on failure and VidioHeader
 * struct on success
 */
const volatile VidioHeader *vidio_read_header(const int fd) {
    static VidioHeader header;

    lseek(fd, 2, 0); /* ignores the 2 magic bytes */
    return read(fd, &header, sizeof(header)) <= 0 ? NULL : &header;
}

/*
 * get the next frame of a vidio file, returns an array of VidioPixel structs,
 * which is allocated on the heap, so **NOTE: dont forget to free()**,
 * returns NULL if theres no other valid frame to read
 */
VidioPixel *vidio_get_next_frame(const int fd,
                                 const unsigned width,
                                 const unsigned height) {
    const vidio_dim_t dim = width * height;
    static vidio_dim_t dim_bytes;
    VidioPixel *frame = malloc((dim_bytes = dim * sizeof(*frame)));

    /* should wrap around to a very large number if returns -1 */
    if (!frame || (unsigned long)read(fd, frame, dim_bytes) != dim_bytes) {
        free(frame); /* malloc returns NULL on error and free() ignores NULL */
        return NULL;
    }

    return frame;
}

/*
 * convert a frame to a ppm file, all output goes to outfd,
 * returns 0 on success and -1 on failure
 */
short vidio_render_frame_to_ppm(const int outfd,
                                const unsigned width,
                                const unsigned height,
                                const VidioPixel *frame) {
    vidio_fdprintf(outfd, 1024, "P6\n%u %u\n255\n", width, height);
    return write(outfd, frame, width * height * sizeof(*frame)) == -1 ? -1 : 0;
}
#endif /* _VIDIO_C */
