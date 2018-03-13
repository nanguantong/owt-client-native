//
//  Copyright (c) 2018 Intel Corporation. All rights reserved.
//

#import <WebRTC/RTCLegacyStatsReport.h>
#import <WebRTC/RTCLogging.h>

#import "talk/ics/sdk/include/objc/ICS/ICSP2PPublication.h"
#import "talk/ics/sdk/include/objc/ICS/ICSP2PPeerConnectionChannelObserver.h"

@interface ICSP2PPublication () <ICSP2PPeerConnectionChannelObserver>

@property(nonatomic, readonly) void (^stopMethod)();
@property(nonatomic, readonly) void (^statsMethod)
    (void (^)(NSArray<RTCLegacyStatsReport*>*), void (^)(NSError*));

- (instancetype)initWithStop:(void (^)())stopMethod
                       stats:
                           (void (^)(void (^)(NSArray<RTCLegacyStatsReport*>*),
                                     void (^)(NSError*)))statsMethod;

/**
  @brief This function will be invoked when received a invitation.
  @param remoteUserId Remote user’s ID
*/
- (void)onInvitedFrom:(NSString*)remoteUserId;
/**
  @brief This function will be invoked when a remote user accepted current
  user's invitation.
  @param remoteUserId Remote user’s ID
*/
- (void)onAcceptedFrom:(NSString*)remoteUserId;
/**
  @brief This function will be invoked when a remote user denied current user's
  invitation.
  @param remoteUserId Remote user’s ID
*/
- (void)onDeniedFrom:(NSString*)remoteUserId;
/**
  @brief This function will be invoked when a chat is started. (This event
  haven't been implemented yet)
  @param remoteUserId Remote user’s ID
*/
- (void)onStoppedFrom:(NSString*)remoteUserId;
/**
  @brief This function will be invoked when a chat is stopped. (This event
  haven't been implemented yet)
  @param remoteUserId Remote user’s ID
*/
- (void)onStartedFrom:(NSString*)remoteUserId;
/**
  @brief This function will be invoked when received data from a remote user.
  (This event haven't been implemented yet)
  @param remoteUserId Remote user’s ID
  @param data Data received.
*/
- (void)onDataReceivedFrom:(NSString*)remoteUserId withData:(NSString*)data;
/**
  @brief This function will be invoked when a remote stream is available.
  @param remoteUserId Remote user’s ID
*/
- (void)onStreamAdded:(ICSRemoteStream*)stream;
/**
  @brief This function will be invoked when a remote stream is removed.
  @param remoteUserId Remote user’s ID
*/
- (void)onStreamRemoved:(ICSRemoteStream*)stream;

@end
