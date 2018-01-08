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
#include "get_element_attributes_packet.h"
#include "packet_test_helper.h"

namespace avrcp {

using TestGetElemAttrReqPacket = TestPacketType<GetElementAttributesRequest>;

TEST(GetElementAttributesRequestPacketTest, getterTest) {
  // Only Title is requested
  auto test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_partial);

  ASSERT_EQ(test_packet->GetIdentifier(), 0u);

  auto attribute_list = test_packet->GetAttributesRequested();
  ASSERT_EQ(attribute_list.size(), 1u);
  ASSERT_EQ(attribute_list[0], Attribute::TITLE);

  // Title, Artist, Album, Media Numer, Playing Time, Total Number of Media,
  // and Genre requested
  test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_full);

  ASSERT_EQ(test_packet->GetIdentifier(), 0u);

  attribute_list = test_packet->GetAttributesRequested();
  ASSERT_EQ(attribute_list.size(), 7u);
  ASSERT_EQ(attribute_list[0], Attribute::TITLE);
  ASSERT_EQ(attribute_list[1], Attribute::ARTIST_NAME);
  ASSERT_EQ(attribute_list[2], Attribute::ALBUM_NAME);
  ASSERT_EQ(attribute_list[3], Attribute::TRACK_NUMBER);
  ASSERT_EQ(attribute_list[4], Attribute::PLAYING_TIME);
  ASSERT_EQ(attribute_list[5], Attribute::TOTAL_NUMBER_OF_TRACKS);
  ASSERT_EQ(attribute_list[6], Attribute::GENRE);
}

TEST(GetElementAttributesResponseBuilderTest, builderLengthTest) {
  // Attributes have a size of 8 + string length
  auto builder = GetElementAttributesResponseBuilder::MakeBuilder();
  ASSERT_EQ(builder->GetLength(), 11u);
  builder->AddAttributeEntry(TITLE, "test");
  ASSERT_EQ(builder->GetLength(), 23u);
  builder->AddAttributeEntry(ARTIST_NAME, "test");
  ASSERT_EQ(builder->GetLength(), 35u);
}

TEST(GetElementAttributesResonseBuilderTest, builderTest) {
  auto builder = GetElementAttributesResponseBuilder::MakeBuilder();
  builder->AddAttributeEntry(Attribute::TITLE, "Test Song");
  builder->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  builder->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  builder->AddAttributeEntry(Attribute::TRACK_NUMBER, "1");
  builder->AddAttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2");
  builder->AddAttributeEntry(Attribute::GENRE, "Test Genre");
  builder->AddAttributeEntry(Attribute::PLAYING_TIME, "1000");

  ASSERT_EQ(builder->GetLength(), get_elements_attributes_response_full.size());

  auto test_packet = TestPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_elements_attributes_response_full);
}

}  // namespace avrcp