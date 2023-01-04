#include "../include/vidio.c"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define FRAMES_DIR     "frames/"
#define FRAMES_DIR_LEN 7

int main(int argc, char *argv[]) {
    unsigned pixel;
    int vi_fd, frame_fd, frame = 0, exit_code = 0;
    char frame_path[] = FRAMES_DIR "\0.ppm";
    const volatile VidioHeader *vi_header;
    VidioPixel *vi_frame;
    VidioPixel vi_frame_pixel;

    if (argc < 2) {
        fputs("please supply me with a *.vi file through arguments :)\n",
              stderr);
        return 1;
    }

    vi_fd = open(argv[1], O_RDONLY);

    if (vi_fd == -1) {
        fprintf(stderr, "failed to open(\"%s\", O_RDONLY): %s\n", argv[1],
                strerror(errno));
        return 2;
    }

    vi_header = vidio_read_header(vi_fd);

    printf("video information\n\n"
           "fps:    %u\n"
           "width:  %u\n"
           "height: %u\n",
           vi_header->fps, vi_header->width, vi_header->height);

    mkdir(FRAMES_DIR, 0700);

    while (++frame) {
        if ((vi_frame = vidio_get_next_frame(vi_fd, vi_header->width,
                                             vi_header->height)) == NULL) {
            puts("no (other) frames found");
            goto end;
        }

        frame_path[FRAMES_DIR_LEN] = 'A' + frame;

        frame_fd = open(frame_path, O_WRONLY | O_CREAT);
        if (vidio_render_frame_to_ppm(frame_fd, vi_header->width,
                                      vi_header->height, vi_frame) == -1) {
            fprintf(stderr,
                    "failed to export frame %u to ppm, are you sure %s is "
                    "fully removed ?\n",
                    frame - 1, FRAMES_DIR);

            free(vi_frame);
            close(frame_fd);

            exit_code = 3;
            goto end;
        }
        close(frame_fd);

        for (pixel = 0; pixel < vi_header->width * vi_header->height; ++pixel) {
            vi_frame_pixel = vi_frame[pixel];

            printf("frame %u pixel %u: rgb(%hhu, %hhu, %hhu)\n", frame - 1,
                   pixel, vi_frame_pixel.red, vi_frame_pixel.green,
                   vi_frame_pixel.blue);
        }

        free(vi_frame);
    }

end:
    close(vi_fd);
    return exit_code;
}
