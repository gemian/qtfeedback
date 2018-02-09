/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtFeedback module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=src/feedback

#include <QtTest/QtTest>

#include <qfeedbackeffect.h>
#include <qfeedbackactuator.h>
#include <QSignalSpy>
#include <QFeedbackFileEffect>

class tst_QFeedbackMMK : public QObject
{
    Q_OBJECT
public:
    tst_QFeedbackMMK();
    ~tst_QFeedbackMMK();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void badFile();

private:
    QUrl url;
};

tst_QFeedbackMMK::tst_QFeedbackMMK()
{
}

tst_QFeedbackMMK::~tst_QFeedbackMMK()
{
}

void tst_QFeedbackMMK::initTestCase()
{
    // Only perform tests if audio device exists
    // have to check specific formats supported as test plugin maybe installed
    QStringList mimeTypes = QFeedbackFileEffect::supportedMimeTypes();
    if (!(mimeTypes.contains("audio/x-wav") || mimeTypes.contains("audio/wav")
        || mimeTypes.contains("audio/wave") || mimeTypes.contains("audio/x-pn-wav")))
        QSKIP("No audio devices available");

    // Some of this comes from the qsoundeffect testcase . .. ...
#ifdef QT_QFEEDBACKMMK_USEAPPLICATIONPATH
    url = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QString("/test.wav"));
#else
    url = QUrl::fromLocalFile(QString(SRCDIR "test.wav"));
#endif
}

void tst_QFeedbackMMK::cleanupTestCase()
{
}

void tst_QFeedbackMMK::init()
{
}

void tst_QFeedbackMMK::cleanup()
{
}

Q_DECLARE_METATYPE(QFeedbackEffect::ErrorType);

void tst_QFeedbackMMK::badFile()
{
    QFeedbackFileEffect fe;
    qRegisterMetaType<QFeedbackEffect::ErrorType>();
    QSignalSpy errorSpy(&fe, SIGNAL(error(QFeedbackEffect::ErrorType)));
    QSignalSpy stateSpy(&fe, SIGNAL(stateChanged()));

    fe.setSource(QUrl("file:///does/not/exist/ever.wav"));

    // Depending on event loops we might miss the Loading state.
    QTRY_VERIFY(stateSpy.count() >  0);    // Loading & Stopped
    QTRY_COMPARE(fe.state(), QFeedbackEffect::Stopped);

    QVERIFY(errorSpy.count() > 0);
    QVERIFY(fe.isLoaded() == false);
    stateSpy.clear();
    errorSpy.clear();

    fe.start(); // this actually causes a load, so it goes into LOADING, then fails, should go to STOPPED
    QTRY_VERIFY(stateSpy.count() > 0);    // Loading & Stopped
    QTRY_COMPARE(fe.state(), QFeedbackEffect::Stopped);
    QVERIFY(errorSpy.count() > 0);
    QVERIFY(fe.isLoaded() == false);
}

QTEST_MAIN(tst_QFeedbackMMK)

#include "tst_qfeedbackmmk.moc"
