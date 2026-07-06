#ifdef VG_MAC

#import "metal_view.h"

#import "view_delegate.h"

@implementation MEView

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (ME::GameMain*)gameMain {
    MEViewDelegate* delegate = (MEViewDelegate*)self.delegate;
    return delegate.gameMain;
}

// Keyboard events
- (void)keyDown:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleKeyEvent(event.keyCode, true);
    }
}
- (void)keyUp:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleKeyEvent(event.keyCode, false);
    }
}

// Mouse button events
- (void)mouseDown:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleMouseButton(0, true);
    }
}
- (void)mouseUp:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleMouseButton(0, false);
    }
}
- (void)rightMouseDown:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleMouseButton(1, true);
    }
}
- (void)rightMouseUp:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        gameMain->HandleMouseButton(1, false);
    }
}

// Mouse movement
- (void)mouseMoved:(NSEvent *)event {
    ME::GameMain* gameMain = [self gameMain];
    if (gameMain != nullptr) {
        NSPoint point = [self convertPoint:event.locationInWindow fromView:nil];
        gameMain->HandleMouseMove(point.x, point.y);
    }
}
- (void)mouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}
- (void)rightMouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

// Scroll wheel
- (void)scrollWheel:(NSEvent *)event {
}

@end

#endif // VG_MAC
