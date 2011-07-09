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

Component {
    id: decisionDelegate
    Item {
         width:250; height: 50;
    Text {
            text : "Descrpition:" + description
     }
    MouseArea {
        anchors.fill: parent
        onClicked: ListView.view.currentIndex = index
    }
    }
}

ListView {
    //model : model1
    model: mainModel
    anchors.fill : parent
    delegate :decisionDelegate
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

