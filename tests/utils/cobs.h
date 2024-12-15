#ifndef BDSP_TESTS_UTILS_COBS_H
#define BDSP_TESTS_UTILS_COBS_H

void UnStuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst);

void StuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst);

#endif //BDSP_TESTS_UTILS_COBS_H
