#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t version_be;
    uint16_t length_be;
    uint8_t flags_raw;
} proto_header_raw_t;

typedef struct {
    unsigned ver_major : 4;
    unsigned ver_minor : 4;
    uint16_t length;
    unsigned flags : 8;
} proto_header_bits_t;

#pragma pack(pop)

static uint16_t be16_to_cpu(uint16_t be) {
    return (uint16_t)((be >> 8) | (be << 8));
}

int main(void) {
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    proto_header_raw_t raw = {0};
    memcpy(&raw, stream, sizeof(raw));

    uint16_t version = be16_to_cpu(raw.version_be);
    unsigned ver_major = (unsigned)((version >> 4) & 0x0Fu);
    unsigned ver_minor = (unsigned)(version & 0x0Fu);
    uint16_t length = be16_to_cpu(raw.length_be);
    unsigned flags = (unsigned)(raw.flags_raw & 0x1Fu);

    proto_header_bits_t view = {0};
    view.ver_major = ver_major;
    view.ver_minor = ver_minor;
    view.length = length;
    view.flags = flags;

    printf("version:%u.%u, length:%u, flags:0x%02X\n", view.ver_major, view.ver_minor, view.length,
           view.flags & 0xFFu);

    return 0;
}
