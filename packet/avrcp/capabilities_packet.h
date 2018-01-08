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

#include <set>

#include "vendor_packet.h"

namespace avrcp {

class GetCapabilitiesRequestBuilder : public VendorPacketBuilder {
 public:
  static std::unique_ptr<GetCapabilitiesRequestBuilder> MakeBuilder(
      Capability capability);

  virtual size_t GetLength() const override;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) override;

 protected:
  Capability capability_;

  GetCapabilitiesRequestBuilder(Capability capability)
      : VendorPacketBuilder(CType::STATUS, CommandPdu::GET_CAPABILITIES,
                            PacketType::SINGLE),
        capability_(capability){};
};

class GetCapabilitiesRequest : public VendorPacket {
 public:
  virtual ~GetCapabilitiesRequest() = default;

  using VendorPacket::VendorPacket;

  static std::shared_ptr<GetCapabilitiesRequest> Parse(
      std::shared_ptr<Packet> pkt);
  /**
   *  Get Capabilities Response Packet Layout
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
   *   GetCapabilitiesRequestPacket:
   *     uint8_t capability_requested:
   */
  static constexpr size_t kHeaderSize() {
    return VendorPacket::kHeaderSize() + 1;
  };

  virtual uint8_t GetCapabilityRequested() const;
  virtual std::string ToString() const override;
};

class GetCapabilitiesResponseBuilder : public VendorPacketBuilder {
 public:
  // TODO: Unique Builder names
  static std::unique_ptr<GetCapabilitiesResponseBuilder> MakeCompanyIdBuilder(
      uint32_t company_id_element);

  static std::unique_ptr<GetCapabilitiesResponseBuilder>
  MakeEventsSupportedBuilder(Event event);

  GetCapabilitiesResponseBuilder* AddCompanyId(uint32_t company_id);
  GetCapabilitiesResponseBuilder* AddEvent(Event event);

  virtual size_t GetLength() const override;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) override;

 private:
  Capability capability_;
  std::set<uint32_t> elements_;

  GetCapabilitiesResponseBuilder(Capability capability)
      : VendorPacketBuilder(CType::STABLE, CommandPdu::GET_CAPABILITIES,
                            PacketType::SINGLE),
        capability_(capability){};
};

// TODO: Add Parser and implement getters
class GetCapabilitiesResponse : public VendorPacket {
 public:
  /**
   * Get Capabilities Response Packet Layout
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
   *   GetCapabilitiesRequestPacket;
   *     uint8_t capability_requested;
   *     uint16_t capability_count;
   *     union {
   *        uint8_t event_supported;
   *        uint8_t company_id[3];
   *     } capability_array[];
   */
  static constexpr size_t kHeaderSize() {
    return VendorPacket::kHeaderSize() + 2;
  };

  // TODO: Implement these for AVRCP Controller
  // virtual uint8_t GetCapabilityReturned() const;
  // virtual uint8_t GetCapabilityCount() const;
  // virtual std::vector<Event> GetEventsSupported() const;
  // virtual std::vector<uint32_t> GetCompanyIds() const;

  virtual std::string ToString() const override;
};

}  // namespace avrcp