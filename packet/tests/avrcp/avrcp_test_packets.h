#pragma once

#include <vector>

// AVRCP packets pulled from wireshark
namespace {

// An ACL packet containing an AVRCP Get Capabilities Request
std::vector<uint8_t> get_capabilities_request_acl_packet = {
    0x04, 0x20, 0x12, 0x00, 0x0e, 0x00, 0x4c, 0x00, 0x10, 0x11, 0x0e,
    0x01, 0x48, 0x00, 0x00, 0x19, 0x58, 0x10, 0x00, 0x00, 0x01, 0x03};

// Offset into the ACL packet where the Get Capabilities Request data starts
size_t get_capabilities_request_data_offset = 11;

// AVRCP Get Capabilities Request packet
std::vector<uint8_t> get_capabilities_request = {
    0x01, 0x48, 0x00, 0x00, 0x19, 0x58, 0x10, 0x00, 0x00, 0x01, 0x03};

// AVRCP Get Capabilities Response to Company ID request
std::vector<uint8_t> get_capabilities_response_company_id = {
    0x0c, 0x48, 0x00, 0x00, 0x19, 0x58, 0x10, 0x00, 0x00,
    0x08, 0x02, 0x02, 0x00, 0x19, 0x58, 0x00, 0x23, 0x45};

// AVRCP Get Capabilities Response to Events Supported request
std::vector<uint8_t> get_capabilities_response_events_supported = {
    0x0c, 0x48, 0x00, 0x00, 0x19, 0x58, 0x10, 0x00,
    0x00, 0x05, 0x03, 0x03, 0x01, 0x02, 0x05};

// AVRCP Get Element Attributes request for current playing song and attribute
// Title
std::vector<uint8_t> get_element_attributes_request_partial = {
    0x01, 0x48, 0x00, 0x00, 0x19, 0x58, 0x20, 0x00, 0x00, 0x0d, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01};

// AVRCP Get Element Attributes request for current playing song and attributes
// Title, Artist, Album, Media Number, Playing Time, Total Number of Media, and
// Genre
std::vector<uint8_t> get_element_attributes_request_full = {
    0x01, 0x48, 0x00, 0x00, 0x19, 0x58, 0x20, 0x00, 0x00, 0x25, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06};

// AVRCP Get Element Attributes response with attribute values as follows
// Title: "Test Song"
// Artist: "Test Artist"
// Album: "Test Album"
// Track Number: "1"
// Number of Tracks: "2"
// Genre: "Test Genre"
// Duration: "1000"
std::vector<uint8_t> get_elements_attributes_response_full = {
    0x0c, 0x48, 0x00, 0x00, 0x19, 0x58, 0x20, 0x00, 0x00, 0x67, 0x07, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x6a, 0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x20,
    0x53, 0x6f, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x02, 0x00, 0x6a, 0x00, 0x0b,
    0x54, 0x65, 0x73, 0x74, 0x20, 0x41, 0x72, 0x74, 0x69, 0x73, 0x74, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x6a, 0x00, 0x0a, 0x54, 0x65, 0x73, 0x74, 0x20,
    0x41, 0x6c, 0x62, 0x75, 0x6d, 0x00, 0x00, 0x00, 0x04, 0x00, 0x6a, 0x00,
    0x01, 0x31, 0x00, 0x00, 0x00, 0x05, 0x00, 0x6a, 0x00, 0x01, 0x32, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x6a, 0x00, 0x0a, 0x54, 0x65, 0x73, 0x74, 0x20,
    0x47, 0x65, 0x6e, 0x72, 0x65, 0x00, 0x00, 0x00, 0x07, 0x00, 0x6a, 0x00,
    0x04, 0x31, 0x30, 0x30, 0x30};

// AVRCP Get Play Status Request
std::vector<uint8_t> get_play_status_request = {0x01, 0x48, 0x00, 0x00, 0x19,
                                                0x58, 0x30, 0x00, 0x00, 0x00};

// AVRCP Get Play Status Response
std::vector<uint8_t> get_play_status_response = {
    0x0c, 0x48, 0x00, 0x00, 0x19, 0x58, 0x30, 0x00, 0x00, 0x09,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00};

// AVRCP Pass Through Command Play Pushed Request
std::vector<uint8_t> pass_through_command_play_pushed = {0x00, 0x48, 0x7c, 0x44,
                                                         0x00};

// AVRCP Pass Through Command Play Pushed Response
std::vector<uint8_t> pass_through_command_play_released = {0x09, 0x48, 0x7c,
                                                           0xc4, 0x00};

// AVRCP Register Playback Status Notification
std::vector<uint8_t> register_play_status_notification = {
    0x03, 0x48, 0x00, 0x00, 0x19, 0x58, 0x31, 0x00,
    0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x05};

// AVRCP Interim Playback Status Notification
std::vector<uint8_t> interim_play_status_notification = {
    0x0f, 0x48, 0x00, 0x00, 0x19, 0x58, 0x31, 0x00, 0x00, 0x02, 0x01, 0x00};

// AVRCP Interim Track Changed Notification
std::vector<uint8_t> interim_track_changed_notification = {
    0x0f, 0x48, 0x00, 0x00, 0x19, 0x58, 0x31, 0x00, 0x00, 0x09,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// AVRCP Changed Playback Position Notification
std::vector<uint8_t> changed_play_pos_notification = {
    0x0d, 0x48, 0x00, 0x00, 0x19, 0x58, 0x31, 0x00,
    0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00};

}  // namespace