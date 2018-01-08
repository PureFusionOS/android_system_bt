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

#include <map>
#include "vendor_packet.h"

namespace avrcp {

// TODO: GetElementAttributesRequestBuilder Implementation

class GetElementAttributesRequest : public VendorPacket {
 public:
  virtual ~GetElementAttributesRequest() = default;

  using VendorPacket::VendorPacket;

  static std::shared_ptr<GetElementAttributesRequest> Parse(
      std::shared_ptr<Packet> pkt);

  /**
   *  Register Notificaiton Request Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t param_length;
   *   RegisterNotificationRequestPacket:
   *     uint64_t identifier;
   *     uint8_t number_of_attributes;
   *     uint32_t attributes_requested[];
   */

  // TODO: This doesn't make much sense here, this packet type doesn't really
  // have a header
  static constexpr size_t kHeaderSize() {
    return VendorPacket::kHeaderSize() + 9;
  }

  virtual uint64_t GetIdentifier() const;
  virtual std::vector<Attribute> GetAttributesRequested() const;

  virtual std::string ToString() const override;
};

class GetElementAttributesResponseBuilder : public VendorPacketBuilder {
 public:
  static std::unique_ptr<GetElementAttributesResponseBuilder> MakeBuilder();

  GetElementAttributesResponseBuilder* AddAttributeEntry(AttributeEntry entry);
  GetElementAttributesResponseBuilder* AddAttributeEntry(Attribute attribute,
                                                         std::string value);

  virtual size_t GetLength() const override;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) override;

 private:
  std::map<Attribute, std::string> entries_;

  GetElementAttributesResponseBuilder()
      : VendorPacketBuilder(CType::STABLE, CommandPdu::GET_ELEMENT_ATTRIBUTES,
                            PacketType::SINGLE){};
};

// TODO: GetElementAttributesResponse Parser Implementation

}  // namespace avrcp