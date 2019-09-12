/**
 * SECTION:depth-meta-buffer
 * @title: DepthMetaBuffer
 * @short_description: Object short description
 * @include: bla-meta-buffer.h
 *
 * Long description
 *
 */

#include "depth-meta-buffer.h"

GType depth_meta_buffer_api_get_type(void) {
  static volatile GType type;
  static const gchar *tags[] = {NULL};

  if (g_once_init_enter(&type)) {
    GType _type = gst_meta_api_type_register("DepthMetaBufferAPI", tags);
    g_once_init_leave(&type, _type);
  }
  return type;
}

gboolean depth_meta_buffer_init(GstMeta *meta, gpointer params, GstBuffer *buffer) {
  return TRUE;
}

gboolean depth_meta_buffer_transform(GstBuffer *dest_buf, GstMeta *src_meta,
                               GstBuffer *src_buf, GQuark type, gpointer data) {
  DepthMetaBuffer *src = (DepthMetaBuffer *)src_meta;
  depth_meta_buffer_add(dest_buf, src->buffer);

  return TRUE;
}

void depth_meta_buffer_free(GstMeta *meta, GstBuffer *buffer) {
  DepthMetaBuffer *local_meta = (DepthMetaBuffer *)meta;
  gst_buffer_unref(local_meta->buffer);
}

const GstMetaInfo *depth_meta_buffer_get_info(void) {
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter(&meta_info)) {
    const GstMetaInfo *meta = gst_meta_register(
        depth_meta_buffer_api_get_type(), "DepthMetaBuffer", sizeof(DepthMetaBuffer),
        (GstMetaInitFunction)depth_meta_buffer_init,
        (GstMetaFreeFunction)depth_meta_buffer_free,
        (GstMetaTransformFunction)depth_meta_buffer_transform);
    g_once_init_leave(&meta_info, meta);
  }
  return meta_info;
}

DepthMetaBuffer *depth_meta_buffer_get(GstBuffer *buffer) {
  g_return_val_if_fail(GST_IS_BUFFER(buffer), NULL);

  return (
      (DepthMetaBuffer *)gst_buffer_get_meta(buffer, depth_meta_buffer_api_get_type()));
}

DepthMetaBuffer *depth_meta_buffer_add(GstBuffer *buffer, GstBuffer *meta_buffer) {
  DepthMetaBuffer *meta;

  g_return_val_if_fail(GST_IS_BUFFER(buffer), NULL);

  meta =
      (DepthMetaBuffer *)gst_buffer_add_meta(buffer, depth_meta_buffer_get_info(), NULL);

  if (!meta) return NULL;

  meta->buffer = gst_buffer_ref(meta_buffer);

  return meta;
}
