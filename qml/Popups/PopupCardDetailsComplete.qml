import QtQuick 2.0
import model 1.0

Item {
    id: popupCardDetailBasic1
    width: 1000
    height: 500

    property AbstractCard card

    onCardChanged: {
        if(card != undefined)
        {
            var description = "";

            if(card.type() === AbstractCard.TypeOfCard_Pokemon)
            {
                //description = card.id() + " - " + card.name();
                //description += " (" + card.lifeTotal() + ")";
                imageCard.source = card.image;

                description += "----- " + card.name + " -----\n\n";
                description += "ID: " + card.id() + "\n";
                description += "Element: " + card.elementFormatString() + "\n";
                description += "Vie restante: " + card.lifeLeft + "\n";
                description += "Vie totale: " + card.lifeTotal + "\n";
                description += "Status: " + card.statusFormatString() + "\n\n";

                for(var i=0;i<card.attacksCount();++i)
                {
                    description += card.attackName(i) + " - " + card.attackDescription(i) + " (" + card.attackDamage(i) + ")\n\n";
                }

                //description += "ID: " + card.id() + "\n";

            }
            else if(card.type() === AbstractCard.TypeOfCard_Energy)
            {
                //description = card.id() + " - " + card.name();
                //description += " (" + card.quantity() + ")";
                imageCard.source = card.image();

                description += "----- Energie " + card.name() + " -----\n\n";
                description += "Element: " + card.elementFormatString() + "\n";
                description += "Quantité: " + card.quantity();
            }
            else if(card.type() === AbstractCard.TypeOfCard_Action)
            {
                imageCard.source = card.image();

                description += "----- Dresseur " + card.name() + " -----\n\n";
                description += "Element: " + card.description();
            }

            textDescription.text = description;
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#AAAAAAAA"

        Image {
            id: imageCard
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width / 2

            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            //source: card.imageCard
        }

        Text {
            id: textDescription
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: imageCard.right
            anchors.leftMargin: 20
            anchors.right: parent.right
            //anchors.bottom: parent.bottom
            font.pixelSize: 30
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
        }

        ListView {
            id: listViewEnergies
            anchors.top: textDescription.bottom
            anchors.left: imageCard.right
            anchors.leftMargin: 20
            anchors.right: parent.right

            model: card.modelListOfEnergies()
            delegate:
                Item {
                width: 25
                height: 25

                Image {
                    id: imageIconEnergy
                    anchors.fill: parent
                    source: model.icon
                }
            }
        }

        MouseArea {
            id: mouseAreaClosing
            anchors.fill: parent
            onClicked: popupCardDetailBasic1.visible = false
        }
    }

}
