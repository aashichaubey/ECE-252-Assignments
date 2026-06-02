/**
 * @file: pnginfo.c
 * @brief: pnginfo <file>
 *         Prints "<file>: W x H" for a valid PNG file.
 *         Prints "<file>: Not a PNG file" if signature is wrong.
 *         Prints CRC error info (pngcheck-style) if any chunk has bad CRC.
 *
 * Examples (from the lab handout):
 *   ./pnginfo WEEF_1.png
 *     -> WEEF_1.png: 450 x 229
 *   ./pnginfo Disguise.png
 *     -> Disguise.png: Not a PNG file
 *   ./pnginfo red-green-16x16-corrupted.png
 *     -> red-green-16x16-corrupted.png: 16 x 16
 *        IDAT chunk CRC error: computed 34324f1e, expected dc5f7b84
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab_png.h"
#include "crc.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <png_file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "%s: cannot open file\n", path);
        return 1;
    }

    /* --- Step 1: read the 8-byte signature and check with is_png() --- */
    U8 sig[PNG_SIG_SIZE];
    if (fread(sig, 1, PNG_SIG_SIZE, fp) != PNG_SIG_SIZE || !is_png(sig, PNG_SIG_SIZE)) {
        printf("%s: Not a PNG file\n", path);
        fclose(fp);
        return 0;
    }

    /* --- Step 2: extract width/height from the IHDR chunk --- */
    struct data_IHDR ihdr;
    if (get_png_data_IHDR(&ihdr, fp, PNG_SIG_SIZE, SEEK_SET) != 0) {
        printf("%s: Not a PNG file\n", path);
        fclose(fp);
        return 0;
    }
    printf("%s: %d x %d\n", path, get_png_width(&ihdr), get_png_height(&ihdr));

    /* --- Step 3: walk all chunks, verify CRC of each, report mismatches --- */
    /* Rewind back to right after the signature and parse chunks one by one. */
    if (fseek(fp, PNG_SIG_SIZE, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    while (1) {
        chunk_p ch = get_chunk(fp);
        if (ch == NULL) break;                 /* end-of-file or read error */

        U32 expected = get_chunk_crc(ch);
        U32 computed = calculate_chunk_crc(ch);

        if (computed != expected) {
            /* Print chunk type as 4 ASCII chars then the pngcheck-style msg */
            printf("%c%c%c%c chunk CRC error: computed %08x, expected %08x\n",
                   ch->type[0], ch->type[1], ch->type[2], ch->type[3],
                   computed, expected);
        }

        /* IEND marks the end of the PNG datastream */
        int is_iend = (memcmp(ch->type, "IEND", 4) == 0);
        free_chunk(ch);
        if (is_iend) break;
    }

    fclose(fp);
    return 0;
}
