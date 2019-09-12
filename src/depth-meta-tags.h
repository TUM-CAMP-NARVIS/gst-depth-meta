#pragma once

#include <gst/gst.h>
#include <gst/gstmeta.h>
#include <gst/gsttaglist.h>

G_BEGIN_DECLS

typedef struct _DepthMetaTags DepthMetaTags;

struct _DepthMetaTags {
  GstMeta meta;
  GstTagList *tags;
};

GType depth_meta_tags_api_get_type(void);
const GstMetaInfo *depth_meta_tags_get_info(void);
DepthMetaTags *depth_meta_tags_get(GstBuffer *buffer);
DepthMetaTags *depth_meta_tags_add(GstBuffer *buffer, GstTagList *tags);

G_END_DECLS
