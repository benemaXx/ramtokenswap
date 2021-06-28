# pvttokenswap contract
Swap UTX vs. PVT tokens 

Utility smart contract for UX Network


## SETUP

After the contract is deployed, call the **setup** action with following parameters:

- **ownername:** [enter the owner account] Sets the owner's account which will be allowed to change bid/ask prices for the contract, turn swap mode on/off or send additional UTX or PVT tokens to the contract when needed. 
- **blockNewSwaps:** [enter 0 or 1] Turns swap mode on/off. If set to 1, the swaps will be temporarily halted/blocked.
- **askPriceK:** [enter ask price for PVT in UTX * 1'000]
- **bidPriceK:** [enter bid price for PVT in UTX * 1'000]


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
        "askPriceK": 11000,
        "bidPriceK": 9000
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

- **askPriceK:** [enter ask price for PVT in UTX * 1'000]
- **bidPriceK:** [enter bid price for PVT in UTX * 1'000]

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
        "askPriceK": 10500,
        "bidPriceK": 9500
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

- To any account sending UTX tokens, the contract will return the corresponding amount of PVT according to the contract's *askPriceK* price;
- To any account sending PVT tokens, the contract will return the corresponding amount of UTX according to the contract's *bidPriceK* price;

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
        "quantity": "10.0000 PVT",
        "memo": "swap 10 PVT for UTX"
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


## HOW TO ADD UTX OR PVT TOKENS TO THE CONTRACT
Any UTX or PVT tokens sent by the *OWNER_ACCOUNT* will be added to the contract resources (i.e. these will not get swapped).
