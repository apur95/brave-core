/* Copyright 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "net/socket/ssl_client_socket_impl.h"

#define ShouldSuppressLegacyTLSWarning ShouldSuppressLegacyTLSWarning_ChromiumImpl  // NOLINT
#include "../../../../../services/network/ssl_config_service_mojo.cc"
#undef ShouldSuppressLegacyTLSWarning

namespace network {

bool SSLConfigServiceMojo::ShouldSuppressLegacyTLSWarning(
    const std::string& hostname) const {
  return false;
}

}  // namespace network
