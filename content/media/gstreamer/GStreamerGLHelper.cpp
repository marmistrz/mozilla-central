/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GStreamerGLHelper.h"
#include "GStreamerLoader.h"

#include <gst/gst.h>
#include <stdio.h>

#define GST_NATIVE_BUFFER_QDATA "GstNativeBufferData"
#define GST_NATIVE_BUFFER_BUFFER_NAME "ANativeWindowBuffer"

namespace mozilla {

void GStreamerGLHelper::BufferRef(void* aBuffer)
{

  gst_buffer_ref(GST_BUFFER(aBuffer));
}

void GStreamerGLHelper::BufferUnRef(void* aBuffer)
{
  printf(">>>>>>Func:%s::%d bufferUnref:%i\n", __PRETTY_FUNCTION__, __LINE__, GST_MINI_OBJECT_REFCOUNT(GST_BUFFER(aBuffer)));
  gst_buffer_unref(GST_BUFFER(aBuffer));
}

void* GStreamerGLHelper::GetHandleBuffer(void* aBuffer)
{
  if (!aBuffer) {
    return nullptr;
  }

  const GstStructure* s = gst_buffer_get_qdata(GST_BUFFER(aBuffer), g_quark_from_string(GST_NATIVE_BUFFER_QDATA));
  if (!s) {
    return nullptr;
  }

  if (gst_structure_has_field(s, GST_NATIVE_BUFFER_BUFFER_NAME) &&
    gst_structure_get_field_type(s, GST_NATIVE_BUFFER_BUFFER_NAME) == G_TYPE_POINTER) {

    gpointer ptr;
    if (!gst_structure_get(s, GST_NATIVE_BUFFER_BUFFER_NAME, G_TYPE_POINTER, &ptr, NULL)) {
      return nullptr;
    }

    return ptr;
  }

  return nullptr;
}

} // namespace mozilla

