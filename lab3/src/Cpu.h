#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"
#include <optional>

class Cpu
{
public:
    Cpu(IMem& mem)
        : _mem(mem)
    {
        status = Status::Begin;
    }
    void Clock()
    {
        _csrf.Clock();
        if (status == Status::Begin){
            _mem.Request(_ip);
            status = Status::Read; 
        }
        if (status == Status::Read){
            instr=_mem.Response();
            if(!instr.has_value())
                return;
            cur_instr=_decoder.Decode(instr.value());
            _rf.Read(cur_instr);
            _csrf.Read(cur_instr);
            _exe.Execute(cur_instr,_ip);
            _mem.Request(cur_instr);
            status = Status::Write;
        }
        if (status == Status::Write){
            if(!_mem.Response(cur_instr))
                return;
            _rf.Write(cur_instr);
            _csrf.Write(cur_instr);
            _csrf.InstructionExecuted();
            _ip=cur_instr->_nextIp;
            status = Status::Begin;
        }
    }

    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }

    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }

private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    IMem& _mem;

    InstructionPtr cur_instr;
    std::optional<Word> instr;
    enum Status{
        Begin,
        Read,
        Write,
    }status;
    // Add your code here, if needed
};


#endif //RISCV_SIM_CPU_H
