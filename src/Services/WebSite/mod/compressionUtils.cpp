#include "contentUtils.hpp"
// #include <zlib.h>

// std::string decompress(const std::string& compressed) {
//     z_stream zs;
//     memset(&zs, 0, sizeof(zs));

//     if (inflateInit2(&zs, -MAX_WBITS) != Z_OK) {
//         throw std::runtime_error("inflateInit2 failed");
//     }

//     zs.next_in = (Bytef*)compressed.data();
//     zs.avail_in = compressed.size();

//     int ret;
//     char outbuffer[32768];
//     std::string outstring;

//     do {
//         zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//         zs.avail_out = sizeof(outbuffer);

//         ret = inflate(&zs, 0);

//         if (outstring.size() < zs.total_out) {
//             outstring.append(outbuffer, zs.total_out - outstring.size());
//         }

//     } while (ret == Z_OK);

//     inflateEnd(&zs);

//     if (ret != Z_STREAM_END) {
//         throw std::runtime_error("decompression failed");
//     }

//     return outstring;
// }

Buffer send_websocket_message(const std::string& message)
{
    Buffer databuffer;
    size_t message_length = message.length();
    int num_bytes = 1;
    uint8_t len_bytes[4];

    if (message_length <= 125) {
        len_bytes[0] = 0x81;
        len_bytes[1] = message_length;
        num_bytes = 2;
    } else if (message_length <= 65535) {
        len_bytes[0] = 0x81;
        len_bytes[1] = 126;
        len_bytes[2] = (message_length >> 8) & 0xff;
        len_bytes[3] = message_length & 0xff;
        num_bytes = 4;
    } else {
        len_bytes[0] = 0x81;
        len_bytes[1] = 127;
        for (int i = 0; i < 8; i++) {
            len_bytes[2 + i] = (message_length >> (56 - i*8)) & 0xff;
        }
        num_bytes = 10;
    }

    databuffer.append(len_bytes, num_bytes);
    databuffer << message;

    return databuffer;
}

// Buffer send_websocket_message(const std::string& message)
// {
// 	Buffer	databuffer;
//     char* data = new char[message.length() + 2];

//     data[0] = 0x81;
//     data[1] = message.length();
//     memcpy(data + 2, message.c_str(), message.length());
// 	databuffer << data;
//     delete[] data;
// 	return (databuffer);
// }

std::string	receive_websocket_message(uint8_t const *data, uint64_t const len) {
    std::string message;

    // Parse the WebSocket message header
    // uint8_t opcode = data[0] & 0x0f;
    uint64_t payload_len = data[1] & 0x7f;
    bool masked = (data[1] & 0x80) != 0;

    // Read the WebSocket message payload
    uint8_t* payload_data = NULL;
    uint64_t payload_offset = 2;
    if (payload_len == 126) {
        // Read 2 additional bytes for the extended payload length
        if (len < payload_offset + 2) {
            return "";
        }
        payload_len = ntohs(*(uint16_t*)(data + payload_offset));
        payload_offset += 2;
    } else if (payload_len == 127) {
        // Read 8 additional bytes for the extended payload length
        if (len < payload_offset + 8) {
            return "";
        }
        payload_len = be64toh(*(uint64_t*)(data + payload_offset));
        payload_offset += 8;
    }

    // Read the masked payload data
    uint8_t masking_key[4];
    if (masked) {
        if (len < payload_offset + 4) {
            return "";
        }
        memcpy(masking_key, data + payload_offset, 4);
        payload_offset += 4;
    }

    // Copy the payload data
    if (len < payload_offset + payload_len) {
        return "";
    }
    payload_data = new uint8_t[payload_len];
    memcpy(payload_data, data + payload_offset, payload_len);

    // Unmask the payload data if necessary
    if (masked) {
        for (uint64_t i = 0; i < payload_len; i++) {
            payload_data[i] ^= masking_key[i % 4];
        }
    }

    // Extract the message payload
    message.assign(reinterpret_cast<const char*>(payload_data), payload_len);
    delete[] payload_data;

    return message;
}