/**
* Main entry point for the macOS application.
* This file is specific to macOS and uses Objective-C for application management.
*/

#ifdef VG_MAC
#ifdef __OBJC__

#import "app_delegate.h"

#import <Cocoa/Cocoa.h>

#include <iostream>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        MEAppDelegate *delegate = [[NSClassFromString(@"MEAppDelegate") alloc] init];
        [app setDelegate:delegate];
        [app run];
    }
    return 0;
}

#endif  // __OBJC__
#endif  // VG_MAC
