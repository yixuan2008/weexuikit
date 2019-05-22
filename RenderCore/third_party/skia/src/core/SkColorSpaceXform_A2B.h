/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkColorSpaceXform_A2B_DEFINED
#define SkColorSpaceXform_A2B_DEFINED

#include "SkArenaAlloc.h"
#include "SkColorSpaceXform.h"
#include "SkRasterPipeline.h"

class SkColorSpace_A2B;
class SkColorSpace_XYZ;

struct SkTableTransferFn {
    const float* fData;
    int          fSize;
};

class SkColorSpaceXform_A2B : public SkColorSpaceXform {
public:
    SkColorSpaceXform_A2B(SkColorSpace_A2B* srcSpace, SkColorSpace_XYZ* dstSpace);

    bool apply(ColorFormat dstFormat, void* dst, ColorFormat srcFormat, const void* src,
               int count, SkAlphaType alphaType) const override;

private:
    void addTransferFn(const SkColorSpaceTransferFn& fn, int channelIndex);

    bool buildTableFn(SkTableTransferFn* table);
    void addTableFn(const SkTableTransferFn& table, int channelIndex);

    void addMatrix(const SkMatrix44& matrix);

    SkRasterPipeline fElementsPipeline;
    bool             fLinearDstGamma;
    SkArenaAlloc     fAlloc{128};  // TODO: tune?

    template <typename T>
    T* copy(const T& val) { return fAlloc.make<T>(val); }
};

#endif
