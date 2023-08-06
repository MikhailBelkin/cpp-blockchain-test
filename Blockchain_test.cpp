#include <openssl/sha.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <deque>

// Blockchain_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

const size_t HASH_LENGHT = SHA256_DIGEST_LENGTH;



class Block{
public:
    Block(const std::vector<unsigned char>& prev_hash, double value):value_(value) {
        hash_.resize(HASH_LENGHT);
        SHA256(prev_hash.data(), prev_hash.size(), hash_.data());
        
    }
    
    const std::vector<unsigned char> GetHash() const {
        return hash_;
    }

    double GetValue() const {
        return value_;
    }

    double GetActualValue() const {
        if (!transactions_.empty()) {
            return transactions_.back().GetValue();
        }
        else {
            return value_;
        }

    }

    void SetValue(double value) {
        if (transactions_.empty()) {
            std::string strval = std::to_string(value);
            std::vector<unsigned char> hash_plus_val(strval.begin(), strval.end());
            size_t hash_size = hash_plus_val.size();
            hash_plus_val.resize(hash_size + hash_.size());
            std::copy(hash_.begin(), hash_.end(), hash_plus_val.begin() + hash_size);

            transactions_.push_back(std::move(Block(hash_plus_val, value)));
        }
        else {
            std::string strval = std::to_string(value);
            std::vector<unsigned char> hash_plus_val(strval.begin(), strval.end());
            size_t hash_size = hash_plus_val.size();
            std::vector<unsigned char> prev_hash = transactions_.back().GetHash();
            hash_plus_val.resize(hash_size + prev_hash.size());
            std::copy(prev_hash.begin(), prev_hash.end(), hash_plus_val.begin() + hash_size);

            transactions_.push_back(std::move(Block(hash_plus_val, value)));
        }
        
    }


    void DumpBlock(std::ostream& out) const{
        out << "Hash: ";
        for (auto c : hash_) {
            out << std::hex << (int)c << " ";
        }
        
        out << "Value: " << value_ << " Actual value="<< GetActualValue()<< std::endl;

        if (!transactions_.empty()) {
            out << "Found transactions for this block:" << std::endl;
            for (auto t : transactions_) {
                out << "\tTransaction Hash: ";
                for (auto c : t.GetHash()) {
                    out << std::hex << (int)c << " ";
                }
                out << "Value: " << t.GetValue() << std::endl;
            }
        }
        
    }
    void SetID(uint64_t id) {
        id_ = id;
    }

    uint64_t GetId() const {
        return id_;
    }

    bool VerifyTrasations() {
        std::vector<unsigned char> prev_hash = this->GetHash();
        for (auto b : transactions_) {
            std::vector<unsigned char> check_hash(HASH_LENGHT);
            std::string strval = std::to_string(b.GetValue());
            std::vector<unsigned char> hash_plus_val(strval.begin(), strval.end());
            size_t hash_size = hash_plus_val.size();
            hash_plus_val.resize(hash_size + prev_hash.size());
            std::copy(prev_hash.begin(), prev_hash.end(), hash_plus_val.begin() + hash_size);
            SHA256(hash_plus_val.data(), hash_plus_val.size(), check_hash.data());
            if (b.GetHash() != check_hash) {
                return false;
            }
            prev_hash = b.GetHash();

        }
        return true;


    }

private:
    std::vector<unsigned char> hash_;
    double value_;
    uint64_t id_;
    std::vector<Block> transactions_;

};


class BlockChain :public std::deque<Block> {
public:
    BlockChain() {
        const double zero = 0.0;
        std::string strval = std::to_string(zero);
        std::vector<unsigned char> first(strval.begin(), strval.end());
        push_back(std::move(Block(first, zero)));
        back().SetID(size()-1);
    }


    Block AddNewBlock(double val) {
          
          std::string strval = std::to_string(val);
          std::vector<unsigned char> hash_plus_val(strval.begin(), strval.end());
          size_t hash_size = hash_plus_val.size();
          hash_plus_val.resize(hash_size + back().GetHash().size());
          auto prev_hash = back().GetHash();
          std::copy(prev_hash.begin(), prev_hash.end(), hash_plus_val.begin()+hash_size);
          push_back(std::move(Block(hash_plus_val, val)));
          back().SetID(size() - 1);
          return back();
    }


    bool Verify() const{
        std::vector<unsigned char> prev_hash;
        for (auto b : *this) {
            std::vector<unsigned char> check_hash(HASH_LENGHT);
            
            if (b.GetId()==0) {
                const double zero = 0.0;
                std::string strval = std::to_string(zero);
                std::vector<unsigned char> first(strval.begin(), strval.end());
                SHA256(first.data(), first.size(), check_hash.data());
                
            }
            else {
                
                std::string strval = std::to_string(b.GetValue());
                std::vector<unsigned char> hash_plus_val(strval.begin(), strval.end());
                size_t hash_size = hash_plus_val.size();
                hash_plus_val.resize(hash_size + prev_hash.size());
                std::copy(prev_hash.begin(), prev_hash.end(), hash_plus_val.begin() + hash_size);
                SHA256(hash_plus_val.data(), hash_plus_val.size(), check_hash.data());
            }
            if (b.GetHash() != check_hash || !b.VerifyTrasations()) {
                return false;
            }
            prev_hash = b.GetHash();

        }
        return true;

    }


};




int main()
{
    BlockChain crypta;
    for (int i = 0; i < 1000; i += 10) {

        crypta.AddNewBlock(static_cast<double>(i));


    }

    for (auto& b: crypta) {
        for (int i = 0; i < 10; i++) {
            b.SetValue(b.GetValue() - 1);
        }

    }


    for (auto block : crypta) {

        block.DumpBlock(std::cout);

    }

    if (crypta.Verify()) {
        std::cout << "Blockchain was verified succesfully" << std::endl;

    }
    else {
        std::cout << "Blockchain verification failed" << std::endl;
    }

}
