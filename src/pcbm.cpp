#include "pcbm.h"

/************************************************************************************************
*                                PCBMotor::PCBMotor                                               *
************************************************************************************************/
PCBMotor::PCBMotor(std::string devname)
{
    bserial = new Boost_serial(devname, 19200,
                               boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                               boost::asio::serial_port_base::character_size(8),
                               boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                               boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    bserial->setTimeout(boost::posix_time::seconds(5));

    open(devname);
}

/************************************************************************************************
*                                PCBMotor::~PCBMotor                                               *
************************************************************************************************/
PCBMotor::~PCBMotor()
{
    close();
}

/************************************************************************************************
*                                PCBMotor::home                                         *
************************************************************************************************/
void PCBMotor::home(uint8_t mnum)
{ 
    std::string cmd = "M" + std::to_string(mnum) + ",s-2880\r";
    write(cmd.data());
}
void PCBMotor::home(std::vector<uint8_t> mnum)
{ 
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M" + std::to_string(mnum[i]) + ",s-2880");
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::stop                                         *
************************************************************************************************/
void PCBMotor::stop(uint8_t mnum)
{ 
    std::string cmd = "M" + std::to_string(mnum) + ",\r";
    write(cmd.data());
}
void PCBMotor::stop(std::vector<uint8_t> mnum)
{ 
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd = "M" + std::to_string(mnum[i]) + ",\r";
        write(cmd.data());
    }
}

/************************************************************************************************
*                                PCBMotor::info                                         *
************************************************************************************************/
void PCBMotor::info()
{
    write("i\r");
}

/************************************************************************************************
*                                PCBMotor::help                                         *
************************************************************************************************/
void PCBMotor::help()
{
    write("h\r");
}

/************************************************************************************************
*                            PCBMotor::frequency_sweep                                  *
************************************************************************************************/
void PCBMotor::frequency_sweep(uint8_t mnum)
{
    std::string cmd = "M" + std::to_string(mnum) + ",p\r";
    write(cmd.data());
}
void PCBMotor::frequency_sweep(std::vector<uint8_t> mnum)
{
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M" + std::to_string(mnum[i]) + ",p");
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}
/************************************************************************************************
*                             PCBMotor::micropulse                                       *
************************************************************************************************/
void PCBMotor::micropulse(uint8_t mnum, int32_t nummpulses, std::string edge)
{
    std::transform(edge.begin(), edge.end(), edge.begin(), [](unsigned char c){ return std::tolower(c); });

    std::string basecmd = "";
    if (edge.compare("p")) {
        basecmd = "UP";
    } else if (edge.compare("n")) {
        basecmd = "UN";
    } else {
        basecmd = "U";
    }
    std::string cmd = "M"+std::to_string(mnum)+","+basecmd+std::to_string(nummpulses)+"\r";
    write(cmd.data());
}
void PCBMotor::micropulse(std::vector<uint8_t> mnum, std::vector<int32_t> nummpulses, std::vector<std::string> edge)
{
    std::string cmd = "";
    std::string basecmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        std::transform(edge[i].begin(), edge[i].end(), edge[i].begin(), [](unsigned char c){ return std::tolower(c); });
        if (edge[i].compare("p")) {
            basecmd = "UP";
        } else if (edge[i].compare("n")) {
            basecmd = "UN";
        } else {
            basecmd = "U";
        }
        cmd.append("M" + std::to_string(mnum[i]) + "," + basecmd + std::to_string(nummpulses[i]));
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                             PCBMotor::sweeppulse                                       *
************************************************************************************************/
void PCBMotor::sweeppulse(uint8_t mnum, int32_t nummpulses, std::string edge)
{
    std::transform(edge.begin(), edge.end(), edge.begin(), [](unsigned char c){ return std::tolower(c); });

    std::string basecmd = "";
    if (edge.compare("p")) {
        basecmd = "UPS";
    } else if (edge.compare("n")) {
        basecmd = "UNS";
    } else {
        basecmd = "US";
    }
    std::string cmd = "M"+std::to_string(mnum)+","+basecmd+std::to_string(nummpulses)+"\r";
    write(cmd.data());
}
void PCBMotor::sweeppulse(std::vector<uint8_t> mnum, std::vector<int32_t> nummpulses, std::vector<std::string> edge)
{
    std::string cmd = "";
    std::string basecmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        std::transform(edge[i].begin(), edge[i].end(), edge[i].begin(), [](unsigned char c){ return std::tolower(c); });
        if (edge[i].compare("p")) {
            basecmd = "UPS";
        } else if (edge[i].compare("n")) {
            basecmd = "UNS";
        } else {
            basecmd = "US";
        }
        cmd.append("M" + std::to_string(mnum[i]) + "," + basecmd + std::to_string(nummpulses[i]));
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                             PCBMotor::sweepstep                                       *
************************************************************************************************/
void PCBMotor::sweepstep(uint8_t mnum, int32_t numsteps)
{
    std::string cmd = "M" + std::to_string(mnum)+",SW" + std::to_string(numsteps)+"\r";
    write(cmd.data());
}
void PCBMotor::sweepstep(std::vector<uint8_t> mnum, std::vector<int32_t> numsteps)
{
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M" + std::to_string(mnum[i]) + ",SW" + std::to_string(numsteps[i]));
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                              PCBMotor::moveabsolute                                       *
************************************************************************************************/
void PCBMotor::moveabsolute(uint8_t mnum, double ang)
{
    if (std::abs(ang > 4096)) {
        throw std::invalid_argument("angle needs to be < 4096deg");
    }
    std::string cmd = "M"+std::to_string(mnum)+",s-2880,s"+std::to_string(ang*8)+"\r";
    write(cmd.data());
}

void PCBMotor::moveabsolute(std::vector<uint8_t> mnum, std::vector<double> ang)
{
    if (std::any_of(ang.cbegin(), ang.cend(), [](double i){ return i > 4096; })) {
        throw std::invalid_argument("angle needs to be < 4096deg");
    }
    std::string cmd;
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",s-2880,s"+std::to_string(ang[i]*8));
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                              PCBMotor::goto_pos                                       *
************************************************************************************************/
void PCBMotor::goto_pos(uint8_t mnum, double ang, bool restrack)
{
    while (ang > 360) {
        ang -= 360;
    }
    std::string basecmd = restrack ? "G" : "GN";
    std::string cmd = "M"+std::to_string(mnum)+","+ basecmd+std::to_string(ang*8)+"\r";
    write(cmd.data());
}

void PCBMotor::goto_pos(std::vector<uint8_t> mnum, std::vector<double> ang, std::vector<bool> restrack)
{
    std::string cmd;
    std::string basecmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        while (ang[i] > 360) {
            ang[i] -= 360;
        }
        basecmd = restrack[i] ? "G" : "GN";
        cmd.append("M"+std::to_string(mnum[i])+","+basecmd+std::to_string(ang[i]*8));
        if (!((i+1)==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::moverelative                                        *
************************************************************************************************/
void PCBMotor::moverelative(uint8_t mnum, double ang)
{
    if (std::abs(ang > 4096)) {
        throw std::invalid_argument("angle needs to be < 4096deg");
    }
    std::string cmd = "M"+std::to_string(mnum)+",s"+std::to_string(ang*8)+"\r";
    write(cmd.data());
}
void PCBMotor::moverelative(std::vector<uint8_t> mnum, std::vector<double> ang)
{
    std::string cmd;
    
    if (std::any_of(ang.cbegin(), ang.cend(), [](double i){ return i > 4096; })) {
        throw std::invalid_argument("angle needs to be < 4096deg");
    }
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",s"+std::to_string(ang[i]*8));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::set_voltage                                        *
************************************************************************************************/
void PCBMotor::set_voltage(uint8_t mnum, uint16_t mV)
{
    if (mV > 5000) {
        throw std::invalid_argument("Voltage needs to be 0...5000mV");
    } 
    std::string cmd = "M"+std::to_string(mnum)+",V"+std::to_string(mV)+"\r";
    write(cmd.data());
}
void PCBMotor::set_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV)
{
    for (uint16_t v : mV) {
        if (v > 5000) {
            throw std::invalid_argument("Voltage needs to be 0...5000mV");
        } 
    }
    std::string cmd;
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",V"+std::to_string(mV[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_voltage                                        *
************************************************************************************************/
uint16_t PCBMotor::get_voltage(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",V\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process and return result
    uint16_t mV = 0;
    return mV;
}
std::vector<uint16_t> PCBMotor::get_voltage(std::vector<uint8_t> mnum)
{
    std::vector<uint16_t> mV_vec;
    std::string cmd;
    for (uint8_t m : mnum) {
        cmd = "M"+std::to_string(m)+",V\r";
        write(cmd.data());
        std::string res = read();
        //TODO: process and return result
        uint16_t mV = 0;
        mV_vec.push_back(mV);
    }
    return mV_vec;
}

/************************************************************************************************
*                                PCBMotor::set_min_voltage                                        *
************************************************************************************************/
void PCBMotor::set_min_voltage(uint8_t mnum, uint16_t mV)
{
    if (mV > 5000) {
        throw std::invalid_argument("Voltage needs to be 0...5000mV");
    } 
    std::string cmd = "M"+std::to_string(mnum)+",Vmin"+std::to_string(mV)+"\r";
    write(cmd.data());
}
void PCBMotor::set_min_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV)
{
    for (uint16_t v : mV) {
        if (v > 5000) {
            throw std::invalid_argument("Voltage needs to be 0...5000mV");
        } 
    }
    std::string cmd;
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",Vmin"+std::to_string(mV[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_min_voltage                                        *
************************************************************************************************/
uint16_t PCBMotor::get_min_voltage(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",Vmin\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process and return result
    uint16_t mV = 0;
    return mV;
}
std::vector<uint16_t> PCBMotor::get_min_voltage(std::vector<uint8_t> mnum)
{
    std::vector<uint16_t> mV_vec;
    std::string cmd;
    for (uint8_t m : mnum) {
        cmd = "M"+std::to_string(m)+",Vmin\r";
        write(cmd.data());
        std::string res = read();
        //TODO: process and return result
        uint16_t mV = 0;
        mV_vec.push_back(mV);
    }
    return mV_vec;
}

/************************************************************************************************
*                                PCBMotor::set_max_voltage                                        *
************************************************************************************************/
void PCBMotor::set_max_voltage(uint8_t mnum, uint16_t mV)
{
    if (mV > 5000) {
        throw std::invalid_argument("Voltage needs to be 0...5000mV");
    } 
    std::string cmd = "M"+std::to_string(mnum)+",Vmax"+std::to_string(mV)+"\r";
    write(cmd.data());
}
void PCBMotor::set_max_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV)
{
    for (uint16_t v : mV) {
        if (v > 5000) {
            throw std::invalid_argument("Voltage needs to be 0...5000mV");
        } 
    }
    std::string cmd;
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",Vmax"+std::to_string(mV[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_max_voltage                                        *
************************************************************************************************/
uint16_t PCBMotor::get_max_voltage(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",Vmax\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process and return result
    uint16_t mV = 0;
    return mV;
}
std::vector<uint16_t> PCBMotor::get_max_voltage(std::vector<uint8_t> mnum)
{
    std::vector<uint16_t> mV_vec;
    std::string cmd;
    for (uint8_t m : mnum) {
        cmd = "M"+std::to_string(m)+",Vmax\r";
        write(cmd.data());
        std::string res = read();
        //TODO: process and return result
        uint16_t mV = 0;
        mV_vec.push_back(mV);
    }
    return mV_vec;
}

/************************************************************************************************
*                                PCBMotor::optimize_sensor_current                                        *
************************************************************************************************/
void PCBMotor::optimize_sensor_current(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",LS\r";
    write(cmd.data());
}
void PCBMotor::optimize_sensor_current(std::vector<uint8_t> mnum)
{
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",LS");
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::set_sensor_current                                        *
************************************************************************************************/
void PCBMotor::set_sensor_current(uint8_t mnum, uint8_t cur)
{
    if (cur > 200) {
        throw std::invalid_argument("Sensor current needs to be 0...200");
    } 
    std::string cmd = "M"+std::to_string(mnum)+",L"+std::to_string(cur)+"\r";
    write(cmd.data());
}
void PCBMotor::set_sensor_current(std::vector<uint8_t> mnum, std::vector<uint8_t> cur)
{
    for (uint16_t c : cur) {
        if (c > 200) {
            throw std::invalid_argument("Sensor current needs to be 0...200");
        } 
    }
    std::string cmd = "";
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",L"+std::to_string(cur[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_sensor_current                                        *
************************************************************************************************/
uint8_t PCBMotor::get_sensor_current(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",L\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process and return result
    uint8_t mV = 0;
    return mV;
}
std::vector<uint8_t> PCBMotor::get_sensor_current(std::vector<uint8_t> mnum)
{
    std::vector<uint8_t> c_vec;
    std::string cmd;
    for (uint8_t m : mnum) {
        cmd = "M"+std::to_string(m)+",L\r";
        write(cmd.data());
        std::string res = read();
        //TODO: process and return result
        uint8_t c = 0;
        c_vec.push_back(c);
    }
    return c_vec;
}

/************************************************************************************************
*                                PCBMotor::resonance_sweep                                        *
************************************************************************************************/
void PCBMotor::resonance_sweep(uint8_t mnum, std::string mode)
{
    std::string basecmd = "P";
    if (mode.compare("reverse")) {
        basecmd = "PR";
    } else if (mode.compare("alternating")) {
        basecmd = "PA";
    } 
    
    std::string cmd = "M"+std::to_string(mnum)+","+basecmd+"\r";
    write(cmd.data());
}
void PCBMotor::resonance_sweep(std::vector<uint8_t> mnum, std::vector<std::string> mode)
{
    std::string cmd = "";
    std::string basecmd = "P";
    for (size_t i=0; i<mnum.size(); ++i) {
        if (mode[i].compare("reverse")) {
            basecmd = "PR";
        } else if (mode[i].compare("alternating")) {
            basecmd = "PA";
        } 
        cmd.append("M"+std::to_string(mnum[i])+","+basecmd);
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::set_voltage_scaling                                        *
************************************************************************************************/
void PCBMotor::set_voltage_scaling(uint8_t mnum, std::string dir, uint8_t scale)
{
    std::string basecmd = "";
    if (dir.compare("fwd")) {
        basecmd = "CW";
    } else if (dir.compare("bwd")) {
        basecmd = "CCW";
    } else {
        throw std::invalid_argument("direction needs to be fwd or bwd.");
    }
    
    std::string cmd = "M"+std::to_string(mnum)+","+basecmd+std::to_string(scale)+"\r";
    write(cmd.data());
}
void PCBMotor::set_voltage_scaling(std::vector<uint8_t> mnum, std::vector<std::string> dir, std::vector<uint8_t> scale)
{
    std::string cmd = "";
    std::string basecmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        if (dir[i].compare("fwd")) {
            basecmd = "CW";
        } else if (dir[i].compare("bwd")) {
            basecmd = "CCW";
        } else {
            throw std::invalid_argument("direction needs to be fwd or bwd.");
        }
        cmd.append("M"+std::to_string(mnum[i])+","+basecmd+std::to_string(scale[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_voltage_scaling                                        *
************************************************************************************************/
uint8_t PCBMotor::get_voltage_scaling(uint8_t mnum, std::string dir)
{
        std::string basecmd = "";
    if (dir.compare("fwd")) {
        basecmd = "CW";
    } else if (dir.compare("bwd")) {
        basecmd = "CCW";
    } else {
        throw std::invalid_argument("direction needs to be fwd or bwd.");
    }
    
    std::string cmd = "M"+std::to_string(mnum)+","+basecmd+"\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process response
    uint8_t scale = 0;
    
    return scale;
}
std::vector<uint8_t> PCBMotor::get_voltage_scaling(std::vector<uint8_t> mnum, std::vector<std::string> dir)
{
    std::string cmd = "";
    std::string basecmd = "";
    std::string res = "";
    uint8_t scale = 0;
    std::vector<uint8_t> scale_vec = std::vector<uint8_t>();
    for (size_t i=0; i<mnum.size(); ++i) {
        if (dir[i].compare("fwd")) {
            basecmd = "CW";
        } else if (dir[i].compare("bwd")) {
            basecmd = "CCW";
        } else {
            throw std::invalid_argument("direction needs to be fwd or bwd.");
        }
        cmd = "M"+std::to_string(mnum[i])+","+basecmd + "\r";
        write(cmd.data());
        res = read();
        //TODO: process result
        scale = 0;
        scale_vec.push_back(scale);
    }
    return scale_vec;
}

/************************************************************************************************
*                                PCBMotor::backup_memort                                        *
************************************************************************************************/
void PCBMotor::backup_memory(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",RB\r";
    write(cmd.data());
}
void PCBMotor::backup_memory(std::vector<uint8_t> mnum)
{
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",RB");
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::restore_memort                                        *
************************************************************************************************/
void PCBMotor::restore_memory(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",RS\r";
    write(cmd.data());
}
void PCBMotor::restore_memory(std::vector<uint8_t> mnum)
{
    std::string cmd = "";
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",RS");
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::set_voltage                                        *
************************************************************************************************/
void PCBMotor::set_vco_freq(uint8_t mnum, uint32_t Hz)
{
    if (Hz > 125000) {
        throw std::invalid_argument("Frequency needs to be 0...125000Hz");
    } 
    std::string cmd = "M"+std::to_string(mnum)+",F"+std::to_string(Hz)+"\r";
    write(cmd.data());
}
void PCBMotor::set_vco_freq(std::vector<uint8_t> mnum, std::vector<uint32_t> Hz)
{
    for (uint32_t h : Hz) {
        if (h > 125000) {
            throw std::invalid_argument("Frequency needs to be 0...125000Hz");
        } 
    }
    std::string cmd;
    
    for (size_t i=0; i<mnum.size(); ++i) {
        cmd.append("M"+std::to_string(mnum[i])+",F"+std::to_string(Hz[i]));
        if (!(i+1==mnum.size())) {
            cmd.append(",");
        }
    }
    cmd.append("\r");
    write(cmd.data());
}

/************************************************************************************************
*                                PCBMotor::get_voltage                                        *
************************************************************************************************/
uint32_t PCBMotor::get_vco_freq(uint8_t mnum)
{
    std::string cmd = "M"+std::to_string(mnum)+",F\r";
    write(cmd.data());
    std::string res = read();
    //TODO: process and return result
    uint32_t Hz = 0;
    return Hz;
}
std::vector<uint32_t> PCBMotor::get_vco_freq(std::vector<uint8_t> mnum)
{
    std::vector<uint32_t> Hz_vec;
    std::string cmd;
    for (uint8_t m : mnum) {
        cmd = "M"+std::to_string(m)+",F\r";
        write(cmd.data());
        std::string res = read();
        //TODO: process and return result
        uint32_t Hz = 0;
        Hz_vec.push_back(Hz);
    }
    return Hz_vec;
}
