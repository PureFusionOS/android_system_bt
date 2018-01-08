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

#include <base/logging.h>
#include <base/macros.h>
#include <iostream>

#include "iterator.h"
#include "packet.h"
#include "packet_builder.h"

#include "avrcp_common.h"

namespace avrcp {

class AvrcpPacketBuilder : public PacketBuilder {
 public:
  static std::unique_ptr<AvrcpPacketBuilder> MakeBuilder(
      CType cType, uint8_t subunit_type, uint8_t subunit_id, Opcode opcode,
      std::unique_ptr<PacketBuilder> packet);

  virtual size_t GetLength() const override;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) override;

 protected:
  CType c_type_;
  uint8_t subunit_type_ : 5;
  uint8_t subunit_id_ : 3;
  Opcode opcode_;
  std::unique_ptr<PacketBuilder> payload_;

  void PushHeader(const std::shared_ptr<Packet>& pkt);
  bool PushCompanyId(const std::shared_ptr<Packet>& pkt, uint32_t company_id);

  AvrcpPacketBuilder(CType type, uint8_t subunit_type, uint8_t subunit_id,
                     Opcode opcode)
      : c_type_(type),
        subunit_type_(subunit_type),
        subunit_id_(subunit_id),
        opcode_(opcode){};
};

class AvrcpPacket : public Packet {
 public:
  virtual ~AvrcpPacket() = default;

  // Returns nullptr if the packet is poorly formatted
  // static std::shared_ptr<const AvrcpPacket> Parse(AvctpPacket);
  static std::shared_ptr<AvrcpPacket> Parse(std::shared_ptr<Packet> pkt);

  /**
   * Avrcp Packet Layout
   *   CType c_type_;
   *   uint8_t subunit_type_ : 5;
   *   uint8_t subunit_id_ : 3;
   *   Opcode opcode_;
   *   uint8_t[] payload_;
   */
  static constexpr size_t kHeaderSize() { return 3; };

  // Getter functions
  virtual uint8_t GetCType() const;
  virtual uint8_t GetSubunitType() const;
  virtual uint8_t GetSubunitId() const;
  virtual uint8_t GetOpcode() const;

  // Overloaded functions
  virtual std::string ToString() const override;

 protected:
  using Packet::Packet;

 private:
  // You will not pass Avrcppackets around, only builders and parsers.
  virtual std::pair<size_t, size_t> GetPayloadIndecies() const;
  DISALLOW_COPY_AND_ASSIGN(AvrcpPacket);
};

}  // namespace avrcp