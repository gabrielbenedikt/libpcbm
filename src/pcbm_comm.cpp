#include "pcbm.h"

std::string PCBMotor::read()
{
    std::string response = bserial->readStringUntil("\r\n");
    return response;
}

void PCBMotor::write(const std::string &data)
{
    bserial->writeString(data);
}

std::string PCBMotor::query(const std::string &data) {
    bserial->writeString(data);
    std::string response = bserial->readStringUntil("\r\n");
    return response;
}

void PCBMotor::close()
{
    if (bserial->isOpen())
        bserial->close();
}

bool PCBMotor::isopen()
{
    return bserial->isOpen();
}

void PCBMotor::open(std::string port) {
    if (!bserial->isOpen()) {
        try {
             bserial->open(port, 9600,
                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                           boost::asio::serial_port_base::character_size(8),
                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
         }  catch (std::exception & ex) {
             std::cout << ex.what() << std::endl;
         }
    }
}
