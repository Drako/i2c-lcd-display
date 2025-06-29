#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#include <pico/rand.h>

int _getentropy(void *const buf, size_t const buflen) { // NOLINT(*-reserved-identifier)
    if (buflen > 256u) {
        errno = EINVAL;
        return -1;
    }

    for (size_t i = 0; i < buflen; ++i) {
        ((uint8_t *) buf)[i] = get_rand_32() & 0xFF;
    }
    return 0;
}
