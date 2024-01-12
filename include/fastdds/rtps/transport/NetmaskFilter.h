#ifndef _FASTDDS_RTPS_TRANSPORT_NETMASKFILTER_H_
#define _FASTDDS_RTPS_TRANSPORT_NETMASKFILTER_H_

#include <cstdint>

#include <fastrtps/fastrtps_dll.h>

namespace eprosima {
namespace fastrtps {  // TODO: change namespace to fastdds? Just following what it's done in the folder
namespace rtps {

enum class NetmaskFilterKind : uint8_t  // TODO: SubnetFilterKind
{
    OFF,
    AUTO,
    ON
};

class NetmaskFilter  // TODO: or SubnetFilter
{
public:

    // TODO: actually this should not be exposed, only enum -> only enum in header
    RTPS_DllAPI static bool validate_and_transform(
            NetmaskFilterKind& contained_netmask_filter,
            const NetmaskFilterKind& container_netmask_filter)
    {
        if (contained_netmask_filter == NetmaskFilterKind::AUTO)
        {
            contained_netmask_filter = container_netmask_filter;
            return true;
        }
        else
        {
            if (container_netmask_filter == NetmaskFilterKind::AUTO)
            {
                return true;
            }
            else
            {
                return container_netmask_filter == contained_netmask_filter;
            }
        }
    }
};

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif // _FASTDDS_RTPS_TRANSPORT_NETMASKFILTER_H_
