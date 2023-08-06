# cpp-blockchain-test
raw blockchain interface

includes classes:

class Block, with hash, id and value
block may include trasaction
If you try to change value block will add new transaction. 
Transaktion is block, using parent has as base to itown. 
class block can check interity of theirown transaction.

class Blockchain is chain of blocks.
It haso peration Adding new block into the chain and check integrity of whole blocks chain

Classes using external open_ssl library. Exsactly SHA256 algorithm for hash value generation.

It's only example of blockchain. Blockchain does not use any external hashes from wallet to make transaction. 

