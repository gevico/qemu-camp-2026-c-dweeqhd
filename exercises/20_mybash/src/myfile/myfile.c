#include "myfile.h"

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
        case ET_NONE:
            type_str = "Unknown (ET_NONE)";
            break;
        case ET_REL:
            type_str = "Relocatable (ET_REL)";
            break;
        case ET_EXEC:
            type_str = "Executable (ET_EXEC)";
            break;
        case ET_DYN:
            type_str = "Shared Object/PIE (ET_DYN)";
            break;
        case ET_CORE:
            type_str = "Core Dump (ET_CORE)";
            break;
        default:
            type_str = "Unknown";
            break;
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char *filename) {
    if (!filename) {
        return 1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror(filename);
        return 1;
    }

    Elf64_Ehdr ehdr;
    ssize_t n = read(fd, &ehdr, sizeof(ehdr));
    close(fd);
    if (n != (ssize_t)sizeof(ehdr) || memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        printf("ELF Type: Unknown (ET_NONE) (0x0)\n");
        return 1;
    }

    print_elf_type(ehdr.e_type);
    return 0;
}
