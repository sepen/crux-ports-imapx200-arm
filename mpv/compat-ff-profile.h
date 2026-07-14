/* compat-ff-profile.h — this system's FFmpeg removed the deprecated
 * FF_PROFILE_* aliases in favor of AV_PROFILE_*; mpv 0.36.0 predates that
 * rename. Alias the old names back defensively — harmless no-ops if the
 * names are still defined by this FFmpeg version. */
#include <libavcodec/avcodec.h>

#ifndef FF_PROFILE_UNKNOWN
#define FF_PROFILE_UNKNOWN AV_PROFILE_UNKNOWN
#endif
#ifndef FF_PROFILE_DTS_HD_HRA
#define FF_PROFILE_DTS_HD_HRA AV_PROFILE_DTS_HD_HRA
#endif
#ifndef FF_PROFILE_DTS_HD_MA
#define FF_PROFILE_DTS_HD_MA AV_PROFILE_DTS_HD_MA
#endif
#ifndef FF_PROFILE_ARIB_PROFILE_A
#define FF_PROFILE_ARIB_PROFILE_A AV_PROFILE_ARIB_PROFILE_A
#endif
#ifndef FF_PROFILE_ARIB_PROFILE_C
#define FF_PROFILE_ARIB_PROFILE_C AV_PROFILE_ARIB_PROFILE_C
#endif
