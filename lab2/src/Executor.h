
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip){
        if(instr->_src1.has_value() && (instr->_imm.has_value() || instr->_src2.has_value())){
            Word second_arg = instr->_imm.value_or(instr->_src2Val);
            Word result_alu_func = get_result_alu_func(instr);        

            switch (instr->_type) {
                case IType::Alu:
                    instr->_data = result_alu_func; 
                    break;
                case IType::St:
                    instr->_data = instr->_src2Val; 
                    instr->_addr = result_alu_func;  
                    break;
                case IType::Ld:
                    instr->_addr = result_alu_func;  
                    break;
            }
        }
        switch (instr->_type) {
                case IType::Csrr:
                    instr->_data = instr->_csrVal;  
                    break;
                case IType::Csrw:
                    instr->_data = instr->_src1Val;  
                    break;
                case IType::J:
                    instr->_data = ip + 4; 
                    break;
                case IType::Jr:
                    instr->_data = ip + 4; 
                    break;
                case IType::Auipc:
                    instr->_data = ip + instr->_imm.value(); 
                    break;
            }

        Word next_addr;
        bool result_br_func = false;
        if(instr->_src1.has_value() && instr->_src2.has_value()){
            switch (instr->_type) {
                case IType::Br:
                    next_addr = ip + instr->_imm.value();
                    break;
                case IType::J:
                    next_addr = ip + instr->_imm.value();
                    break;
                case IType::Jr:
                    next_addr = instr->_src1Val + instr->_imm.value();
                    break;
            }
            bool (*get_result_br_func)(Word, Word);                   // указатель на выбранную функцию
            get_result_br_func = select_br_func((int)instr->_brFunc);
            auto result_br_func = get_result_br_func(instr->_src1Val, instr->_src2Val);
        }
        
        if(result_br_func)
            instr->_nextIp = next_addr;
        else
            instr->_nextIp = ip + 4;
    }

private:
    Word get_result_alu_func(InstructionPtr& instr){ 
        Word A = instr->_src1Val;  
        Word B = instr->_imm.value_or(instr->_src2Val);
        
        switch (instr->_aluFunc){
            case AluFunc::Add: {
                return A + B;
            }
            case AluFunc::Sub: {
                return A - B;
            }
            case AluFunc::And: {
            return A & B;
            }
            case AluFunc::Or: {
                return A | B;
            }
            case AluFunc::Xor: {
            return A ^ B;
            }
            case AluFunc::Slt: {
                auto a_first_bit = (A >> 16) & 1; 
                auto b_first_bit = (B >> 16) & 1; 
                if (a_first_bit < b_first_bit){
                    return false;
                }
                else if (a_first_bit == 0 && b_first_bit == 0){
                    return A < B;
                }
                else if (a_first_bit == 1 && b_first_bit == 1){
                    return A > B;
                }
                else{
                    return true;
                } 
            }
            case AluFunc::Sltu:{
            return A < B;
            }
            case AluFunc::Sll: {
                return A << (B % 32);
            }
            case AluFunc::Srl: {
                return A >> (B % 32);
            }
            case AluFunc::Sra: {
                auto a_first_bit = (A << 16) & 1;
                if (a_first_bit == 0){
                    return A >> (B % 32);
                } 
                else {
                    Word left_interval = 0b0;
                    for(int i = 0, j = 16; i < (B % 32); i++, j--){
                        left_interval = left_interval | ((A >> j) & 1) << j;
                    }
                    A = A >> (B % 32);
                    for(int i = 0, j = 16; i < (B % 32); i++, j--){
                        A = A | ((left_interval >> j) & 1) << j;
                    }
                    return A;
                }
            }
        }
        return 0;
    }
    ///////////////////////////Br_FUNC////////////////////////////////
    static bool Eq(Word A, Word B){
        return A == B;
    }

    static bool Neq(Word A, Word B){
        return A !=B ;
    }

    static bool Lt(Word A, Word B){
        auto a_first_bit = (A >> 16) & 1; 
        auto b_first_bit = (B >> 16) & 1; 
        if (a_first_bit < b_first_bit){
            return false;
        }
        else if (a_first_bit == 0 && b_first_bit == 0){
            return A < B;
        }
        else if (a_first_bit == 1 && b_first_bit == 1){
            return A > B;
        }
        else {
            return true;
        } 
    }

    static bool Ltu(Word A, Word B){
        return A < B;
    }

    static bool Ge(Word A, Word B){ 
        auto a_first_bit = (A >> 16) & 1; 
        auto b_first_bit = (B >> 16) & 1; 
        if (a_first_bit > b_first_bit){
            return false;
        }
        else if (a_first_bit == 0 && b_first_bit == 0){
            return A >= B;
        }
        else if (a_first_bit == 1 && b_first_bit == 1){
            return A <= B;
        }
        else{
            return true;
        } 
    }

    static bool Geu(Word A, Word B){
        return A >= B;
    }

    static bool AT(Word A, Word B){
        return true;
    }

    static bool NT(Word A, Word B){
        return false;
    }

    static bool(*select_br_func(int br_func))(Word, Word){
        // массив указателей на функции, которые будут возвращаться
        bool (*func[])(Word A, Word B) = { Eq, Neq, Lt, Ltu, Ge, Geu, AT, NT}; 
        return func[br_func];
    }
};

#endif // RISCV_SIM_EXECUTOR_H
