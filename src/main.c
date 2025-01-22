// Copyright (c) 1ukidev <leo.monteiro06@live.com>. Licensed under the GPL-3.0 Licence.
// See the LICENCE file in the repository root for full licence text.

#define FUSE_USE_VERSION 31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <fuse.h>

static int leofs_getattr(const char *path, struct stat *st);
static int leofs_readdir(const char *path, void *buffer, 
                         fuse_fill_dir_t filler, off_t offset, 
                         struct fuse_file_info *fi);
static int leofs_open(const char *path, struct fuse_file_info *fi);
static int leofs_read(const char *path, char *buffer, 
                      size_t size, off_t offset, 
                      struct fuse_file_info *fi);
static struct fuse_operations operations;

static int leofs_getattr(const char *path, struct stat *st)
{
    printf("[getattr] Atributos de %s requisitado\n", path);

    st->st_uid = getuid();
    st->st_gid = getgid();

    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if (strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }

    return 0;
}

static int leofs_readdir(const char *path, void *buffer, 
                         fuse_fill_dir_t filler, off_t offset, 
                         struct fuse_file_info *fi)
{
    printf("[readdir] Pegando a lista de arquivos de %s\n", path);

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if (strcmp(path, "/") == 0)
        filler(buffer, "poema", NULL, 0);

    return 0;
}

static int leofs_open(const char *path, struct fuse_file_info *fi)
{
    printf("[open] Tentando abrir %s\n", path);

    if (strcmp(path, "/poema") != 0)
        return -ENOENT;

    return 0;
}

static int leofs_read(const char *path, char *buffer, 
                      size_t size, off_t offset, 
                      struct fuse_file_info *fi)
{
    printf("[read] Tentando ler %s, %ld, %zu\n", path, offset, size);

    char *text = NULL;

    if (strcmp(path, "/poema") == 0)
        text = "Rosas são vermelhas\nVioletas são azuis\n";
    else
        return -ENOENT;
    
    size_t len = strlen(text);

    if (offset >= len)
        return 0;

    if (offset + size > len)
        size = len - offset;

    memcpy(buffer, text + offset, size);

    return size;
}

// https://libfuse.github.io/doxygen/structfuse__operations.html
static struct fuse_operations operations = {
    .getattr = leofs_getattr,
    .readdir = leofs_readdir,
    .open    = leofs_open,
    .read    = leofs_read
};

void print_help(void)
{
    fputs("Uso: ./leofs <ponto_de_montagem>\n", stderr);
    fputs("Para debugar: ./leofs -f <ponto_de_montagem>\n", stderr);
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        print_help();
        return EXIT_FAILURE;
    }

    return fuse_main(argc, argv, &operations, NULL);
}
