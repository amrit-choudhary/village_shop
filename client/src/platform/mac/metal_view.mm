#ifdef VG_MAC

#import "metal_view.h"

#import <iostream>

@implementation MEView

- (BOOL)acceptsFirstResponder {
    return YES;
}

// Keyboard events
- (void)keyDown:(NSEvent *)event {
    NSString *characters = [event characters];
    if (characters.length > 0) {
        unichar keyChar = [characters characterAtIndex:0];
        std::cout << "Key down: " << keyChar << std::endl;
    }
}
- (void)keyUp:(NSEvent *)event {
    NSString *characters = [event characters];
    if (characters.length > 0) {
        unichar keyChar = [characters characterAtIndex:0];
        std::cout << "Key up: " << keyChar << std::endl;
    }
}

// Mouse button events
- (void)mouseDown:(NSEvent *)event {
}
- (void)mouseUp:(NSEvent *)event {
}
- (void)rightMouseDown:(NSEvent *)event {
}
- (void)rightMouseUp:(NSEvent *)event {
}

// Mouse movement
- (void)mouseMoved:(NSEvent *)event {
}
- (void)mouseDragged:(NSEvent *)event {
}
- (void)rightMouseDragged:(NSEvent *)event {
}

// Scroll wheel
- (void)scrollWheel:(NSEvent *)event {
}

@end

#endif // VG_MAC
