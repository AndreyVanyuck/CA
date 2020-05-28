#include <gtest/gtest.h>
#include "../src/Decoder.h"
#include "../src/Executor.h"
#include "../src/BaseTypes.h"

class Tester {
public:
	IType _type;
    AluFunc _func;
    Word _imm;

	Tester() {
        _type = IType::Br;
        _func = AluFunc ::Add;
        _imm = 0b00000000000000000000100000001010;
    }

    InstructionPtr TestDecoder(){
		auto oppcode=0b1100011;
		auto rs1 = 0b000100;
		auto rs2 = 0b000100;
		auto imm = 0b00000000000000000000100000001010;
		auto const_bit = 0b000;

		auto first_imm_interval = 0b000000;
		first_imm_interval = first_imm_interval | ((imm >> 11) & 1) << 0;
		for(int i = 1, j = 1; i < 5; i++, j++){
			first_imm_interval = first_imm_interval | ((imm >> i) & 1) << j;
		}

		auto second_imm_interval = 0b0000000;
		for(int i = 5, j = 0; i < 11; i++, j++){
			second_imm_interval = second_imm_interval | ((imm >> i) & 1) << j;
		}
		second_imm_interval = second_imm_interval | ((imm >> 12) & 1) << 7;

		Word word = (oppcode | (first_imm_interval << 7) |  (const_bit << 12) | (rs1 << 15) | (rs2 << 20) | (second_imm_interval << 25));
        Decoder decoder;
        InstructionPtr instr_ptr = decoder.Decode(word);
        return instr_ptr;
    }

    InstructionPtr TestExecutor(){
		auto oppcode=0b1100011;
		auto rs1 = 0b000100;
		auto rs2 = 0b000100;
		auto imm = 0b00000000000000000000100000001010;
		auto const_bit = 0b000;

		auto first_imm_interval = 0b000000;
		first_imm_interval = first_imm_interval | ((imm >> 11) & 1) << 0;
		for(int i = 1, j = 1; i < 5; i++, j++){
			first_imm_interval = first_imm_interval | ((imm >> i) & 1) << j;
		}

		auto second_imm_interval = 0b0000000;
		for(int i = 5, j = 0; i < 11; i++, j++){
			second_imm_interval = second_imm_interval | ((imm >> i) & 1) << j;
		}
		second_imm_interval = second_imm_interval | ((imm >> 12) & 1) << 7;

		Word word = (oppcode | (first_imm_interval << 7) |  (const_bit << 12) | (rs1 << 15) | (rs2 << 20) | (second_imm_interval << 25));

        Decoder decoder;
        Executor executor;
        InstructionPtr instr_ptr = decoder.Decode(word);
        executor.Execute(instr_ptr, 1234);
        return instr_ptr;
    }
};

class Tests : public ::testing::Test {
public:
	void SetUp()
	{
		dec = new Tester();
	}
	void TearDown(){
		delete dec;
	}
	Tester *dec;
};

TEST_F(Tests, test_decoder) {
	auto instr = dec->TestDecoder();

    ASSERT_EQ(dec->_type, instr->_type);
    ASSERT_EQ(dec->_func, instr->_aluFunc);
    ASSERT_EQ(dec->_imm, instr->_imm);
}

TEST_F(Tests, test_executor) {
	auto instr = dec->TestExecutor();

    ASSERT_EQ(dec->_type, instr->_type);
    ASSERT_EQ(dec->_func, instr->_aluFunc);
    ASSERT_EQ(dec->_imm, instr->_imm);
}