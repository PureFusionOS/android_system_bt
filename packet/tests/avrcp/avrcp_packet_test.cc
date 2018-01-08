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

#include <base/logging.h>

#include <gtest/gtest.h>

#include "avrcp_packet.h"
#include "avrcp_test_packets.h"
#include "packet_test_helper.h"

namespace avrcp {

using TestAvrcpPacket = TestPacketType<AvrcpPacket>;

TEST(AvrcpPacketBuilderTest, buildPacketTest) {
  std::vector<uint8_t> get_capabilities_request_payload = {
      0x00, 0x19, 0x58, 0x10, 0x00, 0x00, 0x01, 0x03};
  auto cap_req_builder =
      TestPacketBuilder::MakeBuilder(get_capabilities_request_payload);

  auto builder = AvrcpPacketBuilder::MakeBuilder(
      CType::STATUS, 0x09, 0x00, Opcode::VENDOR, std::move(cap_req_builder));

  ASSERT_EQ(builder->GetLength(), get_capabilities_request.size());

  auto test_packet = TestPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_capabilities_request);
}

TEST(AvrcpPacketTest, getterTests) {
  auto test_avrcp_packet = TestAvrcpPacket::Make(get_capabilities_request);

  ASSERT_EQ(test_avrcp_packet->GetCType(), CType::STATUS);
  ASSERT_EQ(test_avrcp_packet->GetSubunitType(), 0x09);
  ASSERT_EQ(test_avrcp_packet->GetSubunitId(), 0x00);
  ASSERT_EQ(test_avrcp_packet->GetOpcode(), Opcode::VENDOR);
}

TEST(AvrcpPacketTest, getterMaskTests) {
  auto bad_get_cap_data = get_capabilities_request;
  bad_get_cap_data[0] = 0xFF;  // CType
  bad_get_cap_data[1] = 0xFF;  // Subunit Type & ID

  auto test_avrcp_packet = TestAvrcpPacket::Make(bad_get_cap_data);

  ASSERT_EQ(test_avrcp_packet->GetCType(), CType::INTERIM);
  ASSERT_EQ(test_avrcp_packet->GetSubunitType(), 0b00011111);
  ASSERT_EQ(test_avrcp_packet->GetSubunitId(), 0b00000111);
}

TEST(AvrcpPacketTest, payloadBoundsTest) {
  auto test_avrcp_packet = TestAvrcpPacket::Make(get_capabilities_request);

  std::vector<uint8_t> get_cap_payload_data = {0x00, 0x19, 0x58, 0x10,
                                               0x00, 0x00, 0x01, 0x03};
  auto get_cap_payload_packet = TestAvrcpPacket::Make(test_avrcp_packet);

  // Can't do a direct vector compare here since the underlying data is
  // larger than the visible data.
  for (size_t i = 0; i < get_cap_payload_data.size(); i++) {
    ASSERT_EQ(get_cap_payload_data[i], (*get_cap_payload_packet)[i]);
  }
}

}  // namespace avrcp