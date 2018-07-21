#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QResource>
#include "common/constantesqml.h"
#include "src_Models/factorymainpageloader.h"
#include "src_Controler/ctrlanimation.h"
#include "src_Controler/ctrlgameboard.h"
#include "src_Controler/ctrlpopups.h"
#include "src_Controler/ctrlselectingcards.h"
#include "src_Models/modellistallplayers.h"
#include "src_Models/modellistofgamesavailable.h"
#include "src_Models/modellistenergies.h"
#include "src_Models/modelpopupselectcardinpacket.h"
#include "src_Models/modelpopupselectenergyinpokemon.h"
#include "src_Models/modelselectingcards.h"
#include "player.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QResource::registerResource("Ressources/cards.rcc");

    ConstantesQML::declareQML();
    FactoryMainPageLoader::declareQML();
    CtrlGameBoard::declareQML();
    CtrlPopups::declareQML();
    CtrlSelectingCards::declareQML();
    ModelListAllPlayers::declareQML();
    ModelListOfGamesAvailable::declareQML();
    ModelListEnergies::declareQML();
    ModelPopupSelectCardInPacket::declareQML();
    ModelPopupSelectEnergyInPokemon::declareQML();
    ModelSelectingCards::declareQML();
    Player::declareQML();
    AbstractCard::declareQML();
    CardAction::declareQML();
    CardEnergy::declareQML();
    CardPokemon::declareQML();
    BenchArea::declareQML();
    PacketDeck::declareQML();
    FightArea::declareQML();
    PacketHand::declareQML();
    PacketRewards::declareQML();
    PacketTrash::declareQML();

    Utils::initRandomValues();

    QQmlApplicationEngine engine;
    CtrlAnimation ctrlAnim;
    CtrlSelectingCards ctrlSC;
    CtrlPopups ctrlPopups;
    CtrlGameBoard ctrlGB(ctrlSC, ctrlPopups, ctrlAnim);
    ctrlAnim.install(&engine);
    ctrlGB.install(&engine);
    ctrlPopups.install(&engine);
    ctrlSC.install(&engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
