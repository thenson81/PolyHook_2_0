//
// Created by steve on 4/5/17.
//

#ifndef POLYHOOK_2_0_MEMALLOCATOR_HPP
#define POLYHOOK_2_0_MEMALLOCATOR_HPP
#include <vector>
#include <memory>
#include "../ErrorSystem.hpp"
#include "../Misc.hpp"
#include "../Enums.hpp"
#include "MemoryBlock.hpp"
#include "AllocatedMemoryBlock.hpp"
#include <iostream>

//http://altdevblog.com/2011/06/27/platform-abstraction-with-cpp-templates/
namespace PLH{
    template<typename PlatformImp>
    class ARangeMemAllocator : private PlatformImp, public virtual PLH::Errant
    {
    public:
        AllocatedMemoryBlock AllocateMemory(uint64_t MinAddress, uint64_t MaxAddress, size_t Size, ProtFlag Protections)
        {
            AllocatedMemoryBlock Block = PlatformImp::AllocateMemory(MinAddress,MaxAddress, Size, Protections);
            if(Block.GetParentBlock() != nullptr)
                return Block;
            else{
                //TO-DO: Handle this case properly
                this->SendError("Failed To Allocate Memory");
                throw "ERRORS";
            }
        }

        int TranslateProtection(const ProtFlag flags) const
        {
            return PlatformImp::TranslateProtection(flags);
        }

        std::vector<PLH::MemoryBlock> GetAllocatedVABlocks() const
        {
            return PlatformImp::GetAllocatedVABlocks();
        }

        std::vector<PLH::MemoryBlock> GetFreeVABlocks()
        {
            return PlatformImp::GetFreeVABlocks();
        }

        std::vector<std::shared_ptr<uint8_t>> GetAllocatedCaves()
        {
            return m_Caves;
        }
    protected:
        //[MinAddress, MaxAddress)
        bool VerifyMemInRange(uint64_t MinAddress, uint64_t MaxAddress, uint64_t Needle)
        {
            if (Needle >= MinAddress && Needle < MaxAddress)
                return true;
            return false;
        }
        std::vector<AllocatedMemoryBlock> m_Caves;
    };
}

//Implementation instantiations
#include "UnixImpl/RangeMemAllocatorUnixImp.hpp"
namespace PLH{
    using MemAllocatorUnix = PLH::ARangeMemAllocator<PLH::RangeMemAllocatorUnixImp>;
}
#endif //POLYHOOK_2_0_MEMALLOCATOR_HPP

