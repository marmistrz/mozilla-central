// checksum 0x78c version 0x60010
/*
  This file was generated by the Qt Quick Application wizard of Qt Creator.
  QmlApplicationViewer is a convenience class containing mobile device specific
  code such as screen orientation handling. Also QML paths and debugging are
  handled here.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

#include "qmlapplicationviewer.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtGui/QApplication>
#include <QGraphicsObject>
#include <QWindowStateChangeEvent>

#ifdef HARMATTAN_BOOSTER
#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN
#include <MDeclarativeCache>
#endif

#if defined(QMLJSDEBUGGER) && QT_VERSION < 0x040800

#include <qt_private/qdeclarativedebughelper_p.h>

#if !defined(NO_JSDEBUGGER)
#include <jsdebuggeragent.h>
#endif
#if !defined(NO_QMLOBSERVER)
#include <qdeclarativeviewobserver.h>
#endif

// Enable debugging before any QDeclarativeEngine is created
struct QmlJsDebuggingEnabler
{
    QmlJsDebuggingEnabler()
    {
        QDeclarativeDebugHelper::enableDebugging();
    }
};

// Execute code in constructor before first QDeclarativeEngine is instantiated
static QmlJsDebuggingEnabler enableDebuggingHelper;

#endif // QMLJSDEBUGGER

#include <sys/time.h>
#include <stdio.h>

class MozFPSCounter
{
public:
    MozFPSCounter(const char* aEnv = NULL, const char* aMsg = NULL, int aFreq = 5)
        : mEnv(aEnv), mMsg(aMsg), mFreq(aFreq), mFpsCounter(0), mEnabled(true) {
        if (mEnv) {
            char* envVal = getenv(mEnv);
            if (!envVal) {
                mEnabled = false;
            }
            if (envVal && *envVal) {
                mFreq = atoi(envVal);
            }
        }
    }
    void Count(void) {
        if (!mEnabled) return;
        if (!mFpsCounter) {
            gettimeofday(&mPrevTime, NULL);
        }
        mFpsCounter++;
        if (mFpsCounter > mFreq) {
            gettimeofday(&mCurrTime, NULL);
            timersub(&mCurrTime, &mPrevTime, &mDiffTime);
            float time = mFpsCounter / (mDiffTime.tv_sec + (float)mDiffTime.tv_usec/1000000);
            printf("%s: time:%ld.%06ld fps:%g\n", mMsg, mDiffTime.tv_sec, mDiffTime.tv_usec, time);
            mFpsCounter = 0;
        }
    }
    virtual ~MozFPSCounter() {};
private:
    const char* mEnv;
    const char* mMsg;
    int mFreq;
    int mFpsCounter;
    bool mEnabled;
    struct timeval mDiffTime;
    struct timeval mCurrTime;
    struct timeval mPrevTime;
};

class QmlApplicationViewerPrivate
{
    QmlApplicationViewerPrivate(QDeclarativeView *view_) : view(view_) {}

    QString mainQmlFile;
    QDeclarativeView *view;
    friend class QmlApplicationViewer;
    QString adjustPath(const QString &path);
};

QString QmlApplicationViewerPrivate::adjustPath(const QString &path)
{
    if( path.startsWith("qrc:/") )
        return path;

#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QCoreApplication::applicationDirPath()
                + QLatin1String("/../Resources/") + path;
#else
    QString pathInInstallDir;
    const QString applicationDirPath = QCoreApplication::applicationDirPath();
    pathInInstallDir = QString::fromAscii("%1/../%2").arg(applicationDirPath, path);

    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
#endif
    return path;
}

QmlApplicationViewer::QmlApplicationViewer(QWidget *parent)
    : QDeclarativeView(parent)
    , d(new QmlApplicationViewerPrivate(this))
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    // Qt versions prior to 4.8.0 don't have QML/JS debugging services built in
#if defined(QMLJSDEBUGGER) && QT_VERSION < 0x040800
#if !defined(NO_JSDEBUGGER)
    new QmlJSDebugger::JSDebuggerAgent(d->view->engine());
#endif
#if !defined(NO_QMLOBSERVER)
    new QmlJSDebugger::QDeclarativeViewObserver(d->view, d->view);
#endif
#endif
}

QmlApplicationViewer::QmlApplicationViewer(QDeclarativeView *view, QWidget *parent)
    : QDeclarativeView(parent)
    , d(new QmlApplicationViewerPrivate(view))
{
    connect(view->engine(), SIGNAL(quit()), view, SLOT(close()));
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    // Qt versions prior to 4.8.0 don't have QML/JS debugging services built in
#if defined(QMLJSDEBUGGER) && QT_VERSION < 0x040800
#if !defined(NO_JSDEBUGGER)
    new QmlJSDebugger::JSDebuggerAgent(d->view->engine());
#endif
#if !defined(NO_QMLOBSERVER)
    new QmlJSDebugger::QDeclarativeViewObserver(d->view, d->view);
#endif
#endif
}

QmlApplicationViewer::~QmlApplicationViewer()
{
    delete d;
}

QmlApplicationViewer *QmlApplicationViewer::create()
{
#ifdef HARMATTAN_BOOSTER
    return new QmlApplicationViewer(MDeclarativeCache::qDeclarativeView(), 0);
#else
    return new QmlApplicationViewer();
#endif
}

void QmlApplicationViewer::setMainQmlFile(const QString &file)
{
    if(file.startsWith("qrc:/")) {
        d->view->setSource(QUrl(file));
        return;
    }
    d->mainQmlFile = d->adjustPath(file);
    d->view->setSource(QUrl::fromLocalFile(d->mainQmlFile));
}

void QmlApplicationViewer::addImportPath(const QString &path)
{
    d->view->engine()->addImportPath(d->adjustPath(path));
}

void QmlApplicationViewer::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void QmlApplicationViewer::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_SIMULATOR)
    d->view->showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    d->view->showMaximized();
#else
    d->view->show();
#endif
}

void
QmlApplicationViewer::paintEvent(QPaintEvent* ev)
{
    static MozFPSCounter counter("SHOW_FPS", "QGVFPS", 30);
    counter.Count();
    QDeclarativeView::paintEvent(ev);
}

bool
QmlApplicationViewer::event(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent* change = static_cast<QWindowStateChangeEvent*>(event);
        if (!(change->oldState() & Qt::WindowMinimized) && isMinimized())
            Q_EMIT(d->view, displayExited());
        if ((change->oldState() & Qt::WindowMinimized) && !isMinimized())
            Q_EMIT(d->view, displayEntered());
    }

    return QDeclarativeView::event(event);
}

QApplication *createApplication(int &argc, char **argv)
{
#ifdef HARMATTAN_BOOSTER
    return MDeclarativeCache::qApplication(argc, argv);
#else
    return new QApplication(argc, argv);
#endif
}

QDeclarativeView* QmlApplicationViewer::getView() {
    return d->view;
}
