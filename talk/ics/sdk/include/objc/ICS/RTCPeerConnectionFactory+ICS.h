//
//  Copyright (c) 2016 Intel Corporation. All rights reserved.
//

#import "WebRTC/RTCPeerConnection.h"
#import "WebRTC/RTCPeerConnectionFactory.h"

@interface RTCPeerConnectionFactory (ICS)

+ (RTCPeerConnectionFactory*)sharedInstance;

@end