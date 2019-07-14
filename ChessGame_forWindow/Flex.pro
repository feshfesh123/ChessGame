#-------------------------------------------------
#
# Project created by QtCreator 2019-06-25T23:19:34
#
#-------------------------------------------------

QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flex
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Engine/ai.cpp \
        Engine/attacks.cpp \
        Engine/board.cpp \
        Engine/move.cpp \
        Engine/movegen.cpp \
        Engine/psquaretable.cpp \
        Engine/rays.cpp \
        GUI/tile.cpp \
        GUI/validation.cpp \
        Material/button/qtmaterialflatbutton.cpp \
        Material/button/qtmaterialflatbutton_internal.cpp \
        Material/button/qtmaterialraisedbutton.cpp \
        Material/dialog/qtmaterialdialog.cpp \
        Material/dialog/qtmaterialdialog_internal.cpp \
        Material/lib/qtmaterialoverlaywidget.cpp \
        Material/lib/qtmaterialripple.cpp \
        Material/lib/qtmaterialrippleoverlay.cpp \
        Material/lib/qtmaterialstatetransition.cpp \
        Material/lib/qtmaterialstyle.cpp \
        Material/lib/qtmaterialtheme.cpp \
        main.cpp \
        game.cpp

HEADERS += \
        Engine/ai.h \
        Engine/attacks.h \
        Engine/bitutils.h \
        Engine/board.h \
        Engine/defs.h \
        Engine/move.h \
        Engine/movegen.h \
        Engine/psquaretable.h \
        Engine/rays.h \
        GUI/tile.h \
        GUI/validation.h \
        Material/button/qtmaterialflatbutton.h \
        Material/button/qtmaterialflatbutton_internal.h \
        Material/button/qtmaterialflatbutton_p.h \
        Material/button/qtmaterialraisedbutton.h \
        Material/button/qtmaterialraisedbutton_p.h \
        Material/dialog/qtmaterialdialog.h \
        Material/dialog/qtmaterialdialog_internal.h \
        Material/dialog/qtmaterialdialog_p.h \
        Material/lib/qtmaterialoverlaywidget.h \
        Material/lib/qtmaterialripple.h \
        Material/lib/qtmaterialrippleoverlay.h \
        Material/lib/qtmaterialstatetransition.h \
        Material/lib/qtmaterialstatetransitionevent.h \
        Material/lib/qtmaterialstyle.h \
        Material/lib/qtmaterialstyle_p.h \
        Material/lib/qtmaterialtheme.h \
        Material/lib/qtmaterialtheme_p.h \
        game.h

FORMS += \
        game.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    GUI/Images.qrc \
    imgsrc.qrc \
    resources.qrc

DISTFILES += \
    GUI/Images/attack.wav \
    GUI/Images/bb.png \
    GUI/Images/bishop_black.svg \
    GUI/Images/bishop_white.svg \
    GUI/Images/bk.png \
    GUI/Images/bk_check.png \
    GUI/Images/bn.png \
    GUI/Images/bp.png \
    GUI/Images/bq.png \
    GUI/Images/br.png \
    GUI/Images/chessflex_logo.jpg \
    GUI/Images/exp.png \
    GUI/Images/king_black.svg \
    GUI/Images/king_white.svg \
    GUI/Images/knight_black.svg \
    GUI/Images/knight_white.svg \
    GUI/Images/move.wav \
    GUI/Images/pawn_black.svg \
    GUI/Images/pawn_white.svg \
    GUI/Images/profile.png \
    GUI/Images/queen_black.svg \
    GUI/Images/queen_white.svg \
    GUI/Images/rook_black.svg \
    GUI/Images/rook_white.svg \
    GUI/Images/wb.png \
    GUI/Images/wk.png \
    GUI/Images/wk_check.png \
    GUI/Images/wn.png \
    GUI/Images/wp.png \
    GUI/Images/wq.png \
    GUI/Images/wr.png
