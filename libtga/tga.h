/*
 * Copyright (c) 2013, 2014-2016
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#ifndef TGA_H_
#define TGA_H_

#include <stdio.h>
#include <inttypes.h>

/** @file */
/** @defgroup libtga libtga */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup libtga
 * @brief Basic library to parse Truevision TGA (TARGA) files
 *
 * @{ */

/**
 * @enum tga_status Status
 * @brief TGA status */
typedef enum {
        TGA_FILE_OK = 0, /**< Operation is successful */
        TGA_FILE_UNKNOWN_FORMAT = -1, /**< TGA header could not be parsed in file */
        TGA_FILE_CORRUPTED = -2, /**< TGA header is corrupted in file */
        TGA_FILE_NOT_SUPPORTED = -3, /**< TGA feature is unsupported in file*/
        TGA_MEMORY_ERROR = -4, /**< TGA memory error (ran out of memory) */
        TGA_MEMORY_UNALIGNMENT_ERROR = -5 /**< TGA address is unaligned (not on a 2 or 4-byte boundary) */
} tga_status;

/**
 * @brief TGA */
typedef struct {
        const void *tga_context; /**< Context */

        /**
         * @brief User modifiable options
         *
         * The options must be set prior to calling tga_read() */
         */
        struct {
                uint32_t transparent_pixel; /**< Determine the transparent pixel color (in RGB555) */
                bool msb; /**< Enable setting MSB bit for transparent pixel color */
        } tga_options;
} tga;

/**
 * @brief Read TGA from a file buffer.
 *
 * Parses buffer and allocates a private TGA context.
 *
 * @param[in] tga TGA opaque pointer
 * @param[in] file Read-only pointer to TGA file buffer
 *
 * @return @ref tga_status.TGA_FILE_OK if reading TGA file buffer was successful.
 * @return @ref tga_status.TGA_FILE_CORRUPTED if TGA header is corrupted.
 * @return @ref tga_status.TGA_FILE_NOT_SUPPORTED if a feature set in the TGA is not
 *         supported.
 */
tga_status tga_read(tga *tga, const uint8_t *file);

/**
 * @brief Decode image as a tiles (cells) and write each tile sequentially to memory.
 *
 * @note
 *   - Function tga_read() must be called prior to calling this function.
 *   - The image dimensions must be multiples of 8 as the Saturn's smallest tile (cell) dimension is 8x8 pixels.
 *   - The pointer destination can point to another buffer or VDP1/VDP2 VRAM.
 *
 * The entire image buffer parsed from the TGA file buffer is subdivided into
 * cells and sequentially written to memory, starting from the top-left corner
 * of the image buffer.
 *
 * @param[in] tga TGA opaque pointer
 * @param[out] dst Pointer to buffer to write to
 *
 * @return @ref tga_status.TGA_FILE_OK if reading TGA file buffer was successful.
 * @return @ref tga_status.TGA_FILE_CORRUPTED if TGA header is corrupted.
 * @return @ref tga_status.TGA_FILE_NOT_SUPPORTED if a feature set in the TGA is not
 *         supported.
 */
tga_status tga_image_decode_tiled(const tga *tga, void *dst);

/**
 * @brief Decode image and write raw buffer of color pixels to memory.
 *
 * @note
 *   - Function tga_read() must be called prior to calling this function.
 *   - The pointer destination can point to another buffer or VDP1/VDP2 VRAM.
 *
 * The entire image is treated as a bitmap. Specifically, each pixel (whether
 * indexed or RGB) is sequentially written to memory.
 *
 * @param[in] tga TGA opaque pointer
 * @param[out] dst Pointer to buffer to write to
 *
 * @return @ref tga_status.TGA_FILE_OK if reading TGA file buffer was successful.
 * @return @ref tga_status.TGA_FILE_CORRUPTED if TGA header is corrupted.
 * @return @ref tga_status.TGA_FILE_NOT_SUPPORTED if a feature set in the TGA is not
 *         supported.
 */
tga_status tga_image_decode(const tga *tga, void *dst);

/**
 * @brief Decode color map (palette) and write to memory.
 *
 * @note
 *   - Function tga_read() must be called prior to calling this function.
 *   - The pointer destination can point to another buffer, CRAM, or VDP1 VRAM.
 *
 * The color map (palette) is extracted from the TGA file buffer in is converted
 * from BGR16 to RGB555.
 *
 * Depending on what options were set in @ref tga.tga_options, the MSB of
 * each color may or may not be set.
 *
 * @param[in] tga TGA opaque pointer
 * @param[out] dst Pointer to buffer to write to
 *
 * @return Status
 */
tga_status tga_cmap_decode(const tga *tga, uint16_t *dst);

/**
 * @brief Convert TGA status (@ref tga_status) to a string.
 *
 * @param[in] status Status value from @ref tga_status
 *
 * @return Read-only string equivalent of @ref tga_status member
 */
const char *tga_error_stringify(tga_status status);
/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TGA_H_ */
