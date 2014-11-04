extern "C"{
#include <sys/capability.h>
#include <unistd.h>
#include <sys/stat.h>
}

#include <cstdlib>
#include <cstdio>

#include <stdint.h>
#include <uchar.h>
#include <cstring>
#include <memory>
#include <utility>
#include <chrono>
#include <thread>

#include <assert.h>

extern "C" {
	#include <libappindicator/app-indicator.h>
	#include <gtk/gtk.h>
}

#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QVector>
#include <QFontDatabase>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QDesktopWidget>

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#define DIM( a ) (sizeof(a)/sizeof(*a))

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

/// Signed 8-bit integer
typedef int8_t                   si8;
/// Signed 16-bit integer
typedef int16_t                  si16;
/// Signed 32-bit integer
typedef int32_t                  si32;
/// Signed 64-bit integer
typedef int64_t                  si64;

/// Unsigned 8-bit integer
typedef uint8_t                  ui8;
/// Unsigned 16-bit integer
typedef uint16_t                 ui16;
/// Unsigned 32-bit integer
typedef uint32_t                 ui32;
/// Unsigned 64-bit integer
typedef uint64_t                 ui64;
