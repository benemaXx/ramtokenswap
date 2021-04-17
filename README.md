# ramtokenswap contract
Swap UTX vs. UTXRAM tokens 

Utility smart contract for UX Network


## SETUP

After the contract is deployed, call the **setup** action with following parameters:

- **ownername:** [enter the owner account] Sets the owner's account which will be allowed to change bid/ask prices for the contract, turn swap mode on/off or send additional UTX or UTXRAM tokens to the contract when needed. 
- **blockNewSwaps:** [enter 0 or 1] Turns swap mode on/off. If set to 1, the swaps will be temporarily halted/blocked.
- **uxRAMask:** [enter ask price for UTXRAM in UTX]
- **uxRAMbid:** [enter bid price for UTXRAM in UTX]


##### CLEOS COMMAND:
```
cleos -u http://explorer.uxnetwork.io push transaction '{
  "delay_sec": 0,
  "max_cpu_usage_ms": 0,
  "actions": [
    {
      "account": "[SWAP_CONTRACT]",
      "name": "setup",
      "data": {
        "ownername": "[OWNER_ACCOUNT]",
        "blockNewSwaps": 0,
        "uxRAMask": 26,
        "uxRAMbid": 10
      },
      "authorization": [
        {
          "actor": "[SWAP_CONTRACT]",
          "permission": "active"
        }
      ]
    }
  ]
}'
```


## SET BID/ASK PRICES
Call the **setprice** action of the smart contract with the following parameters:

- **uxRAMask:** [enter ask price for UTXRAM in UTX]
- **uxRAMbid:** [enter bid price for UTXRAM in UTX]

This action requires authority from [OWNER_ACCOUNT].


##### CLEOS COMMAND:
```
cleos -u http://explorer.uxnetwork.io push transaction '{
  "delay_sec": 0,
  "max_cpu_usage_ms": 0,
  "actions": [
    {
      "account": "[SWAP_CONTRACT]",
      "name": "setprice",
      "data": {
        "uxRAMask": 26,
        "uxRAMbid": 10
      },
      "authorization": [
        {
          "actor": "[OWNER_ACCOUNT]",
          "permission": "active"
        }
      ]
    }
  ]
}'
```



## USAGE

- To any account sending UTX tokens, the contract will return the corresponding amount of UTXRAM according to the contract's *uxRAMask* price;
- To any account sending UTXRAM tokens, the contract will return the corresponding amount of UTX according to the contract's *uxRAMbid* price;

##### CLEOS COMMAND:
```
cleos -u http://explorer.uxnetwork.io push transaction '{
  "delay_sec": 0,
  "max_cpu_usage_ms": 0,
  "actions": [
    {
      "account": "eosio.token",
      "name": "transfer",
      "data": {
        "from": "[USER_ACCOUNT]",
        "to": "[SWAP_CONTRACT]",
        "quantity": "10.0000 UTXRAM",
        "memo": "swap 10 UTXRAM for UTX"
      },
      "authorization": [
        {
          "actor": "[USER_ACCOUNT]",
          "permission": "active"
        }
      ]
    }
  ]
}'
```


## HOW TO ADD UTX OR UTXRAM TOKENS TO THE CONTRACT
Any UTX or UTXRAM tokens sent by the *OWNER_ACCOUNT* will be added to the contract resources (i.e. these will not get swapped).
