#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
//#include <eosio/system.hpp>

using namespace eosio;
using namespace std;
using std::string;

#define UTX symbol("UTX", 4)
#define PVT symbol("PVT", 4)

class [[eosio::contract]] pvttokenswap : public eosio::contract {


private:

    struct [[eosio::table]] config {
        uint64_t  id;
        uint64_t version=0;
        name owner;
        uint8_t blockNewSwaps = 0;
        uint64_t askPriceK;
        uint64_t bidPriceK;
        auto primary_key() const { return id; }
    };
    typedef eosio::multi_index<"config"_n, config> config_table;
    config_table _config;
     
   
    
public:

 pvttokenswap( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds)
                       , _config(receiver, receiver.value) {}
 
 [[eosio::action]]
 void setup(name ownername, uint8_t blockNewSwaps, uint64_t askPriceK, uint64_t bidPriceK);

 [[eosio::action]]
 void setprice(uint64_t askPriceK, uint64_t bidPriceK);

 [[eosio::on_notify("eosio.token::transfer")]]
 void ontransfer(name from, name to, asset quantity, string memo); 

 [[eosio::on_notify("uxpvttkntest::transfer")]]
 void onpvttransfer(name from, name to, asset quantity, string memo);    

    
};

