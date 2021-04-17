#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
//#include <eosio/system.hpp>

using namespace eosio;
using namespace std;
using std::string;

#define UTX symbol("UTX", 4)
#define UTXRAM symbol("UTXRAM", 4)

class [[eosio::contract]] ramtokenswap : public eosio::contract {


private:

    struct [[eosio::table]] config {
        uint64_t  id;
        uint64_t version=0;
        name owner;
        uint8_t blockNewSwaps = 0;
        uint64_t uxRAMask;
        uint64_t uxRAMbid;
        auto primary_key() const { return id; }
    };
    typedef eosio::multi_index<"config"_n, config> config_table;
    config_table _config;
     
   
    
public:

 ramtokenswap( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds)
                       , _config(receiver, receiver.value) {}
 
 [[eosio::action]]
 void setup(name ownername, uint8_t blockNewSwaps, uint64_t uxRAMask, uint64_t uxRAMbid);

 [[eosio::action]]
 void setprice(uint64_t uxRAMask, uint64_t uxRAMbid);

 [[eosio::on_notify("eosio.token::transfer")]]
 void ontransfer(name from, name to, asset quantity, string memo); 
    
};

