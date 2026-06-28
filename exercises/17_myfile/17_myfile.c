#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
        case ET_NONE:
            type_str = "Unknown";
            break;
        case ET_REL:
            type_str = "Relocatable";
            break;
        case ET_EXEC:
            type_str = "Executable";
            break;
        case ET_DYN:
            type_str = "Shared Object/PIE";
            break;
        default:
            type_str = "Unknown";
            break;
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(void) {
    const char *paths[] = {"./17_myfile.o", "./17_myfile"};

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        int fd = open(paths[i], O_RDONLY);
        if (fd < 0) {
            continue;
        }
        Elf64_Ehdr ehdr;
        ssize_t n = read(fd, &ehdr, sizeof(ehdr));
        close(fd);
        if (n != (ssize_t)sizeof(ehdr) || memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
            printf("ELF Type: Unknown (0x0)\n");
            continue;
        }
        print_elf_type(ehdr.e_type);
    }
    return 0;
}
