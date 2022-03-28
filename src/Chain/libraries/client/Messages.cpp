#include <client/Messages.hpp>
namespace thinkyoung {
    namespace client {

        const MessageTypeEnum TrxMessage::type = MessageTypeEnum::trx_message_type;
        const MessageTypeEnum BlockMessage::type = MessageTypeEnum::block_message_type;
		const MessageTypeEnum BatchTrxMessage::type = MessageTypeEnum::batch_trx_message_type;
		const MessageTypeEnum ReplyMessage::type = MessageTypeEnum::reply_message_type;
		const MessageTypeEnum MissingRequestMessage::type = MessageTypeEnum::missing_request_message_type;
		const MessageTypeEnum MissingReplyMessage::type = MessageTypeEnum::missing_reply_message_type;
		const MessageTypeEnum SeedOpenMessage::type = MessageTypeEnum::seed_open_message_type;
		const MessageTypeEnum SeedCloseMessage::type = MessageTypeEnum::seed_close_message_type;
		const MessageTypeEnum SyncRequestMessage::type = MessageTypeEnum::sync_request_message_type;
		const MessageTypeEnum SyncReplyMessage::type = MessageTypeEnum::sync_reply_message_type;
		const MessageTypeEnum StatusRequestMessage::type = MessageTypeEnum::status_request_message_type;
		const MessageTypeEnum StatusReplyMessage::type = MessageTypeEnum::status_reply_message_type;
		const MessageTypeEnum StatusStartMessage::type = MessageTypeEnum::status_start_message_type;
		const MessageTypeEnum WalletUnlockRequestMessage::type = MessageTypeEnum::wallet_unlock_request_message_type;
		const MessageTypeEnum WalletUnlockReplyMessage::type = MessageTypeEnum::wallet_unlock_reply_message_type;
		const MessageTypeEnum WalletCloseMessage::type = MessageTypeEnum::wallet_close_message_type;
		const MessageTypeEnum WalletForcelyCloseMessage::type = MessageTypeEnum::wallet_forcely_close_message_type;
		const MessageTypeEnum WalletForcelyLockMessage::type = MessageTypeEnum::wallet_forcely_lock_message_type;

    }
} // thinkyoung::client
