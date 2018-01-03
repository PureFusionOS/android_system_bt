/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "packet.h"
#include "packet_test_helper.h"

namespace packet {

class PacketImpl : public Packet {
 public:
  using Packet::Packet;  // Inherit constructors

  virtual std::string ToString() const {
    std::stringstream ss;
    ss << "TestPacket: Start = " << packet_start_index_
       << " : End = " << packet_end_index_ << std::endl;
    ss << "  └ Payload =";
    for (auto it = begin(); it != end(); it++) {
      ss << " " << loghex(*it);
    }
    ss << std::endl;

    return ss.str();
  };

  virtual std::pair<size_t, size_t> GetPayloadIndecies() const {
    return std::pair<size_t, size_t>(packet_start_index_, packet_end_index_);
  }
};

using TestPacket = TestPacketType<PacketImpl>;

}  // namespace packet