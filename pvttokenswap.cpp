
#include "pvttokenswap.hpp"

//the contract receives a given amount of PVT tokens and returns the corresponding amount of UTX tokens. 
//exchange rate is taken from the config table
void pvttokenswap::onpvttransfer(name from, name to, asset quantity, string memo) {
    //auto itr = _config.find(1);
    //check(itr != _config.end(), "Missing contract configuration");
    auto& cfg_itr = _config.get(1, "Missing contract configuration");  

    check(quantity.symbol.is_valid(),"invalid quantity");
    check(quantity.amount>0, "only positive quantity allowed");

    if (quantity.symbol == PVT && from != cfg_itr.owner && from != _self && to == _self) {
        check(quantity.amount>100, "minimum 0.0100 PVT per swap");
        check(cfg_itr.blockNewSwaps != 1 && cfg_itr.blockNewSwaps != 2, "sell functionality is currently disabled");
        
        asset bought(0, UTX);
        auto xchange = cfg_itr.bidPriceK;    //GET THE EXCHANGE RATE BETWEEN UTX AND PVT
        bought.amount = quantity.amount * xchange / 1000;   //CALCULATE AMOUNT OF UTX
        //check(bought.amount>1, "minimum 0.0001 UTX per swap");    
        action(
            permission_level{_self, "transfer"_n},
            "eosio.token"_n, "transfer"_n,
            std::make_tuple(_self, from, bought, string("transfer from pvttokenswap"))
        ).send();
    
    }
}

//the contract receives a given amount of UTX tokens and returns the corresponding amount of PVT tokens. 
//exchange rate is taken from the config table
void pvttokenswap::ontransfer(name from, name to, asset quantity, string memo) {

    auto& cfg_itr = _config.get(1, "Missing contract configuration");

    check(quantity.symbol.is_valid(),"invalid quantity");
    check(quantity.amount>0, "only positive quantity allowed");

    if(quantity.symbol == UTX && from != cfg_itr.owner && from != _self && to == _self) {

        check(quantity.amount>100, "minimum 0.0100 UTX per swap");
        check(cfg_itr.blockNewSwaps != 1 && cfg_itr.blockNewSwaps != 3, "buy functionality is currently disabled");
        
        asset bought(0, PVT);
        auto xchange = cfg_itr.askPriceK;    //GET THE EXCHANGE RATE BETWEEN UTX AND PVT
        bought.amount = quantity.amount * 1000 / xchange;   //CALCULATE AMOUNT OF PVT
        //check(bought.amount>1, "minimum 0.0001 PVT per swap");   
        
        action(
            permission_level{_self, "transfer"_n},
            "uxpvttkntest"_n, "transfer"_n,
            std::make_tuple(_self, from, bought, string("transfer from pvttokenswap"))
        ).send();
     
    } 

}

void pvttokenswap::setprice(uint64_t askPriceK, uint64_t bidPriceK) {
    auto itr = _config.find(1);
    check(itr != _config.end(), "Missing contract configuration");
    auto& cfg_itr = _config.get(1);  

    require_auth(cfg_itr.owner);

    //update exchange rate
    _config.modify(cfg_itr, _self,  [&](auto& newrow) {
        newrow.askPriceK = askPriceK;
        newrow.bidPriceK = bidPriceK;
    });
}
  
    
void pvttokenswap::setup(name ownername, uint8_t blockNewSwaps, uint64_t askPriceK, uint64_t bidPriceK) {
    auto itr = _config.find(1);
    if (itr == _config.end()) {
        require_auth(_self);
        _config.emplace(_self,  [&](auto& newrow) {
            newrow.id = 1;
            newrow.owner = ownername;
            newrow.version ++;
            newrow.blockNewSwaps = blockNewSwaps;
            newrow.askPriceK = askPriceK;
            newrow.bidPriceK = bidPriceK;
        });
    }
    else {
        auto& cfg_itr = _config.get(1);
        require_auth(cfg_itr.owner);
        // if bidPriceK == 0 || askPriceK == 0 clear the existing _config table
        if (bidPriceK == 0 || askPriceK == 0) {
            auto rem_itr = _config.find(1);
            if (rem_itr != _config.end()) {
                _config.erase(rem_itr);
            }
        } else {
            _config.modify(cfg_itr, _self,  [&](auto& newrow) {
                newrow.owner = ownername;
                newrow.version ++;
                newrow.blockNewSwaps = blockNewSwaps;
                newrow.askPriceK = askPriceK;
                newrow.bidPriceK = bidPriceK;
            });
        }    
    }
}



