
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"

class Cpu
{
public:
    Cpu(Memory& mem)
        : _mem(mem)
    {

    }

    void ProcessInstruction()
    {
        auto word_instraction = _mem.Request(_ip);
        auto instaction = _decoder.Decode(word_instraction);
        _rf.Read(instaction);
        _csrf.Read(instaction);
        _exe.Execute(instaction, _ip);
        _mem.Request(instaction);
        _rf.Write(instaction);
        _csrf.Write(instaction);
        _csrf.InstructionExecuted();
        _ip = instaction->_nextIp;
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
    Memory& _mem;
};


#endif //RISCV_SIM_CPU_H
