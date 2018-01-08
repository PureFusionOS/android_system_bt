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

#pragma once

#include "avrcp_packet.h"

namespace avrcp {

class VendorPacketBuilder : public AvrcpPacketBuilder {
 public:
  static std::unique_ptr<VendorPacketBuilder> MakeBuilder(
      CType ctype, CommandPdu pdu, PacketType packet_type,
      std::unique_ptr<PacketBuilder> payload);

  virtual size_t GetLength() const override;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) override;

 protected:
  CommandPdu pdu_;
  PacketType packet_type_;
  uint16_t param_length_;

  void PushHeader(const std::shared_ptr<Packet>& pkt,
                  uint16_t parameter_length);

  // Helper function used a couple other AVRCP packet builders
  bool PushAttributeValue(const std::shared_ptr<Packet>& pkt,
                          const AttributeEntry& entry);

  VendorPacketBuilder(CType ctype, CommandPdu pdu, PacketType packet_type)
      : AvrcpPacketBuilder(ctype, 0x09, 0x00, Opcode::VENDOR),
        pdu_(pdu),
        packet_type_(packet_type){};
};

class VendorPacket : public AvrcpPacket {
 public:
  using AvrcpPacket::AvrcpPacket;

  virtual ~VendorPacket() = default;

  /**
   * Avrcp Vendor Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t parameter_length;
   *   uint8_t[] payload;
   *
   */
  static constexpr size_t kHeaderSize() {
    return AvrcpPacket::kHeaderSize() + 7;
  };

  virtual uint32_t GetCompanyId() const;
  virtual uint8_t GetCommandPdu() const;
  virtual uint8_t GetPacketType() const;
  virtual uint16_t GetParameterLength() const;

  virtual std::string ToString() const override;
};

}  // namespace avrcp