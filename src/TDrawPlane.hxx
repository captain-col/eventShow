#ifndef TDrawPlane_hxx_seen
#define TDrawPlane_hxx_seen

#include <TDigitContainer.hxx>
#include <THandle.hxx>

void DrawPlane(CP::THandle<CP::TDigitContainer> drift, int plane, int minSample, int maxSample, int minWire, int maxWire, std::string planeName);

#endif
