# cpp-blockchain-test
raw blockchain interface

includes classes:

class Block, with hash, id and value<br>
block may include trasaction<br>
If you try to change value,  block will add new transaction. 
Transaction is new block, using parent hash as base to make self hash. <br>
class block can check interity of theirown transaction.<br>

class Blockchain is chain of blocks.<br>
It haso peration Adding new block into the chain and check integrity of whole blocks chain<br>

Classes using external open_ssl library. Exactly SHA256 algorithm for hash value generation.<br>

It's only example of blockchain. Blockchain does not use any external hashes from wallet to make transaction. <br>

