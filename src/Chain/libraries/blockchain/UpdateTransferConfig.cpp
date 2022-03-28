#include <blockchain/UpdateTransferConfig.hpp>
#include <blockchain/Exceptions.hpp>
#include <blockchain/PendingChainState.hpp>
#include <blockchain/TransactionEvaluationState.hpp>
#include <blockchain/api_extern.hpp>

namespace thinkyoung {
    namespace blockchain {

		void thinkyoung::blockchain::UpdateTransferConfigOperation::evaluate(TransactionEvaluationState& eval_state)const
        {
            try {
				oAccountEntry delegate_entry = eval_state._current_state->get_account_entry(this->delegate_account_name);
				FC_ASSERT(delegate_entry.valid() && delegate_entry->is_delegate(), "${n} is not a delegate!", ("n", delegate_account_name));

				eval_state._current_state->store_property_entry(PropertyIdType::transaction_min_fee, variant(this->min_transaction_fee));
				eval_state._current_state->store_property_entry(PropertyIdType::transaction_max_fee, variant(this->max_transaction_fee));
				eval_state._current_state->store_property_entry(PropertyIdType::transaction_fee_ratio, variant(this->transaction_fee_ratio));

				fc::path db_path = thinkyoung::client::g_client->get_data_dir();
				if (fc::exists(db_path))
				{
					fc::path configuration_file_name(db_path / TRANSACTION_CONFIGURATION_FILENAME);
					thinkyoung::wallet::TransactionFeeConfiguration transfee_configuration;
					transfee_configuration.min_transaction_fee = this->min_transaction_fee;
					transfee_configuration.max_transaction_fee = this->max_transaction_fee;
					transfee_configuration.transaction_fee_ratio = this->transaction_fee_ratio;

					try
					{
						fc::json::save_to_file(transfee_configuration, configuration_file_name);
					}
					catch (const fc::canceled_exception&)
					{
						throw;
					}
					catch (const fc::exception& except)
					{
						elog("error writing transaction fee configuration to file ${filename}: ${error}",
							("filename", configuration_file_name)("error", except.to_detail_string()));
					}
				}

            } FC_CAPTURE_AND_RETHROW((*this))
        }

    }
} // thinkyoung::blockchain
