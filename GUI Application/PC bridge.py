from re import S
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
import time
import serial

# declare port to be initialized later
port = 0
class Ui_MainWindow(object):
    modeOperation=''

    def load_ports(self):
        ports = QSerialPortInfo().availablePorts()
        for port in ports:
            self.comboBox.addItem(port.portName())

    # connect to selected port in combobox
    def connect(self):
        port = self.comboBox.currentText()
        #self.serial = QSerialPort(port)
        self.serial = serial.Serial(port, 9600, timeout=1)
        #if self.serial is open

        if self.serial.isOpen():
            self.connectionLabel.setText("Connected")
            self.connectionLabel.setStyleSheet("color: green")
            self.ledGroup.setEnabled(True)
            self.receivedDataGroup.setEnabled(True)
            self.fanSpeedGroup.setEnabled(True)
            self.memoryGroup.setEnabled(True)
        else:
            self.connectionLabel.setText("Not connected")
            self.connectionLabel.setStyleSheet("color: red")
    # disconnect from serial port
    def disconnect(self):
        self.serial.close()

        self.connectionLabel.setText("No connection")
        self.connectionLabel.setStyleSheet("color: red")
        self.ledGroup.setEnabled(False)
        self.receivedDataGroup.setEnabled(False)
        self.fanSpeedGroup.setEnabled(False)
        self.memoryGroup.setEnabled(False)

    # send character to serial port
    def send_to_port(self, data, mode):
        self.serial.write(mode.encode())
        time.sleep(0.1)
        self.serial.write(data.encode())
    # refreshBtn ports
    def refresh_ports(self):
        self.comboBox.clear()
        self.load_ports()

    # recieve data from serial port into recieveDataLabel
    def receive_data(self):
        data = self.serial.read()
        self.receivedDataLabel.setText(data.decode().strip())

    #motor control
    def sliderChanged(self):
        # if value in motorSpeedLabel is greater than the value in the slider send '-' to the port
        if(self.motorSpeedSlider.value()==100):
            self.motor_access('+')
        else:
            self.motor_access(str(self.motorSpeedSlider.value()//10))    
        self.motorSpeedLabel.setText(str(self.motorSpeedSlider.value()))

    #readBtn
    def read(self):
        self.addressLine.setEnabled(True)
        self.valueLine.setEnabled(False)
        self.exeBtn.setEnabled(True)
        self.ramRBtn.setEnabled(True)
        self.romRBtn.setEnabled(True)
        self.modeOperation="r"

    #writeBtn
    def write(self):
        self.valueLine.setMaxLength(1)
        self.valueLine.setText('')
        self.valueLine.setEnabled(True)
        self.addressLine.setEnabled(True)
        self.exeBtn.setEnabled(True)
        self.ramRBtn.setEnabled(True)
        self.romRBtn.setEnabled(True)
        self.modeOperation="w"

    def send_to_memory(self, data):
        self.serial.write(data.encode())

    def memory_access(self, serviceMode,addressingElement, operation, address, value):
        # @[1] mode[1] addressingElement[1] operation[1] address[4] value[1] ;[1]
        # Ram-->0
        # Rom-->1
        # r,w

        self.send_to_memory('@') # @ or #
        self.send_to_memory(serviceMode) # memory or fan speed or led
        self.send_to_memory(addressingElement) # RAM or ROM
        self.send_to_memory(operation) # r or w
        self.send_to_memory(address) # address
        self.send_to_memory(value) #value
        self.send_to_memory(";") #end of command
        self.valueLine.setEnabled(False)
        self.addressLine.setEnabled(False)
        self.exeBtn.setEnabled(False)
        if operation=="r":
            try:
                self.valueLine.setText(self.serial.read().decode())
            except UnicodeDecodeError:
                self.valueLine.setMaxLength(18)
                self.valueLine.setText("Uninitialized byte")
        
    def led_access(self, operation):
        # @[1] mode[1] on/off[1] dummy[6] ;[1]
        self.send_to_memory('@') # @ or #
        self.send_to_memory('0') # memory or fan speed or led
        self.send_to_memory(operation) # 0 or 1
        self.send_to_memory("~~~~~~;") #end of command

    def motor_access(self, speed):
        
        # @[1] mode[1] speed[1] dummy[6] ;[1]
        self.send_to_memory('@') # @ or #
        self.send_to_memory('1') # memory or fan speed or led
        self.send_to_memory(speed) # 0 or 1
        self.send_to_memory("~~~~~~;") #end of command
    
    def wave_access(self, wave, amp, freq):
        # @[1] mode[1] wave[1] amp[3] freq[3] ;[1]
        self.send_to_memory('@')
        self.send_to_memory('3')
        self.send_to_memory(wave)
        self.send_to_memory(amp)
        self.send_to_memory(freq)
        self.send_to_memory(';')

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setEnabled(True)
        MainWindow.resize(390, 728)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.comboBox = QtWidgets.QComboBox(self.centralwidget)
        self.comboBox.setGeometry(QtCore.QRect(50, 80, 291, 31))
        self.comboBox.setObjectName("comboBox")
        self.connectBtn = QtWidgets.QPushButton(self.centralwidget)
        self.connectBtn.setGeometry(QtCore.QRect(150, 130, 93, 28))
        self.connectBtn.setObjectName("connectBtn")
        self.refreshBtn = QtWidgets.QPushButton(self.centralwidget)
        self.refreshBtn.setGeometry(QtCore.QRect(50, 130, 93, 28))
        self.refreshBtn.setObjectName("refreshBtn")
        self.connectionLabel = QtWidgets.QLabel(self.centralwidget)
        self.connectionLabel.setGeometry(QtCore.QRect(220, 40, 81, 16))
        self.connectionLabel.setObjectName("connectionLabel")
        self.connectionLabel.setStyleSheet("color: red")
        self.ledGroup = QtWidgets.QGroupBox(self.centralwidget)
        self.ledGroup.setEnabled(False)
        self.ledGroup.setGeometry(QtCore.QRect(50, 180, 291, 80))
        self.ledGroup.setObjectName("ledGroup")
        self.ledON = QtWidgets.QPushButton(self.ledGroup)
        self.ledON.setGeometry(QtCore.QRect(30, 30, 93, 28))
        self.ledON.setObjectName("ledON")
        self.ledOFF = QtWidgets.QPushButton(self.ledGroup)
        self.ledOFF.setGeometry(QtCore.QRect(170, 30, 93, 28))
        self.ledOFF.setObjectName("ledOFF")
        self.receivedDataGroup = QtWidgets.QGroupBox(self.centralwidget)
        self.receivedDataGroup.setEnabled(False)
        self.receivedDataGroup.setGeometry(QtCore.QRect(50, 280, 291, 80))
        self.receivedDataGroup.setObjectName("receivedDataGroup")
        self.receivedDataLabel = QtWidgets.QLabel(self.receivedDataGroup)
        self.receivedDataLabel.setGeometry(QtCore.QRect(10, 20, 131, 41))
        self.receivedDataLabel.setText("")
        self.receivedDataLabel.setObjectName("receivedDataLabel")
        self.receivedDataBtn = QtWidgets.QPushButton(self.receivedDataGroup)
        self.receivedDataBtn.setGeometry(QtCore.QRect(170, 30, 93, 28))
        self.receivedDataBtn.setObjectName("receivedDataBtn")
        self.fanSpeedGroup = QtWidgets.QGroupBox(self.centralwidget)
        self.fanSpeedGroup.setEnabled(False)
        self.fanSpeedGroup.setGeometry(QtCore.QRect(49, 370, 291, 101))
        self.fanSpeedGroup.setObjectName("fanSpeedGroup")
        self.motorSpeedSlider = QtWidgets.QSlider(self.fanSpeedGroup)
        self.motorSpeedSlider.setGeometry(QtCore.QRect(30, 30, 231, 22))
        self.motorSpeedSlider.setOrientation(QtCore.Qt.Horizontal)
        self.motorSpeedSlider.setTickPosition(QtWidgets.QSlider.TicksBothSides)
        self.motorSpeedSlider.setMinimum(0)
        self.motorSpeedSlider.setMaximum(100)
        self.motorSpeedSlider.setTickInterval(10)
        self.motorSpeedSlider.setObjectName("motorSpeedSlider")
        self.fanSpeedText = QtWidgets.QLabel(self.fanSpeedGroup)
        self.fanSpeedText.setGeometry(QtCore.QRect(30, 70, 55, 16))
        self.fanSpeedText.setObjectName("fanSpeedText")
        self.motorSpeedLabel = QtWidgets.QLabel(self.fanSpeedGroup)
        self.motorSpeedLabel.setGeometry(QtCore.QRect(90, 70, 55, 16))
        self.motorSpeedLabel.setText("")
        self.motorSpeedLabel.setObjectName("motorSpeedLabel")
        self.disconnectBtn = QtWidgets.QPushButton(self.centralwidget)
        self.disconnectBtn.setGeometry(QtCore.QRect(250, 130, 93, 28))
        self.disconnectBtn.setObjectName("disconnectBtn")
        self.memoryGroup = QtWidgets.QGroupBox(self.centralwidget)
        self.memoryGroup.setEnabled(False)
        self.memoryGroup.setGeometry(QtCore.QRect(50, 490, 291, 201))
        self.memoryGroup.setObjectName("memoryGroup")
        self.readBtn = QtWidgets.QPushButton(self.memoryGroup)
        self.readBtn.setGeometry(QtCore.QRect(30, 20, 93, 28))
        self.readBtn.setObjectName("readBtn")
        self.writeBtn = QtWidgets.QPushButton(self.memoryGroup)
        self.writeBtn.setGeometry(QtCore.QRect(160, 20, 93, 28))
        self.writeBtn.setObjectName("writeBtn")
        self.addressLine = QtWidgets.QLineEdit(self.memoryGroup)
        self.addressLine.setEnabled(False)
        self.addressLine.setGeometry(QtCore.QRect(70, 80, 131, 22))
        self.addressLine.setObjectName("addressLine")
        self.valueLine = QtWidgets.QLineEdit(self.memoryGroup)
        self.valueLine.setEnabled(False)
        self.valueLine.setGeometry(QtCore.QRect(70, 130, 131, 22))
        self.valueLine.setObjectName("valueLine")
        self.exeBtn = QtWidgets.QPushButton(self.memoryGroup)
        self.exeBtn.setGeometry(QtCore.QRect(100, 160, 93, 28))
        self.exeBtn.setObjectName("exeBtn")
        self.exeBtn.setEnabled(False)
        self.addressLab = QtWidgets.QLabel(self.memoryGroup)
        self.addressLab.setGeometry(QtCore.QRect(10, 70, 51, 41))
        self.addressLab.setObjectName("addressLab")
        self.valueLab = QtWidgets.QLabel(self.memoryGroup)
        self.valueLab.setGeometry(QtCore.QRect(10, 120, 41, 41))
        self.valueLab.setObjectName("valueLab")
        self.ramRBtn = QtWidgets.QRadioButton(self.memoryGroup)
        self.ramRBtn.setGeometry(QtCore.QRect(210, 90, 95, 20))
        self.ramRBtn.setChecked(True)
        self.ramRBtn.setObjectName("ramRBtn")
        self.romRBtn = QtWidgets.QRadioButton(self.memoryGroup)
        self.romRBtn.setGeometry(QtCore.QRect(210, 120, 95, 20))
        self.romRBtn.setObjectName("romRBtn")
        self.connectionStatusLabel = QtWidgets.QLabel(self.centralwidget)
        self.connectionStatusLabel.setGeometry(QtCore.QRect(50, 40, 111, 16))
        self.connectionStatusLabel.setObjectName("connectionStatusLabel")
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        self.load_ports()
        self.connectBtn.clicked.connect(self.connect)
        self.disconnectBtn.clicked.connect(self.disconnect)
        self.refreshBtn.clicked.connect(self.refresh_ports)
        self.receivedDataBtn.clicked.connect(self.receive_data)
        self.ledON.clicked.connect(lambda: self.led_access("1"))
        self.ledOFF.clicked.connect(lambda: self.led_access("0"))
        self.motorSpeedSlider.valueChanged.connect(self.sliderChanged)
        self.readBtn.clicked.connect(self.read)
        self.writeBtn.clicked.connect(self.write)
        self.addressLine.setValidator(QtGui.QRegExpValidator(QtCore.QRegExp("[A-F0-9]{1,4}")))
        self.addressLine.setMaxLength(4)
        self.valueLine.setMaxLength(1)
        #self.exeBtn.clicked.connect(lambda: self.exe('#' if self.ramRBtn.isChecked() else '@',self.modeOperation,self.addressLine.text(),'~' if self.valueLine.text() == '' else self.valueLine.text()))
        self.exeBtn.clicked.connect(lambda: self.memory_access('2','0' if self.ramRBtn.isChecked() else '1',self.modeOperation,self.addressLine.text(),'~' if self.valueLine.text() == '' else self.valueLine.text()))


    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "PC Memory"))
        self.connectBtn.setText(_translate("MainWindow", "Connect"))
        self.refreshBtn.setText(_translate("MainWindow", "Refresh"))
        self.connectionLabel.setText(_translate("MainWindow", "No connection"))
        self.ledGroup.setTitle(_translate("MainWindow", "LED"))
        self.ledON.setText(_translate("MainWindow", "ON"))
        self.ledOFF.setText(_translate("MainWindow", "OFF"))
        self.receivedDataGroup.setTitle(_translate("MainWindow", "Received data"))
        self.receivedDataBtn.setText(_translate("MainWindow", "Receive"))
        self.fanSpeedGroup.setTitle(_translate("MainWindow", "Fan speed"))
        self.fanSpeedText.setText(_translate("MainWindow", "Speed:"))
        self.disconnectBtn.setText(_translate("MainWindow", "Disconnect"))
        self.memoryGroup.setTitle(_translate("MainWindow", "Memory"))
        self.readBtn.setText(_translate("MainWindow", "Read"))
        self.writeBtn.setText(_translate("MainWindow", "Write"))
        self.exeBtn.setText(_translate("MainWindow", "Execute"))
        self.addressLab.setText(_translate("MainWindow", "Address"))
        self.ramRBtn.setText(_translate("MainWindow", "RAM"))
        self.romRBtn.setText(_translate("MainWindow", "ROM"))
        self.valueLab.setText(_translate("MainWindow", "Value"))
        self.connectionStatusLabel.setText(_translate("MainWindow", "Connection status:"))

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())