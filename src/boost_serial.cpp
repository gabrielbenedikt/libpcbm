/*
 * File:   Boost_serial.cpp
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 * Created on September 12, 2009, 3:47 PM
 *
 * v1.06: C++11 support
 *
 * v1.05: Fixed a bug regarding reading after a timeout (again).
 *
 * v1.04: Fixed bug with timeout set to zero
 *
 * v1.03: Fix for Mac OS X, now fully working on Mac.
 *
 * v1.02: Code cleanup, speed improvements, bug fixes.
 *
 * v1.01: Fixed a bug that caused errors while reading after a timeout.
 *
 * v1.00: First release.
 */

#include "boost_serial.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

Boost_serial::Boost_serial(): io(), port(io), timer(io),
        timeout(boost::posix_time::seconds(0)) {}

Boost_serial::Boost_serial(const std::string& devname, unsigned int baud_rate,
        asio::serial_port_base::parity opt_parity,
        asio::serial_port_base::character_size opt_csize,
        asio::serial_port_base::flow_control opt_flow,
        asio::serial_port_base::stop_bits opt_stop)
        : io(), port(io), timer(io), timeout(boost::posix_time::seconds(0))
{
    open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}

void Boost_serial::open(const std::string& devname, unsigned int baud_rate,
        asio::serial_port_base::parity opt_parity,
        asio::serial_port_base::character_size opt_csize,
        asio::serial_port_base::flow_control opt_flow,
        asio::serial_port_base::stop_bits opt_stop)
{
    if(isOpen()) close();
    port.open(devname);
    port.set_option(asio::serial_port_base::baud_rate(baud_rate));
    port.set_option(opt_parity);
    port.set_option(opt_csize);
    port.set_option(opt_flow);
    port.set_option(opt_stop);
}

bool Boost_serial::isOpen() const
{
    return port.is_open();
}

void Boost_serial::close()
{
    if(isOpen()==false) return;
    port.close();
}

void Boost_serial::setTimeout(const boost::posix_time::time_duration& t)
{
    timeout=t;
}

void Boost_serial::write(const char *data, size_t size)
{
    asio::write(port,asio::buffer(data,size));
}

void Boost_serial::write(const std::vector<char>& data)
{
    asio::write(port,asio::buffer(&data[0],data.size()));
}

void Boost_serial::writeString(const std::string& s)
{
    asio::write(port,asio::buffer(s.c_str(),s.size()));
}

void Boost_serial::read(char *data, size_t size)
{
    if(readData.size()>0)//If there is some data from a previous read
    {
        istream is(&readData);
        size_t toRead=min(readData.size(),size);//How many bytes to read?
        is.read(data,toRead);
        data+=toRead;
        size-=toRead;
        if(size==0) return;//If read data was enough, just return
    }

    setupParameters=ReadSetupParameters(data,size);
    performReadSetup(setupParameters);

    //For this code to work, there should always be a timeout, so the
    //request for no timeout is translated into a very long timeout
    if(timeout!=boost::posix_time::seconds(0)) timer.expires_from_now(timeout);
    else timer.expires_from_now(boost::posix_time::hours(100000));

    timer.async_wait(boost::bind(&Boost_serial::timeoutExpired,this,
                asio::placeholders::error));

    result=resultInProgress;
    bytesTransferred=0;
    for(;;)
    {
        io.run_one();
        switch(result)
        {
            case resultSuccess:
                timer.cancel();
                return;
            case resultTimeoutExpired:
                port.cancel();
                throw(timeout_exception("Timeout expired"));
            case resultError:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            case resultInProgress:
                ;
                //if resultInProgress remain in the loop
        }
    }
}

std::vector<char> Boost_serial::read(size_t size)
{
    vector<char> result(size,'\0');//Allocate a vector with the desired size
    read(&result[0],size);//Fill it with values
    return result;
}

std::string Boost_serial::readString(size_t size)
{
    string result(size,'\0');//Allocate a string with the desired size
    read(&result[0],size);//Fill it with values
    return result;
}

std::string Boost_serial::readStringUntil(const std::string& delim)
{
    // Note: if readData contains some previously read data, the call to
    // async_read_until (which is done in performReadSetup) correctly handles
    // it. If the data is enough it will also immediately call readCompleted()
    setupParameters=ReadSetupParameters(delim);
    performReadSetup(setupParameters);

    //For this code to work, there should always be a timeout, so the
    //request for no timeout is translated into a very long timeout
    if(timeout!=boost::posix_time::seconds(0)) timer.expires_from_now(timeout);
    else timer.expires_from_now(boost::posix_time::hours(100000));

    timer.async_wait(boost::bind(&Boost_serial::timeoutExpired,this,
                asio::placeholders::error));

    result=resultInProgress;
    bytesTransferred=0;
    for(;;)
    {
        io.run_one();
        switch(result)
        {
            case resultSuccess:
                {
                    timer.cancel();
                    bytesTransferred-=delim.size();//Don't count delim
                    istream is(&readData);
                    string result(bytesTransferred,'\0');//Alloc string
                    is.read(&result[0],bytesTransferred);//Fill values
                    is.ignore(delim.size());//Remove delimiter from stream
                    return result;
                }
            case resultTimeoutExpired:
                port.cancel();
                throw(timeout_exception("Timeout expired"));
            case resultError:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            case resultInProgress:
                ;
                //if resultInProgress remain in the loop
        }
    }
}

Boost_serial::~Boost_serial() {}

void Boost_serial::performReadSetup(const ReadSetupParameters& param)
{
    if(param.fixedSize)
    {
        asio::async_read(port,asio::buffer(param.data,param.size),boost::bind(
                &Boost_serial::readCompleted,this,asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    } else {
        asio::async_read_until(port,readData,param.delim,boost::bind(
                &Boost_serial::readCompleted,this,asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    }
}

void Boost_serial::timeoutExpired(const boost::system::error_code& error)
{
     if(!error && result==resultInProgress) result=resultTimeoutExpired;
}

void Boost_serial::readCompleted(const boost::system::error_code& error,
        const size_t bytesTransferred)
{
    if(!error)
    {
        result=resultSuccess;
        this->bytesTransferred=bytesTransferred;
        return;
    }

    //In case a asynchronous operation is cancelled due to a timeout,
    //each OS seems to have its way to react.
    #ifdef _WIN32
    if(error.value()==995) return; //Windows spits out error 995
    #elif defined(__APPLE__)
    if(error.value()==45)
    {
        //Bug on OS X, it might be necessary to repeat the setup
        //http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
        performReadSetup(setupParameters);
        return;
    }
    #else //Linux
    if(error.value()==125) return; //Linux outputs error 125
    #endif

    result=resultError;
}
