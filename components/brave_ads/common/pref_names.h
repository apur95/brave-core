/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_COMMON_PREF_NAMES_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_COMMON_PREF_NAMES_H_

namespace brave_ads {

namespace prefs {

extern const char kEnabled[];

extern const char kShouldAllowAdConversionTracking[];

extern const char kAdsPerHour[];
extern const char kAdsPerDay[];

extern const char kShouldAllowAdsSubdivisionTargeting[];
extern const char kAdsSubdivisionTargetingCode[];
extern const char kAutomaticallyDetectedAdsSubdivisionTargetingCode[];

extern const char kAdsWereDisabled[];
extern const char kHasAdsP3AState[];

extern const char kIdleThreshold[];

extern const char kShouldShowOnboarding[];
extern const char kOnboardingTimestamp[];

extern const char kShouldShowMyFirstAdNotification[];

extern const char kSupportedCountryCodesLastSchemaVersion[];
extern const char kSupportedCountryCodesSchemaVersion[];
extern const int kSupportedCountryCodesSchemaVersionNumber;

extern const char kVersion[];
extern const int kCurrentVersionNumber;

extern const char kAdViewConfirmationCountPrefName[];

}  // namespace prefs

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_COMMON_PREF_NAMES_H_
