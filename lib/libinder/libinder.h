#ifndef LIBINDER_H
#define LIBINDER_H

#include <stddef.h>
#include <stdint.h>

typedef struct libinder libinder_t;

/**
 * Callback function type for handling incoming Binder RPC calls.
 * 
 * @param code     User-defined transaction code/command ID.
 * @param in_data  Pointer to received payload buffer.
 * @param in_len   Length of received payload in bytes.
 * @param out_buf  Buffer provided to write reply data back to caller.
 * @param out_len  In: max size of out_buf. Out: actual bytes written to out_buf.
 * @param user_data Opaque pointer passed during binder_loop.
 * @return 0 on success, or non-zero status code.
 */
typedef int (*binder_handler_fn)(uint32_t code, const void *in_data, size_t in_len, 
                                 void *out_buf, size_t *out_len, void *user_data);

/**
 * Open and initialize the Binder device.
 * 
 * @param devpath   Path to binder device (e.g., "/dev/binderfs/binder" or "/dev/binder"). 
 *                  Pass NULL for auto-detection.
 * @param map_size  Memory mapping size in bytes (e.g. 128KB). Pass 0 for default 128KB.
 * @return Handle to libinder_t on success, NULL on failure.
 */
libinder_t *libinder_open(const char *devpath, size_t map_size);

/**
 * Register current process as the Context Manager (ServiceManager / Handle 0).
 */
int libinder_set_context_manager(libinder_t *b);

/**
 * Retrieve the underlying file descriptor for polling or non-blocking I/O.
 */
int libinder_get_fd(const libinder_t *b);

/**
 * Send a synchronous transaction call to a target handle.
 * 
 * @param handle   Target binder handle (0 for Context Manager).
 * @param code     User-defined transaction code.
 * @param in_data  Payload bytes to send.
 * @param in_len   Length of payload bytes.
 * @param out_data Buffer to store response. Pass NULL if reply data isn't needed.
 * @param out_len  In: Max buffer capacity. Out: Received response length.
 * @return 0 on success, -1 on error.
 */
int libinder_call(libinder_t *b, uint32_t handle, uint32_t code, 
                const void *in_data, size_t in_len, 
                void *out_data, size_t *out_len);

/**
 * Enter the event loop to serve incoming Binder calls. (Blocks until error or close)
 */
int libinder_loop(libinder_t *b, binder_handler_fn handler, void *user_data);

/**
 * Unmap memory and close binder device descriptor.
 */
void libinder_close(libinder_t *b);

#endif // LIBINDER_H
