#include "webrtc_stu.h"

#include <iostream>

#include "api/peer_connection_interface.h"
#include "rtc_base/version.h"

webrtc_stu::webrtc_stu(){
    std::cout << "WebRTC version: " << rtc::GetVersionString() << std::endl;
}

webrtc_stu::~webrtc_stu() = default;