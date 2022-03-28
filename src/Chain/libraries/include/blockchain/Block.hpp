#pragma once

#include <blockchain/Transaction.hpp>

namespace thinkyoung {
    namespace blockchain {

		// 새로운 블록번호를 표현하는 구조체
		// 노드번호와 블록발생번호로 64비트 정수를 구현한다
		struct BlockNumberType 
		{
			union {
				uint64_t block_num;
				struct {
					uint64_t produced : 48,
					node_index : 16;
				}bn;
			}un;


			BlockNumberType(uint64_t num)
			{
				un.block_num = num;
			}
			BlockNumberType(uint64_t num, uint16_t node)
			{
				un.bn.produced = num;
				un.bn.node_index = node;
			}
			uint16_t node_index()
			{
				return un.bn.node_index;
			}
			uint64_t produced()
			{
				return un.bn.produced;
			}

			uint64_t block_number()
			{
				return un.block_num;
			}
		};

		// 블록헤더에는 랭킹과 어미다이제스트를 추가하였다.
		// 기존의 일부 마당들은 없앤다.
        struct BlockHeader
        {
            DigestType  digest()const;

            //BlockIdType        previous;
			uint64_t             block_num = 0;
			uint64_t             rank = 0;			// 랭크 마당
            fc::time_point_sec   timestamp;
            DigestType           transaction_digest;
			DigestType           parent_digest;		// 어미리스트에 대한 다이제스트 마당
            /** used for random number generation on the blockchain */
            //SecretHashType       delegate_sign;
            //SecretHashType     previous_secret;

			uint16_t nodeindex() {
				return BlockNumberType(block_num).node_index();
			}

			uint64_t produced() {
				return BlockNumberType(block_num).produced();
			}
		};

        struct SignedBlockHeader : public BlockHeader
        {
            BlockIdType    id()const;
            bool             validate_signee(const fc::ecc::public_key& expected_signee, bool enforce_canonical = false)const;
            PublicKeyType  signee(bool enforce_canonical = false)const;
            void             sign(const fc::ecc::private_key& signer);

            SignatureType delegate_signature;
        };

		// 풀블록에는 DAG구조를 위해 어미/자식 블록아이디 리스트를 넣는다.
        struct FullBlock : public SignedBlockHeader
        {
            size_t               block_size()const;
			vector<BlockIdType>  parents;		// 어미 블록 아이디리스트
			vector<BlockIdType>  childs;		// 자식 블록 아이디리스트
            signed_transactions  user_transactions;
			bool                 existChild(BlockIdType child)
			{
				for (auto ch : childs)
				{
					if (ch == child)
						return true;
				}
				return false;
			}

			bool                 existParent(BlockIdType parent)
			{
				for (auto ch : parents)
				{
					if (ch == parent)
						return true;
				}
				return false;
			}

        };

		struct FullBlockForPrint : public FullBlock
		{
			FullBlockForPrint() {}
			FullBlockForPrint(const FullBlock& block_data);

			BlockIdType			block_id_;
			size_t              node_index_;
			size_t              block_size_;
			fc::string			signee_name_;
			fc::ripemd160       random_seed_;
			std::vector<TransactionIdType> user_transaction_ids;
		};


        struct DigestBlock : public SignedBlockHeader
        {
            DigestBlock(){}
            DigestBlock(const FullBlock& block_data);

            DigestType                      calculate_transaction_digest()const;
            bool                            validate_digest()const;
            bool                            validate_unique()const;
            std::vector<TransactionIdType>	user_transaction_ids;

			DigestType                      calculate_parent_digest()const;
			bool                            validate_parent_digest()const;
			bool                            validate_parent_unique()const;
			std::vector<BlockIdType>		parent_block_ids;
        };

		struct BlockNumberStruct
		{
			uint64_t blockIndex;
			uint64_t nodeIndex;
		};


		typedef struct _StatusNodeInfo {
			fc::string ipaddr;
			int index;
			fc::time_point active_time;
		} StatusNodeInfo;

		//typedef struct _StatusNodeInfoDisplay {
		//	int index;
		//	bool is_master;
		//	int total_wallets_unlocked;
		//	fc::time_point active_time;
		//} StatusNodeInfoDisplay;

		typedef struct _WalletStatusInfo {
			uint64_t id;
			fc::string peer_addr;
			fc::string wallet_name;
			int status;
			fc::time_point unlocked_time;
			fc::time_point expire_time;
		} WalletStatusInfo;


		/* =========== 버전 1.0의 블록구조체 ============= */
		struct BlockHeader1
		{
			DigestType  digest()const;

			BlockIdType        previous;
			uint32_t             block_num = 0;
			fc::time_point_sec   timestamp;
			DigestType          transaction_digest;
			/** used for random number generation on the blockchain */
			SecretHashType     next_secret_hash;
			SecretHashType     previous_secret;
		};

		struct SignedBlockHeader1 : public BlockHeader1
		{
			BlockIdType    id()const;
			bool             validate_signee(const fc::ecc::public_key& expected_signee, bool enforce_canonical = false)const;
			PublicKeyType  signee(bool enforce_canonical = false)const;
			void             sign(const fc::ecc::private_key& signer);

			SignatureType delegate_signature;
		};

		struct FullBlock1 : public SignedBlockHeader1
		{
			size_t               block_size()const;

			signed_transactions  user_transactions;
		};

		/* ------------ 버전 1.0의 블록구조체 ---------- */

    }

} // thinkyoung::blockchain

FC_REFLECT(thinkyoung::blockchain::BlockHeader, (block_num)(rank)(timestamp)(transaction_digest)(parent_digest))
FC_REFLECT_DERIVED(thinkyoung::blockchain::SignedBlockHeader, (thinkyoung::blockchain::BlockHeader), (delegate_signature))
FC_REFLECT_DERIVED(thinkyoung::blockchain::FullBlock, (thinkyoung::blockchain::SignedBlockHeader), (parents)(childs)(user_transactions))
FC_REFLECT_DERIVED(thinkyoung::blockchain::DigestBlock, (thinkyoung::blockchain::SignedBlockHeader), (user_transaction_ids)(parent_block_ids))
FC_REFLECT_DERIVED(thinkyoung::blockchain::FullBlockForPrint, (thinkyoung::blockchain::FullBlock), (block_id_)(node_index_)(block_size_)(signee_name_)(random_seed_)(user_transaction_ids))
FC_REFLECT(thinkyoung::blockchain::BlockNumberStruct, (blockIndex)(nodeIndex))
FC_REFLECT(thinkyoung::blockchain::StatusNodeInfo, (ipaddr)(index)(active_time))
//FC_REFLECT(thinkyoung::blockchain::StatusNodeInfoDisplay, (index)(is_master)(total_wallets_unlocked)(active_time))
FC_REFLECT(thinkyoung::blockchain::WalletStatusInfo, (id)(peer_addr)(wallet_name)(status)(unlocked_time)(expire_time))

FC_REFLECT(thinkyoung::blockchain::BlockHeader1, (previous)(block_num)(timestamp)(transaction_digest)(next_secret_hash)(previous_secret))
FC_REFLECT_DERIVED(thinkyoung::blockchain::SignedBlockHeader1, (thinkyoung::blockchain::BlockHeader1), (delegate_signature))
FC_REFLECT_DERIVED(thinkyoung::blockchain::FullBlock1, (thinkyoung::blockchain::SignedBlockHeader1), (user_transactions))
