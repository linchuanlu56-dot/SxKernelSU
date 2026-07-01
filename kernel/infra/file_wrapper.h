#ifndef SKS_FILE_WRAPPER_H
#define SKS_FILE_WRAPPER_H

#include <linux/file.h>
#include <linux/fs.h>

int sksu_install_file_wrapper(int fd);
void sksu_file_wrapper_init(void);

#endif // SKS_FILE_WRAPPER_H
