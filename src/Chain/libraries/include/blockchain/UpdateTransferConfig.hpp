#pragma once

#include <blockchain/AccountEntry.hpp>
#include <blockchain/Operations.hpp>

namespace thinkyoung {
    namespace blockchain {
    
        struct UpdateTransferConfigOperation {
            static const OperationTypeEnum type;
            
            UpdateTransferConfigOperation() {}

			UpdateTransferConfigOperation(const std::string& delegate_account_name,
				const int& min_transaction_fee,
				const int& max_transaction_fee,
				const double& transaction_fee_ratio)
				:delegate_account_name(delegate_account_name), min_transaction_fee(min_transaction_fee), max_transaction_fee(max_transaction_fee), transaction_fee_ratio(transaction_fee_ratio) {}
                
			std::string         delegate_account_name;
			int                 min_transaction_fee;
			int                 max_transaction_fee;
			double              transaction_fee_ratio;
            
            void evaluate(TransactionEvaluationState& eval_state)const;
        };        
    }
} // thinkyoung::blockchain

FC_REFLECT(thinkyoung::blockchain::UpdateTransferConfigOperation, (delegate_account_name)(min_transaction_fee)(max_transaction_fee)(transaction_fee_ratio))
