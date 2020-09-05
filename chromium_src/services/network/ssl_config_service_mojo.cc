/* Copyright 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "services/network/ssl_config_service_mojo.h"

#define BRAVE_SHOW_LEGACY_TLS_WARNINGS \
    return false;
#include "../../../../services/network/ssl_config_service_mojo.cc"
#undef BRAVE_SHOW_LEGACY_TLS_WARNINGS
