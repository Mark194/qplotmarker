#pragma once

#include <QtCore/qglobal.h>

#if defined(QPLOTMARKER_LIBRARY)
#define QPLOTMARKER_EXPORT Q_DECL_EXPORT
#else
#define QPLOTMARKER_EXPORT Q_DECL_IMPORT
#endif
