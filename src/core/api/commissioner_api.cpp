/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread Commissioner API.
 */

#include "openthread-core-config.h"

#include <openthread/commissioner.h>

#include "common/instance.hpp"
#include "common/locator-getters.hpp"

using namespace ot;

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
otError otCommissionerStart(otInstance *                 aInstance,
                            otCommissionerStateCallback  aStateCallback,
                            otCommissionerJoinerCallback aJoinerCallback,
                            void *                       aCallbackContext)
{
    otError error;

    Instance &instance = *static_cast<Instance *>(aInstance);

#if OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
    SuccessOrExit(error = instance.Get<MeshCoP::BorderAgent>().Stop());
#endif
    SuccessOrExit(error =
                      instance.Get<MeshCoP::Commissioner>().Start(aStateCallback, aJoinerCallback, aCallbackContext));
exit:
    return error;
}

otError otCommissionerStop(otInstance *aInstance)
{
    otError   error;
    Instance &instance = *static_cast<Instance *>(aInstance);

    SuccessOrExit(error = instance.Get<MeshCoP::Commissioner>().Stop());
#if OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
    SuccessOrExit(error = instance.Get<MeshCoP::BorderAgent>().Start());
#endif

exit:
    return error;
}

otError otCommissionerAddJoiner(otInstance *aInstance, const otExtAddress *aEui64, const char *aPSKd, uint32_t aTimeout)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().AddJoiner(static_cast<const Mac::ExtAddress *>(aEui64), aPSKd,
                                                           aTimeout);
}

otError otCommissionerRemoveJoiner(otInstance *aInstance, const otExtAddress *aEui64)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().RemoveJoiner(static_cast<const Mac::ExtAddress *>(aEui64), 0);
}

otError otCommissionerSetProvisioningUrl(otInstance *aInstance, const char *aProvisioningUrl)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().SetProvisioningUrl(aProvisioningUrl);
}

const char *otCommissionerGetProvisioningUrl(otInstance *aInstance, uint16_t *aLength)
{
    const char *url = NULL;

    Instance &instance = *static_cast<Instance *>(aInstance);

    if (aLength != NULL)
    {
        url = instance.Get<MeshCoP::Commissioner>().GetProvisioningUrl(*aLength);
    }

    return url;
}

otError otCommissionerAnnounceBegin(otInstance *        aInstance,
                                    uint32_t            aChannelMask,
                                    uint8_t             aCount,
                                    uint16_t            aPeriod,
                                    const otIp6Address *aAddress)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().GetAnnounceBeginClient().SendRequest(
        aChannelMask, aCount, aPeriod, *static_cast<const Ip6::Address *>(aAddress));
}

otError otCommissionerEnergyScan(otInstance *                       aInstance,
                                 uint32_t                           aChannelMask,
                                 uint8_t                            aCount,
                                 uint16_t                           aPeriod,
                                 uint16_t                           aScanDuration,
                                 const otIp6Address *               aAddress,
                                 otCommissionerEnergyReportCallback aCallback,
                                 void *                             aContext)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().GetEnergyScanClient().SendQuery(
        aChannelMask, aCount, aPeriod, aScanDuration, *static_cast<const Ip6::Address *>(aAddress), aCallback,
        aContext);
}

otError otCommissionerPanIdQuery(otInstance *                        aInstance,
                                 uint16_t                            aPanId,
                                 uint32_t                            aChannelMask,
                                 const otIp6Address *                aAddress,
                                 otCommissionerPanIdConflictCallback aCallback,
                                 void *                              aContext)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().GetPanIdQueryClient().SendQuery(
        aPanId, aChannelMask, *static_cast<const Ip6::Address *>(aAddress), aCallback, aContext);
}

otError otCommissionerSendMgmtGet(otInstance *aInstance, const uint8_t *aTlvs, uint8_t aLength)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().SendMgmtCommissionerGetRequest(aTlvs, aLength);
}

otError otCommissionerSendMgmtSet(otInstance *                  aInstance,
                                  const otCommissioningDataset *aDataset,
                                  const uint8_t *               aTlvs,
                                  uint8_t                       aLength)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().SendMgmtCommissionerSetRequest(*aDataset, aTlvs, aLength);
}

uint16_t otCommissionerGetSessionId(otInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().GetSessionId();
}

otCommissionerState otCommissionerGetState(otInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<MeshCoP::Commissioner>().GetState();
}

otError otCommissionerGeneratePSKc(otInstance *           aInstance,
                                   const char *           aPassPhrase,
                                   const char *           aNetworkName,
                                   const otExtendedPanId *aExtPanId,
                                   uint8_t *              aPSKc)
{
    OT_UNUSED_VARIABLE(aInstance);

    return MeshCoP::Commissioner::GeneratePSKc(aPassPhrase, aNetworkName,
                                               *static_cast<const Mac::ExtendedPanId *>(aExtPanId), aPSKc);
}
#endif // OPENTHREAD_FTD && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
