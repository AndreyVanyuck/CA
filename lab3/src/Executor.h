#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        switch(instr->_type)
        {
            case IType::Alu: {
                instr->_data = get_alu_res(instr);
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Ld:
            {
                instr->_addr = get_alu_res(instr);
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::St:
            {
                instr->_addr = get_alu_res(instr);
                instr->_data = instr->_src2Val;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Csrw:
            {
                instr->_data = instr->_src1Val;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Csrr:
            {
                instr->_data = instr->_csrVal;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::J:
            {
                instr->_data = ip + 4;
            }
            case IType::Br:
            {
                if (get_br_res(instr))
                    instr->_nextIp = ip + *instr->_imm;
                else
                    instr->_nextIp = ip + 4;
                break;
            }
            case IType::Jr:
            {
                instr->_data = ip + 4;
                if (get_br_res(instr))
                    instr->_nextIp = *instr->_imm + instr->_src1Val;
                else
                    instr->_nextIp = ip + 4;
                break;
            }
            case IType::Auipc:
            {
                instr->_data = ip + *instr->_imm;
                instr->_nextIp = ip + 4;
                break;
            }
        }
    }

private:
    Word get_alu_res (InstructionPtr& instr)
    {
        Word A, B;
        bool is_valid = true;

        if (instr->_src1.has_value())
            A = instr->_src1Val;
        else
            is_valid = false;

        if (instr->_imm.has_value())
            B = *instr->_imm;
        else if (instr->_src2.has_value())
            B = instr->_src2Val;
        else
            is_valid = false;

        if (is_valid) {
            switch (instr->_aluFunc)
            {
                case AluFunc::Add:
                    return A + B;
                case AluFunc::Sub:
                    return A - B;
                case AluFunc::And:
                    return A & B;
                case AluFunc::Or:
                    return A | B;
                case AluFunc::Xor:
                    return A ^ B;
                case AluFunc::Slt:
                    return (int)A < (int)B;
                case AluFunc::Sltu:
                    return A < B;
                case AluFunc::Sll:
                    return A << (B % 32);
                case AluFunc::Srl:
                    return A >> (B % 32);
                case AluFunc::Sra:
                    return (int)A >> (B % 32);
            }
        }
        return 0;
    }

    bool get_br_res(InstructionPtr& instr)
    {
        Word A, B;
        if (instr->_src1)
            A = instr->_src1Val;
        if (instr->_src2)
            B = instr->_src2Val;

        switch (instr->_brFunc){
            case BrFunc :: Eq:
                return A == B;
            case BrFunc :: Neq:
                return A != B;
            case BrFunc :: Lt:
                return (int)A < (int)B;
            case BrFunc :: Ltu:
                return A < B;
            case BrFunc :: Ge:
                return (int)A >= (int)B;
            case BrFunc :: Geu:
                return A>=B;
            case BrFunc :: AT:
                return true;
            case BrFunc :: NT:
                return false;
        }
        return 0;
    }
};

#endif // RISCV_SIM_EXECUTOR_H