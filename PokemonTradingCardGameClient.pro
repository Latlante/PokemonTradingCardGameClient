QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    common/constantesqml.cpp \
    common/database.cpp \
    src_Cards/abstractcard.cpp \
    src_Cards/cardaction.cpp \
    src_Cards/cardenergy.cpp \
    src_Cards/cardpokemon.cpp \
    src_Controler/ctrlanimation.cpp \
    src_Controler/ctrlgameboard.cpp \
    src_Controler/ctrlpopups.cpp \
    src_Controler/ctrlselectingcards.cpp \
    src_Models/factorymainpageloader.cpp \
    src_Models/modellistenergies.cpp \
    src_Models/modelpopupselectcardinpacket.cpp \
    src_Models/modelpopupselectenergyinpokemon.cpp \
    src_Models/modelselectingcards.cpp \
    src_Packets/abstractpacket.cpp \
    src_Packets/abstractpacketdynamic.cpp \
    src_Packets/abstractpacketstatic.cpp \
    src_Packets/bencharea.cpp \
    src_Packets/fightarea.cpp \
    src_Packets/packetdeck.cpp \
    src_Packets/packetdynamiccustom.cpp \
    src_Packets/packethand.cpp \
    src_Packets/packetrewards.cpp \
    src_Packets/packettrash.cpp \
    gamemanager.cpp \
    player.cpp \
    src_Communication/socketclient.cpp \
    common/utils.cpp \
    src_Models/modellistallplayers.cpp \
    src_Models/modellistofgamesavailable.cpp \
    Share/constantesshared.cpp \
    src_Communication/abstractsocketclient.cpp \
    src_Communication/socketclienttest.cpp \
    src_Models/proxymodelselectingcards.cpp \
    src_Models/modellistselection.cpp

RESOURCES += qml.qrc \
    img.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    common/constantesqml.h \
    common/database.h \
    src_Cards/abstractcard.h \
    src_Cards/cardaction.h \
    src_Cards/cardenergy.h \
    src_Cards/cardpokemon.h \
    src_Controler/ctrlanimation.h \
    src_Controler/ctrlgameboard.h \
    src_Controler/ctrlpopups.h \
    src_Controler/ctrlselectingcards.h \
    src_Models/factorymainpageloader.h \
    src_Models/modellistenergies.h \
    src_Models/modelpopupselectcardinpacket.h \
    src_Models/modelpopupselectenergyinpokemon.h \
    src_Models/modelselectingcards.h \
    src_Packets/abstractpacket.h \
    src_Packets/abstractpacketdynamic.h \
    src_Packets/abstractpacketstatic.h \
    src_Packets/bencharea.h \
    src_Packets/fightarea.h \
    src_Packets/packetdeck.h \
    src_Packets/packetdynamiccustom.h \
    src_Packets/packethand.h \
    src_Packets/packetrewards.h \
    src_Packets/packettrash.h \
    gamemanager.h \
    player.h \
    src_Communication/socketclient.h \
    common/utils.h \
    src_Models/modellistallplayers.h \
    src_Models/modellistofgamesavailable.h \
    Share/constantesshared.h \
    src_Communication/abstractsocketclient.h \
    src_Communication/socketclienttest.h \
    src_Models/proxymodelselectingcards.h \
    src_Models/modellistselection.h
