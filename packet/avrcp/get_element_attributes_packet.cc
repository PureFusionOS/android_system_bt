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

#include <algorithm>

#include "get_element_attributes_packet.h"

namespace avrcp {

std::shared_ptr<GetElementAttributesRequest> GetElementAttributesRequest::Parse(
    std::shared_ptr<Packet> pkt) {
  return std::shared_ptr<GetElementAttributesRequest>(
      new GetElementAttributesRequest(pkt));
}

uint64_t GetElementAttributesRequest::GetIdentifier() const {
  auto it = get_begin() + VendorPacket::kHeaderSize();
  return it.extract<uint64_t>();
}

std::vector<Attribute> GetElementAttributesRequest::GetAttributesRequested()
    const {
  auto it = get_begin() + VendorPacket::kHeaderSize() + static_cast<size_t>(8);

  size_t number_of_attributes = it.extract<uint8_t>();

  std::vector<Attribute> attribute_list;

  for (size_t i = 0; i < number_of_attributes; i++) {
    attribute_list.push_back((Attribute)base::ByteSwap(it.extract<uint32_t>()));
  }

  return attribute_list;
}

std::string GetElementAttributesRequest::ToString() const {
  std::stringstream ss;
  ss << "RegisterNotificationPacket: " << std::endl;
  ss << "  └ cType = " << loghex(GetCType()) << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << loghex(GetOpcode()) << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << loghex(GetCommandPdu()) << std::endl;
  ss << "  └ PacketType = " << loghex(GetPacketType()) << std::endl;
  ss << "  └ Parameter Length = " << GetParameterLength() << std::endl;
  ss << "  └ Identifier = " << loghex(GetIdentifier()) << std::endl;

  auto attr_list = GetAttributesRequested();

  ss << "  └ Attribute List: Size: " << attr_list.size() << std::endl;
  for (auto it = attr_list.begin(); it != attr_list.end(); it++) {
    ss << "      └ " << loghex((uint32_t)(*it)) << std::endl;
  }
  ss << std::endl;

  return ss.str();
}

std::unique_ptr<GetElementAttributesResponseBuilder>
GetElementAttributesResponseBuilder::MakeBuilder() {
  std::unique_ptr<GetElementAttributesResponseBuilder> builder(
      new GetElementAttributesResponseBuilder());

  return builder;
}

GetElementAttributesResponseBuilder*
GetElementAttributesResponseBuilder::AddAttributeEntry(AttributeEntry entry) {
  CHECK(entries_.size() < 0xFF);

  entries_.insert(entry);

  return this;
}

GetElementAttributesResponseBuilder*
GetElementAttributesResponseBuilder::AddAttributeEntry(Attribute attribute,
                                                       std::string value) {
  return AddAttributeEntry(AttributeEntry(attribute, value));
}

size_t GetElementAttributesResponseBuilder::GetLength() const {
  size_t attr_list_size = 0;

  for (auto attribute_entry : entries_) {
    attr_list_size += 4;  // Size of attr entry
    attr_list_size += 2;  // Size of value length field
    attr_list_size += 2;  // Size of character encoding
    attr_list_size += attribute_entry.second.length();
  }

  return VendorPacket::kHeaderSize() + 1 + attr_list_size;
}

bool GetElementAttributesResponseBuilder::Serialize(
    const std::shared_ptr<Packet>& pkt) {
  ReserveSpace(pkt, GetLength());

  AvrcpPacketBuilder::PushHeader(pkt);

  VendorPacketBuilder::PushHeader(pkt,
                                  GetLength() - VendorPacket::kHeaderSize());

  AddPayloadOctets1(pkt, entries_.size());
  for (auto attribute_entry : entries_) {
    PushAttributeValue(pkt, attribute_entry);
  }

  return true;
}

}  // namespace avrcp