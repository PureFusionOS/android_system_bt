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

#define LOG_TAG "bt_avrcp_thisworks"

#include <base/logging.h>

#include "avrcp_packet.h"

namespace avrcp {

std::unique_ptr<AvrcpPacketBuilder> AvrcpPacketBuilder::MakeBuilder(
    CType type, uint8_t subunit_type, uint8_t subunit_id, Opcode opcode,
    std::unique_ptr<PacketBuilder> payload) {
  std::unique_ptr<AvrcpPacketBuilder> builder =
      std::unique_ptr<AvrcpPacketBuilder>(
          new AvrcpPacketBuilder(type, subunit_type, subunit_id, opcode));

  builder->payload_ = std::move(payload);

  return builder;
}

size_t AvrcpPacketBuilder::GetLength() const {
  // The size of the header for an AvrcpPacket is 3
  return payload_->GetLength() + AvrcpPacket::kHeaderSize();
}

bool AvrcpPacketBuilder::Serialize(const std::shared_ptr<Packet>& pkt) {
  ReserveSpace(pkt, GetLength());

  // Push the header for the packet
  PushHeader(pkt);

  // Push the payload for the packet
  return payload_->Serialize(pkt);
}

void AvrcpPacketBuilder::PushHeader(const std::shared_ptr<Packet>& pkt) {
  AddPayloadOctets1(pkt, c_type_);
  AddPayloadOctets1(pkt, (subunit_type_ << 3) | subunit_id_);
  AddPayloadOctets1(pkt, opcode_);
}

bool AvrcpPacketBuilder::PushCompanyId(const std::shared_ptr<Packet>& pkt,
                                       uint32_t company_id) {
  company_id = base::ByteSwap(company_id);
  for (int i = 0; i < 3; i++) {
    company_id >>= 8;
    AddPayloadOctets1(pkt, company_id & 0xFF);
  }

  return true;
}

std::shared_ptr<AvrcpPacket> AvrcpPacket::Parse(std::shared_ptr<Packet> pkt) {
  return std::shared_ptr<AvrcpPacket>(new AvrcpPacket(pkt));
}

uint8_t AvrcpPacket::GetCType() const { return *get_begin() & 0x0F; }

uint8_t AvrcpPacket::GetSubunitType() const {
  return *(get_begin() + static_cast<size_t>(1)) >> 3;
}

uint8_t AvrcpPacket::GetSubunitId() const {
  return *(get_begin() + static_cast<size_t>(1)) & 0b00000111;
}

uint8_t AvrcpPacket::GetOpcode() const {
  return *(get_begin() + static_cast<size_t>(2));
}

std::string AvrcpPacket::ToString() const {
  std::stringstream ss;
  ss << "Avrcp::AvrcpPacket: " << std::endl;
  ss << "  └ cType = " << loghex(GetCType()) << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << loghex(GetOpcode()) << std::endl;
  ss << "  └ Payload =";
  for (auto it = get_begin() + static_cast<size_t>(3); it != get_end(); it++) {
    ss << " " << loghex(*it);
  }
  ss << std::endl;

  return ss.str();
}

std::pair<size_t, size_t> AvrcpPacket::GetPayloadIndecies() const {
  return std::pair<size_t, size_t>(packet_start_index_ + 3, packet_end_index_);
}

}  // namespace avrcp