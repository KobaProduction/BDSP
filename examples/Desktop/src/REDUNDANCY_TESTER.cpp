#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <BDSP/strategies/byte_stuffing/write.h>
#include <BDSP/strategies/cobs/write.h>

using namespace BDSP::strategies;
using namespace BDSP::strategies::cobs;
using namespace BDSP::strategies::byte_stuffing;

double calc_redundancy(size_t data_size, size_t encoded_data_size, size_t decimal_places = 3) {
    double factor = std::pow(10.0f, decimal_places);
    float value = float(int64_t(encoded_data_size) - int64_t(data_size)) / float(data_size) * 100;
    return std::round(value * factor) / factor;
}

int main(int argc, char *argv[]) {
    COBSWriteStrategy cobs_def_write_strategy;
    COBSGSEWriteStrategy cobs_gse_write_strategy;
    COBSZPEWriteStrategy cobs_zpe_write_strategy;
    PPPWriteStrategy ppp_write_strategy;

    std::vector<IStreamWriteStrategy *> strategies = {
        &cobs_def_write_strategy, &cobs_gse_write_strategy, &cobs_zpe_write_strategy, &ppp_write_strategy};

    std::vector<size_t> wrote_bytes;
    wrote_bytes.resize(strategies.size(), 0);

    for (int i = 0; i < strategies.size(); ++i) {
        wrote_bytes[i] = 0;
        strategies[i]->init(
            [](uint8_t, void *ctx) {
                auto &counter = *reinterpret_cast<size_t *>(ctx);
                counter++;
            },
            nullptr,
            &wrote_bytes[i]);
    }

    std::string filename = "file.bin";
    if (argc == 2) {
        filename = argv[1];
    }

    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);

    if (not file.is_open()) {
        printf("ERROR: File \"%s\" not opened or not exist!\n", filename.c_str());
        return -1;
    }

    file.seekg(0, std::ios::beg);
    const uint64_t data_size = uint64_t(file.tellg());

    printf("Open file \"%s\". Size %llu bytes.\n", filename.c_str(), data_size);

    std::vector<char> buffer;
    buffer.resize(data_size);
    file.read(buffer.data(), std::streamsize(data_size));

    for (auto strategy: strategies) {
        for (auto byte: buffer) {
            strategy->write(byte);
        }
        strategy->finish();
    }
    printf("Encoded bytes size    - ");
    printf("COBS/Default: %llu, COBS/GSE: %llu, COBS/ZPE: %llu, PPP: %llu\n",
        wrote_bytes[0],
        wrote_bytes[1],
        wrote_bytes[2],
        wrote_bytes[3]);
    printf("Redundancy percentage - ");
    printf("COBS/Default: %.3f%%, COBS/GSE: %.3f%%, COBS/ZPE: %.3f%%, PPP: %.3f%%\n",
        calc_redundancy(data_size, wrote_bytes[0]),
        calc_redundancy(data_size, wrote_bytes[1]),
        calc_redundancy(data_size, wrote_bytes[2]),
        calc_redundancy(data_size, wrote_bytes[3])
    );

    return 0;
}
