#pragma once

#if defined(PLATFORMIO) or defined(ARDUINO)
#include "BDSP/types.h"
#include "BDSP/packet.h"
#include "BDSP/receiver.h"
#include "BDSP/transmitter.h"
#include "BDSP/transceiver.h"
#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/writer.h"
#include "BDSP/streams/PPP/reader.h"
#include "BDSP/streams/PPP/writer.h"
#else
#include "BDSP/export.h"
namespace BDSP {

//BDSP_EXPORT int add(int a, int b);
} // namespace BDSP
#endif
