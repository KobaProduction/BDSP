#ifndef BDSP_BDSP_H
#define BDSP_BDSP_H

#include "./packet.h"
#include "./encoders/cobs/writer.h"
#include "./encoders/cobs/reader.h"
#include "./сhecksums/crc.h"

#define DEFAULT_MAX_PACKET_SIZE 65535

enum BDSP_MODE {BDSP_DUPLEX, BDSP_SENDER, BDSP_RECEIVER};

enum bdsp_status_t {
    BDSP_OK,
    BDSP_WRITER_NOT_INSTALLED,
    BDSP_CONFIG_NOT_INSTALLED,
    BDSP_CONFIG_ALREADY_INSTALLED,
    BDSP_ERROR_STATUS_COBS_WRITER,
    BDSP_EXCESS_SIZE_PACKET, // Data packet size exceeded.
    BDSP_INVALID_MAXIMUM_PACKET_SIZE
};

class BDSP {
public:
    explicit BDSP(BDSP_MODE mode);
    ~BDSP();
    bdsp_status_t set_writer(void (*writer_ptr)(uint8_t *data_ptr, size_t size));
    bdsp_status_t set_config(cobs_config_t config);
    bdsp_status_t set_max_packet_size(uint16_t max_size);
    void tick();

    bdsp_status_t send_data(uint8_t id, uint8_t *data, size_t size);
    bdsp_status_t send_packet(Packet &packet);
private:
    BDSP_MODE mode = BDSP_DUPLEX;
    uint16_t max_packet_size = DEFAULT_MAX_PACKET_SIZE;
    void (*writer_h)(uint8_t *data_ptr, size_t size) = nullptr;
    COBSWriter *writer;
    COBSReader *reader;
};

#endif //BDSP_BDSP_H
