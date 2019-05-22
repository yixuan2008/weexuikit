/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2006 Allan Sandfeld Jensen (kde@carewolf.com)
 *           (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2009, 2010, 2011 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef RenderImage_h
#define RenderImage_h

#include "render/core/rendering/RenderReplaced.h"
#include "render/core/rendering/RenderImageResource.h"

namespace blink {


class RenderImage : public RenderReplaced {
public:
    // These are the paddings to use when displaying either alt text or an image.
    static const unsigned short paddingWidth = 4;
    static const unsigned short paddingHeight = 4;

    RenderImage();
    virtual ~RenderImage();
    virtual void destroy() override;

    static RenderImage* createAnonymous();

    void setImageResource(PassOwnPtr<RenderImageResource>);

    RenderImageResource* imageResource() { return m_imageResource.get(); }


    void setIsGeneratedContent(bool generated = true) { m_isGeneratedContent = generated; }

    bool isGeneratedContent() const { return m_isGeneratedContent; }

    inline void setImageDevicePixelRatio(float factor) { m_imageDevicePixelRatio = factor; }
    float imageDevicePixelRatio() const { return m_imageDevicePixelRatio; }

    virtual void intrinsicSizeChanged() override
    {

    }

protected:
    virtual bool needsPreferredWidthsRecalculation() const override final;
    virtual void computeIntrinsicRatioInformation(FloatSize& intrinsicSize, double& intrinsicRatio) const override final;


    virtual void paintReplaced(PaintInfo&, const LayoutPoint&) override;
    void paintIntoRect(GraphicsContext* context, const LayoutRect& rect);


    virtual void layout() override;


private:
    virtual const char* renderName() const override { return "RenderImage"; }

    virtual bool isImage() const override { return true; }


    virtual bool foregroundIsKnownToBeOpaqueInRect(const LayoutRect& localRect, unsigned maxDepthToTest) const  final;
    virtual bool computeBackgroundIsKnownToBeObscured() final;

    virtual bool backgroundShouldAlwaysBeClipped() const { return true; }

    virtual LayoutUnit minimumReplacedHeight() const override;

    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, const HitTestLocation& locationInContainer, const LayoutPoint& accumulatedOffset) override final;


    void repaintOrMarkForLayout(const IntRect* = 0);
    void updateIntrinsicSizeIfNeeded(const LayoutSize&);
    // Update the size of the image to be rendered. Object-fit may cause this to be different from the CSS box's content rect.
    void updateInnerContentRect();

    // Text to display as long as the image isn't available.
    OwnPtr<RenderImageResource> m_imageResource;
    bool m_didIncrementVisuallyNonEmptyPixelCount;
    bool m_isGeneratedContent;
    float m_imageDevicePixelRatio;

    friend class RenderImageScaleObserver;
};

DEFINE_RENDER_OBJECT_TYPE_CASTS(RenderImage, isRenderImage());

} // namespace blink

#endif // RenderImage_h
