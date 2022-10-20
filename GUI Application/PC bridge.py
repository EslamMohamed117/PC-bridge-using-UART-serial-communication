from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo


class Ui_MainWindow(object):


    def load_ports(self):
        ports = QSerialPortInfo().availablePorts()
        for port in ports:
            self.comboBox.addItem(port.portName())

    # connect to selected port in combobox
    def connect(self):
        port = self.comboBox.currentText()
        self.serial = QSerialPort(port)
        if self.serial.open(QSerialPort.ReadWrite):
            self.connectionLabel.setText("Connected")
            self.groupBox.setEnabled(True)
            self.groupBox_2.setEnabled(True)
        else:
            self.connectionLabel.setText("Not connected")

    # send character to serial port
    def send_to_port(self, data):
        self.serial.write(data.encode())
    # refreshBtn ports
    def refresh_ports(self):
        self.comboBox.clear()
        self.load_ports()

    # recieve data from serial port into recieveDataLabel
    def recieve_data(self):
        data = self.serial.readAll()
        self.recievedDataLabel.setText(data.simplified().data().decode())

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setEnabled(True)
        MainWindow.resize(391, 426)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.comboBox = QtWidgets.QComboBox(self.centralwidget)
        self.comboBox.setGeometry(QtCore.QRect(50, 80, 281, 31))
        self.comboBox.setObjectName("comboBox")
        self.connectBtn = QtWidgets.QPushButton(self.centralwidget)
        self.connectBtn.setGeometry(QtCore.QRect(240, 130, 93, 28))
        self.connectBtn.setObjectName("connectBtn")
        self.refreshBtn = QtWidgets.QPushButton(self.centralwidget)
        self.refreshBtn.setGeometry(QtCore.QRect(50, 130, 93, 28))
        self.refreshBtn.setObjectName("refreshBtn")
        self.connectionLabel = QtWidgets.QLabel(self.centralwidget)
        self.connectionLabel.setGeometry(QtCore.QRect(160, 30, 101, 41))
        self.connectionLabel.setObjectName("connectionLabel")
        self.groupBox = QtWidgets.QGroupBox(self.centralwidget)
        self.groupBox.setGeometry(QtCore.QRect(50, 180, 291, 80))
        self.groupBox.setObjectName("groupBox")
        self.groupBox.setEnabled(False)
        self.ledON = QtWidgets.QPushButton(self.groupBox)
        self.ledON.setGeometry(QtCore.QRect(30, 30, 93, 28))
        self.ledON.setObjectName("ledON")
        self.ledOFF = QtWidgets.QPushButton(self.groupBox)
        self.ledOFF.setGeometry(QtCore.QRect(160, 30, 93, 28))
        self.ledOFF.setObjectName("ledOFF")
        self.groupBox_2 = QtWidgets.QGroupBox(self.centralwidget)
        self.groupBox_2.setGeometry(QtCore.QRect(50, 280, 291, 80))
        self.groupBox_2.setObjectName("groupBox_2")
        self.groupBox_2.setEnabled(False)
        self.recievedDataLabel = QtWidgets.QLabel(self.groupBox_2)
        self.recievedDataLabel.setGeometry(QtCore.QRect(10, 30, 131, 41))
        self.recievedDataLabel.setText("")
        self.recievedDataLabel.setObjectName("recievedDataLabel")
        self.recievedDataBtn = QtWidgets.QPushButton(self.groupBox_2)
        self.recievedDataBtn.setGeometry(QtCore.QRect(170, 30, 93, 28))
        self.recievedDataBtn.setObjectName("recievedDataBtn")
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        self.load_ports()
        self.connectBtn.clicked.connect(self.connect)
        self.recieve_data
        self.refreshBtn.clicked.connect(self.refresh_ports)
        self.recievedDataBtn.clicked.connect(self.recieve_data)
        self.ledON.clicked.connect(lambda: self.send_to_port("1"))
        self.ledOFF.clicked.connect(lambda: self.send_to_port("0"))
        


    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.connectBtn.setText(_translate("MainWindow", "Connect"))
        self.refreshBtn.setText(_translate("MainWindow", "Refresh"))
        self.connectionLabel.setText(_translate("MainWindow", "TextLabel"))
        self.groupBox.setTitle(_translate("MainWindow", "LED"))
        self.ledON.setText(_translate("MainWindow", "ON"))
        self.ledOFF.setText(_translate("MainWindow", "OFF"))
        self.groupBox_2.setTitle(_translate("MainWindow", "Recieved data"))
        self.recievedDataBtn.setText(_translate("MainWindow", "Recieve"))

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())



