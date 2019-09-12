/**
 * SECTION:depth-meta-tags
 * @title: DepthMetaTags
 * @short_description: Object short description
 * @include: bla-meta-tags.h
 *
 * Long description
 *
 */

#include "depth-meta-tags.h"

GType depth_meta_tags_api_get_type(void) {
    static volatile GType type;
    static const gchar *tags[] = {NULL};

    if (g_once_init_enter(&type)) {
        GType _type = gst_meta_api_type_register("DepthMetaTagsAPI", tags);
        g_once_init_leave(&type, _type);
    }
    return type;
}

gboolean depth_meta_tags_init(GstMeta *meta, gpointer params, GstBuffer *buffer) {
    return TRUE;
}

gboolean depth_meta_tags_transform(GstBuffer *dest_buf, GstMeta *src_meta,
                               GstBuffer *src_buf, GQuark type, gpointer data) {
    DepthMetaTags *src = (DepthMetaTags *) src_meta;
    depth_meta_tags_add(dest_buf, src->tags);

    return TRUE;
}

void depth_meta_tags_free(GstMeta *meta, GstBuffer *buffer) {
    DepthMetaTags *local_meta = (DepthMetaTags *) meta;
    gst_tag_list_unref(local_meta->tags);
}

const GstMetaInfo *depth_meta_tags_get_info(void) {
    static const GstMetaInfo *meta_info = NULL;

    if (g_once_init_enter(&meta_info)) {
        const GstMetaInfo *meta = gst_meta_register(
                depth_meta_tags_api_get_type(), "DepthMetaTags", sizeof(DepthMetaTags),
                (GstMetaInitFunction) depth_meta_tags_init,
                (GstMetaFreeFunction) depth_meta_tags_free,
                (GstMetaTransformFunction) depth_meta_tags_transform);
        g_once_init_leave(&meta_info, meta);
    }
    return meta_info;
}

DepthMetaTags *depth_meta_tags_get(GstBuffer *buffer) {
    g_return_val_if_fail(GST_IS_BUFFER(buffer), NULL);

    return ((DepthMetaTags *) gst_buffer_get_meta(buffer, depth_meta_tags_api_get_type()));
}

DepthMetaTags *depth_meta_tags_add(GstBuffer *buffer, GstTagList *meta_tags) {
    DepthMetaTags *meta;

    g_return_val_if_fail(GST_IS_BUFFER (buffer), NULL);

    meta = (DepthMetaTags *) gst_buffer_add_meta(buffer, depth_meta_tags_get_info(), NULL);

    if (!meta)
        return NULL;

    meta->tags = gst_tag_list_ref(meta_tags);

    return meta;
}
