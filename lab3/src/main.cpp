
#include "Cpu.h"
#include "Memory.h"
#include "BaseTypes.h"

#include <optional>

int main()
{
    MemoryStorage mem ;
    mem.LoadElf("program");
    std::unique_ptr<IMem> memModelPtr(new UncachedMem (mem));
    Cpu cpu{*memModelPtr};
    cpu.Reset(0x200);

    int32_t print_int = 0;
    while (true)
    {
        cpu.Clock();
        memModelPtr->Clock();
        std::optional<CpuToHostData> msg = cpu.GetMessage();
        if (!msg)
            continue;

        auto type = msg.value().unpacked.type;
        auto data = msg.value().unpacked.data;

        if(type == CpuToHostType::ExitCode) {
            if(data == 0) {
                fprintf(stderr, "PASSED\n");
                break;
            } else {
                fprintf(stderr, "FAILED: exit code = %d\n", data);
                return data;
            }
            break;
        } else if(type == CpuToHostType::PrintChar) {
            fprintf(stderr, "%c", (char)data);
        } else if(type == CpuToHostType::PrintIntLow) {
            print_int = uint32_t(data);
        } else if(type == CpuToHostType::PrintIntHigh) {
            print_int |= uint32_t(data) << 16;
            fprintf(stderr, "%d", print_int);
        }
    }


    MemoryStorage mem2 ;
    mem2.LoadElf("program");
    std::unique_ptr<IMem> memModelPtr2(new CachedMem (mem2));
    Cpu cpu2{*memModelPtr2};
    cpu2.Reset(0x200);

    int32_t print_int2 = 0;
    while (true)
    {
        cpu2.Clock();
        memModelPtr2->Clock();
        std::optional<CpuToHostData> msg = cpu2.GetMessage();
        if (!msg)
            continue;

        auto type = msg.value().unpacked.type;
        auto data = msg.value().unpacked.data;

        if(type == CpuToHostType::ExitCode) {
            if(data == 0) {
                fprintf(stderr, "PASSED\n");
                break;
            } else {
                fprintf(stderr, "FAILED: exit code = %d\n", data);
                return data;
            }
            break;
        } else if(type == CpuToHostType::PrintChar) {
            fprintf(stderr, "%c", (char)data);
        } else if(type == CpuToHostType::PrintIntLow) {
            print_int2 = uint32_t(data);
        } else if(type == CpuToHostType::PrintIntHigh) {
            print_int2 |= uint32_t(data) << 16;
            fprintf(stderr, "%d", print_int2);
        }
    }
    return 0;
}
