
#include "ramtokenswap.hpp"


//the contract receives a given amount of UTX tokens and returns the corresponding amount of UTXRAM tokens. 
//exchange rate is taken from the config table
void ramtokenswap::ontransfer(name from, name to, asset quantity, string memo) {

    auto itr = _config.find(1);
    check(itr != _config.end(), "Missing contract configuration");
    auto& cfg_itr = _config.get(1);  

    check(quantity.symbol.is_valid(),"invalid quantity");
    check(quantity.amount>0, "only positive quantity allowed");

    if(quantity.symbol == UTX && from != cfg_itr.owner && from != _self && to == _self) {

        check(quantity.amount>10, "minimum 10 UTX per swap");
        check(cfg_itr.blockNewSwaps != 1, "swap functionality is currently disabled");
        
        asset bought_ram(0, UTXRAM);
        auto xchange = cfg_itr.uxRAMask;    //GET THE EXCHANGE RATE BETWEEN UTX AND UTXRAM
        bought_ram.amount = quantity.amount / xchange;   //CALCULATE AMOUNT OF UTXRAM
        //check(bought_ram.amount>1, "minimum 1 UTXRAM per swap");   
        
        action(
            permission_level{_self, "transfer"_n},
            "eosio.token"_n, "transfer"_n,
            std::make_tuple(_self, from, bought_ram, string("transfer from ramtokenswap"))
        ).send();
     
    } else if (quantity.symbol == UTXRAM && from != cfg_itr.owner && from != _self && to == _self) {

        check(quantity.amount>0.3, "minimum 0.30 UTXRAM per swap");
        check(cfg_itr.blockNewSwaps != 1, "swap functionality is currently disabled");
        
        asset bought_utx(0, UTX);
        auto xchange = cfg_itr.uxRAMbid;    //GET THE EXCHANGE RATE BETWEEN UTX AND UTXRAM
        bought_utx.amount = quantity.amount * xchange;   //CALCULATE AMOUNT OF UTX
         
        action(
            permission_level{_self, "transfer"_n},
            "eosio.token"_n, "transfer"_n,
            std::make_tuple(_self, from, bought_utx, string("transfer from ramtokenswap"))
        ).send();

    }

}

void ramtokenswap::setprice(uint64_t uxRAMask, uint64_t uxRAMbid) {
    auto itr = _config.find(1);
    check(itr != _config.end(), "Missing contract configuration");
    auto& cfg_itr = _config.get(1);  

    require_auth(cfg_itr.owner);

    //update exchange rate
    _config.modify(cfg_itr, _self,  [&](auto& newrow) {
        newrow.uxRAMask = uxRAMask;
        newrow.uxRAMbid = uxRAMbid;
    });
}
  
    
void ramtokenswap::setup(name ownername, uint8_t blockNewSwaps, uint64_t uxRAMask, uint64_t uxRAMbid) {
    auto itr = _config.find(1);
    if (itr == _config.end()) {
        require_auth(_self);
        _config.emplace(_self,  [&](auto& newrow) {
            newrow.id = 1;
            newrow.owner = ownername;
            newrow.version ++;
            newrow.blockNewSwaps = blockNewSwaps;
            newrow.uxRAMask = uxRAMask;
            newrow.uxRAMbid = uxRAMbid;
        });
    }
    else {
        auto& cfg_itr = _config.get(1);
        require_auth(cfg_itr.owner);
        // if uxRAMxchange == 0 clear the existing _config table
        if (uxRAMbid == 0 || uxRAMask == 0) {
            auto rem_itr = _config.find(1);
            if (rem_itr != _config.end()) {
                _config.erase(rem_itr);
            }
        } else {
            _config.modify(cfg_itr, _self,  [&](auto& newrow) {
                newrow.owner = ownername;
                newrow.version ++;
                newrow.blockNewSwaps = blockNewSwaps;
                newrow.uxRAMask = uxRAMask;
                newrow.uxRAMbid = uxRAMbid;
            });
        }    
    }
}



