// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file LocatorWithMask.hpp
 */

#ifndef _FASTDDS_RTPS_COMMON_LOCATORWITHMASK_HPP_
#define _FASTDDS_RTPS_COMMON_LOCATORWITHMASK_HPP_

#include <cassert>

#include <fastrtps/fastrtps_dll.h>

#include <fastdds/rtps/common/Locator.h>
#include <fastdds/rtps/common/Types.h>

namespace eprosima {
namespace fastdds {
namespace rtps {

/**
 * A Locator with a mask that defines the number of significant bits of its address.
 */
class RTPS_DllAPI LocatorWithMask : public Locator
{
public:

    /**
     * Get the number of significant bits on the address of this locator.
     *
     * @return number of significant bits on the address of this locator.
     */
    uint8_t mask() const
    {
        return mask_;
    }

    /**
     * Set the number of significant bits on the address of this locator.
     *
     * @param mask number of significant bits on the address of this locator.
     */
    void mask(
            uint8_t mask)
    {
        mask_ = mask;
    }

    bool matches(
            const Locator& loc) const      // TODO: move everything to cpp
    {
        if (kind == loc.kind)
        {
            switch (kind)
            {
                case LOCATOR_KIND_UDPv4:
                case LOCATOR_KIND_TCPv4:
                    assert(32 >= mask());
                    return address_matches(loc.address + 12, address + 12, mask());

                case LOCATOR_KIND_UDPv6:
                case LOCATOR_KIND_TCPv6:
                case LOCATOR_KIND_SHM:
                    assert(128 >= mask());
                    return address_matches(loc.address, address, mask());
            }
        }

        return false;
    }

    /**
     * Checks if the first significant bits of two locator addresses match.
     *
     * @param [in] addr1     First address to check.
     * @param [in] addr2     Second address to check.
     * @param [in] num_bits  Number of bits to take into account.
     *
     * @return true when the first @c num_bits of both addresses are equal, false otherwise.
     */
    static bool address_matches(
            const uint8_t* addr1,
            const uint8_t* addr2,
            uint64_t num_bits)
    {
        uint64_t full_bytes = num_bits / 8;
        if ((0 == full_bytes) || std::equal(addr1, addr1 + full_bytes, addr2))
        {
            uint64_t rem_bits = num_bits % 8;
            if (rem_bits == 0)
            {
                return true;
            }

            uint8_t mask = 0xFF << (8 - rem_bits);
            return (addr1[full_bytes] & mask) == (addr2[full_bytes] & mask);
        }

        return false;
    }

    /// Copy assignment
    LocatorWithMask& operator =(
            const Locator& loc)
    {
        kind = loc.kind;
        port = loc.port;
        std::memcpy(address, loc.address, 16 * sizeof(fastrtps::rtps::octet));
        return *this;
    }

private:

    uint8_t mask_ = 24;
};

} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif /* _FASTDDS_RTPS_COMMON_LOCATORWITHMASK_HPP_ */
