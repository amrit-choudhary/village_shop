#ifdef VG_MAC

#import "view_delegate.h"
#import <iostream>

@interface MEViewDelegate ()

@end

@implementation MEViewDelegate

-(nonnull instancetype)initWithView:(nonnull MTKView *)view device:(nonnull id<MTLDevice>)device
{
    self = [super init];
    if(self)
    {
        self.view = view;
        self.device = device;

        self.gameMain = new ME::GameMain();
        self.gameMain->Init();
    }
    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    /// Respond to drawable size or orientation changes here
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    self.gameMain->Update();
    // std::cout << "Drawing frame..." << std::endl;
}

@end

#endif // VG_MAC
