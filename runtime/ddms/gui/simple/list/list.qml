import QtQuick 1.0

Rectangle {

width : 250
height : 500
anchors.fill : parent

ListModel {
    id : model1
    ListElement {
        description : "Fake desc 1"
    }

    ListElement {
        description : "Fake desc 2"
    }
}

ListView {
    model : model1
    anchors.fill : parent
    delegate : Text {
        text : "Descrpition:" + description
    }
    //delegate : ListDelegate {}
    highlight: Rectangle { color : "lightsteelblue"; radius : 5 }
    focus : true
}

}

/*ListView {
    model: MainModel
    delegate : Rectangle {
        border.color: "black"
        anchors.fill: parent
        Text {
            anchors.fill: parent
            text : "Descrpition:" + description
        }
    }
}
*/

