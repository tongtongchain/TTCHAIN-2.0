#pragma once
#include <blockchain/Block.hpp>
#include <wallet/Pretty.hpp>

namespace thinkyoung {
    namespace blockchain {

		struct BlockEntry : public thinkyoung::blockchain::DigestBlock
		{
			BlockIdType       id;
			uint32_t            block_size = 0; /* Bytes */
			fc::microseconds    latency; /* Time between block timestamp and first push_block */

			ShareType          signee_shares_issued = 0;
			ShareType          signee_fees_collected = 0;
			ShareType          signee_fees_destroyed = 0;
			fc::ripemd160       random_seed;

			fc::microseconds    processing_time; /* Time taken for extend_chain to run */
		};
		typedef optional<BlockEntry> oBlockEntry;

		struct FullBlockEntry : public BlockEntry
		{
			uint32_t						node_index;
			std::vector<BlockIdType>		child_block_ids;
			std::vector<thinkyoung::wallet::PrettyTransaction>	user_transactions;
			fc::string						signee;
		};
		typedef optional<FullBlockEntry> oFullBlockEntry;

	}
} // thinkyoung::blockchain

FC_REFLECT_DERIVED(thinkyoung::blockchain::BlockEntry,
	(thinkyoung::blockchain::DigestBlock),
	(id)
	(block_size)
	(latency)
	(signee_shares_issued)
	(signee_fees_collected)
	(signee_fees_destroyed)
	(random_seed)
	(processing_time)
	)

FC_REFLECT_DERIVED(thinkyoung::blockchain::FullBlockEntry,
	(thinkyoung::blockchain::BlockEntry),
	(child_block_ids)
	(node_index)
	(user_transactions)
	(signee)
	)
