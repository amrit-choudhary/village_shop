/**
 * Gets update events from the Mac Framework
 */

#ifdef VG_MAC
#ifdef __OBJC__

#pragma once

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>
#import <Metal/Metal.h>

@interface MEViewDelegate : NSObject <MTKViewDelegate>

@property (nonatomic, nonnull, assign) MTKView *view;
@property (nonatomic, nonnull, assign) id<MTLDevice> device;

-(nonnull instancetype)initWithView:(nonnull MTKView *)view device:(nonnull id<MTLDevice>)device;

@end

#endif  // __OBJC__
#endif  // VG_MAC
