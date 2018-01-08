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

#include "vendor_packet.h"

namespace avrcp {

std::unique_ptr<VendorPacketBuilder> VendorPacketBuilder::MakeBuilder(
    CType ctype, CommandPdu pdu, PacketType packet_type,
    std::unique_ptr<PacketBuilder> payload) {
  // If the payload size is greater than max uint16_t
  // the packet should be fragmented
  CHECK(payload->GetLength() > 0);
  CHECK(payload->GetLength() <= 0XFFFF);

  std::unique_ptr<VendorPacketBuilder> builder(
      new VendorPacketBuilder(ctype, pdu, packet_type));
  builder->payload_ = std::move(payload);

  return builder;
}

size_t VendorPacketBuilder::GetLength() const {
  return VendorPacket::kHeaderSize() + payload_->GetLength();
}

bool VendorPacketBuilder::Serialize(const std::shared_ptr<Packet>& pkt) {
  ReserveSpace(pkt, GetLength());

  // Push the standard avrcp headers
  AvrcpPacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers
  CHECK(payload_->GetLength() < 0xFFFF);
  PushHeader(pkt, payload_->GetLength());

  // Push the payload for the packet
  return payload_->Serialize(pkt);
}

void VendorPacketBuilder::PushHeader(const std::shared_ptr<Packet>& pkt,
                                     uint16_t parameter_length) {
  PushCompanyId(pkt, BLUETOOTH_COMPANY_ID);
  AddPayloadOctets1(pkt, pdu_);
  AddPayloadOctets1(pkt, packet_type_);
  AddPayloadOctets2(pkt, base::ByteSwap(parameter_length));
}

bool VendorPacketBuilder::PushAttributeValue(const std::shared_ptr<Packet>& pkt,
                                             const AttributeEntry& entry) {
  AddPayloadOctets4(pkt, base::ByteSwap(entry.first));
  uint16_t character_set = 0x006a;  // UTF-8
  AddPayloadOctets2(pkt, base::ByteSwap(character_set));
  uint16_t value_length = entry.second.length();
  AddPayloadOctets2(pkt, base::ByteSwap(value_length));
  for (int i = 0; i < value_length; i++) {
    AddPayloadOctets1(pkt, entry.second[i]);
  }

  return true;
}

uint32_t VendorPacket::GetCompanyId() const {
  return PullCompanyId(get_begin() + AvrcpPacket::kHeaderSize());
}

uint8_t VendorPacket::GetCommandPdu() const {
  return *(get_begin() + AvrcpPacket::kHeaderSize() + static_cast<size_t>(3));
}

uint8_t VendorPacket::GetPacketType() const {
  return *(get_begin() + AvrcpPacket::kHeaderSize() + static_cast<size_t>(4));
}

uint16_t VendorPacket::GetParameterLength() const {
  Iterator it = get_begin() + static_cast<size_t>(8);
  // Swap to little endian
  return base::ByteSwap(it.extract<uint16_t>());
}

std::string VendorPacket::ToString() const {
  std::stringstream ss;
  ss << "VendorPacket: " << std::endl;
  ss << "  └ cType = " << loghex(GetCType()) << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << loghex(GetOpcode()) << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << loghex(GetCommandPdu()) << std::endl;
  ss << "  └ PacketType = " << loghex(GetPacketType()) << std::endl;
  ss << "  └ Parameter Length = " << loghex(GetParameterLength()) << std::endl;
  ss << "  └ Payload =";
  for (auto it = get_begin(); it != get_end(); it++) {
    ss << " " << loghex(*it);
  }
  ss << std::endl;
  return ss.str();
}

}  // namespace avrcp