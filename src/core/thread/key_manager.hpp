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
 *   This file includes definitions for Thread security material generation.
 */

#ifndef KEY_MANAGER_HPP_
#define KEY_MANAGER_HPP_

#include "openthread-core-config.h"

#include <stdint.h>

#include <openthread/dataset.h>

#include "common/locator.hpp"
#include "common/timer.hpp"
#include "crypto/hmac_sha256.hpp"
#include "mac/mac_frame.hpp"

namespace ot {

/**
 * @addtogroup core-security
 *
 * @brief
 *   This module includes definitions for Thread security material generation.
 *
 * @{
 */

/**
 * This class represents a Thread Master Key.
 *
 */
OT_TOOL_PACKED_BEGIN
class MasterKey : public otMasterKey
{
public:
    /**
     * This method evaluates whether or not the Thread Master Keys match.
     *
     * @param[in]  aOther  The Thread Master Key to compare.
     *
     * @retval TRUE   If the Thread Master Keys match.
     * @retval FALSE  If the Thread Master Keys do not match.
     *
     */
    bool operator==(const MasterKey &aOther) const { return memcmp(m8, aOther.m8, sizeof(MasterKey)) == 0; }

    /**
     * This method evaluates whether or not the Thread Master Keys match.
     *
     * @param[in]  aOther  The Thread Master Key to compare.
     *
     * @retval TRUE   If the Thread Master Keys do not match.
     * @retval FALSE  If the Thread Master Keys match.
     *
     */
    bool operator!=(const MasterKey &aOther) const { return memcmp(m8, aOther.m8, sizeof(MasterKey)) != 0; }

} OT_TOOL_PACKED_END;

/**
 * This class defines Thread Key Manager.
 *
 */
class KeyManager : public InstanceLocator
{
public:
    enum
    {
        kMaxKeyLength = 16,
        kNonceSize    = 13, ///< Size of IEEE 802.15.4 Nonce (bytes).
    };

    /**
     * This constructor initializes the object.
     *
     * @param[in]  aInstance     A reference to the OpenThread instance.
     *
     */
    explicit KeyManager(Instance &aInstance);

    /**
     * This method starts KeyManager rotation timer and sets guard timer to initial value.
     *
     */
    void Start(void);

    /**
     * This method stops KeyManager timers.
     *
     */
    void Stop(void);

    /**
     * This method returns the Thread Master Key.
     *
     * @returns The Thread Master Key.
     *
     */
    const MasterKey &GetMasterKey(void) const { return mMasterKey; }

    /**
     * This method sets the Thread Master Key.
     *
     * @param[in]  aKey        A Thread Master Key.
     *
     * @retval OT_ERROR_NONE          Successfully set the Thread Master Key.
     * @retval OT_ERROR_INVALID_ARGS  The @p aKeyLength value was invalid.
     *
     */
    otError SetMasterKey(const MasterKey &aKey);

#if OPENTHREAD_FTD || OPENTHREAD_MTD
    /**
     * This method indicates whether the PSKc is configured.
     *
     * A value of all zeros indicates that the PSKc is not configured.
     *
     * @retval TRUE  if the PSKc is configured.
     * @retval FALSE if the PSKc is not not configured.
     *
     */
    bool IsPSKcSet(void) const { return mIsPSKcSet; }

    /**
     * This method returns a pointer to the PSKc.
     *
     * @returns A reference to the PSKc.
     *
     */
    const otPSKc &GetPSKc(void) const { return mPSKc; }

    /**
     * This method sets the PSKc.
     *
     * @param[in]  aPSKc    A reference to the PSKc.
     *
     */
    void SetPSKc(const otPSKc &aPSKc);
#endif

    /**
     * This method returns the current key sequence value.
     *
     * @returns The current key sequence value.
     *
     */
    uint32_t GetCurrentKeySequence(void) const { return mKeySequence; }

    /**
     * This method sets the current key sequence value.
     *
     * @param[in]  aKeySequence  The key sequence value.
     *
     */
    void SetCurrentKeySequence(uint32_t aKeySequence);

    /**
     * This method returns a pointer to the current MAC key.
     *
     * @returns A pointer to the current MAC key.
     *
     */
    const uint8_t *GetCurrentMacKey(void) const { return mKey + kMacKeyOffset; }

    /**
     * This method returns a pointer to the current MLE key.
     *
     * @returns A pointer to the current MLE key.
     *
     */
    const uint8_t *GetCurrentMleKey(void) const { return mKey; }

    /**
     * This method returns a pointer to a temporary MAC key computed from the given key sequence.
     *
     * @param[in]  aKeySequence  The key sequence value.
     *
     * @returns A pointer to the temporary MAC key.
     *
     */
    const uint8_t *GetTemporaryMacKey(uint32_t aKeySequence);

    /**
     * This method returns a pointer to a temporary MLE key computed from the given key sequence.
     *
     * @param[in]  aKeySequence  The key sequence value.
     *
     * @returns A pointer to the temporary MLE key.
     *
     */
    const uint8_t *GetTemporaryMleKey(uint32_t aKeySequence);

    /**
     * This method returns the current MAC Frame Counter value.
     *
     * @returns The current MAC Frame Counter value.
     *
     */
    uint32_t GetMacFrameCounter(void) const { return mMacFrameCounter; }

    /**
     * This method sets the current MAC Frame Counter value.
     *
     * @param[in]  aMacFrameCounter  The MAC Frame Counter value.
     *
     */
    void SetMacFrameCounter(uint32_t aMacFrameCounter) { mMacFrameCounter = aMacFrameCounter; }

    /**
     * This method sets the MAC Frame Counter value which is stored in non-volatile memory.
     *
     * @param[in]  aStoredMacFrameCounter  The stored MAC Frame Counter value.
     *
     */
    void SetStoredMacFrameCounter(uint32_t aStoredMacFrameCounter) { mStoredMacFrameCounter = aStoredMacFrameCounter; }

    /**
     * This method increments the current MAC Frame Counter value.
     *
     */
    void IncrementMacFrameCounter(void);

    /**
     * This method returns the current MLE Frame Counter value.
     *
     * @returns The current MLE Frame Counter value.
     *
     */
    uint32_t GetMleFrameCounter(void) const { return mMleFrameCounter; }

    /**
     * This method sets the current MLE Frame Counter value.
     *
     * @param[in]  aMleFrameCounter  The MLE Frame Counter value.
     *
     */
    void SetMleFrameCounter(uint32_t aMleFrameCounter) { mMleFrameCounter = aMleFrameCounter; }

    /**
     * This method sets the MLE Frame Counter value which is stored in non-volatile memory.
     *
     * @param[in]  aStoredMleFrameCounter  The stored MLE Frame Counter value.
     *
     */
    void SetStoredMleFrameCounter(uint32_t aStoredMleFrameCounter) { mStoredMleFrameCounter = aStoredMleFrameCounter; }

    /**
     * This method increments the current MLE Frame Counter value.
     *
     */
    void IncrementMleFrameCounter(void);

    /**
     * This method returns the KEK.
     *
     * @returns A pointer to the KEK.
     *
     */
    const uint8_t *GetKek(void) const { return mKek; }

    /**
     * This method sets the KEK.
     *
     * @param[in]  aKek  A pointer to the KEK.
     *
     */
    void SetKek(const uint8_t *aKek);

    /**
     * This method returns the current KEK Frame Counter value.
     *
     * @returns The current KEK Frame Counter value.
     *
     */
    uint32_t GetKekFrameCounter(void) const { return mKekFrameCounter; }

    /**
     * This method increments the current KEK Frame Counter value.
     *
     */
    void IncrementKekFrameCounter(void) { mKekFrameCounter++; }

    /**
     * This method returns the KeyRotation time.
     *
     * The KeyRotation time is the time interval after witch security key will be automatically rotated.
     *
     * @returns The KeyRotation value in hours.
     */
    uint32_t GetKeyRotation(void) const { return mKeyRotationTime; }

    /**
     * This method sets the KeyRotation time.
     *
     * The KeyRotation time is the time interval after witch security key will be automatically rotated.
     * Its value shall be larger than or equal to kMinKeyRotationTime.
     *
     * @param[in]  aKeyRotation  The KeyRotation value in hours.
     *
     * @retval  OT_ERROR_NONE          KeyRotation time updated.
     * @retval  OT_ERROR_INVALID_ARGS  @p aKeyRotation is out of range.
     *
     */
    otError SetKeyRotation(uint32_t aKeyRotation);

    /**
     * This method returns the KeySwitchGuardTime.
     *
     * The KeySwitchGuardTime is the time interval during which key rotation procedure is prevented.
     *
     * @returns The KeySwitchGuardTime value in hours.
     *
     */
    uint32_t GetKeySwitchGuardTime(void) const { return mKeySwitchGuardTime; }

    /**
     * This method sets the KeySwitchGuardTime.
     *
     * The KeySwitchGuardTime is the time interval during which key rotation procedure is prevented.
     *
     * @param[in]  aKeySwitchGuardTime  The KeySwitchGuardTime value in hours.
     *
     */
    void SetKeySwitchGuardTime(uint32_t aKeySwitchGuardTime) { mKeySwitchGuardTime = aKeySwitchGuardTime; }

    /**
     * This method returns the Security Policy Flags.
     *
     * The Security Policy Flags specifies network administrator preferences for which
     * security-related operations are allowed or disallowed.
     *
     * @returns The SecurityPolicy Flags.
     *
     */
    uint8_t GetSecurityPolicyFlags(void) const { return mSecurityPolicyFlags; }

    /**
     * This method sets the Security Policy Flags.
     *
     * The Security Policy Flags specifies network administrator preferences for which
     * security-related operations are allowed or disallowed.
     *
     * @param[in]  aSecurityPolicyFlags  The Security Policy Flags.
     *
     */
    void SetSecurityPolicyFlags(uint8_t aSecurityPolicyFlags);

    /**
     * This static method generates IEEE 802.15.4 nonce byte sequence.
     *
     * @param[in]  aAddress        An extended address.
     * @param[in]  aFrameCounter   A frame counter.
     * @param[in]  aSecurityLevel  A security level.
     * @param[out] aNonce          A buffer (with `kNonceSize` bytes) to place the generated nonce.
     *
     */
    static void GenerateNonce(const Mac::ExtAddress &aAddress,
                              uint32_t               aFrameCounter,
                              uint8_t                aSecurityLevel,
                              uint8_t *              aNonce);

private:
    enum
    {
        kMinKeyRotationTime        = 1,
        kDefaultKeyRotationTime    = 672,
        kDefaultKeySwitchGuardTime = 624,
        kMacKeyOffset              = 16,
        kOneHourIntervalInMsec     = 3600u * 1000u,
    };

    void ComputeKey(uint32_t aKeySequence, uint8_t *aKey);

    void        StartKeyRotationTimer(void);
    static void HandleKeyRotationTimer(Timer &aTimer);
    void        HandleKeyRotationTimer(void);

    static const uint8_t     kThreadString[];
    static const otMasterKey kDefaultMasterKey;

    MasterKey mMasterKey;

    uint32_t mKeySequence;
    uint8_t  mKey[Crypto::HmacSha256::kHashSize];

    uint8_t mTemporaryKey[Crypto::HmacSha256::kHashSize];

    uint32_t mMacFrameCounter;
    uint32_t mMleFrameCounter;
    uint32_t mStoredMacFrameCounter;
    uint32_t mStoredMleFrameCounter;

    uint32_t   mHoursSinceKeyRotation;
    uint32_t   mKeyRotationTime;
    uint32_t   mKeySwitchGuardTime;
    bool       mKeySwitchGuardEnabled;
    TimerMilli mKeyRotationTimer;

#if OPENTHREAD_MTD || OPENTHREAD_FTD
    otPSKc mPSKc;
#endif
    uint8_t  mKek[kMaxKeyLength];
    uint32_t mKekFrameCounter;

    uint8_t mSecurityPolicyFlags;
    bool    mIsPSKcSet : 1;
};

/**
 * @}
 */

} // namespace ot

#endif // KEY_MANAGER_HPP_
