#pragma once
#include <blockchain/Block.hpp>
#include <client/Client.hpp>

namespace thinkyoung {
    namespace client {

        enum MessageTypeEnum
        {
            trx_message_type = 21000,
            block_message_type = 21001,
			batch_trx_message_type = 21002,
			reply_message_type = 21003,
			missing_request_message_type = 21004,
			missing_reply_message_type = 21005,

			sync_request_message_type = 21006,
			sync_reply_message_type = 21007,

			seed_open_message_type = 21008,
			seed_close_message_type = 21009,

			status_request_message_type = 21010,
			status_reply_message_type = 21011,
			status_start_message_type = 21012,

			wallet_unlock_request_message_type = 21013,
			wallet_unlock_reply_message_type = 21014,
			wallet_close_message_type = 21015,
			wallet_forcely_close_message_type = 21016,
			wallet_forcely_lock_message_type = 21017
		};

        struct TrxMessage
        {
            static const MessageTypeEnum type;

            thinkyoung::blockchain::SignedTransaction trx;
            TrxMessage() {}
            TrxMessage(thinkyoung::blockchain::SignedTransaction transaction) :
                trx(std::move(transaction))
            {}
        };

        struct BatchTrxMessage
        {
            static const MessageTypeEnum type;
            std::vector<thinkyoung::blockchain::SignedTransaction> trx_vec;
            BatchTrxMessage() {}
            BatchTrxMessage(std::vector<thinkyoung::blockchain::SignedTransaction> transactions) :
                trx_vec(std::move(transactions))
            {}
        };

		struct BlockMessage
		{
			static const MessageTypeEnum type;

			BlockMessage(){}
			BlockMessage(const thinkyoung::blockchain::FullBlock& blk)
				:block(blk), block_id(blk.id()){}

			thinkyoung::blockchain::FullBlock    block;
			thinkyoung::blockchain::BlockIdType block_id;
			vector<uint64_t> numbers;
			fc::string peer_addr;
		};

		struct ReplyMessage
		{
			static const MessageTypeEnum type;

			ReplyMessage(){}
			ReplyMessage(const thinkyoung::blockchain::BlockIdType& blk_id)
				: block_id(blk_id){}

			thinkyoung::blockchain::BlockIdType block_id;
		};

		struct MissingRequestMessage
		{
			static const MessageTypeEnum type;

			MissingRequestMessage() {}
			MissingRequestMessage(const thinkyoung::blockchain::BlockIdType& blk_id, const thinkyoung::blockchain::BlockIdType& ch_id)
				: block_id(blk_id), child_id(ch_id) {}

			thinkyoung::blockchain::BlockIdType block_id;
			thinkyoung::blockchain::BlockIdType child_id;
		};

		struct MissingReplyMessage
		{
			static const MessageTypeEnum type;

			MissingReplyMessage() {}
			MissingReplyMessage(const thinkyoung::blockchain::FullBlock& blk)
				:block(blk), block_id(blk.id()) {}

			thinkyoung::blockchain::FullBlock    block;
			thinkyoung::blockchain::BlockIdType block_id;
		};

		struct SeedOpenMessage
		{
			static const MessageTypeEnum type;

			fc::string peer_addr;
		};

		struct SeedCloseMessage
		{
			static const MessageTypeEnum type;

			fc::string peer_addr;
		};

		struct SyncRequestMessage
		{
			static const MessageTypeEnum type;

			vector<uint64_t> numbers;
			fc::string peer_addr;
		};

		struct SyncReplyMessage
		{
			static const MessageTypeEnum type;

			std::vector<thinkyoung::blockchain::FullBlock> block_list;
			fc::string peer_addr;
		};

		struct StatusRequestMessage {
			static const MessageTypeEnum type;

			fc::string peer_addr;
		};

		struct StatusReplyMessage {
			static const MessageTypeEnum type;

			int index;
			vector<WalletStatusInfo> wallet_status_list;
			fc::string peer_addr;
		};

		struct StatusStartMessage {
			static const MessageTypeEnum type;

			int index;
			fc::string peer_addr;
			fc::time_point active_time;
		};

		struct WalletUnlockRequestMessage {
			static const MessageTypeEnum type;

			uint64_t wallet_id;
			fc::string wallet_name;
			int timeout;
			fc::string peer_addr;
		};

		struct WalletUnlockReplyMessage {
			static const MessageTypeEnum type;

			uint64_t wallet_id;
			int reply;
		};

		struct WalletCloseMessage {
			static const MessageTypeEnum type;

			uint64_t wallet_id;
		};

		struct WalletForcelyLockMessage {
			static const MessageTypeEnum type;

			uint64_t wallet_id;
		};

		struct WalletForcelyCloseMessage {
			static const MessageTypeEnum type;

			uint64_t wallet_id;
		};

	}
} // thinkyoung::client

FC_REFLECT_ENUM(thinkyoung::client::MessageTypeEnum, (trx_message_type)(block_message_type)(batch_trx_message_type)(reply_message_type))
FC_REFLECT(thinkyoung::client::TrxMessage, (trx))
FC_REFLECT(thinkyoung::client::BatchTrxMessage, (trx_vec))
FC_REFLECT(thinkyoung::client::BlockMessage, (block)(block_id)(numbers)(peer_addr))
FC_REFLECT(thinkyoung::client::ReplyMessage, (block_id))
FC_REFLECT(thinkyoung::client::MissingRequestMessage, (block_id)(child_id))
FC_REFLECT(thinkyoung::client::MissingReplyMessage, (block)(block_id))
FC_REFLECT(thinkyoung::client::SeedOpenMessage, (peer_addr))
FC_REFLECT(thinkyoung::client::SeedCloseMessage, (peer_addr))
FC_REFLECT(thinkyoung::client::SyncRequestMessage, (numbers)(peer_addr))
FC_REFLECT(thinkyoung::client::SyncReplyMessage, (block_list)(peer_addr))
FC_REFLECT(thinkyoung::client::StatusRequestMessage, (peer_addr))
FC_REFLECT(thinkyoung::client::StatusReplyMessage, (index)(wallet_status_list)(peer_addr))
FC_REFLECT(thinkyoung::client::StatusStartMessage, (index)(peer_addr)(active_time))
FC_REFLECT(thinkyoung::client::WalletUnlockRequestMessage, (wallet_id)(wallet_name)(timeout)(peer_addr))
FC_REFLECT(thinkyoung::client::WalletUnlockReplyMessage, (wallet_id)(reply))
FC_REFLECT(thinkyoung::client::WalletCloseMessage, (wallet_id))
FC_REFLECT(thinkyoung::client::WalletForcelyLockMessage, (wallet_id))
FC_REFLECT(thinkyoung::client::WalletForcelyCloseMessage, (wallet_id))

