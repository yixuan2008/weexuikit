/*
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
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

#ifndef WEEX_UIKIT_CORE_RENDERING_HITTESTRESULT_H_
#define WEEX_UIKIT_CORE_RENDERING_HITTESTRESULT_H_
#include <vector>
#include <set>
#include "render/core/rendering/HitTestLocation.h"
#include "render/core/rendering/HitTestRequest.h"
#include "render/platform/geometry/FloatQuad.h"
#include "render/platform/geometry/FloatRect.h"
#include "render/platform/geometry/LayoutRect.h"
#include "render/platform/heap/Handle.h"
#include "render/platform/text/TextDirection.h"
#include "render/wtf/Forward.h"
#include "render/wtf/ListHashSet.h"
#include "render/wtf/OwnPtr.h"
#include "render/wtf/RefPtr.h"

namespace blink {

class Image;
class RenderObject;

class HitTestResult {
  DISALLOW_ALLOCATION();

 public:
  HitTestResult();
  HitTestResult(const LayoutPoint&);
  // Pass non-negative padding values to perform a rect-based hit test.
  HitTestResult(const LayoutPoint& centerPoint,
                unsigned topPadding,
                unsigned rightPadding,
                unsigned bottomPadding,
                unsigned leftPadding);
  HitTestResult(const HitTestLocation&);
  HitTestResult(const HitTestResult&);
  ~HitTestResult();
  HitTestResult& operator=(const HitTestResult&);

  bool isOverWidget() const { return m_isOverWidget; }

  // Forwarded from HitTestLocation
  bool isRectBasedTest() const { return m_hitTestLocation.isRectBasedTest(); }

  // The hit-tested point in the coordinates of the main frame.
  const LayoutPoint& pointInMainFrame() const {
    return m_hitTestLocation.point();
  }
  IntPoint roundedPointInMainFrame() const {
    return roundedIntPoint(pointInMainFrame());
  }

  // The hit-tested point in the coordinates of the inner node.
  const LayoutPoint& localPoint() const { return m_localPoint; }
  void setLocalPoint(const LayoutPoint& p) { m_localPoint = p; }

  RenderObject* renderer() const;

  bool hasRenderObject(){
      return renderObject != nullptr;
  }

  void setRenderObject(RenderObject* renderObjectPtr){
       renderObject  = renderObjectPtr;
  }

  const HitTestLocation& hitTestLocation() const { return m_hitTestLocation; }

  bool isSelected() const;
  Image* image() const;
  IntRect imageRect() const;
  bool isMisspelled() const;
  bool isContentEditable() const;

  void append(const HitTestResult&);

 private:
  HitTestLocation m_hitTestLocation;

  // FIXME: Nothing changes this to a value different from m_hitTestLocation!
  LayoutPoint m_pointInInnerNodeFrame;  // The hit-tested point in innerNode
                                        // frame coordinates.
  LayoutPoint m_localPoint;  // A point in the local coordinate space of
                             // m_innerNonSharedNode's renderer. Allows us to
                             // efficiently determine where inside the renderer
                             // we hit on subsequent operations.
  bool m_isOverWidget;       // Returns true if we are over a widget.
  RenderObject* renderObject;

public:
    std::set<blink::RenderObject*> hitTestPathEntrys;
};

}  // namespace blink

#endif  // WEEX_UIKIT_CORE_RENDERING_HITTESTRESULT_H_
