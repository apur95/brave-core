/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { createStateManager } from '../../shared/lib/state_manager'

import {
  Host,
  HostState,
  PublisherInfo,
  RewardsParameters,
  BalanceInfo,
  ExternalWalletInfo,
  DialogArgs,
  EntryPoint,
  TipKind,
  ShareTarget,
  StringKey
} from './interfaces'

function getEntryPoint (value: string): EntryPoint {
  switch (value) {
    case 'set-monthly': return value
    case 'clear-monthly': return value
    default: return 'one-time'
  }
}

function getDialogArgs (): DialogArgs {
  let args: any = {}
  try {
    args = Object(JSON.parse(chrome.getVariableValue('dialogArguments')))
  } catch (error) {
    console.error(error)
  }

  return {
    url: String(args.url || ''),
    publisherKey: String(args.publisherKey || ''),
    entryPoint: getEntryPoint(String(args.entryPoint)),
    mediaMetaData: args.mediaMetaData
      ? Object(args.mediaMetaData)
      : { mediaType: 'none' }
  }
}

export function createHost (): Host {
  const stateManager = createStateManager<HostState>({})
  const dialogArgs = getDialogArgs()

  window.cr.define('brave_rewards_tip', () => ({

    rewardsInitialized () {
      if (!dialogArgs.publisherKey) {
        stateManager.update({
          hostError: { type: 'INVALID_DIALOG_ARGS' }
        })
        return
      }

      chrome.send('brave_rewards_tip.getRewardsParameters')
      chrome.send('brave_rewards_tip.fetchBalance')
      chrome.send('brave_rewards_tip.getReconcileStamp')
      chrome.send('brave_rewards_tip.getExternalWallet', ['uphold'])
      chrome.send('brave_rewards_tip.onlyAnonWallet')
      chrome.send('brave_rewards_tip.getPublisherBanner', [
        dialogArgs.publisherKey
      ])
    },

    publisherBanner (publisherInfo: PublisherInfo) {
      stateManager.update({ publisherInfo })
    },

    rewardsParameters (rewardsParameters: RewardsParameters) {
      stateManager.update({ rewardsParameters })
    },

    recurringTips (list: string[]) {
      // Unused
    },

    reconcileStamp (stamp: number) {
      stateManager.update({ nextReconcileDate: new Date(stamp * 1000) })
    },

    recurringTipRemoved (success: boolean) {
      // Unused
    },

    recurringTipSaved (success: boolean) {
      // Unused
    },

    balance (result: { status: number, balance: BalanceInfo }) {
      if (result.status === 0) {
        stateManager.update({
          balanceInfo: result.balance
        })
      } else {
        stateManager.update({
          hostError: {
            type: 'ERROR_FETCHING_BALANCE',
            code: result.status
          }
        })
      }
    },

    externalWallet (externalWalletInfo: ExternalWalletInfo) {
      stateManager.update({ externalWalletInfo })
    },

    onlyAnonWallet (onlyAnonWallet: boolean) {
      stateManager.update({ onlyAnonWallet })
    },

    reconcileComplete (reconcile: { type: number, result: number }) {
      if (reconcile.result === 0) {
        chrome.send('brave_rewards_tip.fetchBalance')
      }
    }

  }))

  self.i18nTemplate.process(document, self.loadTimeData)

  chrome.send('brave_rewards_tip.isRewardsInitialized')

  return {

    getString (key: StringKey) {
      return self.loadTimeData.getString(key)
    },

    getDialogArgs () {
      return dialogArgs
    },

    closeDialog () {
      chrome.send('dialogClose')
    },

    processTip (amount: number, kind: TipKind) {
      if (!dialogArgs.publisherKey) {
        stateManager.update({
          hostError: { type: 'INVALID_PUBLISHER_KEY' }
        })
        return
      }

      if (amount < 0 || kind === 'one-time' && amount === 0) {
        stateManager.update({
          hostError: { type: 'INVALID_TIP_AMOUNT' }
        })
        return
      }

      chrome.send('brave_rewards_tip.onTip', [
        dialogArgs.publisherKey,
        amount,
        kind === 'monthly' ? true : false
      ])

      stateManager.update({
        tipProcessed: true,
        tipAmount: amount
      })
    },

    shareTip (target: ShareTarget) {
      const { publisherInfo } = stateManager.state
      if (!publisherInfo) {
        stateManager.update({
          hostError: { type: 'PUBLISHER_INFO_UNAVAILABLE' }
        })
        return
      }

      let name = publisherInfo.name
      let tweetId = ''

      if (dialogArgs.mediaMetaData.mediaType === 'twitter') {
        name = '@' + dialogArgs.mediaMetaData.screenName
        tweetId = dialogArgs.mediaMetaData.tweetId
      } else if (publisherInfo.provider === 'twitter' && dialogArgs.url) {
        name = '@' + dialogArgs.url.replace(/^.*\//, '')
      }

      chrome.send('brave_rewards_tip.tweetTip', [name, tweetId])
      chrome.send('dialogClose')
    },

    addListener: stateManager.addListener

  }
}
