#include "arduino.h"
#define arduino_uno_vendor_id  0x2341  // Vendor ID for Arduino Uno (can vary depending on the model)
#define arduino_uno_product_id 0x0043  // Product ID for Arduino Uno (can vary depending on the model)

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;

}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
    return serial;
}

int Arduino::connect_arduino()
{
    // Search for the port where the Arduino board is connected
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }
    qDebug() << "arduino_port_name is :" << arduino_port_name;

    if (arduino_is_available) { // Configure communication settings (baud rate, etc.)
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600); // Baud rate: 9600 bps
            serial->setDataBits(QSerialPort::Data8);    // Data length: 8 bits
            serial->setParity(QSerialPort::NoParity);   // No parity bit
            serial->setStopBits(QSerialPort::OneStop);  // 1 stop bit
            serial->setFlowControl(QSerialPort::NoFlowControl); // No flow control
            return 0; // Success
        }
        return 1; // Failed to open the port
    }
    return -1; // Arduino not found
}

int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0; // Success
    }
    return 1; // Failed to close the port
}


QByteArray Arduino::read_from_arduino() {
    static QByteArray buffer;
    const int MAX_BUFFER_SIZE = 1024;

    if (serial->isReadable()) {
        QByteArray newData = serial->readAll();
        buffer.append(newData);  // Append to the buffer

        // Prevent buffer overflow
        if (buffer.size() > MAX_BUFFER_SIZE) {
            buffer.clear();
            qDebug() << "[WARNING] Buffer size exceeded. Clearing buffer.";
        }

        // Log raw data to debug
        qDebug() << "Raw data: " << newData.toHex();

        // Look for end of a complete message (you can adjust this for your case)
        int endIdx = buffer.indexOf("\n");  // Looking for newline
        if (endIdx != -1) {
            QByteArray completeData = buffer.left(endIdx);
            buffer.remove(0, endIdx + 1);
            return completeData.trimmed();  // Return complete message
        }
    }

    return QByteArray();  // Return empty if no complete message found
}





int Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d);  // Send data to Arduino
        return 0;           // Success
    } else {
        qDebug() << "Couldn't write to serial!";
        return 1;           // Failure
    }
}
