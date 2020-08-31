/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { storiesOf } from '@storybook/react'
import * as knobs from '@storybook/addon-knobs'

import { App } from '../components/app'
import { DialogArgs, Host, HostState } from '../lib/interfaces'
import { HostContext } from '../lib/host_context'

import { localeStrings } from './locale_strings'

const hostState: HostState = {
  publisherInfo: {
    publisherKey: 'brave.com',
    name: 'brave.com',
    title: 'Brave Software',
    description: 'Thanks for stopping by. Brave is on a mission to fix ' +
      'the web by giving users a safer, faster and better browsing experience ' +
      'while growing support for content creators through a new attention-based ' +
      'ecosystem of rewards. Join us. It’s time to fix the web together!',
    background: '',
    logo: 'https://rewards.brave.com/LH3yQwkb78iP28pJDSSFPJwU',
    amounts: [1, 10, 100],
    provider: '',
    links: {
      twitter: 'https://twitter.com/brave',
      youtube: 'https://www.youtube.com/bravesoftware'
    },
    status: 2
  },
  balanceInfo: {
    total: 5,
    wallets: {}
  },
  externalWalletInfo: {
    token: '',
    address: '1234',
    status: 0,
    type: 'uphold',
    verifyUrl: 'about:blank',
    addUrl: 'about:blank',
    withdrawUrl: 'about:blank',
    userName: 'username',
    accountUrl: 'about:blank',
    loginUrl: 'about:blank'
  },
  rewardsParameters: {
    tipChoices: [1, 2, 3],
    monthlyTipChoices: [1, 2, 3],
    rate: 0.333
  },
  hostError: undefined,
  nextReconcileDate: new Date(Date.now() + 15 * 14 * 60 * 60 * 1000),
  onlyAnonWallet: false,
  tipProcessed: false,
  currentMontlyTip: 1
}

const dialogArgs: DialogArgs = {
  publisherKey: 'test-publisher',
  url: '',
  mediaMetaData: {
    mediaType: 'none'
  },
  entryPoint: 'one-time'
}

function createHost (): Host {
  return {
    getString (key) {
      return localeStrings[key] || 'MISSING'
    },
    getDialogArgs () {
      return knobs.object('dialogArgs', dialogArgs)
    },
    closeDialog () {
      console.log('closeDialog')
    },
    processTip (amount, kind) {
      console.log('processTip', amount, kind)
    },
    shareTip (target) {
      console.log('shareTip', target)
    },
    addListener (callback) {
      callback(knobs.object('hostState', hostState))
      return () => {
        // No-op
      }
    }
  }
}

storiesOf('Rewards/Tip Dialog', module)
  .addDecorator(knobs.withKnobs)
  .add('Tip Dialog', () => {
    return (
      <HostContext.Provider value={createHost()}>
        <App />
      </HostContext.Provider>
    )
  })
