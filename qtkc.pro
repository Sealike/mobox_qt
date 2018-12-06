QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = qtkc

TEMPLATE = app

SOURCES += \
    main.cpp \
    nwthread.cpp \
    keyBoard.cpp \
    Http.cpp \
    b2auth.cpp \
    bdthread.cpp \
    usart_init.cpp \
    tool.cpp \
    mmmd.cpp \
    network_job.cpp \
    network_data_process.cpp \
    gnssthread.cpp \
    suo.cpp \
    widget_btn_slot.cpp \
    widget_timer_slot.cpp \
    widget_work_slot.cpp \
    widget_func.cpp \
    Widget.cpp \
    Widget_work_jsonparse.cpp \
    JsonConfig.cpp

HEADERS += \
    nwthread.h \
    keyBoard.h \
    Http.h \
    b2auth.h \
    bdthread.h \
    usart_init.h \
    tool.h \
    stdafx.h \
    mmmd.h \
    bd_type.h \
    usart_init.h \
    tool.h \
    bd_type.h \
    mmmd.h \
    stdafx.h \
    network_job.h \
    network_data_process.h \
    Widget.h \
    gnssthread.h \
    suo.h \
    JsonConfig.h

FORMS +=
