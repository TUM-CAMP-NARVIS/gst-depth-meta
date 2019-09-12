#pragma once

#include <gst/gst.h>
#include <gst/gstmeta.h>
#include <gst/gsttaglist.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

typedef struct _DepthMetaBuffer DepthMetaBuffer;

struct _DepthMetaBuffer {
  GstMeta meta;
  GstBuffer *buffer;
};

GType depth_meta_buffer_api_get_type(void);
const GstMetaInfo *depth_meta_buffer_get_info(void);
DepthMetaBuffer *depth_meta_buffer_get(GstBuffer *buffer);
DepthMetaBuffer *depth_meta_buffer_add(GstBuffer *buffer, GstBuffer *meta_buffer);

G_END_DECLS
