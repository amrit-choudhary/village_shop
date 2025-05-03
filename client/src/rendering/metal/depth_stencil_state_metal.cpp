#ifdef VG_MAC

#include "depth_stencil_state_metal.h"

ME::DepthStencilStateMetal::DepthStencilStateMetal(MTL::Device* device) {
    CreateDefaultStates(device);
}
ME::DepthStencilStateMetal::~DepthStencilStateMetal() {
    dssDefault->release();
}
void ME::DepthStencilStateMetal::CreateDefaultStates(MTL::Device* device) {
    MTL::DepthStencilDescriptor* dsDesc = MTL::DepthStencilDescriptor::alloc()->init();
    dsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    dsDesc->setDepthWriteEnabled(true);

    dssDefault = device->newDepthStencilState(dsDesc);

    dsDesc->release();
}

#endif
