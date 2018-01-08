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

#include "avrcp_test_packets.h"
#include "capabilities_packet.h"
#include "packet_test_helper.h"

namespace avrcp {

using GetCapRequestTestPacket = TestPacketType<GetCapabilitiesRequest>;

// Test parsing a GetCapabilities Request
TEST(GetCapabilitiesRequestPacketTest, getterTest) {
  auto test_packet = GetCapRequestTestPacket::Make(get_capabilities_request);

  ASSERT_EQ(test_packet->GetCapabilityRequested(),
            Capability::EVENTS_SUPPORTED);
}

// Test that the length returned by the builder grows correctlyas fields are
// added
TEST(GetCapabilityResponseBuilder, builderLengthTest) {
  auto builder = GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(0x000000);
  ASSERT_EQ(builder->GetLength(), 15u);
  builder->AddCompanyId(0x000001);
  ASSERT_EQ(builder->GetLength(), 18u);
  builder->AddCompanyId(0x000002);
  ASSERT_EQ(builder->GetLength(), 21u);

  builder = GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(
      Event::PLAYBACK_STATUS_CHANGED);
  ASSERT_EQ(builder->GetLength(), 13u);
  builder->AddEvent(Event::TRACK_CHANGED);
  ASSERT_EQ(builder->GetLength(), 14u);
  builder->AddEvent(Event::PLAYBACK_POS_CHANGED);
  ASSERT_EQ(builder->GetLength(), 15u);
}

// Check to see that adding the same value multiple times does nothing
TEST(GetCapabilityResponseBuilder, duplicateAddTest) {
  auto builder = GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(0x000000);
  ASSERT_EQ(builder->GetLength(), 15u);
  builder->AddCompanyId(0x000000);
  ASSERT_EQ(builder->GetLength(), 15u);

  builder = GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(
      Event::PLAYBACK_STATUS_CHANGED);
  ASSERT_EQ(builder->GetLength(), 13u);
  builder->AddEvent(Event::PLAYBACK_STATUS_CHANGED);
  ASSERT_EQ(builder->GetLength(), 13u);
}

// Test that trying to to add the wrong type of field to a builder causes death
TEST(GetCapabilityResponseBuilder, mismatchAddDeathTest) {
  auto builder = GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(0x000000);
  ASSERT_DEATH(builder->AddEvent(Event::PLAYBACK_STATUS_CHANGED),
               "capability_ == Capability::EVENTS_SUPPORTED");

  builder = GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(
      Event::PLAYBACK_STATUS_CHANGED);
  ASSERT_DEATH(builder->AddCompanyId(0x000000),
               "capability_ == Capability::COMPANY_ID");
}

// Test building a GetCapabilities Response to a Company ID request
TEST(GetCapabilityResponseBuilder, comanyIdBuilderTest) {
  auto builder = GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(0x002345);
  builder->AddCompanyId(BLUETOOTH_COMPANY_ID);

  auto test_packet = TestPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_capabilities_response_company_id);
}

// Test building a GetCapabilities Response to an Events Supported request
TEST(GetCapabilityResponseBuilder, eventsSupportedBuilderTest) {
  auto builder = GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(
      Event::PLAYBACK_STATUS_CHANGED);
  builder->AddEvent(Event::TRACK_CHANGED);
  builder->AddEvent(Event::PLAYBACK_POS_CHANGED);

  auto test_packet = TestPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_capabilities_response_events_supported);
}

}  // namespace avrcp